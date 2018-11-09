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
#include "../senet/CHttpClient.h"
#include "../seglobal/globallogger.h"
#include "CParseFormIndexFunctionJson.h"
#include "../semain/semain.h"
#include <cmath>

using namespace boost::gregorian;
using namespace std;
//using namespace log4cxx;
using namespace se;
using namespace se::data;
using namespace se::xbrl;
using namespace se::net;
using namespace se::config;
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

    //sample url: https://www.sec.gov/Archives/edgar/full-index/2017/QTR4/xbrl.idx
    
    // last processed year/quarter
    SP<IApp> pApp;
    SEOpenApp(&pApp);
    auto pCfg = pApp->get_Configurator();

    I16 lastProcessedYear = -1;
    I16 lastProcessedQtr = -1;
    SP<IConfigItemList> pSettings;
    if (ISOK(pCfg->get_Settings(USAREGIONID, "SECSTATEMENTS", &pSettings)))
    {
        auto pItem = pSettings->get_Item("IndexFileProcessingCurrentYear");
        if (pItem)
            lastProcessedYear = std::stoi(pItem->get_Value());
        pItem = pSettings->get_Item("IndexFileProcessingCurrentQtr");
        if (pItem)
            lastProcessedQtr = std::stoi(pItem->get_Value());
    }

    auto now = CDate::Now();
    I16 startYear = lastProcessedYear == -1 ? 1993 : lastProcessedYear;
    I16 endYear = now.get_Year();
    I16 startQtr = lastProcessedQtr == -1 ? 1 : lastProcessedQtr;
    I16 endQtr = now.get_Quarter();
    CHttpClient httpClient;
    const string securl = "https://www.sec.gov/Archives/edgar/full-index/";
    for(I16 year = startYear; year <= endYear; year++)
    {
        for (I16 qtr=startQtr; qtr<=endQtr; qtr++)
        {
            ostringstream url;
            url << securl;
            url << "/" << year;
            url << "/QTR" << qtr << "/xbrl.idx";
            ostringstream downloadedfilepath ;
            downloadedfilepath << _tmpdir << "xbrl-" << year << "-" << qtr << ".idx";
            if (FAILED(httpClient.GetFile(url.str().c_str(), downloadedfilepath.str().c_str())))
            {
                BERR << "Failed to download file from url. Probably no quarter available. " << url.str();
                return;
            }
            if (FAILED(ProcessForm(year, qtr, downloadedfilepath.str())))
            {
                BERR << "Failed to process xbrl index file (" << url.str() << ").";
                return;
            }
            lastProcessedQtr = qtr;
            lastProcessedYear = year;
        }
    }
    
    // save last processed
    auto pSession = ((IAppEx*)pApp._ptr)->CreateAnonymousSession();
    SP<IConfigItem> pItem;
    IConfigItem::Create(pSession, &pItem);
    pItem->set_Name("IndexFileProcessingCurrentYear");
    pItem->set_Value(lastProcessedYear);
    pCfg->set_Settings(USAREGIONID, "SECSTATEMENTS", pItem);
   
    pItem->set_Name("IndexFileProcessingCurrentQtr");
    pItem->set_Value(lastProcessedQtr);
    pCfg->set_Settings(USAREGIONID, "SECSTATEMENTS", pItem);
    
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
    ifstream fin;
    CSmartConnection conn(SEDBCONN);
    try
    {
        BINF << "Processing " << filepath << endl;
        
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
            
            // Create inst and context tables if not already
            auto q = conn->query();
            q << "CREATE TABLE IF NOT EXISTS se_fininst_" << fixed << setprecision(0) << std:: floor(formrecs.cik/10000) << " LIKE se_fininst_0;";
            BDBG << q.str() << endl;
            q.exec();
            q = conn->query();
            q << "CREATE TABLE IF NOT EXISTS se_fininstctxt_" << floor(formrecs.cik/10000) << " LIKE se_fininstctxt_0; ";
            BDBG << q.str() << endl;
            q.exec();
            
        }        //end idx

        fin.close();
       // commit entire document to the table.
        conn.CommitTransaction();

        BINF << "Completed with processing " << filepath << endl;

        return S_Success;

    }
    catch (const mysqlpp::BadQuery& er) 
    {
        if (fin.is_open())
            fin.close();
        conn.RollbackTransaction();
        BERR << "Failed to process " << filepath << " with erro " << er.what() << endl;
        return E_FORMINDEXING;
    }
    catch (const mysqlpp::BadConversion& er) 
    {
        if (fin.is_open())
            fin.close();
        conn.RollbackTransaction();
        BERR << "Failed to process " << filepath << " with erro " << er.what() << endl;
        return E_FORMINDEXING;
    }
    catch (const mysqlpp::Exception& er) 
    {
        if (fin.is_open())
            fin.close();
        conn.RollbackTransaction();
        BERR << "Failed to process " << filepath << " with erro " << er.what() << endl;
        return E_FORMINDEXING;
    }
    catch (const std::exception& er)
    {
        if (fin.is_open())
            fin.close();
        conn.RollbackTransaction();
        BERR << "Failed to process " << filepath << " with erro " << er.what() << endl;
        return E_FORMINDEXING;
    }
}
