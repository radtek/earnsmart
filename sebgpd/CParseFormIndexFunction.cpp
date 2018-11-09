/* m
 * File:   CParseFormIndexFunction.cpp
 * Author: santony
 * 
 * Created on October 12, 2012, 6:17 PM
 */

#include <string.h>

#include "../secommon.h"
#include "CParseFormIndexFunction.h"
////#include <log4cxx/logger.h>
//#include <log4cxx/propertyconfigurator.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "../semysql/CSEConnections.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <functional>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/find.hpp>

#include <map>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/parsers.hpp>
#include <boost/date_time/format_date_parser.hpp>
#include <iosfwd>
#include "../seglobal/CQuarter.h"
#include "../senet/senetapi.h"
#include <fstream>
#include "../seglobal/CDirectory.h"
#include <jsoncpp/json/json.h>

using namespace boost::gregorian;
using namespace std;
//using namespace log4cxx;
using namespace se;
using namespace se::data;
using namespace se::xbrl;
using namespace Json;

#define FOLDERSTARTYEAR 2005

//static LoggerPtr _logger = Logger::getLogger("se.bg.formindexer");

#define SEDBCONN        CSEConnections::GetMain()

//-f parseformindex -a 10-Q;10-K

sql_create_6(
        rfeedstats, 
        1, 
        6, 
        sql_bigint, feedstats_id,
        sql_smallint, folderlabelyear,
        sql_smallint, folderlabelquarter,
        sql_date, folderdate, 
        sql_date_null, xbrlidxdate,
        sql_datetime_null, processedon
        );

struct rfeedstats_comparer
{
    rfeedstats _r;
    
    rfeedstats_comparer()
    {
        
    }
    
    rfeedstats_comparer(const rfeedstats& d): _r(d)
    {
        
    }
    
    bool operator<(const rfeedstats& o)
    {
        return _r.feedstats_id < o.feedstats_id;
    }
};

typedef std::set<rfeedstats, rfeedstats_comparer> FeedStats;

CParseFormIndexFunction::CParseFormIndexFunction(ISession* ps, CSTR formType) : CTimedFunction()
{
    _pSession = ps;
    CSmartConnection conn(SEDBCONN);
    FORMTYPES formTypes;
    CSTR tok = 0;
    char* sTypes = const_cast<char*>(formType);
    tok = strtok(sTypes, ";");
    //LOG4CXX_INFO(_logger, "Function will index ");
    while (tok != 0)
    {
        //LOG4CXX_INFO(_logger, tok << " ");
        formTypes.push_back(tok);
        tok = strtok(0, ";");
    }
    
    // cache a relationship between form type and its id.
    stringstream strmsqldoctypes;
    strmsqldoctypes << "select doc_id, doc_name from se_edgar_doctypes where doc_name in (";
    for (auto ft : formTypes)
        strmsqldoctypes << "'" << ft << "',";
    string ssqlsqldoctypes = strmsqldoctypes.str().substr(0, strmsqldoctypes.str().size()-1) + ")";
    conn->query(ssqlsqldoctypes).for_each([this](Row& row)
        {
            string str = (string)row[1];           
           // LOGI(_logger, "Inserting form types of " << (I64)row[0] << ":" << str);
            _formTypes.insert(make_pair(str, (I64)row[0]));
        });
    
    //LOG4CXX_INFO(_logger, "form types.");
    
    struct passwd* pPwd = getpwuid(getuid()); 
    _tmpdir = pPwd->pw_dir;
    _tmpdir += "/edgar/";
    // create direcotry
    CDirectory::Create(_tmpdir.c_str());
    
   // LOGI(_logger, "Tempdirectory created in " <<_tmpdir);
}

CParseFormIndexFunction::~CParseFormIndexFunction() 
{
    
}

void CParseFormIndexFunction::run()
{

    BoolCompletion busy(_bIsBusy);
    
   // LOGIP("Begin to execute form indexing function.");
    
    /* 
     * 1. Check the last entry in forms table (quarter, year)
     * 2. We will save those values. 
     * 3. Construct a permutation of quarters and years from current year until 2008.
     * 4. loop this vector of permutation
     * 5. download ftp file from the folder in sec server (always over-write earlier file downloaded).
     * 6. begin a transaction
     * 7. read lines in the file, look for lines which contains 
     *          form types _formTypes vector.
     * 8. insert row.
     * 9. once the file is complete, close the transaction.
     * 10. delete the downloaded file to save server disk space
     * 11. if there is any error, rollback transaction.
     */
    
    
    //What id today's date ?
    
    /*
     * 1. open the file, go line by line and read time and directory name
     * 
     * 2. Check if this dir with datetime exists in the table, 
     * 
     * 3. if no, 
     *          a. then download the file content's all quarters and parse xbrl.idx files.
     *          b. upload records of matching doc types to the formrecs table with is_processed set to -1.
     *          
     * 4. if yes, 
     *          a. check date in the table. if less than date from the remote directory listing, then 
     *                  go through quarterly directories, download xbrl.idx files and scan for docs which
     *                  are new for that quarter.
     *          b. add into formsrecs table and set is_processed = -1.
     * 
     * 5. 
    */

   // LOGIP("Connecting to edgar server to download directory list in full-index.");

    CFtpClient ftp;
    stringstream sList ;
    ftp.BrowseRemoteFolders("ftp://ftp.sec.gov/edgar/full-index/?", [this, &sList](CSTR data)
        {
            sList << data;
        }
    );
    
    if (sList.fail())
    {
       // LOGEP("Failed to successfuly parse the directory list.");
        return ;
    }
    
   // LOGI(_logger, "Downloaded directory list -> " << sList);
   // LOGIP("Parsing directory list....");

    string line;
    std::map< int, boost::gregorian::date > directories;
    CDate dt = CDate::Now();
    stringstream year ;
    year << dt.get_Year();
    int currentQuarter ;
    CQuarter q(dt);
    currentQuarter = q.get_Quarter();
    
    try
    {
        while (getline(sList, line))
        {
            // capture the file time and directory name
            // this sample line looks like this.
            /*
            1            1416 	25	     38	  43	       56			
            drwxr-xr-x   6 1019     bin          4096 Aug  3 05:00 1993
            drwxr-xr-x   6 1019     bin          4096 Aug  3 05:00 1994
            drwxr-xr-x   6 1019     bin          4096 Aug  3 05:01 1995
            */
            string sFileTime = year.str() + " ";
            string sDirName;
            const char* szLine = line.c_str();
            for (int i=0; i!= line.size(); ++i)
            {
                if (szLine[0] != 'd')
                    break;
                if (i>= 42 && i<55)
                    sFileTime += szLine[i];
                if (i>= 55 )
                    sDirName += szLine[i];
            }
            if (szLine[0] == 'd')
                directories.insert(make_pair(atoi(sDirName.c_str()), from_string(sFileTime)));
        }
       // LOGIP("Successfully parsed the directory list.");
    }
    catch (std::exception const& ex)
    {
       // LOGE(_logger, "Failed to successfuly parse the directory list." << ex.what());
        return ;
    }    
    
    /*

    std::map< int, se::DateTime > directories;
    
    CSTR indexjsonfile = "ftp://ftp.sec.gov/edgar/full-index/index.json";
    string destjsonfile = _tmpdir + "edgarindex.json";
    ftp.GetFile(indexjsonfile, destjsonfile.c_str());
    fstream fs(destjsonfile);
    Value root;
    Reader jsonreader;
    if (!jsonreader.parse(fs, root))
    {
       // LOGI(_logger, "Failed to read edgar full-index json file. Exiting run method. Exception reported: " << jsonreader.getFormattedErrorMessages())
        if (fs.is_open())
            fs.close();
        return;
    }
    if (fs.is_open())
        fs.close();
    
    
    
//{
  //  "directory": 
    //        {"item": [
        //{"last-modified": "07\/11\/2013 11:32:18 AM", "name": "1993", "type": "dir", "href": "1993\/", "size": "4 KB"}, 
        //{"last-modified": "07\/10\/2013 10:01:51 PM", "name": "master.zip", "type": "file", "href": "master.zip", "size": "381 KB"}, 
      //]   
    
    // item array
    Value &items = root["directory"]["item"];
    for (int i=0; i != items.size(); ++i)
    {
        Value& item = items[i]; 
        if (item["type"].asString() == "dir")
        {
            int year = std::atoi(item["name"].asString().c_str());
            if (year >= 2005)
            {
                string mdate = item["last-modified"].asString();
                se::DateTime dt = se::DateTime::Parse(mdate.c_str(), "%m/%d/%Y %H:%M:%S %P");
                directories.insert(make_pair(year, dt));
            }
           // LOGI(_logger, "Found directory " << item["name"].asString() << " modified on " << item["last-modified"].asString());
        }
    }
     * 
     */
    
    
   // LOGI(_logger, "Collected a list of directories.");
    
    ErrorId errCode = E_UnknownError;
    CSmartConnection conn(SEDBCONN);
   // LOGIP("Comparing directory list with data in the table.");
   // LOGIP("Fetching records that are modified at the edgar sites.");
    for (auto p : directories)
    {
        int year = p.first;
        for(int quarter = 1; quarter <=4; quarter++)
        {
            if (year == dt.get_Year() && quarter > currentQuarter)
            {
                //goto COMPLETED;
                return;
            }
            // not processed before, process.
            string downloadedfile;
           // LOGIP("Now downloading xbrl.idx document.");
            errCode = DownloadFile(ftp, (short)year, (short)quarter, downloadedfile);
            if (FAILED(errCode))
            {
               // LOGE(_logger, "Failed to download file for the year " << year << " quarter " << quarter << " with errorcode" << errCode);
                return ;
            }
           // LOGI(_logger, "Downloading xbrl.idx document completed(" << downloadedfile << ")");
           // LOGIP("Now start to process the xbrl.idx document.");
            errCode = ProcessForm((short)year, (short)quarter, downloadedfile);
            if (FAILED(errCode))
            {
               // LOGE(_logger, "Failed to process xbrl.idx document.");
                return ;
            }
            CDirectory::Delete(downloadedfile.c_str());
           // LOGI(_logger, "Completed parsing records for " << year << " and quarter " << quarter);
        }        

    }        
    
//COMPLETED:
   // LOGIP("Successfully completed form index parsing.");
}

ErrorId CParseFormIndexFunction::DownloadFile(CFtpClient& ftp, I16 year, I16 quarter, string& outlocalfile)
{
    try
    {
        // sort from new to old.
        //sort(modifieddirectories.begin(), modifieddirectories.end(), [](int p, int prev){ return p > prev ; });
       // LOGI(_logger, "Parsing records for year " << year << " and quarter " << quarter);
        stringstream sFtpPath;
        sFtpPath << "ftp://ftp.sec.gov/edgar/full-index/" << year << "/QTR" << quarter << "/xbrl.idx";
        stringstream sOutputPath ;
        sOutputPath << _tmpdir << "xbrl-" << year << "-" << quarter << ".idx";
        ErrorId errCode = ftp.GetFile(sFtpPath.str().c_str(), sOutputPath.str().c_str());
        if (FAILED(errCode))
        {
           // LOGE(_logger, "Error downloading xbrl file from edgar site.");
            return errCode;
        }
        outlocalfile = sOutputPath.str();
        return S_Success;
    }
    catch(std::exception const& ex)
    {
       // LOGE(_logger, "Completed parsing records for " << year << " and quarter " << quarter << ". Error : " << ex.what() << ". Returning from function.");
        return E_FORMINDEXING;
    }
}

sql_create_6(se_edgar_formrecs, 1, 0,
        sql_int,cik,
        sql_bigint, form_type_id,
        sql_date,filedate,
        sql_varchar,location,
        sql_int, is_processed,
        sql_datetime_null, processed_on
        );

sql_create_3(se_edgar_feedstats,1,0,
        sql_smallint, year,
        sql_smallint, qtr,
        sql_timestamp,lastfedon
        
);


ErrorId CParseFormIndexFunction::ProcessForm(I16 Year,I16 qtr, const string& filepath)
{

    //LOG4CXX_INFO(_logger, "Entering ProcessForm function.");
    ifstream fin;
    CSmartConnection conn(SEDBCONN);
    try
    {
        fin.open(filepath);       
        /*
         * CIK|Company Name|Form Type|Date Filed|Filename
        --------------------------------------------------------------------------------
        1004155|AGL RESOURCES INC|8-K/A|2008-02-13|edgar/data/1004155/0001140361-08-003746.txt
        101829|UNITED TECHNOLOGIES CORP /DE/|8-K|2008-01-23|edgar/data/101829/0001193125-08-010089.txt
        1045309|W R GRACE & CO|8-K|2008-03-20|edgar/data/1045309/0001104659-08-018716.txt
        1045309|W R GRACE & CO|8-K|2008-03-20|edgar/data/1045309/0001104659-08-018717.txt
         */

        using namespace boost;
        typedef tokenizer< char_separator<char> > Tokenizer;
        typedef vector<string> TokenVector;
        TokenVector vec;
        char_separator<char> sep("|", "", keep_empty_tokens);
        string line;
        bool blinefound = false;

        while (getline(fin, line))
        {
            trim(line);
            // ignore the following types of lines
            // run until "CIK|" is found
            if (line.substr(0, 4) == "CIK|")
            {
                blinefound = true;
                break;
            }
        }        
        
        if (!blinefound)
        {
           // LOGWP("Find did contain form records. Exiting.");
            fin.close();
            return E_FORMINDEXING;
        }
        
        getline(fin, line);
        conn.BeginTransaction();
        // not start from next line
        while (getline(fin, line))  //start idx
        {           
            trim(line);
            // tokenize the line
            Tokenizer tok(line, sep);
            vec.assign(tok.begin(), tok.end());
           
            // what we need from the vector are
            /*
             * File type, CIK, SUBMISSION DATE and PATH to the xbrl files.
             */

             //1004155|AGL RESOURCES INC|8-K/A|2008-02-13|edgar/data/1004155/0001140361-08-003746.txt
            string frmtype = vec[2];
            if (_formTypes.find(frmtype) == _formTypes.end())
            {
                continue;
            }
            string cik = vec[0];
            string submitdate = vec[3];
            string folder = vec[4];
            I64 form_type_id = _formTypes[frmtype];
            
            // remove - and .txt from the folder.
            replace_all(folder, "-", "");
            erase_last(folder, ".txt");
            
            // verify if this line is already for a processed file.
            Query qIsProcessed = conn->query();
            qIsProcessed << "select rec_id from se_edgar_formrecs where cik = " << quote 
                    <<  cik << " and form_type_id = " << quote <<  form_type_id 
                    << " and filedate = " << quote << submitdate 
                    << " and location = " << quote <<  folder ;
            
           // LOGIP(qIsProcessed.str());
            
            auto qResult = qIsProcessed.store();
            if (qResult.size())
                continue;

            
            se_edgar_formrecs formrecs;
            formrecs.instance_table("se_edgar_formrecs");
            formrecs.cik = atoi(cik.c_str());
            sql_date sqlDt(submitdate);
            formrecs.filedate = sqlDt;
            formrecs.form_type_id = form_type_id;
            formrecs.location = folder;           
            formrecs.is_processed = -1;
            formrecs.processed_on = mysqlpp::null;
            conn->query().insert(formrecs).execute();
        }        //end idx

        fin.close();
       // commit entire document to the table.
        conn.CommitTransaction();
        //LOG4CXX_INFO(_logger, "Exiting ProcessForm.");
        return S_Success;

    }
    catch (const mysqlpp::BadQuery& er) 
    {
        if (fin.is_open())
            fin.close();
        conn.RollbackTransaction();
        //LOG4CXX_ERROR(_logger, "Query Error: " << er.what());
        return E_FORMINDEXING;
    }
    catch (const mysqlpp::BadConversion& er) 
    {
        if (fin.is_open())
            fin.close();
        conn.RollbackTransaction();
        //LOG4CXX_ERROR(_logger, "Conversion Error: " << er.what());
        return E_FORMINDEXING;
    }
    catch (const mysqlpp::Exception& er) 
    {
        if (fin.is_open())
            fin.close();
        conn.RollbackTransaction();
        //LOG4CXX_ERROR(_logger, "Error: " << er.what());
        return E_FORMINDEXING;
    }
    catch (const std::exception& er)
    {
        if (fin.is_open())
            fin.close();
        conn.RollbackTransaction();
        //LOG4CXX_ERROR(_logger, "Error: " << er.what());
        return E_FORMINDEXING;
    }
}
