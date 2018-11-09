/* 
 * File:   CProcessEdgarStatements.cpp
 * Author: aashok
 * 
 * Created on June 29, 2013, 7:12 AM
 */

#include "sexbrlinternal.h"
#include "CProcessEdgarStatementsjson.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#include<boost/regex.hpp>
#include <fstream> 
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include "../semain/semain.h"
#include "../seglobal/CDirectory.h"
#include "../senet/CHttpClient.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <libxml2/libxml/xmlreader.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <time.h>
#include "../seglobal/exceptions.h"
#include <Poco/ScopedLock.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <malloc.h>
#include <pthread.h>
#include "CXbrl.h"
#include "../seglobal/globallogger.h"
#include <jsoncpp/json/json.h>
#include "CXbrlUtilities.h"
#include "../seglobal/cpplinq.hpp"
#define SEDBCONN        CSEConnections::GetMain()
#define EDGARURL "https://www.sec.gov/Archives/"

using namespace std;
using namespace se;
using namespace se::xbrl;
using namespace se::data;
using namespace se::net;
using namespace boost;
using namespace boost::algorithm;
using namespace ZThread;
using namespace boost::property_tree;
using namespace Json;

#define XPathNsExpr(ns) (ns.empty() ? "" : ns + ":")

#define PRESENTATION            "PRESENTATION"
#define DEFINITION              "DEFINITION"
#define SCHEMA                  "SCHEMA"
#define INSTANCE                "INSTANCE"
#define LABEL                   "LABEL"
#define CALCULATION             "CALCULATION"
#define SCHEMACACHEFOLDER       "EdgarSchema"

//#define HASEXCHANDLER


const char* Fdate = "(<FILING-DATE>)(.*)";
const char* Cdate = "(<DATE-OF-FILING-DATE-CHANGE>)(.*)";
const char* CName = "(<CONFORMED-NAME>)(.*)";
const char* cik = "(<CIK>)(.*)";
const char* Sicnumber = "(<ASSIGNED-SIC>)(.*)";
const char* Ipsnumber = "(<IRS-NUMBER>)(.*)";
const char* Fyear = "(<FISCAL-YEAR-END>)(.*)";

const char* FType = "(<FORM-TYPE>)(.*)";
const char* BS1 = "(<STREET1>)(.*)";
const char* BS2 = "(<STREET2>)(.*)";
const char* BC = "(<CITY>)(.*)";
const char* BS = "(<STATE>)(.*)";
const char* BZ = "(<ZIP>)(.*)";
const char* PH = "(<PHONE>)(.*)"; //13

const char* MS1 = "(<FORM-TYPE>)(.*)";
const char* MS2 = "(<FORM-TYPE>)(.*)";
const char* MC = "(<FORM-TYPE>)(.*)";
const char* MS = "(<FORM-TYPE>)(.*)";
const char* MZ = "(<FORM-TYPE>)(.*)";
const char* SC = "(<STATE-OF-INCORPORATION>)(.*)";
const char* SCN = "(.*)(STANDARD INDUSTRIAL CLASSIFICATION:)(.*)";

//Mutex _mutex;

//string CProcessEdgarStatements_strDest = "";
//string CProcessEdgarStatements_configFile = "";

#define XMLREADFILEOPTIONS  (XML_PARSE_RECOVER | XML_PARSE_NOENT | XML_PARSE_DTDLOAD | XML_PARSE_DTDATTR | XML_PARSE_NOERROR | XML_PARSE_BIG_LINES | XML_PARSE_HUGE | XML_PARSE_IGNORE_ENC)
#define XMLREADFILEOPTIONSWITHERROR  (XML_PARSE_RECOVER | XML_PARSE_NOENT | XML_PARSE_DTDLOAD | XML_PARSE_DTDATTR | XML_PARSE_BIG_LINES | XML_PARSE_HUGE | XML_PARSE_IGNORE_ENC)

bool CProcessEdgarStatements::_bFixConcurrency = false;
std::mutex CProcessEdgarStatements::_mutex;

CProcessEdgarStatements::CProcessEdgarStatements(ISession* ps) :
_ps(ps),
_deleteInstcontext("delete a.* from %s a, %s b where a.instance_id = b.instance_id and b.rec_id = %llu"),
_deleteInst("delete from %s where rec_id = %llu"),
_deleteCalc("delete from se_fin_calculation where statement_id = "),
_deletePre("delete from se_fin_presentation where statement_id = "),
_deleteStmt("delete from se_fin_statements where rec_id = "),
_cik(0)
{
    _bFixConcurrency = true;
    Init();
}

CProcessEdgarStatements::~CProcessEdgarStatements()
{
    xbrlNs.clear();
    for (auto xrf : xbrlRefs)
    {
        auto ptr = xrf.second;
        xmlFreeDoc(ptr);
    }
    malloc_trim(0);
}

void CProcessEdgarStatements::Init()
{
    if (_cacheSchemaPath.empty())
    {
        struct passwd* pPwd = getpwuid(getuid());
        _cacheSchemaPath = pPwd->pw_dir;
        _cacheSchemaPath += "/" + (string) SCHEMACACHEFOLDER + "/";
        if (!CDirectory::Create(_cacheSchemaPath.c_str()))
        {
            //BDBG << "Already created edgar schema folder.";
        }
        _cacheSchemaPath = _cacheSchemaPath;
    }

    DownloadUsGaapSchemas();

    //        CSTR sfindsql ="select * from se_edgar_formrecs where is_processed = -1 order by filedate desc limit 1";
    // get exe dir
    if (_sqlRecord.empty())
    {

        SP<se::IApp> pApp;
        SP<se::config::IConfigurator> pConfig;
        SEOpenApp(&pApp);
        pApp->get_Configurator(&pConfig);


        ifstream filestrm;
        filestrm.open(pConfig->get_ConfigFilePath());
        // read configuration info from json file.
        Value configRoot;
        Reader jsonreader;
        if (!jsonreader.parse(filestrm, configRoot))
        {
            BERR << "CoProcessSecDailyPricenfiguration file sebgpd.config.json is invalid and cannot be parsed as a json file.";
            return;
        }

        _sqlRecord = "select * from se_edgar_formrecs where is_processed = -1 order by filedate asc limit 1;";
        if (configRoot.isMember("sebgpd"))
        {
            const Value& bgcfg = configRoot["sebgpd"];
            if (bgcfg.isMember("mainsql"))
                _sqlRecord = bgcfg["mainsql"].asString();
        }
    }

}

ErrorId CProcessEdgarStatements::Process()
{
    /* 1. Open table se_edgar_formrecs table order by filedate ascending where is_processed = false and  limit to 10
     * 2. while looping records
     *  a. get directory field and ftp into the directory 
     *  b. obtain the index html file.
     *  c. create the special structure with the fields (pp1)
     *  d. start a transaction.
     *  e. get the sic info, insert a sic record in se_edgar_sics table if not available already.
     *  f. if the cik is not available, insert a record into se_edgar_ciks or if available, update other fields (company name, address, phone) (also fetch stateid from se_gbl_states table).
     *  g. check if this cik is available in se_edgar_productid_cik. If not available do these steps below:
     *       i. take edgar's company name. strip -, comma, spaces, #, and store in a variable (say x)
     *      ii. use this sql statement 
     *          select * from sa.se_sec_products where REPLACE(product_name, REPLACE(product_name, ',', ''), '#', '') = x
     *          to find them matching product_id
     *      iii. insert to se_edgar_productid_cik (cik and productid)
     *  h. close transaction
     *  i. go to next item in the loop
     * 3. End;
     */

    BINF << "Processing statements begins here.....";
    _bFixConcurrency = false;
#ifdef HASEXCHANDLER    
    try
    {
#endif
        CSmartConnection conn(SEDBCONN);
        BINF << "Connection acquired.";
        Row row;
#ifdef HASEXCHANDLER    
        try
        {
#endif
            // LOGI(_logger, "Thread " << pthread_self() << " is attempting to acquire formrec record");
            _lastRecId = 0;
            BINF << "Acquiring record.";
            std::lock_guard<std::mutex> g(_mutex);
            ErrorId ret = AquireRecord(conn, row);
            if (FAILED(ret))
            {
                _bFixConcurrency = true;
                // LOGI(_logger, "Thread " << pthread_self() << " failed to acquire record. Returning.......");
                BERR << "Thread " << pthread_self() << " failed to acquire record. Returning.......";
                return S_OK;
            }
            _bFixConcurrency = true;
            BINF << "Succesfully acquired record.";
#ifdef HASEXCHANDLER    
        }
        catch (const std::exception& ex)
        {
            _bFixConcurrency = true;
            BERR << ex.what();
            //throw ex;
            return E_UnknownError;
        }
#endif
        // LOGI(_logger, "Thread " << pthread_self() << " done with acquiring record. Now going to process statement.");
        auto retCode = ProcessRecord(conn, row);
        if (FAILED(retCode))
        {
            BERR << "Failed to process record for " << (I64) row["rec_id"] << ". Skipping to next one.";
            //throw seexception("Failed to process record");
            return retCode;
        }
        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (const std::exception& ex)
    {
        BERR << "Failed to process the statements. Check logs to locate the error." << ex.what();
        return E_XMLParseError;
    }
#endif
    BINF << "Processing statements ends here....";

    // LOGI(_logger, "Concurrency state is set to " << _bFixConcurrency);
    return S_OK;
}

ErrorId CProcessEdgarStatements::Process(unsigned long recId)
{
    CSmartConnection conn(SEDBCONN);
#ifdef HASEXCHANDLER    
    try
    {
#endif
        _lastRecId = 0;
        Row row;
        RETURNIFFAILED(AquireRecord(conn, recId, row));
        auto retCode = ProcessRecord(conn, row);
        if (FAILED(retCode))
        {
            BERR << "Failed to process record for " << (I64) row["rec_id"] << ". Skipping to next one.";
            //throw seexception("Failed to process record");
            return retCode;
        }
        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (const std::exception& ex)
    {
        BERR << "Failed to process the statements. Check logs to locate the error." << ex.what();
        return E_XMLParseError;
    }
#endif
}

ErrorId CProcessEdgarStatements::DeleteIfFailed(CSmartConnection& conn, UID rec_id)
{
#ifdef HASEXCHANDLER    
    try
    {
#endif        
        BINF << "DeleteIfFailed: Deleting relevant table records";
        UID recId = rec_id == 0 ? _lastRecId : rec_id;
        // format delInstctxt and delInstance sqls with actual table names
        char sqlbuff[500];
        snprintf(sqlbuff, sizeof (sqlbuff), _deleteInstcontext, _ctxtTableName.c_str(), _instTableName.c_str(), rec_id);
        BDBG << sqlbuff << endl;
        Query delInstanceCtxt = conn->query(sqlbuff);
        delInstanceCtxt.execute();


        // zero memory
        sqlbuff[0] = '\0';
        snprintf(sqlbuff, sizeof (sqlbuff), _deleteInst, _instTableName.c_str(), rec_id);
        BDBG << sqlbuff << endl;
        Query delInstance = conn->query(sqlbuff);
        delInstance.execute();


        // POPULATE STATEMENTIDS
        Query qStatements = conn->query();
        qStatements << "select statement_id from se_fin_statements where rec_id = " << recId;
        // LOGI(_logger, "DeleteIfFailed: Query-> " << qStatements.str() << " -> being executed.");

        StoreQueryResult statements = qStatements.store();

        for (Row& row : statements)
        {
            UID statementId = (UID) row[0];
            Query delCalc = conn->query();
            delCalc << _deleteCalc << statementId;
            // LOGI(_logger, "DeleteIfFailed: Query-> " << delCalc.str() << " -> being executed.");
            delCalc.execute();


            Query delPre = conn->query();
            delPre << _deletePre << statementId;
            // LOGI(_logger, "DeleteIfFailed: Query-> " << delPre.str() << " -> being executed.");
            delPre.execute();

        }

        Query delStatements = conn->query();
        delStatements << _deleteStmt << recId;
        // LOGI(_logger, "DeleteIfFailed: Query-> " << delStatements.str() << " -> being executed.");
        delStatements.execute();


        BINF << "DeleteIfFailed: Successfully deleted relevant table records";

        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (std::exception& ex)
    {
        BERR << "DeleteIfFailed method failed with error : " << ex.what();
        return E_DBUpdateError;
    }
    catch (...)
    {
        BERR << "Unknown error.";
        return E_DBUpdateError;
    }
#endif
}

int CProcessEdgarStatements::getLastError()
{
    int errsv = errno;
    if (errsv > 0)
    {
        BERR << strerror(errsv) << endl;
    }
    return errsv;
}

ErrorId CProcessEdgarStatements::ProcessRecord(CSmartConnection &conn, mysqlpp::Row& row)
{
    ErrorId retCode = E_UnknownError;
    I64 recId = row["rec_id"];
    _lastRecId = recId;
    properties ppt1;
    // LOGI(_logger, "Processing edgar file record " << recId);
#ifdef HASEXCHANDLER    
    try
    {
#endif
        // store _cik, _instTableName and _ctxtTableName
        _cik = CXbrlUtilities::GetCikFromRecordId(recId);
        assert(_cik > 0);
        
        
        ppt1.cikValue = std::to_string(_cik);
        _ctxtTableName = CXbrlUtilities::GetInstanceCtxtTableName(_cik);
        _instTableName = CXbrlUtilities::GetInstanceTableName(_cik);
        
        auto qHasTable = conn->query();
        qHasTable << "SELECT count(0) FROM information_schema.tables WHERE table_schema = 'sa' AND table_name = " << quote << _instTableName << " LIMIT 1;";
        auto sHasTable = qHasTable.store();
        if ( sHasTable.size() && (int)sHasTable[0][0] == 0 )
        {
            auto qCreateTables = conn->query();
            qCreateTables << "CREATE TABLE " << _instTableName << " LIKE se_fininst_0; ";
            qCreateTables << "CREATE TABLE " << _ctxtTableName << " LIKE se_fininstctxt_0; ";
            qCreateTables.execute();
        }

        BINF << "ProcessRecord started...";
        // https://www.sec.gov/Archives/edgar/data/1497251/000151116417000720/index.json
        //https://www.sec.gov/Archives/edgar/data/1497251/000151116417000720/0001511164-17-000720-index-headers.html
        RETURNIFFAILED(DeleteIfFailed(conn, recId));
        // LOGI(_logger, "Beginning to process statements with rec id " << recId);
        ppt1.recid = recId;
        string location = (CSTR) row["location"];
        edgarUrl = EDGARURL;
        edgarUrl += location;

        // from json, we have to extract urls for index-headers.html file + all xml files
        ostringstream jsonbuffer;
        CHttpClient httpClient;

        httpClient.EnableFollowLocation();
        httpClient.EnableHostNameVerification();
        httpClient.EnablePeerVerification();

        retCode = httpClient.GetBuffer((edgarUrl + "/index.json").c_str(), &jsonbuffer);
        if (FAILED(retCode) || jsonbuffer.str().empty())
        {
            ostringstream ostr;
            retCode = httpClient.GetBuffer((edgarUrl).c_str(), &ostr);
            if (ISOK(retCode))
            {
                BDBG << ostr.str();
            }
            else
                return E_HttpError;
        }

        BDBG << jsonbuffer.str();
        Json::Value value;
        Json::Reader reader;
        if (!reader.parse(jsonbuffer.str(), value, false))
        {
            BERR << "Json file downloaded using url " << edgarUrl << " failed to get parsed.";
            return E_JsonParseError;
        }

        struct passwd* pPwd = getpwuid(getuid());
        string strDest = pPwd->pw_dir;
        char tmpFileBuff[TMP_MAX] = "tmpXbrl_XXXXXX";
        mkstemp(tmpFileBuff);
        strDest += "/edgar/" + (string) tmpFileBuff;
        CDirectory::Create(strDest.c_str());
        temporaryDir = strDest;

        string sfindex, sfxsd, sfcal, sfpre, sflab, sfdef, sfinst;

        std::vector<bool> loopCheck;
        auto gfcode = 0;
        // download specific files
        for (const Json::Value& v : value["directory"]["item"])
        {
            string name = v["name"].asString();
            if (std::strstr(name.c_str(), "-index.html"))
            {
                sfindex = strDest + "/" + name;
                gfcode = 0;
                try
                {
                    gfcode = httpClient.GetFile((edgarUrl + "/" + name).c_str(), sfindex.c_str());
                    if (FAILED(gfcode) || getLastError() > 0)
                    {
                        loopCheck.push_back(true);
                    }
                }
                catch (...)
                {
                    if (getLastError() > 0)
                    {
                        loopCheck.push_back(true);
                    }
                }
            }
            else if (std::strstr(name.c_str(), ".xsd"))
            {
                sfxsd = strDest + "/" + name;
                gfcode = 0;
                try
                {
                    gfcode = httpClient.GetFile((edgarUrl + "/" + name).c_str(), sfxsd.c_str());
                }
                catch (...)
                {

                }
            }
            else if (std::strstr(name.c_str(), "_pre.xml"))
            {
                sfpre = strDest + "/" + name;
                gfcode = 0;
                try
                {
                    gfcode = httpClient.GetFile((edgarUrl + "/" + name).c_str(), sfpre.c_str());
                    if (FAILED(gfcode) || getLastError() > 0)
                    {
                        loopCheck.push_back(true);
                    }
                }
                catch (...)
                {
                    if (getLastError() > 0)
                    {
                        loopCheck.push_back(true);
                    }
                }
            }
            else if (std::strstr(name.c_str(), "_cal.xml"))
            {
                sfcal = strDest + "/" + name;
                gfcode = 0;
                try
                {
                    gfcode = httpClient.GetFile((edgarUrl + "/" + name).c_str(), sfcal.c_str());
                }
                catch (...)
                {

                }
            }
            else if (std::strstr(name.c_str(), "_lab.xml"))
            {
                sflab = strDest + "/" + name;
                gfcode = 0;
                try
                {
                    gfcode = httpClient.GetFile((edgarUrl + "/" + name).c_str(), sflab.c_str());
                    if (FAILED(gfcode) || getLastError() > 0)
                    {
                        loopCheck.push_back(true);
                    }
                }
                catch (...)
                {
                    if (getLastError() > 0)
                    {
                        loopCheck.push_back(true);
                    }
                }
            }
            else if (std::strstr(name.c_str(), "_def.xml"))
            {
                sfdef = strDest + "/" + name;
                gfcode = 0;
                try
                {
                    gfcode = httpClient.GetFile((edgarUrl + "/" + name).c_str(), sfdef.c_str());
                }
                catch (...)
                {

                }
            }
            else if (std::strstr(name.c_str(), ".xml"))
            {
                if (name == "FilingSummary.xml")
                    continue;
                
                sfinst = strDest + "/" + name;
                gfcode = 0;
                try
                {
                    gfcode = httpClient.GetFile((edgarUrl + "/" + name).c_str(), sfinst.c_str());
                    if (FAILED(gfcode) || getLastError() > 0)
                    {
                        loopCheck.push_back(true);
                    }
                }
                catch (...)
                {
                    if (getLastError() > 0)
                    {
                        loopCheck.push_back(true);
                    }
                }
            }
        }
        /*
        using namespace cpplinq;
        auto haserrors = from(loopCheck) >> any([](auto b){return b;});
        if (haserrors)
        {
            throw std::runtime_error("Failed to download one or more files from xbrl server.");
        }
         * */
        for (auto b : loopCheck)
        {
            if (b)
                throw std::runtime_error("Failed to download one or more files from xbrl server.");
        }            
        
        retCode = ParseEdgarHtmlData(conn, sfindex.c_str(), ppt1, nullptr); //parse data from html page uncommente after it
        if (FAILED(retCode))
        {
            BERR << "Failed at ParseEdgarHtmlData";
            return retCode;
        }

        // double check all file types
        if (!ppt1.xbrlDocs[INSTANCE])
        {
            if (sfinst.empty())
            {
                // LOGEP("Instance file is missing.");
            }
            else
            {
                auto d = xmlReadFile(sfinst.c_str(), NULL, XMLREADFILEOPTIONS);
                if (d == nullptr)
                {
                    BERR << "INSTANCE file is empty.";
                }
                ppt1.xbrlDocs[INSTANCE] = d;
                RegisterNamespaces(ppt1.xbrlDocs[INSTANCE], ppt1.nsDocs[INSTANCE]);
                if (!ppt1.xbrlDocs[INSTANCE])
                {
                    BERR << "Instance file cannot be loaded. Exiting";
                    //throw seexception(E_XMLParseError, "Instance file cannot be loaded. ");
                    return E_XMLParseError;
                }
            }
        }

        if (!ppt1.xbrlDocs[CALCULATION])
        {
            if (sfcal.empty())
            {
                // LOGEP("Calculation file is missing.");
            }
            else
            {
                //                ppt1.xbrlDocs[CALCULATION]=xmlReadFile(url2.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                //                ppt1.nsDocs[CALCULATION] = XmlNs();
                //                RegisterNamespaces(ppt1.xbrlDocs[CALCULATION], ppt1.nsDocs[CALCULATION]);

                auto d = xmlReadFile(sfcal.c_str(), NULL, XMLREADFILEOPTIONS);
                if (d == nullptr)
                {
                    BERR << "Calculation file is empty.";
                }
                ppt1.xbrlDocs[CALCULATION] = d;
                RegisterNamespaces(ppt1.xbrlDocs[CALCULATION], ppt1.nsDocs[CALCULATION]);
                if (!ppt1.xbrlDocs[CALCULATION])
                {
                    // LOGEP("Calculation file cannot be loaded. Exiting");
                    BERR << "Calculation file cannot be loaded. ";
                    return E_XMLParseError;
                }
            }
        }

        if (!ppt1.xbrlDocs[PRESENTATION])
        {
            if (sfpre.empty())
            {
                // LOGEP("Presentation file is missing.");
                BERR << "No presentation file.";
                return E_NotFound;
            }
            else
            {
                auto d = xmlReadFile(sfpre.c_str(), NULL, XMLREADFILEOPTIONS);
                if (d == nullptr)
                {
                    BERR << "Presentation file is empty.";
                }
                ppt1.xbrlDocs[PRESENTATION] = d;
                RegisterNamespaces(ppt1.xbrlDocs[PRESENTATION], ppt1.nsDocs[PRESENTATION]);
                if (!ppt1.xbrlDocs[PRESENTATION])
                {
                    BERR << "PRESENTATION file cannot be loaded. Exiting";
                    return E_XMLParseError;
                }
            }
        }

        if (!ppt1.xbrlDocs[LABEL])
        {
            if (sflab.empty())
            {
                // LOGEP("Label file is missing.");
            }
            else
            {
                auto d = xmlReadFile(sflab.c_str(), NULL, XMLREADFILEOPTIONS);
                if (d == nullptr)
                {
                    BERR << "Label file is empty.";
                }

                ppt1.xbrlDocs[LABEL] = d;
                RegisterNamespaces(ppt1.xbrlDocs[LABEL], ppt1.nsDocs[LABEL]);
                if (!ppt1.xbrlDocs[LABEL])
                {
                    BERR << "LABEL file cannot be loaded. ";
                    return E_XMLParseError;
                }
            }
        }
        if (!ppt1.xbrlDocs[SCHEMA])
        {
            if (sfxsd.empty())
            {
                // LOGEP("Schema file is missing.");
                BERR << "No schema file.";
                return E_XMLParseError;
            }
            else
            {
                ppt1.XSD_Fname = sfxsd;
                auto d = xmlReadFile(sfxsd.c_str(), NULL, XMLREADFILEOPTIONS);
                if (d == nullptr)
                {
                    BERR << "Main schema file is empty.";
                }
                ppt1.xbrlDocs[SCHEMA] = d;
                RegisterNamespaces(ppt1.xbrlDocs[SCHEMA], ppt1.nsDocs[SCHEMA]);
                if (!ppt1.xbrlDocs[SCHEMA])
                {
                    BERR << "SCHEMA file cannot be loaded. ";
                    return E_XMLParseError;
                }
            }
        }

        //LOG4CXX_INFOINFO(_logger, "Parse index html successful.");
        //LOG4CXX_INFOINFO(_logger, "Start Inserting Data");

        retCode = StoreEdgarHtmlData(conn, ppt1);
        if (FAILED(retCode))
        {
            BERR << "Failed at StoreEdgarHtmlData.";
            return retCode;
        }

        string path = strDest + "/";
        auto currPath = CDirectory::GetCurDir();

        chdir(path.c_str());



        retCode = ProcessStatement(conn, row, path.c_str(), ppt1);
        if (FAILED(retCode))
        {
            BERR << "Failed at ProcessStatement.";
            return retCode;
        }

        // save finyearend in formrecs table.
        Query qFinYearEnd = conn->query();
        trim(ppt1.FYearEnd);
        qFinYearEnd << "update se_edgar_formrecs set finyearend = " << quote << ppt1.FYearEnd << " where rec_id = " << recId;
        qFinYearEnd.exec();

#ifdef PROCESSFINSTATS

        SP<IXbrl> pxbrl;
        THROWIFFAILED(XbrlOpen(this->_ps, &pxbrl), "Failed to open xbrl api for updating performance ratios.");

        CXbrl* pcxbrl = dynamic_cast<CXbrl*> (pxbrl._ptr);

        SP<IFinancialsConsolidated> pcon;
        retCode = pcxbrl->GetConsolidatedFinancials(ppt1.cikValue.c_str(), &pcon, true);
        if (ISOK(retCode))
        {
            retCode = pcxbrl->_PersistPerformanceData(pcon);
            if (FAILED(retCode))
            {
                BERR << "Failed to run performance ratio value updating engine for " << ppt1.cikValue;
            }
            DATAPERIOD dp;
            CDate dtNow = CDate::Now();
            dp.Quarter = dtNow.get_Quarter();
            dp.Year = dtNow.get_Year();
            retCode = pcxbrl->ProcessProductKeyStats(conn, pxbrl->GetProductId(ppt1.cikValue.c_str()), pcon, dp);
            if (FAILED(retCode))
            {
                BERR << "Failed to apply product financial statistics for " << ppt1.cikValue;
            }
        }

#endif

        SaveProcessStatus(conn, recId, retCode);

        filesystem::remove_all(this->temporaryDir);

        ppt1.clear();


        BINF << "ProcessRecord completed for rec_id = " << recId << " and CIK = " << _cik;

        chdir(currPath.c_str());

        return retCode;
#ifdef HASEXCHANDLER    
    }
    catch (const seexception& ex)
    {
        BERR << "Failed to process the statements for recid << " + recId << ". Check logs to locate the error." << ex.what();
        ppt1.clear();
        SaveProcessStatus(conn, recId, ex.error());
        return ex.error();
    }
    catch (const std::exception& ex)
    {
        BERR << "Failed to process the statements for recid << " + recId << ". Check logs to locate the error." << ex.what();
        ppt1.clear();
        SaveProcessStatus(conn, recId, E_XMLParseError);
        return E_XMLParseError;
    }
    catch (...)
    {
        BERR << "ProcessRecord: Failed to process the statements for recid << " + recId << ". Unknown error.";
        //conn.RollbackTransaction();
        ppt1.clear();
        SaveProcessStatus(conn, recId, E_UnknownError);
        return E_UnknownError;
    }
#endif
}

ErrorId CProcessEdgarStatements::AquireRecord(CSmartConnection &conn, Row& outRow)
{
#ifdef HASEXCHANDLER    
    try
    {
#endif
        Query qfind = conn->query(_sqlRecord);
        mysqlpp::StoreQueryResult res = qfind.store();
        if (res.size())
        {
            outRow = res[0];
            Query qUpdate = conn->query();
            qUpdate << "update se_edgar_formrecs set is_processed = 2 where rec_id = " << (I64) outRow["rec_id"];
            qUpdate.execute();
            return S_Success;
        }
        return E_NoDataToReturn;
#ifdef HASEXCHANDLER    
    }
    catch (std::exception& ex)
    {
        BERR << "Exception in AcquireREcord method. Returning failed. Exception: " << ex.what();
        return E_MutexLock;
    }
    catch (...)
    {
        BERR << "AquireRecord: Unknown error";
        return E_UnknownError;
    }
#endif
}

ErrorId CProcessEdgarStatements::AquireRecord(CSmartConnection &conn, UID recId, Row& outRow)
{
#ifdef HASEXCHANDLER    
    try
    {
#endif
        BINF << "AquireRecord started...";

        Query qfind = conn->query();
        qfind << "select * from se_edgar_formrecs where rec_id = " << recId;
        mysqlpp::StoreQueryResult res = qfind.store();
        if (res.size())
        {
            outRow = res[0];
            Query qUpdate = conn->query();
            qUpdate << "update se_edgar_formrecs set is_processed = 2 where rec_id = " << (I64) outRow["rec_id"];
            qUpdate.execute();
        }
        BINF << "completed started...";

        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (std::exception& ex)
    {
        BERR << "Exception in AcquireREcord method. Returning failed. Exception: " << ex.what();
        return E_MutexLock;
    }
    catch (...)
    {
        BERR << "AquireRecord: Unknown error";
        return E_UnknownError;
    }
#endif
}

ErrorId CProcessEdgarStatements::SaveProcessStatus(CSmartConnection &conn, I64 recId, ErrorId retCode)
{
#ifdef HASEXCHANDLER    
    try
    {
#endif
        BINF << "SaveProcessStatus started...";

        //Guard<Mutex> g(_mutex);
        BINF << "Setting the form recs table's IsProcessed = " << retCode;

        Query q = conn->query();
        q << "update se_edgar_formrecs set is_processed = " << retCode << ", processed_on = NOW() where rec_id = " << recId;

        BINF << q.str();
        q.exec();

        BINF << "Set the form recs table's IsProcessed = " << retCode;

        BINF << "SaveProcessStatus completed...";

        return retCode;
#ifdef HASEXCHANDLER    
    }
    catch (std::exception& ex)
    {
        BERR << "SaveProcessStatus failed with error " << ex.what();
        return E_DBUpdateError;
    }
    catch (...)
    {
        BERR << "SaveProcessStatus failed with an unknown error ";
        return E_UnknownError;
    }
#endif
}

ErrorId CProcessEdgarStatements::ProcessStatement(CSmartConnection& conn, Row& row, CSTR path, properties &ppt1)
{

    BINF << "Process Statement Started for " << ppt1.cikValue << " and for file date " << ppt1.fieldDate;
#ifdef HASEXCHANDLER    
    try
    {
#endif
        BINF << "ProcessStatement started...";

        SE_CODE retCode = S_OK;
        retCode = ProcessPresentation(conn, row, path, ppt1);
        if (FAILED(retCode))
        {
            BERR << "Failed to process presentation file. Error code: " << retCode;
            return retCode;
        }
        if (ppt1.xbrlDocs[CALCULATION])
        {
            retCode = ProcessCalculation(conn, row, path, ppt1);
            if (FAILED(retCode))
            {
                BERR << "Failed to process calculation file. Error code: " << retCode;
                return retCode;
            }
        }
        retCode = ProcessInstance(conn, row, path, ppt1);
        if (FAILED(retCode))
        {
            BERR << "Failed to process instance file. Error code: " << retCode;
            return retCode;
        }
        retCode = ProcessLabel(conn, row, path, ppt1);
        if (FAILED(retCode))
        {
            BERR << "Failed to process label file. Error code: " << retCode;
            return retCode;
        }
        BINF << "Process Statement Ended for " << ppt1.cikValue << " and for file date " << ppt1.fieldDate;

        BINF << "ProcessStatement completed...";

        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (std::exception& ex)
    {
        BERR << "Returned an unknown error. Reported as '" << ex.what() << "'";
        return E_UnknownError;
    }
    catch (...)
    {
        BERR << "ProcessStatement: Failed Unknown error.";
        return E_UnknownError;
    }
#endif
}

ErrorId CProcessEdgarStatements::ProcessLabel(CSmartConnection& conn, mysqlpp::Row& row, const char* path, properties& ppt1)
{
#ifdef HASEXCHANDLER    
    try
    {
#endif
        BINF << "Process Label Started";
        xmlDocPtr docLab = ppt1.xbrlDocs[LABEL];
        //  if not doc lab, simply skip
        if (!docLab)
        {
            BWRN << "Label document is not available. Will skip and try to survive without labels.";
            return S_OK;
        }
        XmlNs& nsLab = ppt1.nsDocs[LABEL];
        auto rootNode = xmlDocGetRootElement(docLab);
        xmlXPathContextPtr xpathContext = xmlXPathNewContext(docLab);
        if (!xpathContext)
        {
            BERR << "Instance xpath context creation failed.";
            return E_XPathContextCreate;
        }
        ErrorId retCode = RegisterNsInXpathContext(xpathContext, nsLab);
        if (retCode != S_Success)
        {
            xmlXPathFreeContext(xpathContext);
            BERR << "Namespace registration failed.";
            return E_XMLParseError;
        }
        string xpathstr = "//" + XPathNsExpr(GetDocDefaultNamespace(rootNode)) + "loc";
        auto xpathobj = xmlXPathEvalExpression(BAD_CAST xpathstr.c_str(), xpathContext);
        if (!xpathobj)
        {
            xmlXPathFreeContext(xpathContext);
            BERR << "Xpath expression creation failed for " << xpathstr;
            return E_XMLParseError;
        }
        string hRef;
        string Label;
        auto contRefNodes = xpathobj->nodesetval;
        auto size = (contRefNodes) ? contRefNodes->nodeNr : 0;
        for (auto i = 0; i != size; ++i)
        {
            xmlNodePtr cur = contRefNodes->nodeTab[i];
            xmlAttr* attr = NULL;
            xmlAttr* attrib = cur->properties;
            for (attr = attrib; attr; attr = attr->next)
            {
                string attrName = (CSTR) attr->name;
                xmlNode* attrVal = attr->children;
                if (attrVal->type == XML_TEXT_NODE)
                {
                    string sAttrVal = (CSTR) attrVal->content;
                    if (attrName == "href")
                    {
                        hRef = sAttrVal;
                    }
                    if (attrName == "label")
                    {
                        Label = sAttrVal;
                    }
                }
            }
            vector<string> splits;
            split(splits, hRef, is_any_of("#"));
            string XbrHref = splits[1];
            retCode = InsertLabel(conn, XbrHref.c_str(), Label.c_str(), xpathContext, ppt1, row);
            if (FAILED(retCode))
            {
                xmlXPathFreeContext(xpathContext);
                xmlXPathFreeObject(xpathobj);
                return retCode;
            }

        }
        xmlXPathFreeContext(xpathContext);
        xmlXPathFreeObject(xpathobj);
        BINF << "Process Label Ended";
        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (...)
    {
        BERR << "ProcessLabel: Failed Unknown error.";
        return E_UnknownError;
    }
#endif
}

ErrorId CProcessEdgarStatements::InsertLabel(CSmartConnection& conn, CSTR XbrHref, CSTR Label, xmlXPathContextPtr xpathContext, properties& ppt1, mysqlpp::Row &recrow)
{
    xmlXPathObjectPtr xpathobj = nullptr;
    xmlXPathObjectPtr xpathLabobj = nullptr;
#ifdef HASEXCHANDLER    
    try
    {
#endif
        xmlDocPtr docLab = ppt1.xbrlDocs[LABEL];
        auto rootNode = xmlDocGetRootElement(docLab);
        //string defNamespace = GetDocDefaultNamespace(rootNode);
        vector<string> splits;
        split(splits, XbrHref, is_any_of("_"));
        string prefix = splits[0];
        string name;
        if (splits.size() == 1)
        {
            //prefex="";
            //name = splits[0];
            BWRN << "Label info " << XbrHref << " is not recognized." << endl;
            return S_OK;
        }
        else
            name = splits[1];

        string xbrl_name;
        I64 xbrl_id = 0;
        Row row;
        {
            Query q = conn->query();
            q << "SELECT xbrl_id,xbrl_name FROM  se_xbrl_lookup WHERE  xbrl_prefix = " << quote << prefix << " AND  xbrl_name = " << quote << name;
            auto qRes = q.store();
            if (qRes.size())
            {
                row = qRes[0];
                xbrl_id = (I64) row["xbrl_id"];
                xbrl_name = (string) row["xbrl_name"];
            }
        }
        string xpathstr = "//*[@xlink:from='";
        xpathstr = xpathstr + Label + "']";
        //string xpathstr = "//" + XPathNsExpr(GetDocDefaultNamespace(rootNode)) + "labelArc[@xlink:from='" + Label + "']";
        xpathobj = xmlXPathEvalExpression(BAD_CAST xpathstr.c_str(), xpathContext);
        if (!xpathobj)
        {
            BERR << "Xpath expression creation failed for " << xpathstr;
            return E_XMLParseError;
        }
        string LabTo;
        string LabValue;
        auto contRefNodes = xpathobj->nodesetval;
        auto size = (contRefNodes) ? contRefNodes->nodeNr : 0;
        if (size == 1)
        {
            xmlNodePtr cur = contRefNodes->nodeTab[0];
            xmlAttr* attr = NULL;
            xmlAttr* attrib = cur->properties;
            for (attr = attrib; attr; attr = attr->next)
            {
                string attrName = (CSTR) attr->name;
                xmlNode* attrVal = attr->children;
                if (attrVal && attrVal->type == XML_TEXT_NODE)
                {
                    string sAttrVal = (CSTR) attrVal->content;
                    if (attrName == "to")
                    {
                        LabTo = sAttrVal;
                    }

                }
            }
            string xpathLabstr = "//*[@xlink:label=\"";
            xpathLabstr = xpathLabstr + LabTo + "\"]";
            //string xpathLabstr = "//" + XPathNsExpr(GetDocDefaultNamespace(rootNode)) + "label[@xlink:label='" + LabTo + "']";
            xpathLabobj = xmlXPathEvalExpression(BAD_CAST xpathLabstr.c_str(), xpathContext);
            if (!xpathLabobj)
            {
                BERR << "Xpath expression creation failed for " << xpathLabstr;
                xmlXPathFreeObject(xpathobj);
                return E_XMLParseError;
            }

            auto labNodes = xpathLabobj->nodesetval;
            auto lsize = (labNodes) ? labNodes->nodeNr : 0;
            if (size > 0)
            {
                xmlNodePtr Labcur = labNodes->nodeTab[0];
                LabValue = Labcur->children && Labcur->children->content ? (CSTR) Labcur->children->content : "";
            }
            xmlXPathFreeObject(xpathLabobj);
            xpathLabobj = nullptr;
        }

        if (LabValue.empty() || size > 1)
        {
            //when meet more than one labarc under one xbrlname(loc)
            string newstr;
            int size;
            int i = 0;
            int j = 0;
            char c;
            size = xbrl_name.size();
            while (i < size)
            {
                c = xbrl_name[i];
                if (isupper(c))
                {
                    newstr = xbrl_name.substr(j, (i - j));
                    LabValue = LabValue + " " + newstr;
                    j = i;
                }
                i++;
            }
            newstr = xbrl_name.substr(j, (size - j));
            LabValue = LabValue + " " + newstr;
        }
        {
            //Guard<Mutex> g(_mutex);
            Query qlabels = conn->query();
            qlabels << "SELECT xbrl_id FROM  se_fin_labels WHERE  cik = " << quote << (I64) recrow["cik"] << " AND  xbrl_id = " << quote << xbrl_id;
            auto qRes = qlabels.store();
            if (qRes.size())
            {
                Query q = conn->query();
                q << "UPDATE `se_fin_labels` SET `label`= " << quote << LabValue << "WHERE  cik = " << quote << (I64) recrow["cik"] << " AND  xbrl_id = " << quote << xbrl_id;
                q.exec();
            }
            else
            {
                Query q = conn->query();
                q << "INSERT INTO `se_fin_labels`(`cik`, `xbrl_id`, `label`) VALUES(" << (I64) recrow["cik"] << "," << quote << xbrl_id << ", " << quote << LabValue << ")";
                q.exec();
            }
        }
        xmlXPathFreeObject(xpathobj);
        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (const std::exception& ex)
    {
        if (xpathobj)
            xmlXPathFreeObject(xpathobj);
        if (xpathLabobj)
            xmlXPathFreeObject(xpathLabobj);
        BERR << "Error inserlabel: " << ex.what();
        return E_XMLParseError;
    }
    catch (...)
    {
        if (xpathobj)
            xmlXPathFreeObject(xpathobj);
        if (xpathLabobj)
            xmlXPathFreeObject(xpathLabobj);
        // LOGEP("InsertLabel: Failed Unknown error.");
        return E_UnknownError;
    }
#endif
}

#include <boost/date_time.hpp>
#include <Poco/String.h>

long unsigned int CProcessEdgarStatements::ProcessInstance(CSmartConnection& conn, mysqlpp::Row& row, const char* path, properties& ppt1)
{
    // simply go through all elements at top level under root node and assort them into above maps
    // LOGI(_logger, "Process Instance Started");
    xmlDocPtr docIns = ppt1.xbrlDocs[INSTANCE];
    if (!docIns)
        return E_XMLParseError;
#ifdef HASEXCHANDLER    

    try
    {
#endif
        auto rootNode = xmlDocGetRootElement(docIns);
        Contexts contexts;
        Units units;
        Elements els;
#ifdef HASEXCHANDLER    
        try
        {
#endif
            for (xmlNodePtr cn = rootNode->children; cn; cn = cn->next)
            {
                if (cn->type == XML_ELEMENT_NODE)
                {
                    //what is id ?
                    xmlChar* ptr = xmlGetProp(cn, BAD_CAST "id");
                    string tagName = (CSTR) cn->name;
                    if (ptr)
                    {
                        string szId = (CSTR) ptr;
                        if (tagName == "context")
                        {
                            contexts.insert(make_pair(szId, cn));
                        }
                        else if (tagName == "unit")
                        {
                            units.insert(make_pair(szId, cn));
                        }
                        else
                        {
                            els.insert(make_pair(tagName, cn));
                        }
                        xmlFree(ptr);
                    }
                    else
                    {
                        els.insert(make_pair(tagName, cn));
                    }
                }
            }
#ifdef HASEXCHANDLER    
        }
        catch (const std::exception& ex)
        {
            BERR << ex.what();
            return E_XMLParseError;
        }

        try
        {
#endif
            for (auto p : els)
            {
                string contextRef, unitRef;
                auto cr = xmlGetProp(p.second, BAD_CAST "contextRef");
                if (cr)
                {
                    contextRef = (CSTR) cr;
                    xmlFree(cr);
                }
                auto ur = xmlGetProp(p.second, BAD_CAST "unitRef");
                if (ur)
                {
                    unitRef = (CSTR) ur;
                    xmlFree(ur);
                }
                try
                {
                    // get context ref.
                    xmlNodePtr cptr = 0, uptr = 0;
                    if (cr)
                    {
                        auto itf1 = contexts.find(contextRef);
                        if (itf1 != contexts.end())
                            cptr = itf1->second;
                    }
                    if (ur)
                    {
                        auto itf2 = units.find(unitRef);
                        if (itf2 != units.end())
                            uptr = itf2->second;
                    }
                    SE_CODE retCode = InsertInstance(
                                                     conn,
                                                     p.second,
                                                     cptr,
                                                     uptr,
                                                     ppt1);
                    if (FAILED(retCode))
                    {
                        BERR << "InsertInstance Failed with error code " << retCode;
                    }
                }
                catch (...)
                {
                    throw;
                }

            }
#ifdef HASEXCHANDLER    
        }
        catch (...)
        {
            throw;
        }
#endif
        contexts.clear();
        units.clear();
        els.clear();
        // LOGI(_logger, "Process Instance End");
        return S_OK;
#ifdef HASEXCHANDLER    
       
    }
    catch (const std::exception& ex)
    {
        BERR << "Failed instance processing - " << ex.what();
        return E_XMLParseError;
    }
#endif
}

long unsigned int CProcessEdgarStatements::InsertInstance(CSmartConnection& conn, const xmlNodePtr elementNode, const xmlNodePtr contextNode, const xmlNodePtr unitNode, properties& ppt1)
{
    if (!elementNode || !elementNode->children || !elementNode->children->content)
    {
        // LOGWP("Insert node is null, or has no children or has no content");
        return S_OK;
    }

    xmlChar* pcontent = nullptr;
#ifdef HASEXCHANDLER    

    try
    {
#endif
        // if currency is not USD, skip this.
        pcontent = xmlNodeGetContent(elementNode);

        string value = (CSTR) pcontent;
        xmlFree(pcontent);
        pcontent = 0;
        string name = (CSTR) elementNode->name;
        string prefix = elementNode->ns ? (CSTR) elementNode->ns->prefix : "";
        string xbrltype;
        Query q = conn->query();
        q << "select xbrl_id,xbrl_type from se_xbrl_lookup ";
        q << "where xbrl_prefix = " << quote << prefix;
        q << " and xbrl_name = " << quote << name;
        BDBG << q.str();
        StoreQueryResult sqr = q.store();
        if (!sqr.size())
        {
            BWRN << "Xbrl Name " << prefix << ":" << name << " not found in xbrl_lookup table. Skipping...";
            return S_OK;
        }
        const Row& lkpRow = sqr[0];
        UID xbrlId = lkpRow[0];
        xbrltype = (CSTR) lkpRow[1];

        string monetaryvalue = "NULL", numericvalue = "NULL", stringvalue = "";

        if (value.empty())
            value = "NULL";
        if (xbrltype == "decimal")
            monetaryvalue = value;
        else if (xbrltype == "xbrli:nonZeroDecimal")
            monetaryvalue = value;
        else if (xbrltype == "xbrli:monetaryItemType")
            monetaryvalue = value;
        else if (xbrltype == "xbrli:sharesItemType")
            numericvalue = value;
        else
            stringvalue = value;

        if (xbrltype == "xbrli:monetaryItemType")
        {
            bool isUsd = false;
            if (unitNode)
            {
                for (xmlNodePtr ccur = unitNode->children; ccur; ccur = ccur->next)
                {
                    if (ccur->type == XML_ELEMENT_NODE)
                    {
                        xmlChar* content = xmlNodeGetContent(ccur);
                        if (content)
                        {
                            if (!xmlStrcmp(content, BAD_CAST "iso4217:USD") || !xmlStrcmp(content, BAD_CAST "USD"))
                            {
                                isUsd = true;
                            }
                            xmlFree(content);
                        }
                    }
                }
                if (!isUsd)
                {
                    // LOGI(_logger, "Skipping instance node " << elementNode->name << " because current is not USD");
                    return S_OK;
                }
            }
        }

        using namespace boost::gregorian;
        string startdate, enddate;
        bool isinstant = false;
        vector<UID> segments;
        vector<UID> dimensions;
        string identifier;

        if (contextNode)
        {
            // loop to get startdate, enddate or instant date
            xmlNodePtr ctxt;
            for (ctxt = contextNode->children; ctxt; ctxt = ctxt->next)
            {
                if (ctxt->type == XML_ELEMENT_NODE)
                {
#ifdef HASEXCHANDLER    
                    try
                    {
#endif
                        if (!xmlStrcmp(xmlStrstr(ctxt->name, BAD_CAST "period"), BAD_CAST "period"))
                        {
                            startdate = enddate = "";
                            // dig to see if it is instance or duration child
                            for (xmlNode* nextlevel = ctxt->children; nextlevel; nextlevel = nextlevel->next)
                            {
                                if (nextlevel->type == XML_ELEMENT_NODE)
                                {
                                    //LOGI(_logger, "Period child: " << (CSTR)nextlevel->name);
                                    isinstant = false;
                                    if (!xmlStrcmp(xmlStrstr(nextlevel->name, BAD_CAST "instant"), BAD_CAST "instant"))
                                    {
                                        pcontent = xmlNodeGetContent(nextlevel);
                                        isinstant = true;
                                        startdate = (CSTR) pcontent;
                                        xmlFree(pcontent);
                                    }
                                    else if (!xmlStrcmp(xmlStrstr(nextlevel->name, BAD_CAST "startDate"), BAD_CAST "startDate"))
                                    {
                                        pcontent = xmlNodeGetContent(nextlevel);
                                        startdate = (CSTR) pcontent;
                                        xmlFree(pcontent);
                                    }
                                    else if (!xmlStrcmp(xmlStrstr(nextlevel->name, BAD_CAST "endDate"), BAD_CAST "endDate"))
                                    {
                                        pcontent = xmlNodeGetContent(nextlevel);
                                        enddate = (CSTR) pcontent;
                                        xmlFree(pcontent);
                                    }
                                    pcontent = 0;
                                }
                            }
                        }
                        else if (!xmlStrcmp(xmlStrstr(ctxt->name, (xmlChar*) "entity"), (xmlChar*) "entity"))
                        {
                            /*
                            <xbrli:context id="I2014Q2_us-gaap_FairValueByFairValueHierarchyLevelAxis_us-gaap_FairValueInputsLevel3Member_us-gaap_FairValueByMeasurementFrequencyAxis_us-gaap_FairValueMeasurementsRecurringMember_us-gaap_MajorTypesOfDebtAndEquitySecuritiesAxis_us-gaap_MortgageBackedSecuritiesMember">
                                <xbrli:entity>
                                  <xbrli:identifier scheme="http://www.sec.gov/CIK">0000940944</xbrli:identifier>
                                  <xbrli:segment>
                                    <xbrldi:explicitMember dimension="us-gaap:FairValueByFairValueHierarchyLevelAxis">us-gaap:FairValueInputsLevel3Member</xbrldi:explicitMember>
                                    <xbrldi:explicitMember dimension="us-gaap:FairValueByMeasurementFrequencyAxis">us-gaap:FairValueMeasurementsRecurringMember</xbrldi:explicitMember>
                                    <xbrldi:explicitMember dimension="us-gaap:MajorTypesOfDebtAndEquitySecuritiesAxis">us-gaap:MortgageBackedSecuritiesMember</xbrldi:explicitMember>
                                  </xbrli:segment>
                                </xbrli:entity>
                                <xbrli:period>
                                  <xbrli:instant>2013-11-24</xbrli:instant>
                                </xbrli:period>
                              </xbrli:context>                                 
                             */
                            // get segment child
                            for (xmlNodePtr entityChild = ctxt->children; entityChild; entityChild = entityChild->next)
                            {
                                if (!xmlStrcmp(xmlStrstr(entityChild->name, (xmlChar*) "identifier"), BAD_CAST "identifier"))
                                {
                                    pcontent = xmlNodeGetContent(entityChild);
                                    identifier = (CSTR) pcontent;
                                    xmlFree(pcontent);
                                    pcontent = 0;
                                    continue;
                                }
                                else if (!xmlStrcmp(xmlStrstr(entityChild->name, BAD_CAST "segment"), BAD_CAST "segment"))
                                {
                                    for (xmlNodePtr n1 = entityChild->children; n1; n1 = n1->next)
                                    {
                                        if (xmlStrcmp(xmlStrstr(n1->name, BAD_CAST "explicitMember"), BAD_CAST "explicitMember"))
                                            continue;

                                        /*
                                         <xbrli:entity>
                                        <xbrli:identifier scheme="http://www.sec.gov/CIK">0000066740</xbrli:identifier>
                                        <xbrli:segment>
                                        <xbrldi:explicitMember dimension="us-gaap:PropertyPlantAndEquipmentByTypeAxis">us-gaap:LandMember</xbrldi:explicitMember>
                                        </xbrli:segment>
                                        </xbrli:entity>
                                         */

                                        pcontent = xmlNodeGetContent(n1);
                                        string segValue = (CSTR) pcontent;
                                        xmlFree(pcontent);
                                        pcontent = 0;
                                        string dimension;
                                        xmlAttrPtr attrs = n1->properties;
                                        if (attrs)
                                        {
                                            for (xmlAttrPtr attr = attrs; attr; attr = attr->next)
                                            {
                                                string attrName = (CSTR) attr->name;
                                                xmlNodePtr attrVal = attr->children;
                                                if (attrName == "dimension" && attrVal && attrVal->type == XML_TEXT_NODE)
                                                {
                                                    dimension = (CSTR) attrVal->content;
                                                    break;
                                                }
                                            }
                                        }

                                        trim(segValue);
                                        trim(dimension);
                                        if (!segValue.empty())
                                        {
                                            vector<string> sgsplit;
                                            split(sgsplit, segValue, is_any_of(":"));
                                            Query q = conn->query();
                                            q << "select xbrl_id from se_xbrl_lookup where xbrl_prefix = "
                                                    << quote << sgsplit[0] << " and xbrl_name = " << quote << sgsplit[1];
                                            StoreQueryResult qrxbrlid = q.store();
                                            if (qrxbrlid.size())
                                                segments.push_back(qrxbrlid[0][0]);
                                            if (!dimension.empty())
                                            {
                                                vector<string> dimsplit;
                                                split(dimsplit, dimension, is_any_of(":"));
                                                Query q = conn->query();
                                                q << "select xbrl_id from se_xbrl_lookup where xbrl_prefix = "
                                                        << quote << dimsplit[0] << " and xbrl_name = " << quote << dimsplit[1];
                                                StoreQueryResult qrxbrlid = q.store();
                                                if (qrxbrlid.size())
                                                    dimensions.push_back(qrxbrlid[0][0]);
                                            }
                                        }
                                        while (segments.size() != dimensions.size())
                                        {
                                            dimensions.push_back(0);
                                        }
                                    }
                                }
                            }
                        }
#ifdef HASEXCHANDLER    
                    }
                    catch (...)
                    {
                        if (pcontent)
                        {
                            xmlFree(pcontent);
                            pcontent = 0;
                        }
                        BERR << "Unknown exception thrown while processing individual period-context ref nodes.";
                        return E_XMLParseError;
                    }
#endif
                }
            }
        }// context node
        else
        {
            if (pcontent)
            {
                xmlFree(pcontent);
                pcontent = 0;
            }
            // LOGWP("No context node");
        }

        q = conn->query();
        q << "INSERT INTO " << _instTableName << " (`xbrl_id`, `rec_id`, `identifier`, `monetaryvalue`, `stringvalue`, `numericvalue` ";
        if (startdate != "")
            q << ", `startdate` ";
        if (enddate != "")
            q << ", `enddate` ";
        q << ", `contexttype` ";
        q << ", `seg_value` ";
        q << ") VALUES (";
        q << xbrlId << ", " << quote << ppt1.recid << ", ";
        q << identifier << ", " << monetaryvalue << ", ";
        q << quote << stringvalue << ", " << numericvalue;
        if (!startdate.empty())
            q << ", " << quote << startdate;
        if (!enddate.empty())
            q << ", " << quote << enddate;
        if (isinstant)
            q << ", " << 0;
        else
            q << ", " << 1;
        q << ", " << quote << "S";
        q << ")";
        BDBG << q.str();
        q.exec();
        // LOGI(_logger, "Data Insert into Statement Table End");
        UID instid = q.insert_id();
        // LOGI(_logger, "New instance_id " << instid);
        // LOGI(_logger, "Insert segments and dimensions - started");
        for (int i = 0; i != segments.size(); ++i)
        {
            Query q = conn->query();
            q << "insert ignore into " << _ctxtTableName;
            q << "(instance_id, segment_id, dimension_id) ";
            q << "values (";
            q << instid << ", " << segments[i] << ", " << dimensions[i];
            q << ")";
            q.execute();
        }
        return S_OK;
#ifdef HASEXCHANDLER    
    }
    catch (const std::exception& ex)
    {
        BERR << "Failed insert instance - " << ex.what();
        return E_XMLParseError;
    }
#endif
}

ErrorId CProcessEdgarStatements::ProcessCalculation(CSmartConnection& conn, mysqlpp::Row& row, CSTR path, properties &ppt1)
{
    xmlXPathContextPtr xpathContext = 0;
    xmlXPathObjectPtr xpathobj = 0;
#ifdef HASEXCHANDLER    
    try
    {
#endif
        //LOG4CXX_INFOINFO(_logger, "Process Calculation Started");
        xmlDocPtr docCal = ppt1.xbrlDocs[CALCULATION];
        XmlNs& nsCal = ppt1.nsDocs[CALCULATION];
        auto rootNode = xmlDocGetRootElement(docCal);
        xpathContext = xmlXPathNewContext(docCal);
        if (!xpathContext)
        {
            // LOGEP("Calculation xpath context creation failed.");
            return E_XPathContextCreate;
        }

        ErrorId retCode = RegisterNsInXpathContext(xpathContext, nsCal);
        if (retCode != S_Success)
        {
            xmlXPathFreeContext(xpathContext);
            // LOGEP("Namespace registration failed.");
            return E_XMLParseError;
        }

        string xpathstr = "/" + XPathNsExpr(GetDocDefaultNamespace(rootNode)) + "linkbase/" + XPathNsExpr(GetDocDefaultNamespace(rootNode)) + "roleRef";
        xpathobj = xmlXPathEvalExpression(BAD_CAST xpathstr.c_str(), xpathContext);
        if (!xpathobj)
        {
            xmlXPathFreeContext(xpathContext);
            BERR << "Xpath expression creation failed for " << xpathstr;
            return E_XMLParseError;
        }
        auto roleRefNodes = xpathobj->nodesetval;
        auto size = (roleRefNodes) ? roleRefNodes->nodeNr : 0;
        for (auto i = 0; i != size; ++i)
        {
            if (roleRefNodes->nodeTab[i]->type == XML_ELEMENT_NODE)
            {
                auto cur = roleRefNodes->nodeTab[i];
                retCode = ProcessCalRoleRef(conn, docCal, cur, row, path, ppt1, STM_Calculation, xpathContext);
                if (retCode != S_Success)
                {
                    xmlXPathFreeObject(xpathobj);
                    xmlXPathFreeContext(xpathContext);
                    // LOGEP("Roleref processing failed.");
                    return E_XMLParseError;
                }
            }
        }

        xmlXPathFreeObject(xpathobj);
        xmlXPathFreeContext(xpathContext);
        xpathobj = 0;
        xpathContext = 0;
        //LOG4CXX_INFOINFO(_logger, "Process Calculation  End");
        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (...)
    {
        if (xpathobj)
            xmlXPathFreeObject(xpathobj);
        if (xpathContext)
            xmlXPathFreeContext(xpathContext);
        // LOGEP("ProcessCalculation: Failed Unknown error.");
        return E_UnknownError;
    }
#endif
}

ErrorId CProcessEdgarStatements::ProcessCalRoleRef(CSmartConnection &conn, const xmlDocPtr doc, const xmlNodePtr roleRefNode, mysqlpp::Row& row, CSTR path, properties &ppt1, STM doctype, xmlXPathContextPtr CalContext)
{

    xmlXPathContextPtr xpathContext = 0;
    xmlXPathObjectPtr xpathobj = 0;
    xmlXPathObjectPtr CalLinkxpathobj = 0;
    xmlXPathObjectPtr usedxpathobj = 0;
    xmlXPathObjectPtr CalLocxpathobj = 0;
#ifdef HASEXCHANDLER    
    try
    {
#endif
        //LOG4CXX_INFOINFO(_logger, "Start Process Schema");
        xmlAttr* attr = NULL;
        xmlAttr* attrib = roleRefNode->properties;
        string RoleRefroleURI;
        string RoleReftype;
        string RoleRefhref;
        UID statement_id;
        string CalArcFrom;
        string CalArcto;
        string CalArcorder = "0";
        string CalLochref;
        I64 xbrl_id;
        I64 xbrl_parent_id;
        string CalArcpriority = "0";
        string CalArcdepth = "0";
        string CalArcweight = "0";
        for (attr = attrib; attr; attr = attr->next)
        {
            string attrName = (CSTR) attr->name;
            xmlNode* attrVal = attr->children;
            if (attrVal->type == XML_TEXT_NODE)
            {
                string sAttrVal = (CSTR) attrVal->content;
                if (attrName == "roleURI")
                {
                    RoleRefroleURI = sAttrVal;
                }
                if (attrName == "type")
                {
                    RoleReftype = sAttrVal;
                }
                if (attrName == "href")
                {
                    RoleRefhref = sAttrVal;
                    if (RoleRefhref.find(ppt1.XSD_Fname) == string::npos)
                    {
                        auto ret = CacheSchema(conn, RoleRefhref, ppt1);
                        if (ret != S_Success)
                        {
                            return E_XMLParseError;
                        }
                    }
                }
            }
        }
        BDBG << "Starting to get the details of fin report role type to store in the statements table.";
        BDBG << "Finding usedOn and description/title";

        // LOGI(_logger, "Preparing to write information about statement: " << RoleRefroleURI);
        vector<string> splits;
        split(splits, RoleRefhref, is_any_of("#"));

        string schemafileName = filesystem::path(splits[0]).filename().string();
        auto& schemaDoc = ((RoleRefhref.find(ppt1.XSD_Fname) == string::npos) ? ppt1.xbrlDocs[schemafileName] : ppt1.xbrlDocs[SCHEMA]);
        auto& schemaNs = ((RoleRefhref.find(ppt1.XSD_Fname) == string::npos) ? ppt1.nsDocs[schemafileName] : ppt1.nsDocs[SCHEMA]);

        //auto rootNode = xmlDocGetRootElement(schemaDoc);
        //string defNamespace = GetDocDefaultNamespace(rootNode);


        string defNamespace = GetDocDefaultNamespace(roleRefNode);
        string SchemaNamespace = "link";
        string roleTypexPath = "//" + XPathNsExpr(SchemaNamespace) + "roleType[@id=\"" + splits[1] + "\"]/" + XPathNsExpr(SchemaNamespace) + "definition";
        //string roleTypexPath = "//" + XPathNsExpr(defNamespace) + "roleType[@id='" + splits[1] + "']/"+XPathNsExpr(defNamespace) + "definition";
        xpathContext = xmlXPathNewContext(schemaDoc);
        if (!xpathContext)
        {
            // LOGEP("Schema xpath context creation failed.");
            return E_XPathContextCreate;
        }

        ErrorId retCode = RegisterNsInXpathContext(xpathContext, schemaNs);
        if (retCode != S_Success)
        {
            xmlXPathFreeContext(xpathContext);
            // LOGEP("Namespace registration failed.");
            return E_XMLParseError;
        }
        xpathobj = xmlXPathEvalExpression(BAD_CAST roleTypexPath.c_str(), xpathContext);
        if (!xpathobj)
        {
            auto rootNode = xmlDocGetRootElement(schemaDoc);
            string defSchemaNspace = GetDocDefaultNamespace(rootNode);
            roleTypexPath = "//" + XPathNsExpr(defSchemaNspace) + "roleType[@id=\"" + splits[1] + "\"]/" + XPathNsExpr(defSchemaNspace) + "definition";
            xpathobj = xmlXPathEvalExpression(BAD_CAST roleTypexPath.c_str(), xpathContext);
        }

        if (!xpathobj)
        {
            xmlXPathFreeContext(xpathContext);
            BERR << "Xpath expression creProcessCaation failed for " << roleTypexPath;
            return E_XMLParseError;
        }

        auto roleRefNodes = xpathobj->nodesetval;
        auto rsize = (roleRefNodes) ? roleRefNodes->nodeNr : 0;
        xmlNodePtr cur;
        if (rsize > 0)
        {
            cur = roleRefNodes->nodeTab[0];
            string SchemaDefinition = (CSTR) cur->children->content;

            string usedOnexPath = "//" + XPathNsExpr(SchemaNamespace) + "roleType[@id=\"" + splits[1] + "\"]/" + XPathNsExpr(SchemaNamespace) + "usedOn";
            usedxpathobj = xmlXPathEvalExpression(BAD_CAST usedOnexPath.c_str(), xpathContext);
            if (!usedxpathobj)
            {
                xmlXPathFreeContext(xpathContext);
                xmlXPathFreeObject(xpathobj);
                BERR << "Xpath expression creation failed for " << roleTypexPath;
                return E_XMLParseError;
            }
            auto usedRefNodes = usedxpathobj->nodesetval;
            auto Usize = (usedRefNodes) ? usedRefNodes->nodeNr : 0;
            I32 usedOnMask = 0;
            string UsedOn;
            for (auto i = 0; i != Usize; ++i)
            {
                auto UserCur = usedRefNodes->nodeTab[i];
                UsedOn = (CSTR) UserCur->children->content;
                if (UsedOn == "link:presentationLink")
                    usedOnMask |= STM_Presentation;
                if (UsedOn == "link:calculationLink")
                    usedOnMask |= STM_Calculation;
                if (UsedOn == "link:label")
                    usedOnMask |= STM_Label;
                if (UsedOn == "link:definition")
                    usedOnMask |= STM_Definition;

            }
#ifdef HASEXCHANDLER    
            try
            {
#endif
                //LOG4CXX_INFOINFO(_logger, "Data Insert into Statement Table Stared");
                {
                    mysqlpp::Row StatRow;
                    string rec_id = (CSTR) row[0];
                    string StatFind = "select statement_id from se_fin_statements where rec_id = " + rec_id + " and roleuri =\"" + RoleRefroleURI + "\"";
                    BDBG << StatFind;
                    auto qRes1 = conn->query(StatFind).store();
                    if (qRes1.size())
                    {
                        StatRow = qRes1[0];
                        statement_id = (UID) StatRow[0];
                    }
                    else
                    {
                        Query q = conn->query();
                        q << "INSERT INTO se_fin_statements (`rec_id`, `title`, `href`, `type`, `usedon`, `roleuri`)VALUES ("\
                            << (I32) row["rec_id"] << "," << quote << SchemaDefinition << "," << quote << RoleRefhref << "," << quote << RoleReftype << ","\
                            << usedOnMask << "," << quote << RoleRefroleURI << ")";
                        BDBG << q.str();
                        q.exec();
                        statement_id = q.insert_id();
                    }
                }
                /* staement id is obtained using insert_id method.
                {
                    string rec_id1 =(CSTR)row[0];
                    Query q = conn->query();
                    q <<"select * from se_fin_statements where rec_id = " << quote <<rec_id1 << " and roleuri ="<< quote <<RoleRefroleURI;
                    auto qRes =q.store();
                    if (qRes.size())
                    {
                        mysqlpp::Row& findRow = qRes[0];
                        statement_id = (string)findRow[0];
                    }
                }
                 * */
                string CalLinkXpath;
                if (defNamespace == "def")
                {
                    CalLinkXpath = "//" + XPathNsExpr(SchemaNamespace) + "calculationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + XPathNsExpr(SchemaNamespace) + "calculationArc";
                }
                else
                {
                    CalLinkXpath = "//" + XPathNsExpr(defNamespace) + "calculationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + XPathNsExpr(defNamespace) + "calculationArc";
                }

                CalLinkxpathobj = xmlXPathEvalExpression(BAD_CAST CalLinkXpath.c_str(), CalContext);
                if (defNamespace == "def" && !CalLinkxpathobj)
                {
                    CalLinkXpath = "//" + XPathNsExpr(defNamespace) + "calculationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + XPathNsExpr(defNamespace) + "calculationArc";
                    CalLinkxpathobj = xmlXPathEvalExpression(BAD_CAST CalLinkXpath.c_str(), CalContext);
                }

                if (!CalLinkxpathobj)
                {
                    BERR << "Xpath expression creation failed for " << CalLinkXpath;
                    xmlXPathFreeContext(xpathContext);
                    xmlXPathFreeObject(xpathobj);
                    xmlXPathFreeObject(usedxpathobj);
                    return E_XMLParseError;
                }
                auto CalLinkRefNodes = CalLinkxpathobj->nodesetval;
                auto Calsize = (CalLinkRefNodes) ? CalLinkRefNodes->nodeNr : 0;
                for (auto i = 0; i != Calsize; ++i)
                {
                    if (CalLinkRefNodes->nodeTab[i]->type == XML_ELEMENT_NODE)
                    {
                        auto CalLinkCur = CalLinkRefNodes->nodeTab[i];
                        xmlAttr* CalArcAttr = NULL;
                        xmlAttr* CalArcAttrib = CalLinkCur->properties;
                        for (CalArcAttr = CalArcAttrib; CalArcAttr; CalArcAttr = CalArcAttr->next)
                        {
                            string CalArcAttrName = (CSTR) CalArcAttr->name;
                            xmlNode* CalArcAttrVal = CalArcAttr->children;
                            if (CalArcAttrVal->type == XML_TEXT_NODE)
                            {
                                string SAttrVal = (CSTR) CalArcAttrVal->content;
                                if (CalArcAttrName == "from")
                                {
                                    CalArcFrom = SAttrVal;
                                    string CalLocXpath;
                                    if (defNamespace == "def")
                                    {
                                        CalLocXpath = "//" + XPathNsExpr(SchemaNamespace) + "calculationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(SchemaNamespace) + "loc[@xlink:label=\"" + CalArcFrom + "\"]";
                                    }
                                    else
                                    {
                                        CalLocXpath = "//" + XPathNsExpr(defNamespace) + "calculationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(defNamespace) + "loc[@xlink:label=\"" + CalArcFrom + "\"]";
                                    }

                                    CalLocxpathobj = xmlXPathEvalExpression(BAD_CAST CalLocXpath.c_str(), CalContext);
                                    if (defNamespace == "def" && !CalLocxpathobj)
                                    {
                                        CalLocXpath = "//" + XPathNsExpr(defNamespace) + "calculationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(defNamespace) + "loc[@xlink:label=\"" + CalArcFrom + "\"]";
                                        CalLocxpathobj = xmlXPathEvalExpression(BAD_CAST CalLocXpath.c_str(), CalContext);
                                    }
                                    if (!CalLocxpathobj)
                                    {

                                        xmlXPathFreeContext(xpathContext);
                                        xmlXPathFreeObject(xpathobj);
                                        xmlXPathFreeObject(usedxpathobj);
                                        xmlXPathFreeObject(CalLinkxpathobj);
                                        BERR << "Xpath expression creation failed for " << CalLocXpath;
                                        return E_XMLParseError;
                                    }
                                    auto CalLocRefNodes = CalLocxpathobj->nodesetval;
                                    xmlAttr* LocAttr = NULL;
                                    auto csize = (CalLocRefNodes) ? CalLocRefNodes->nodeNr : 0;
                                    if (csize > 0)
                                    {
                                        xmlAttr* CalLocAttrib = CalLocRefNodes->nodeTab[0]->properties;
                                        for (LocAttr = CalLocAttrib; LocAttr; LocAttr = LocAttr->next)
                                        {
                                            string CalLocAttrName = (CSTR) LocAttr->name;
                                            xmlNode* CalLocAttrVal = LocAttr->children;
                                            string CalLocsAttrVal = (CSTR) CalLocAttrVal->content;
                                            if (CalLocAttrName == "href")
                                            {
                                                CalLochref = CalLocsAttrVal;
                                                if (CalLochref.find(ppt1.XSD_Fname) == string::npos)
                                                {
                                                    auto ret = CacheSchema(conn, CalLochref, ppt1);
                                                    if (FAILED(ret))
                                                    {
                                                        xmlXPathFreeContext(xpathContext);
                                                        xmlXPathFreeObject(xpathobj);
                                                        xmlXPathFreeObject(usedxpathobj);
                                                        xmlXPathFreeObject(CalLinkxpathobj);
                                                        xmlXPathFreeObject(CalLocxpathobj);

                                                        // LOGEP("Error caching schema.");
                                                        return ret;
                                                    }
                                                }
                                                xbrl_parent_id = getFnElement(conn, row, path, ppt1, CalLochref.c_str(), roleRefNode);
                                                if (xbrl_parent_id == -1)
                                                {
                                                    // LOGEP("Error getting function element.");

                                                    xmlXPathFreeContext(xpathContext);
                                                    xmlXPathFreeObject(xpathobj);
                                                    xmlXPathFreeObject(usedxpathobj);
                                                    xmlXPathFreeObject(CalLinkxpathobj);
                                                    xmlXPathFreeObject(CalLocxpathobj);
                                                    return E_XMLParseError;
                                                }
                                            }

                                        }
                                    }
                                    xmlXPathFreeObject(CalLocxpathobj);
                                    CalLocxpathobj = 0;

                                } //end from

                                if (CalArcAttrName == "to")
                                {
                                    CalArcto = SAttrVal;
                                    string CalLocXpath;

                                    if (defNamespace == "def")
                                    {
                                        CalLocXpath = "//" + XPathNsExpr(SchemaNamespace) + "calculationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(SchemaNamespace) + "loc[@xlink:label=\"" + CalArcto + "\"]";
                                    }
                                    else
                                    {
                                        CalLocXpath = "//" + XPathNsExpr(defNamespace) + "calculationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(defNamespace) + "loc[@xlink:label=\"" + CalArcto + "\"]";
                                    }

                                    CalLocxpathobj = xmlXPathEvalExpression(BAD_CAST CalLocXpath.c_str(), CalContext);
                                    if (defNamespace == "def" && !CalLocxpathobj)
                                    {
                                        CalLocXpath = "//" + XPathNsExpr(defNamespace) + "calculationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(defNamespace) + "loc[@xlink:label=\"" + CalArcto + "\"]";
                                        CalLocxpathobj = xmlXPathEvalExpression(BAD_CAST CalLocXpath.c_str(), CalContext);
                                    }
                                    if (!CalLocxpathobj)
                                    {

                                        xmlXPathFreeContext(xpathContext);
                                        xmlXPathFreeObject(xpathobj);
                                        xmlXPathFreeObject(usedxpathobj);
                                        xmlXPathFreeObject(CalLinkxpathobj);
                                        BERR << "Xpath expression creation failed for " << CalLocXpath;
                                        return E_XMLParseError;
                                    }

                                    auto CalLocRefNodes = CalLocxpathobj->nodesetval;
                                    xmlAttr* LocAttr = NULL;
                                    auto csize = (CalLocRefNodes) ? CalLocRefNodes->nodeNr : 0;
                                    if (csize > 0)
                                    {
                                        xmlAttr* CalLocAttrib = CalLocRefNodes->nodeTab[0]->properties;
                                        for (LocAttr = CalLocAttrib; LocAttr; LocAttr = LocAttr->next)
                                        {
                                            string CalLocAttrName = (CSTR) LocAttr->name;
                                            xmlNode* CalLocAttrVal = LocAttr->children;
                                            string CalLocsAttrVal = (CSTR) CalLocAttrVal->content;
                                            if (CalLocAttrName == "href")
                                            {
                                                CalLochref = CalLocsAttrVal;
                                                if (CalLochref.find(ppt1.XSD_Fname) == string::npos)
                                                {
                                                    auto ret = CacheSchema(conn, CalLochref, ppt1);
                                                    if (FAILED(ret))
                                                    {

                                                        xmlXPathFreeContext(xpathContext);
                                                        xmlXPathFreeObject(xpathobj);
                                                        xmlXPathFreeObject(usedxpathobj);
                                                        xmlXPathFreeObject(CalLinkxpathobj);
                                                        xmlXPathFreeObject(CalLocxpathobj);

                                                        // LOGEP("Error caching schema.");
                                                        return ret;
                                                    }
                                                }
                                                xbrl_id = getFnElement(conn, row, path, ppt1, CalLochref.c_str(), roleRefNode);
                                                if (xbrl_id == -1)
                                                {
                                                    // LOGEP("Error getting function element.");
                                                    xmlXPathFreeContext(xpathContext);
                                                    xmlXPathFreeObject(xpathobj);
                                                    xmlXPathFreeObject(usedxpathobj);
                                                    xmlXPathFreeObject(CalLinkxpathobj);
                                                    xmlXPathFreeObject(CalLocxpathobj);
                                                    return E_XMLParseError;
                                                }
                                            }

                                        }
                                    }


                                    xmlXPathFreeObject(CalLocxpathobj);
                                    CalLocxpathobj = 0;

                                }//end to

                                if (CalArcAttrName == "order")
                                {
                                    CalArcorder = SAttrVal;
                                }
                                if (CalArcAttrName == "priority")
                                {
                                    CalArcpriority = SAttrVal;
                                }
                                if (CalArcAttrName == "depth")
                                {
                                    CalArcdepth = SAttrVal;
                                }
                                if (CalArcAttrName == "weight")
                                {
                                    CalArcweight = SAttrVal;
                                }
                            }
                        }

                    }
                    {

                        Query q = conn->query();
                        q << "INSERT INTO se_fin_calculation (`xbrl_id`, `xbrl_parent_id`, `priority`, `se_order`, `depth`, `weight`, `statement_id`) VALUES ("\
                        << xbrl_id << ", " << xbrl_parent_id << ", " << CalArcpriority << ", " << CalArcorder << "," << CalArcdepth << "," << CalArcweight \
                        << "," << statement_id << ")";
                        BDBG << q.str();
                        q.exec();

                    }

                }
                //LOG4CXX_INFOINFO(_logger, "Data Insert into Statement Table Complete");

                xmlXPathFreeObject(CalLinkxpathobj);
                CalLinkxpathobj = 0;

#ifdef HASEXCHANDLER    
            }
            catch (const std::exception& ex)
            {
                if (CalLinkxpathobj)
                {
                    xmlXPathFreeObject(CalLinkxpathobj);
                    CalLinkxpathobj = 0;
                }
                if (xpathContext)
                {
                    xmlXPathFreeContext(xpathContext);
                    xpathContext = 0;
                }
                if (xpathobj)
                {
                    xmlXPathFreeObject(xpathobj);
                    xpathobj = 0;
                }
                if (usedxpathobj)
                {
                    xmlXPathFreeObject(usedxpathobj);
                    usedxpathobj = 0;
                }
                BERR << "Error: " << ex.what();
                return E_XMLParseError;
            }
#endif
            xmlXPathFreeObject(usedxpathobj);
            usedxpathobj = nullptr;
        }

        if (CalLinkxpathobj)
        {
            xmlXPathFreeObject(CalLinkxpathobj);
            CalLinkxpathobj = 0;
        }
        if (xpathobj)
        {
            xmlXPathFreeObject(xpathobj);
            xpathobj = 0;
        }
        if (usedxpathobj)
        {
            xmlXPathFreeObject(usedxpathobj);
            usedxpathobj = 0;
        }
        if (xpathContext)
        {
            xmlXPathFreeContext(xpathContext);
            xpathContext = 0;
        }

        //LOG4CXX_INFOINFO(_logger, "End Process Schema");
        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (...)
    {
        if (CalLinkxpathobj)
        {
            xmlXPathFreeObject(CalLinkxpathobj);
            CalLinkxpathobj = 0;
        }
        if (xpathContext)
        {
            xmlXPathFreeContext(xpathContext);
            xpathContext = 0;
        }
        if (xpathobj)
        {
            xmlXPathFreeObject(xpathobj);
            xpathobj = 0;
        }
        if (usedxpathobj)
        {
            xmlXPathFreeObject(usedxpathobj);
            usedxpathobj = 0;
        }

        // LOGEP("ProcessCalcRoleRef: Failed Unknown error.");
        return E_UnknownError;
    }
#endif
}
//start presentation section

ErrorId CProcessEdgarStatements::ProcessPresentation(CSmartConnection& conn, mysqlpp::Row& row, CSTR path, properties &ppt1)
{
    xmlXPathContextPtr xpathContext = nullptr;
    xmlXPathObjectPtr xpathobj = nullptr;
#ifdef HASEXCHANDLER    
    try
    {
#endif
        BDBG << "ProcessPresentation started...";

        //LOG4CXX_INFOINFO(_logger, "Process Presentation Started");

        xmlDocPtr docPre = ppt1.xbrlDocs[PRESENTATION];
        XmlNs& nsPre = ppt1.nsDocs[PRESENTATION];
        auto rootNode = xmlDocGetRootElement(docPre);
        xpathContext = xmlXPathNewContext(docPre);
        if (!xpathContext)
        {
            // LOGEP("Presentation xpath context creation failed.");
            return E_XPathContextCreate;
        }

        ErrorId retCode = RegisterNsInXpathContext(xpathContext, nsPre);
        if (retCode != S_Success)
        {
            xmlXPathFreeContext(xpathContext);
            // LOGEP("Namespace registration failed.");
            return E_XMLParseError;
        }

        string xpathstr = "/" + XPathNsExpr(GetDocDefaultNamespace(rootNode)) + "linkbase/" + XPathNsExpr(GetDocDefaultNamespace(rootNode)) + "roleRef";
        xpathobj = xmlXPathEvalExpression(BAD_CAST xpathstr.c_str(), xpathContext);
        if (!xpathobj)
        {
            xmlXPathFreeContext(xpathContext);
            BERR << "Xpath expression creation failed for " << xpathstr;
            return E_XMLParseError;
        }
        auto roleRefNodes = xpathobj->nodesetval;
        auto size = (roleRefNodes) ? roleRefNodes->nodeNr : 0;
        for (auto i = 0; i != size; ++i)
        {
            if (roleRefNodes->nodeTab[i]->type == XML_ELEMENT_NODE)
            {
                auto cur = roleRefNodes->nodeTab[i];
                retCode = ProcessRoleRef(conn, docPre, cur, row, path, ppt1, STM_Presentation, xpathContext);
                if (FAILED(retCode))
                {
                    xmlXPathFreeContext(xpathContext);
                    xmlXPathFreeObject(xpathobj);
                    // LOGEP("Roleref processing failed.");
                    return E_XMLParseError;
                }
            }
        }

        xmlXPathFreeContext(xpathContext);
        xmlXPathFreeObject(xpathobj);
        xpathContext = 0;
        xpathobj = 0;
        //LOG4CXX_INFOINFO(_logger, "Process Presentation End");

        BDBG << "ProcessPresentation completed...";

        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (...)
    {
        if (xpathContext)
            xmlXPathFreeContext(xpathContext);
        if (xpathobj)
            xmlXPathFreeObject(xpathobj);
        // LOGEP("ProcessPresentation: Failed Unknown error.");
        return E_UnknownError;
    }
#endif
}

ErrorId CProcessEdgarStatements::ProcessRoleRef(CSmartConnection &conn, const xmlDocPtr doc, const xmlNodePtr roleRefNode, mysqlpp::Row& row, CSTR path, properties &ppt1, STM doctype, xmlXPathContextPtr preContext)
{
    xmlXPathContextPtr xpathContext = nullptr;
    xmlXPathObjectPtr xpathobj = nullptr;
    xmlXPathObjectPtr usedxpathobj = nullptr;
    xmlXPathObjectPtr preLinkxpathobj = nullptr;
    xmlXPathObjectPtr preLocxpathobj = nullptr;
#ifdef HASEXCHANDLER    

    try
    {
#endif
        //LOG4CXX_INFOINFO(_logger, "Start Process Schema");
        xmlAttr* attr = NULL;
        xmlAttr* attrib = roleRefNode->properties;
        string RoleRefroleURI;
        string RoleReftype;
        string RoleRefhref;
        string statement_id;
        string preArcFrom;
        string preArcto;
        string preArcorder = "0";
        string preLochref;
        I64 xbrl_id;
        I64 xbrl_parent_id;
        string preArcpriority = "0";
        string preArcdepth = "0";
        string use = "";
        string preferredLabel = "";
        string arcrole = "";
        for (attr = attrib; attr; attr = attr->next)
        {
            string attrName = (CSTR) attr->name;
            xmlNode* attrVal = attr->children;
            if (attrVal->type == XML_TEXT_NODE)
            {
                string sAttrVal = (CSTR) attrVal->content;
                if (attrName == "roleURI")
                {
                    RoleRefroleURI = sAttrVal;
                }
                if (attrName == "type")
                {
                    RoleReftype = sAttrVal;
                }
                if (attrName == "href")
                {
                    RoleRefhref = sAttrVal;
                    if (RoleRefhref.find(ppt1.XSD_Fname) == string::npos)
                    {
                        auto ret = CacheSchema(conn, RoleRefhref, ppt1);
                        if (FAILED(ret))
                        {
                            // LOGEP("Error caching schema.");
                            return ret;
                        }
                    }
                }
            }
        }

        BDBG << "Starting to get the details of fin report role type to store in the statements table.";
        BDBG << "Finding usedOn and description/title";
        vector<string> splits;
        split(splits, RoleRefhref, is_any_of("#"));

        string schemafileName = filesystem::path(splits[0]).filename().string();
        auto& schemaDoc = ((RoleRefhref.find(ppt1.XSD_Fname) == string::npos) ? ppt1.xbrlDocs[schemafileName] : ppt1.xbrlDocs[SCHEMA]);
        auto& schemaNs = ((RoleRefhref.find(ppt1.XSD_Fname) == string::npos) ? ppt1.nsDocs[schemafileName] : ppt1.nsDocs[SCHEMA]);

        string defNamespace = GetDocDefaultNamespace(roleRefNode);

        string SchemaNamespace = "link";

        string roleTypexPath = "//" + XPathNsExpr(SchemaNamespace) + "roleType[@id=\"" + splits[1] + "\"]/" + XPathNsExpr(SchemaNamespace) + "definition";

        xpathContext = xmlXPathNewContext(schemaDoc);
        if (!xpathContext)
        {
            // LOGEP("Schema xpath context creation failed.");
            return E_XPathContextCreate;
        }
        ErrorId retCode = RegisterNsInXpathContext(xpathContext, schemaNs);
        if (retCode != S_Success)
        {
            xmlXPathFreeContext(xpathContext);
            // LOGEP("Namespace registration failed.");
            return E_XMLParseError;
        }

        xpathobj = xmlXPathEvalExpression(BAD_CAST roleTypexPath.c_str(), xpathContext);
        if (!xpathobj)
        {
            // some companies forgot to specify link namespace in their schema documents. we will add it.
            // link="http://www.xbrl.org/2003/linkbase
            RegisterNamespace("link", "http://www.xbrl.org/2003/linkbase", schemaNs);

            xmlXPathFreeContext(xpathContext);
            xpathContext = nullptr;
            xpathContext = xmlXPathNewContext(schemaDoc);
            if (!xpathContext)
            {
                // LOGEP("Schema xpath context creation failed.");
                return E_XPathContextCreate;
            }

            retCode = RegisterNsInXpathContext(xpathContext, schemaNs);
            if (retCode != S_Success)
            {
                xmlXPathFreeContext(xpathContext);
                // LOGEP("Namespace registration failed.");
                return E_XMLParseError;
            }
            // try again.
            xpathobj = xmlXPathEvalExpression(BAD_CAST roleTypexPath.c_str(), xpathContext);
        }
        if (!xpathobj)
        {
            auto rootNode = xmlDocGetRootElement(schemaDoc);
            string defSchemaNspace = GetDocDefaultNamespace(rootNode);
            roleTypexPath = "//" + XPathNsExpr(defSchemaNspace) + "roleType[@id=\"" + splits[1] + "\"]/" + XPathNsExpr(defSchemaNspace) + "definition";
            xpathobj = xmlXPathEvalExpression(BAD_CAST roleTypexPath.c_str(), xpathContext);
        }

        if (!xpathobj)
        {
            xmlXPathFreeContext(xpathContext);
            BERR << "Xpath expression creation failed for " << roleTypexPath;
            return E_XMLParseError;
        }

        auto roleRefNodes = xpathobj->nodesetval;
        auto rsize = (roleRefNodes) ? roleRefNodes->nodeNr : 0;
        xmlNodePtr cur;
        if (rsize > 0)
        {
            cur = roleRefNodes->nodeTab[0];
            string SchemaDefinition = (CSTR) cur->children->content;

            string usedOnexPath = "//" + XPathNsExpr(SchemaNamespace) + "roleType[@id='" + splits[1] + "']/" + XPathNsExpr(SchemaNamespace) + "usedOn";
            usedxpathobj = xmlXPathEvalExpression(BAD_CAST usedOnexPath.c_str(), xpathContext);
            if (!usedxpathobj)
            {
                xmlXPathFreeContext(xpathContext);
                xmlXPathFreeObject(xpathobj);
                BERR << "Xpath expression creation failed for " << roleTypexPath;
                return E_XMLParseError;
            }
            auto usedRefNodes = usedxpathobj->nodesetval;
            auto Usize = (usedRefNodes) ? usedRefNodes->nodeNr : 0;
            I32 usedOnMask = 0;
            string UsedOn;
            for (auto i = 0; i != Usize; ++i)
            {
                auto UserCur = usedRefNodes->nodeTab[i];
                UsedOn = (CSTR) UserCur->children->content;
                if (UsedOn == "link:presentationLink")
                    usedOnMask |= STM_Presentation;
                if (UsedOn == "link:calculationLink")
                    usedOnMask |= STM_Calculation;
                if (UsedOn == "link:label")
                    usedOnMask |= STM_Label;
                if (UsedOn == "link:definition")
                    usedOnMask |= STM_Definition;

            }
#ifdef HASEXCHANDLER    
            try
            {
#endif
                //LOG4CXX_INFOINFO(_logger, "Data Insert into Statement Table Stared");
                {

                    Query q = conn->query();
                    q << "INSERT INTO se_fin_statements (`rec_id`, `title`, `href`, `type`, `usedon`, `roleuri`)VALUES ("\
                            << (I32) row["rec_id"] << "," << quote << SchemaDefinition << "," << quote << RoleRefhref << "," << quote << RoleReftype << ","\
                            << usedOnMask << "," << quote << RoleRefroleURI << ")";


                    q.exec();


                }
                {
                    string rec_id = (CSTR) row[0];
                    Query q = conn->query();
                    q << "select * from se_fin_statements where rec_id = " << quote << rec_id << " and roleuri =" << quote << RoleRefroleURI;
                    //string sqlFind = "select * from se_fin_statements where rec_id = " +rec_id + " and roleuri ='"+RoleRefroleURI+"'";
                    auto qRes = q.store();
                    if (qRes.size())
                    {
                        mysqlpp::Row findRow = qRes[0];
                        statement_id = (string) findRow[0];
                    }
                }
                string preLinkXpath;

                if (defNamespace == "def")
                {
                    preLinkXpath = "//" + XPathNsExpr(SchemaNamespace) +
                            "presentationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" +
                            XPathNsExpr(SchemaNamespace) + "presentationArc";
                }
                else
                {
                    preLinkXpath = "//" + XPathNsExpr(defNamespace) +
                            "presentationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" +
                            XPathNsExpr(defNamespace) + "presentationArc";
                }




                preLinkxpathobj = xmlXPathEvalExpression(BAD_CAST preLinkXpath.c_str(), preContext);
                if (defNamespace == "def" && !preLinkxpathobj)
                {
                    preLinkXpath = "//" + XPathNsExpr(defNamespace) +
                            "presentationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" +
                            XPathNsExpr(defNamespace) + "presentationArc";
                    preLinkxpathobj = xmlXPathEvalExpression(BAD_CAST preLinkXpath.c_str(), preContext);
                }

                if (!preLinkxpathobj)
                {
                    xmlXPathFreeContext(xpathContext);
                    xmlXPathFreeObject(xpathobj);
                    xmlXPathFreeObject(usedxpathobj);

                    BERR << "Xpath expression creation failed for " << preLinkXpath;
                    return E_XMLParseError;
                }

                auto preLinkRefNodes = preLinkxpathobj->nodesetval;
                auto presize = (preLinkRefNodes) ? preLinkRefNodes->nodeNr : 0;
                for (auto i = 0; i != presize; ++i)
                {
                    if (preLinkRefNodes->nodeTab[i]->type == XML_ELEMENT_NODE)
                    {
                        auto preLinkCur = preLinkRefNodes->nodeTab[i];
                        xmlAttr* preArcAttr = NULL;
                        xmlAttr* preArcAttrib = preLinkCur->properties;
                        for (preArcAttr = preArcAttrib; preArcAttr; preArcAttr = preArcAttr->next)
                        {
                            string preArcAttrName = (CSTR) preArcAttr->name;
                            xmlNode* preArcAttrVal = preArcAttr->children;
                            if (preArcAttrVal->type == XML_TEXT_NODE)
                            {
                                string SAttrVal = (CSTR) preArcAttrVal->content;
                                if (preArcAttrName == "from")
                                {
                                    preArcFrom = SAttrVal;
                                    string preLocXpath;
                                    if (defNamespace == "def")
                                    {
                                        preLocXpath = "//" + XPathNsExpr(SchemaNamespace) + "presentationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(SchemaNamespace) + "loc[@xlink:label=\"" + preArcFrom + "\"]";
                                    }
                                    else
                                    {
                                        preLocXpath = "//" + XPathNsExpr(defNamespace) + "presentationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(defNamespace) + "loc[@xlink:label=\"" + preArcFrom + "\"]";
                                    }

                                    preLocxpathobj = xmlXPathEvalExpression(BAD_CAST preLocXpath.c_str(), preContext);
                                    if (defNamespace == "def" && !preLocxpathobj)
                                    {
                                        preLocXpath = "//" + XPathNsExpr(defNamespace) + "presentationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(defNamespace) + "loc[@xlink:label=\"" + preArcFrom + "\"]";
                                        preLocxpathobj = xmlXPathEvalExpression(BAD_CAST preLocXpath.c_str(), preContext);
                                    }

                                    if (!preLocxpathobj)
                                    {

                                        xmlXPathFreeContext(xpathContext);
                                        xmlXPathFreeObject(xpathobj);
                                        xmlXPathFreeObject(usedxpathobj);
                                        xmlXPathFreeObject(preLinkxpathobj);

                                        BERR << "Xpath expression creation failed for " << preLocXpath;
                                        return E_XMLParseError;
                                    }

                                    auto preLocRefNodes = preLocxpathobj->nodesetval;
                                    xmlAttr* LocAttr = NULL;
                                    xmlNodePtr cur;
                                    auto psize = (preLocRefNodes) ? preLocRefNodes->nodeNr : 0;
                                    if (psize > 0)
                                    {
                                        xmlAttr* preLocAttrib = preLocRefNodes->nodeTab[0]->properties;
                                        for (LocAttr = preLocAttrib; LocAttr; LocAttr = LocAttr->next)
                                        {
                                            string preLocAttrName = (CSTR) LocAttr->name;
                                            xmlNode* preLocAttrVal = LocAttr->children;
                                            string preLocsAttrVal = (CSTR) preLocAttrVal->content;
                                            if (preLocAttrName == "href")
                                            {
                                                preLochref = preLocsAttrVal;
                                                if (preLochref.find(ppt1.XSD_Fname) == string::npos)
                                                {
                                                    auto ret = CacheSchema(conn, preLochref, ppt1);
                                                    if (FAILED(ret))
                                                    {
                                                        xmlXPathFreeContext(xpathContext);
                                                        xmlXPathFreeObject(xpathobj);
                                                        xmlXPathFreeObject(usedxpathobj);
                                                        xmlXPathFreeObject(preLinkxpathobj);
                                                        xmlXPathFreeObject(preLocxpathobj);

                                                        // LOGEP("Error caching schema.");
                                                        return ret;
                                                    }
                                                }

                                                xbrl_parent_id = getFnElement(conn, row, path, ppt1, preLochref.c_str(), roleRefNode);
                                                if (xbrl_parent_id == -1)
                                                {
                                                    // LOGEP("Error getting function element.");
                                                    xmlXPathFreeContext(xpathContext);
                                                    xmlXPathFreeObject(xpathobj);
                                                    xmlXPathFreeObject(usedxpathobj);
                                                    xmlXPathFreeObject(preLinkxpathobj);
                                                    xmlXPathFreeObject(preLocxpathobj);
                                                    return E_XMLParseError;
                                                }
                                            }

                                        }

                                    }

                                    xmlXPathFreeObject(preLocxpathobj);
                                    preLocxpathobj = nullptr;

                                } //end from

                                if (preArcAttrName == "to")
                                {
                                    preArcto = SAttrVal;
                                    string preLocXpath;
                                    if (defNamespace == "def")
                                    {
                                        preLocXpath = "//" + XPathNsExpr(SchemaNamespace) + "presentationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(SchemaNamespace) + "loc[@xlink:label='" + preArcto + "']";
                                    }
                                    else
                                    {
                                        preLocXpath = "//" + XPathNsExpr(defNamespace) + "presentationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(defNamespace) + "loc[@xlink:label='" + preArcto + "']";
                                    }


                                    preLocxpathobj = xmlXPathEvalExpression(BAD_CAST preLocXpath.c_str(), preContext);
                                    if (defNamespace == "def" && !preLocxpathobj)
                                    {
                                        preLocXpath = "//" + XPathNsExpr(defNamespace) + "presentationLink[@xlink:role=\"" + RoleRefroleURI + "\"]/" + \
 XPathNsExpr(defNamespace) + "loc[@xlink:label='" + preArcto + "']";
                                        preLocxpathobj = xmlXPathEvalExpression(BAD_CAST preLocXpath.c_str(), preContext);
                                    }
                                    if (!preLocxpathobj)
                                    {
                                        xmlXPathFreeContext(xpathContext);
                                        xmlXPathFreeObject(xpathobj);
                                        xmlXPathFreeObject(usedxpathobj);
                                        xmlXPathFreeObject(preLinkxpathobj);
                                        BERR << "Xpath expression creation failed for " << preLocXpath;
                                        return E_XMLParseError;
                                    }

                                    auto preLocRefNodes = preLocxpathobj->nodesetval;
                                    xmlAttr* LocAttr = NULL;
                                    auto psize = (preLocRefNodes) ? preLocRefNodes->nodeNr : 0;
                                    if (psize > 0)
                                    {
                                        xmlAttr* preLocAttrib = preLocRefNodes->nodeTab[0]->properties;
                                        for (LocAttr = preLocAttrib; LocAttr; LocAttr = LocAttr->next)
                                        {
                                            string preLocAttrName = (CSTR) LocAttr->name;
                                            xmlNode* preLocAttrVal = LocAttr->children;
                                            string preLocsAttrVal = (CSTR) preLocAttrVal->content;
                                            if (preLocAttrName == "href")
                                            {
                                                preLochref = preLocsAttrVal;
                                                if (preLochref.find(ppt1.XSD_Fname) == string::npos)
                                                {
                                                    auto ret = CacheSchema(conn, preLochref, ppt1);
                                                    if (FAILED(ret))
                                                    {
                                                        xmlXPathFreeContext(xpathContext);
                                                        xmlXPathFreeObject(xpathobj);
                                                        xmlXPathFreeObject(usedxpathobj);
                                                        xmlXPathFreeObject(preLinkxpathobj);
                                                        xmlXPathFreeObject(preLocxpathobj);

                                                        // LOGEP("Error caching schema.");
                                                        return ret;
                                                    }

                                                }
                                                xbrl_id = getFnElement(conn, row, path, ppt1, preLochref.c_str(), roleRefNode);
                                                if (xbrl_id == -1)
                                                {
                                                    // LOGEP("Error getting function element.");
                                                    xmlXPathFreeContext(xpathContext);
                                                    xmlXPathFreeObject(xpathobj);
                                                    xmlXPathFreeObject(usedxpathobj);
                                                    xmlXPathFreeObject(preLinkxpathobj);
                                                    xmlXPathFreeObject(preLocxpathobj);
                                                    return E_XMLParseError;
                                                }

                                            }

                                        }
                                    }

                                    xmlXPathFreeObject(preLocxpathobj);
                                    preLocxpathobj = nullptr;

                                }//end to
                                if (preArcAttrName == "order")
                                {
                                    preArcorder = SAttrVal;
                                }
                                if (preArcAttrName == "priority")
                                {
                                    preArcpriority = SAttrVal;
                                }
                                if (preArcAttrName == "depth")
                                {
                                    preArcdepth = SAttrVal;
                                }
                                if (preArcAttrName == "preferredLabel")
                                {
                                    preferredLabel = SAttrVal;
                                    preferredLabel = preferredLabel.substr(preferredLabel.find_last_of("/") + 1);
                                }

                                if (preArcAttrName == "use")
                                {
                                    use = SAttrVal;
                                }

                                if (preArcAttrName == "arcrole")
                                {
                                    arcrole = SAttrVal;
                                    arcrole = arcrole.substr(arcrole.find_last_of("/") + 1);
                                }
                            }
                        }

                    }
                    {
                        //LOG4CXX_INFOINFO(_logger, "Data Insert into Presentation Table Started"); 
                        Query q = conn->query();
                        q << "INSERT INTO se_fin_presentation (`xbrl_id`, `xbrl_parent_id`, `priority`, `se_order`, `depth`, `use`, `preferredlabel`, `arcrole`, `statement_id`) VALUES ("\
                        << quote << xbrl_id << ", " << quote << xbrl_parent_id << ", " << quote << preArcpriority << ", " << quote << preArcorder << "," << quote << preArcdepth
                                << "," << quote << use << "," << quote << preferredLabel << "," << quote << arcrole << "," << quote << statement_id << ")";
                        q.exec();
                    }


                    /*
                     * 1. retrieve newly created statement id from the table
                     * 2. using the presentation context, construct an xpath expression //link:presentationLink[@xlink:role='xxx']/presentationArc
                     * 3. Now you have all the presentationArcs for this presentationlink
                     * 4. Now loop on it, create from to list, 
                     * 5. go to schema, get the element details
                     */

                }

                //LOG4CXX_INFOINFO(_logger, "Data Insert into Statement Table Complete");
                xmlXPathFreeObject(preLinkxpathobj);
                preLinkxpathobj = nullptr;

#ifdef HASEXCHANDLER    
            }
            catch (const std::exception& ex)
            {
                if (xpathContext)
                {
                    xmlXPathFreeContext(xpathContext);
                    xpathContext = nullptr;
                }
                if (xpathobj)
                {
                    xmlXPathFreeObject(xpathobj);
                    xpathobj = nullptr;
                }
                if (usedxpathobj)
                {
                    xmlXPathFreeObject(usedxpathobj);
                    usedxpathobj = nullptr;
                }
                if (preLinkxpathobj)
                {
                    xmlXPathFreeObject(preLinkxpathobj);
                    preLinkxpathobj = nullptr;
                }
                if (preLocxpathobj)
                {
                    xmlXPathFreeObject(preLocxpathobj);
                    preLocxpathobj = nullptr;
                }
                BERR << "Error: " << ex.what();
                return E_XMLParseError;
            }
#endif
        }

        if (xpathContext)
        {
            xmlXPathFreeContext(xpathContext);
            xpathContext = nullptr;
        }
        if (xpathobj)
        {
            xmlXPathFreeObject(xpathobj);
            xpathobj = nullptr;
        }
        if (usedxpathobj)
        {
            xmlXPathFreeObject(usedxpathobj);
            usedxpathobj = nullptr;
        }
        if (preLinkxpathobj)
        {
            xmlXPathFreeObject(preLinkxpathobj);
            preLinkxpathobj = nullptr;
        }
        if (preLocxpathobj)
        {
            xmlXPathFreeObject(preLocxpathobj);
            preLocxpathobj = nullptr;
        }

        //LOG4CXX_INFOINFO(_logger, "End Process Schema");
        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (...)
    {
        if (xpathContext)
        {
            xmlXPathFreeContext(xpathContext);
            xpathContext = nullptr;
        }
        if (xpathobj)
        {
            xmlXPathFreeObject(xpathobj);
            xpathobj = nullptr;
        }
        if (usedxpathobj)
        {
            xmlXPathFreeObject(usedxpathobj);
            usedxpathobj = nullptr;
        }
        if (preLinkxpathobj)
        {
            xmlXPathFreeObject(preLinkxpathobj);
            preLinkxpathobj = nullptr;
        }
        if (preLocxpathobj)
        {
            xmlXPathFreeObject(preLocxpathobj);
            preLocxpathobj = nullptr;
        }

        // LOGEP("ProcessRoleRef: Failed Unknown error.");
        return E_UnknownError;
    }
#endif
}

I64 CProcessEdgarStatements::getFnElement(CSmartConnection &conn, mysqlpp::Row& row, CSTR path, properties &ppt1, CSTR href, const xmlNodePtr roleRefNode)
{
    xmlXPathContextPtr xpathContext = nullptr;
    xmlXPathObjectPtr Elexpathobj = nullptr;
#ifdef HASEXCHANDLER    
    try
    {
#endif
        BDBG << "Starting to get the details of element name in the look table.";
        string prefix;
        vector<string> splits;
        split(splits, href, is_any_of("#"));
        string id = filesystem::path(splits[1]).filename().string();
        string schemafileName = filesystem::path(splits[0]).filename().string();
        string Elhref = (string) href;
        auto& schemaDoc = ((Elhref.find(ppt1.XSD_Fname) == string::npos) ? ppt1.xbrlDocs[schemafileName] : ppt1.xbrlDocs[SCHEMA]);
        auto& schemaNs = ((Elhref.find(ppt1.XSD_Fname) == string::npos) ? ppt1.nsDocs[schemafileName] : ppt1.nsDocs[SCHEMA]);
        auto rootNode = xmlDocGetRootElement(schemaDoc);
        string defNamespace = GetDocDefaultNamespace(rootNode);
        xpathContext = xmlXPathNewContext(schemaDoc);
        if (!xpathContext)
        {
            // LOGEP("Schema xpath context for element name creation failed.");
            return -1;
        }

        ErrorId retCode = RegisterNsInXpathContext(xpathContext, schemaNs);
        if (retCode != S_Success)
        {
            xmlXPathFreeContext(xpathContext);
            // LOGEP("Namespace registration for element name failed.");
            return -1;
        }

        string ElexPath = "//" + XPathNsExpr(defNamespace) + "element[@id='" + id + "']";
        Elexpathobj = xmlXPathEvalExpression(BAD_CAST ElexPath.c_str(), xpathContext);
        if (!Elexpathobj)
        {
            xmlXPathFreeContext(xpathContext);
            BERR << "Xpath expression creation failed for " << ElexPath;
            return -1;
        }

        auto EleRefNodes = Elexpathobj->nodesetval;

        string EleName;
        string Eletype;
        string Elesub;
        string EleBalance;
        string EleperiodType;
        I16 EleAbstract;
        string Eleenum;
        xmlAttr* EleAttr = NULL;
        I64 xbrl_id;
        auto esize = (EleRefNodes) ? EleRefNodes->nodeNr : 0;

        if (esize > 0)
        {
            xmlAttr* EleAttrib = EleRefNodes->nodeTab[0]->properties;
            for (EleAttr = EleAttrib; EleAttr; EleAttr = EleAttr->next)
            {
                string EleAttrName = (CSTR) EleAttr->name;
                xmlNode* EleAttrVal = EleAttr->children;
                string ElesAttrVal = (CSTR) EleAttrVal->content;
                if (EleAttrName == "name")
                {
                    EleName = ElesAttrVal;
                }
                else if (EleAttrName == "type")
                {
                    Eletype = ElesAttrVal;
                }
                else if (EleAttrName == "substitutionGroup")
                {
                    Elesub = ElesAttrVal;
                }
                else if (EleAttrName == "balance")
                {
                    EleBalance = ElesAttrVal;
                }
                else if (EleAttrName == "periodType")
                {
                    EleperiodType = ElesAttrVal;
                }
                else if (EleAttrName == "abstract")
                {
                    if (ElesAttrVal == "true")
                        EleAbstract = 1;
                    else
                        EleAbstract = 0;
                }
                else if (EleAttrName == "enumerations")
                {
                    Eleenum = ElesAttrVal;
                }
                else if (EleAttrName == "id")
                {
                    string Eleid = ElesAttrVal;
                    vector<string> splitsId;
                    split(splitsId, Eleid, is_any_of("_"));
                    prefix = filesystem::path(splitsId[0]).filename().string();

                }
            }
        }
#ifdef HASEXCHANDLER    
        try
        {
#endif
            //LOG4CXX_INFOINFO(_logger, "Data Insert into Statement Table Stared");
            //Guard<Mutex> g(_mutex);
            mysqlpp::Row findRow;
            string sqlFind = "select * from se_xbrl_lookup where xbrl_prefix = '" + prefix + "' and xbrl_name ='" + EleName + "'";
            auto qRes = conn->query(sqlFind).store();
            if (qRes.size())
                findRow = qRes[0];

            if (findRow)
            {
                xbrl_id = (I64) findRow[0];
                {
                    Query q = conn->query();
                    q << "UPDATE `se_xbrl_lookup` SET `xbrl_href` =" << quote << href << " WHERE xbrl_id = " << xbrl_id;
                    q.exec();
                }
            }
            else
            {
                {
                    Query q = conn->query();
                    q << "INSERT INTO se_xbrl_lookup ( `xbrl_src_id`, `xbrl_prefix`, `xbrl_name`, `xbrl_type`, `xbrl_enumerations`, `xbrl_substitutiongroup`, ";
                    q << "`xbrl_balance`, `xbrl_PeriodType`, `xbrl_IsAbstract`,`xbrl_href`) VALUES (";
                    q << ppt1.cikValue << ", " << quote << prefix << "," << quote << EleName << "," << quote << Eletype << "," << quote << Eleenum;
                    q << ", " << quote << Elesub << "," << quote << EleBalance << "," << quote << EleperiodType << "," << EleAbstract << "," << quote << href << ")";

                    BDBG << q.str();

                    q.exec();


                    xbrl_id = q.insert_id();

                }
                /* insert_id method above.
                {
                    string sqlSearch = "select * from se_xbrl_lookup where xbrl_prefix = '" +prefix + "' and xbrl_name ='"+EleName+"'";
                    auto qRSearch = conn->query(sqlSearch).store();
                    if (qRSearch.size())
                    {
                        mysqlpp::Row SearchRow = qRSearch[0];
                        xbrl_id = (I64)SearchRow[0];
                    }
                }
                 * */
            }
#ifdef HASEXCHANDLER    
        }
        catch (const std::exception& ex)
        {
            if (xpathContext)
            {
                xmlXPathFreeContext(xpathContext);
                xpathContext = nullptr;
            }
            if (Elexpathobj)
            {
                xmlXPathFreeObject(Elexpathobj);
                Elexpathobj = nullptr;
            }


            BERR << "Error: " << ex.what();
            return -1;
        }
#endif
        if (xpathContext)
        {
            xmlXPathFreeContext(xpathContext);
            xpathContext = nullptr;
        }
        if (Elexpathobj)
        {
            xmlXPathFreeObject(Elexpathobj);
            Elexpathobj = nullptr;
        }

        BDBG << "End to get the details of element name in the look table.";
        return xbrl_id;
#ifdef HASEXCHANDLER    
    }
    catch (...)
    {
        if (xpathContext)
        {
            xmlXPathFreeContext(xpathContext);
            xpathContext = nullptr;
        }
        if (Elexpathobj)
        {
            xmlXPathFreeObject(Elexpathobj);
            Elexpathobj = nullptr;
        }

        // LOGEP("getFnelement: Failed Unknown error.");
        return E_UnknownError;
    }
#endif
}

string CProcessEdgarStatements::GetDocDefaultNamespace(const xmlNodePtr rootNode)
{
    if (rootNode->ns && rootNode->ns->prefix)
        return (string) (CSTR) rootNode->ns->prefix;
    return "def";
}

ErrorId CProcessEdgarStatements::RegisterNsInXpathContext(xmlXPathContextPtr context, const XmlNs& nsdoc)
{
    for (auto it : nsdoc)
    {
        if (-1 == xmlXPathRegisterNs(context, BAD_CAST it.first.c_str(), BAD_CAST it.second.c_str()))
        {
            // LOGEP("Presentation xpath namespace registration failed.");
            return E_XPathNsRegister;
        }
    }
    return S_Success;
}

long unsigned int CProcessEdgarStatements::DownloadUsGaapSchemas()
{
    //http://xbrl.fasb.org/us-gaap/2011/elts/us-gaap-2011-01-31.xsd
    //http://xbrl.fasb.org/us-gaap/2012/elts/us-gaap-2012-01-31.xsd
    //http://xbrl.fasb.org/us-gaap/2013/elts/us-gaap-2013-01-31.xsd
    //http://xbrl.fasb.org/us-gaap/2014/elts/us-gaap-2014-01-31.xsd
    //http://xbrl.fasb.org/us-gaap/2014/elts/us-gaap-2014-01-31.xsd
    //http://xbrl.fasb.org/us-gaap/2015/elts/us-gaap-2015-01-31.xsd

    //http://xbrl.fasb.org/us-gaap/2014/elts/us-roles-2014-01-31.xsd    
    //http://xbrl.fasb.org/us-gaap/2011/elts/us-types-2011-01-31.xsd

    const auto& now = CDate::Now();
    string url = "http://xbrl.fasb.org/us-gaap/";
    CHttpClient httpClient;
    httpClient.EnableFollowLocation();
    httpClient.EnableHostNameVerification();
    httpClient.EnablePeerVerification();
    for (int yr = 2011; yr <= now.get_Year(); yr++)
    {
        ostringstream ostr, ostrfilename;
        ostr << url << yr << "/" << "elts/us-gaap-" << yr << "-01-31.xsd";
        ostrfilename << _cacheSchemaPath << "us-gaap-" << yr << "-01-31.xsd";
        BDBG << ostr.str();
        BDBG << ostrfilename.str();
        auto bfileExists = filesystem::exists(ostrfilename.str().c_str());
        if (!bfileExists || filesystem::is_empty(ostrfilename.str().c_str()))
        {
            auto status = httpClient.GetFile(ostr.str().c_str(), ostrfilename.str().c_str());
            if (filesystem::is_empty(ostrfilename.str().c_str()))
                filesystem::remove(ostrfilename.str().c_str());
        }
        ostr.str("");
        ostrfilename.str("");
        ostr << url << yr << "/" << "elts/us-types-" << yr << "-01-31.xsd";
        ostrfilename << _cacheSchemaPath << "us-types-" << yr << "-01-31.xsd";
        BDBG << ostr.str();
        BDBG << ostrfilename.str();
        bfileExists = filesystem::exists(ostrfilename.str().c_str());
        if (!bfileExists || filesystem::is_empty(ostrfilename.str().c_str()))
        {
            auto status = httpClient.GetFile(ostr.str().c_str(), ostrfilename.str().c_str());
            if (filesystem::is_empty(ostrfilename.str().c_str()))
                filesystem::remove(ostrfilename.str().c_str());
        }
        ostr.str("");
        ostrfilename.str("");
        ostr << url << yr << "/" << "elts/us-roles-" << yr << "-01-31.xsd";
        ostrfilename << _cacheSchemaPath << "us-roles-" << yr << "-01-31.xsd";
        BDBG << ostr.str();
        BDBG << ostrfilename.str();
        bfileExists = filesystem::exists(ostrfilename.str().c_str());
        if (!bfileExists || filesystem::is_empty(ostrfilename.str().c_str()))
        {
            auto status = httpClient.GetFile(ostr.str().c_str(), ostrfilename.str().c_str());
            if (filesystem::is_empty(ostrfilename.str().c_str()))
                filesystem::remove(ostrfilename.str().c_str());
        }
    }
}

ErrorId CProcessEdgarStatements::CacheSchema(CSmartConnection &conn, string& url, properties &ppt1)
{

    xmlDocPtr doc = nullptr;
    xmlXPathContextPtr xpathContext = nullptr;
    xmlXPathObjectPtr Impxpathobj = nullptr;
    xmlXPathObjectPtr Elexpathobj = nullptr;
#ifdef HASEXCHANDLER    

    try
    {
#endif
        // LOGI(_logger, "Processing/caching schema file " << url);

        if (url.empty()) return S_Success;

        vector<string> splits;
        split(splits, url, is_any_of("#"));
        // we got the url. now we need the file name.
        string fileName = filesystem::path(splits[0]).filename().string();

        if (fileName == "us-gaap-2013-01-31.xsd" || fileName == "us-gaap-2012-01-31.xsd")
        {
            BDBG << "Break here.";
        }

        // now check for this file.
        auto mapfind = ppt1.xbrlDocs.find(fileName);
        if (mapfind == ppt1.xbrlDocs.end() || mapfind->second == nullptr)
        {
            bool bfileExists = false;
            bfileExists = filesystem::exists(_cacheSchemaPath + fileName);
            if (bfileExists)
            {
                doc = xmlReadFile((STR) (_cacheSchemaPath + fileName).c_str(), NULL, XML_PARSE_IGNORE_ENC);
                if (doc)
                {
                    XMLRefItem p(fileName, doc);
                    xbrlRefs.insert(p);
                    ppt1.xbrlDocs[fileName] = doc;
                    ppt1.nsDocs[fileName] = XmlNs();
                    RegisterNamespaces(ppt1.xbrlDocs[fileName], ppt1.nsDocs[fileName]);
                }
            }

            if (!bfileExists || doc == nullptr)
            {
                // download
                string sfileName = _cacheSchemaPath + fileName;
                string urlPath;
                if (url.find("http") == 0)
                {
                    urlPath = url;
                    replaceInPlace(urlPath, (const string) "http", (const string) "https", 0);
                }
                else
                {
                    urlPath = (string) edgarUrl + "/" + splits[0];
                }
                CHttpClient httpClient;
                httpClient.EnableFollowLocation();
                httpClient.EnableHostNameVerification();
                httpClient.EnablePeerVerification();
                auto status = httpClient.GetFile(urlPath.c_str(), sfileName.c_str());
                if (FAILED(status))
                {
                    BERR << "Failed to download file from url " << urlPath;
                    return E_HttpError;
                }
                doc = xmlReadFile((STR) sfileName.c_str(), NULL, XML_PARSE_IGNORE_ENC);

                if (doc == nullptr)
                {
                    // try http
                    replaceInPlace(urlPath, (const string) "https", (const string) "http");
                    httpClient.GetFile(urlPath.c_str(), sfileName.c_str());
                    doc = xmlReadFile((STR) sfileName.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                }

                if (doc == nullptr)
                {
                    BERR << "document " << urlPath << " cannot be loaded.";
                    return E_HttpError;
                }

                if (doc)
                {
                    XMLRefItem p(fileName, doc);
                    xbrlRefs.insert(p);
                    string importedFile = _cacheSchemaPath + fileName;
                    ppt1.xbrlDocs[fileName] = doc;
                    ppt1.nsDocs[fileName] = XmlNs();
                    RegisterNamespaces(ppt1.xbrlDocs[fileName], ppt1.nsDocs[fileName]);

                    //xmlSaveFile((CSTR) importedFile.c_str(), doc);

                    auto rootNode = xmlDocGetRootElement(doc);
                    string defNamespace = GetDocDefaultNamespace(rootNode);
                    xpathContext = xmlXPathNewContext(doc);
                    if (!xpathContext)
                    {
                        // LOGEP("Schema xpath context for element name creation failed.");
                        return E_XMLParseError;
                    }

                    ErrorId retCode = RegisterNsInXpathContext(xpathContext, ppt1.nsDocs[fileName]);
                    if (retCode != S_Success)
                    {
                        xmlXPathFreeContext(xpathContext);
                        // LOGEP("Namespace registration for element name failed.");
                        return E_XMLParseError;
                    }

                    string ImpxPath = "//" + XPathNsExpr(defNamespace) + "import";
                    Impxpathobj = xmlXPathEvalExpression(BAD_CAST ImpxPath.c_str(), xpathContext);
                    if (!Impxpathobj)
                    {
                        xmlXPathFreeContext(xpathContext);
                        BERR << "Xpath expression creation failed for " << ImpxPath;
                        return E_XMLParseError;
                    }

                    auto ImpRefNodes = Impxpathobj->nodesetval;
                    auto Impsize = (ImpRefNodes) ? ImpRefNodes->nodeNr : 0;

                    for (auto i = 0; i != Impsize; ++i)
                    {
                        if (ImpRefNodes->nodeTab[i]->type == XML_ELEMENT_NODE)
                        {
                            xmlAttr* ImpAttr = NULL;
                            xmlAttr* ImpAttrib = ImpRefNodes->nodeTab[i]->properties;
                            for (ImpAttr = ImpAttrib; ImpAttr; ImpAttr = ImpAttr->next)
                            {
                                string ImpAttrName = (CSTR) ImpAttr->name;
                                xmlNode* ImpAttrVal = ImpAttr->children;
                                string ImpsAttrVal = (CSTR) ImpAttrVal->content;
                                if (ImpAttrName == "schemaLocation")
                                {
                                    string importFile = ImpsAttrVal;
                                    if (importFile.find("/") != string::npos)
                                    {
                                        auto ret = CacheSchema(conn, importFile, ppt1);
                                        if (FAILED(ret))
                                        {
                                            xmlXPathFreeContext(xpathContext);
                                            xmlXPathFreeObject(Impxpathobj);
                                            // LOGEP("Error caching schema.");
                                            return ret;
                                        }

                                    }

                                }
                            }

                        }
                    }

                    string EleName;
                    string Eletype;
                    string Elesub;
                    string EleBalance;
                    string EleperiodType;
                    I16 EleAbstract;
                    string Eleenum;
                    I64 xbrl_id;
                    string prefix;
                    string ElexPath = "//" + XPathNsExpr(defNamespace) + "element";

                    Elexpathobj = xmlXPathEvalExpression(BAD_CAST ElexPath.c_str(), xpathContext);
                    if (!Elexpathobj)
                    {
                        xmlXPathFreeContext(xpathContext);
                        xmlXPathFreeObject(Impxpathobj);
                        BERR << "Xpath expression creation failed for " << ElexPath;
                        return E_XMLParseError;
                    }

                    auto EleRefNodes = Elexpathobj->nodesetval;
                    auto size = (EleRefNodes) ? EleRefNodes->nodeNr : 0;
                    // LOGI(_logger, "Start Data insertion into lookup table" );
                    for (auto i = 0; i != size; ++i)
                    {
                        if (EleRefNodes->nodeTab[i]->type == XML_ELEMENT_NODE)
                        {
                            xmlAttr* EleAttr = NULL;
                            xmlAttr* EleAttrib = EleRefNodes->nodeTab[i]->properties;
                            for (EleAttr = EleAttrib; EleAttr; EleAttr = EleAttr->next)
                            {
                                string EleAttrName = (CSTR) EleAttr->name;
                                xmlNode* EleAttrVal = EleAttr->children;
                                string ElesAttrVal = (CSTR) EleAttrVal->content;
                                if (EleAttrName == "name")
                                {
                                    EleName = ElesAttrVal;
                                }
                                if (EleAttrName == "type")
                                {
                                    Eletype = ElesAttrVal;
                                }
                                if (EleAttrName == "substitutionGroup")
                                {
                                    Elesub = ElesAttrVal;
                                }
                                if (EleAttrName == "balance")
                                {
                                    EleBalance = ElesAttrVal;
                                }
                                if (EleAttrName == "periodType")
                                {
                                    EleperiodType = ElesAttrVal;
                                }
                                if (EleAttrName == "abstract")
                                {
                                    if (ElesAttrVal == "true")
                                        EleAbstract = 1;
                                    else
                                        EleAbstract = 0;
                                }
                                if (EleAttrName == "enumerations")
                                {
                                    Eleenum = ElesAttrVal;
                                }
                                if (EleAttrName == "id")
                                {
                                    string Eleid = ElesAttrVal;
                                    vector<string> splitsId;
                                    split(splitsId, Eleid, is_any_of("_"));
                                    prefix = filesystem::path(splitsId[0]).filename().string();

                                }
                            }
                        }
#ifdef HASEXCHANDLER    
                        try
                        {
#endif
                            //Guard<Mutex> g(_mutex);
                            mysqlpp::Row findRow;
                            string sqlFind = "select * from se_xbrl_lookup where xbrl_prefix = '" + prefix + "' and xbrl_name ='" + EleName + "'";
                            auto qRes = conn->query(sqlFind).store();
                            if (qRes.size())
                            {
                                findRow = qRes[0];
                                xbrl_id = (I64) findRow[0];
                            }
                            else
                            {

                                Query q = conn->query();
                                q << "INSERT INTO se_xbrl_lookup ( `xbrl_src_id`, `xbrl_prefix`, `xbrl_name`, `xbrl_type`, `xbrl_enumerations`, `xbrl_substitutiongroup`, ";
                                q << "`xbrl_balance`, `xbrl_PeriodType`, `xbrl_IsAbstract`) VALUES (";
                                q << ppt1.cikValue << "," << quote << prefix << "," << quote << EleName << "," << quote << Eletype << "," << quote << Eleenum;
                                q << ", " << quote << Elesub << "," << quote << EleBalance << "," << quote << EleperiodType << "," << EleAbstract << ")";

                                BDBG << q.str();

                                q.exec();


                                xbrl_id = q.insert_id();
                            }
#ifdef HASEXCHANDLER    
                        }
                        catch (const std::exception& ex)
                        {
                            xmlXPathFreeObject(Elexpathobj);
                            xmlXPathFreeContext(xpathContext);
                            xmlXPathFreeObject(Impxpathobj);
                            BERR << "Error: " << ex.what();
                            return -1;
                        }
#endif
                    }
                    // LOGI(_logger, "End Data insertion into lookup table" );
                    // Now upload schema into xbrl_lookup table.
                    /*
                     * 1    processing imported schema from the receive schema file.
                     * 1.1. create xpath expression for all import elements //import
                     * 1.2. Loop through nodesets and download each imported file (schemaLocation)
                     * 1.3. Recursive call CacheSchema 
                     * 
                     * 2. Processing that takes place any schema file.
                     * 2.1 Get all elements //element
                     * 2.2 insert if not found in xbrl_lookup table
                     * 
                     */
                    if (Elexpathobj)
                    {
                        xmlXPathFreeObject(Elexpathobj);
                        Elexpathobj = nullptr;
                    }
                    if (xpathContext)
                    {
                        xmlXPathFreeContext(xpathContext);
                        xpathContext = nullptr;
                    }
                    if (Impxpathobj)
                    {
                        xmlXPathFreeObject(Impxpathobj);
                        Impxpathobj = nullptr;
                    }
                }
            }
        }
        // LOGI(_logger, "Successfully processed/cached schema file " << url);
        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (...)
    {
        if (Elexpathobj)
        {
            xmlXPathFreeObject(Elexpathobj);
            Elexpathobj = nullptr;
        }
        if (xpathContext)
        {
            xmlXPathFreeContext(xpathContext);
            xpathContext = nullptr;
        }
        if (Impxpathobj)
        {
            xmlXPathFreeObject(Impxpathobj);
            Impxpathobj = nullptr;
        }

        // LOGEP("CacheSchema: Failed Unknown error.");
        return E_UnknownError;
    }
#endif
}

void CProcessEdgarStatements::RegisterNamespaces(xmlDocPtr doc, XmlNs& nsmap)
{
    string sDefPfx;
    auto node = xmlDocGetRootElement(doc); // get root node.
    if (node && node->ns && node->ns->prefix)
        sDefPfx = (CSTR) node->ns->prefix;
    xmlNsPtr* orig = xmlGetNsList(doc, node);
    if (orig)
    {
        xmlNsPtr* namespaces = orig;
        xmlNsPtr ns = *namespaces;
        while (ns != NULL)
        {
            string pfx;
            if (!ns->prefix)
            {
                if (sDefPfx != "")
                    pfx = sDefPfx;
                else
                    pfx = "def";
            }
            else
                pfx = (CSTR) ns->prefix;
            if (nsmap.find(pfx) == nsmap.end())
            {
                StringPair p(pfx, (CSTR) ns->href);
                nsmap.insert(p);
            }
            namespaces++;
            ns = *namespaces;
        }
        delete [] orig;
    }
}

void CProcessEdgarStatements::RegisterNamespace(string nsname, string nsurl, XmlNs& nsmap)
{
    if (nsmap.find(nsname) == nsmap.end())
        nsmap.insert(make_pair(nsname, nsurl));
}

void HtmlEncode(std::string& data)
{
    std::string buffer;
    buffer.reserve(data.size());
    for (size_t pos = 0; pos != data.size(); ++pos)
    {
        switch (data[pos])
        {
        case '&': buffer.append("&amp;");
            break;
        case '\"': buffer.append("&quot;");
            break;
        case '\'': buffer.append("&apos;");
            break;
        case '<': buffer.append("&lt;");
            break;
        case '>': buffer.append("&gt;");
            break;
        default: buffer.append(&data[pos], 1);
            break;
        }
    }
    data.swap(buffer);
}

void HtmlDecode(std::string& data)
{
    string encodes[5][2];
    encodes[0][0] = "&amp;";
    encodes[0][1] = "&";
    encodes[1][0] = "&quot;";
    encodes[1][1] = "\"";
    encodes[2][0] = "&apos;";
    encodes[2][1] = "\'";
    encodes[3][0] = "&lt;";
    encodes[3][1] = "<";
    encodes[4][0] = "&gt;";
    encodes[4][1] = ">";
    for (int i = 0; i != 5; ++i)
    {
        replace_all(data, encodes[i][0], encodes[i][1]);
    }
    trim(data);
}

ErrorId CProcessEdgarStatements::StoreEdgarHtmlData(CSmartConnection& conn, properties &ppt1)
{
#ifdef HASEXCHANDLER    
    try
    {
#endif
        string bsID = "51";
        string msID = "51";
        string isID = "51";
        HtmlDecode(ppt1.sic.second);
        HtmlDecode(ppt1.BStreet1);
        HtmlDecode(ppt1.BStreet2);
        HtmlDecode(ppt1.City);
        HtmlDecode(ppt1.CompanyName);
        HtmlDecode(ppt1.MCity);
        HtmlDecode(ppt1.MPhone);
        HtmlDecode(ppt1.MState);
        HtmlDecode(ppt1.MStreet1);
        HtmlDecode(ppt1.MStreet2);
        HtmlDecode(ppt1.MZIP);
        HtmlDecode(ppt1.State);
        HtmlDecode(ppt1.taxid);
        HtmlDecode(ppt1.ZIP);

        Query q = conn->query();
        q << "select * from se_edgar_sic where sic_id = " << quote << ppt1.sic.first;
        StoreQueryResult sqrSic = q.store();
        if (!sqrSic.size())
        {
            Query qSicInsert = conn->query();
            qSicInsert << "INSERT INTO `se_edgar_sic`(`sic_id`, `sic_name`) VALUES (";
            qSicInsert << ppt1.sic.first << ", ";
            qSicInsert << quote << ppt1.sic.second;
            qSicInsert << ")";
            qSicInsert.execute();
        }

        Query qFind = conn->query();
        qFind << "SELECT * FROM  `se_gbl_states` WHERE country_id=1 and `code` ";
        qFind << "LIKE " << quote << ppt1.State;
        StoreQueryResult sqrStateFind = qFind.store();
        if (sqrStateFind.size())
        {
            bsID = (CSTR) sqrStateFind[0][0];
        }

        qFind = conn->query();
        qFind << "SELECT * FROM  `se_gbl_states` WHERE country_id=1 and `code` ";
        qFind << "LIKE " << quote << ppt1.MState;
        sqrStateFind = qFind.store();
        if (sqrStateFind.size())
        {
            msID = (CSTR) sqrStateFind[0][0];
        }

        qFind = conn->query();
        qFind << "SELECT * FROM  `se_gbl_states` WHERE country_id = 1 and `code` ";
        qFind << "LIKE " << quote << ppt1.IncState;
        sqrStateFind = qFind.store();
        if (sqrStateFind.size())
        {
            isID = (CSTR) sqrStateFind[0][0];
        }

        {//start1

            //Guard<Mutex> g(_mutex);
            UID Pid = FindProductId(conn, ppt1.CompanyName);
            if (Pid)
            {
                Query qcheck = conn->query();
                qcheck << "select * from se_edgar_productid_cik where product_id = ";
                qcheck << Pid;
                qcheck << " and cik = " << ppt1.cikValue;
                if (!qcheck.store().size())
                {
                    Query qse_edgar_productid_cik = conn->query();
                    qse_edgar_productid_cik << "INSERT IGNORE INTO se_edgar_productid_cik (product_id, cik) ";
                    qse_edgar_productid_cik << "VALUES (";
                    qse_edgar_productid_cik << Pid << ", ";
                    qse_edgar_productid_cik << ppt1.cikValue;
                    qse_edgar_productid_cik << ")";

                    // LOGEP(qse_edgar_productid_cik.str());

                    qse_edgar_productid_cik.execute();
                }
            }

            Query qcikFind = conn->query();
            qcikFind << "select * from se_edgar_ciks where cik = " << ppt1.cikValue;
            if (qcikFind.store().size())
            {
                //row1 = qRes1[0];
                //LOG4CXX_INFOINFO(_logger, "cik "<<ppt1.cikValue<<"Found in Table");

                Query q1 = conn->query();
                q1 << "UPDATE se_edgar_ciks SET companyname = " << quote << ppt1.CompanyName;
                q1 << ", sic_id =" << quote << ppt1.sic.first;
                q1 << ", taxnumber = " << quote << ppt1.taxid;
                q1 << ", incorp_state_id = " << quote << isID;
                q1 << ", fin_year_end = " << quote << ppt1.FYearEnd;
                q1 << ", bus_street1 = " << quote << ppt1.BStreet1;
                q1 << ", bus_street2 =" << quote << ppt1.BStreet2;
                q1 << ", bus_city = " << quote << ppt1.City;
                q1 << ", bus_zip =" << quote << ppt1.ZIP;
                q1 << ", bus_state_id =" << quote << bsID;
                q1 << ", mail_street1 =" << quote << ppt1.MStreet1;
                q1 << ", mail_street2 =" << quote << ppt1.MStreet2;
                q1 << ", mail_city =" << quote << ppt1.MCity;
                q1 << ", mail_zip =" << quote << ppt1.MZIP;
                q1 << ", mail_state_id =" << quote << msID;
                q1 << ", bus_phone =" << quote << ppt1.MPhone;
                q1 << " WHERE cik = " << ppt1.cikValue;

                // LOGI(_logger," "<<q1.str());
                q1.exec();
            }
            else
            {
                //LOG4CXX_INFOINFO(_logger, "cik "<<ppt1.cikValue<<" Not Found in Table");
                if (Pid)
                    CIKMissingEdgarHtmlData(conn, Pid, ppt1, bsID.c_str(), msID.c_str(), isID.c_str());
            }
        }//end1
        //LOG4CXX_INFOINFO(_logger, "cik "<<ppt1.cikValue<<" insert successful");
        return S_Success;
#ifdef HASEXCHANDLER    
    }//end try
    catch (const std::exception& ex)
    {
        BERR << "Error: " << ex.what();
        return E_XMLParseError;
    }
    catch (...)
    {
        // LOGEP("StoreEdgarHtmlData: Failed Unknown error.");
        return E_UnknownError;
    }
#endif
}

UID CProcessEdgarStatements::FindProductId(CSmartConnection& conn, const string& companyName)
{
    string str = companyName;
    //boost::to_upper(str);
    //replace_all(str, " ", "");
    //replace_all(str, ",", "");
    //replace_all(str, ".", "");
    //replace_all(str, "'", "");
    //replace_all(str, "-", "");
    trim(str);
    string repl[5] = {",", ".", "&", "-", " "};
    for (int i = 0; i != 5; ++i)
    {
        replace_all(str, repl[i], "%");
    }

    Query qcofind = conn->query();
    qcofind << "select * from se_sec_products ";
    qcofind << "where product_name like " << quote << str;
    BDBG << qcofind.str();
    StoreQueryResult rcofind = qcofind.store();
    if (rcofind.size())
    {
        return (UID) rcofind[0]["product_id"];
    }
    return 0;
}

ErrorId CProcessEdgarStatements::CIKMissingEdgarHtmlData(CSmartConnection& conn, UID Pid, properties &ppt1, CSTR isID, CSTR bsID, CSTR msID)
{
    /*
     g. check if this cik is available in se_edgar_productid_cik. If not available do these steps below:
     *       i. take edgar's company name. strip -, comma, spaces, #, and store in a variable (say x)
     *      ii. use this sql statement 
     *          select * from sa.se_sec_products where REPLACE(product_name, REPLACE(product_name, ',', ''), '#', '') = x
     *          to find them matching product_id
     *      iii. insert to se_edgar_productid_cik (cik and productid)
     */
#ifdef HASEXCHANDLER    
    try
    {
#endif
        // LOGW(_logger,"cik  "<<ppt1.CompanyName);
        // LOGW(_logger,"CIK not found in the table");

        Query qse_edgar_ciks = conn->query();
        qse_edgar_ciks << "INSERT IGNORE INTO se_edgar_ciks VALUES (";
        qse_edgar_ciks << ppt1.cikValue << ", ";
        qse_edgar_ciks << quote << ppt1.CompanyName << ", ";
        qse_edgar_ciks << ppt1.sic.first << ", ";
        qse_edgar_ciks << quote << ppt1.taxid << ", ";
        qse_edgar_ciks << quote << isID << ", ";
        qse_edgar_ciks << quote << ppt1.FYearEnd << ", ";
        qse_edgar_ciks << quote << ppt1.BStreet1 << ", ";
        qse_edgar_ciks << quote << ppt1.BStreet2 << ", ";
        qse_edgar_ciks << quote << ppt1.City << ", ";
        qse_edgar_ciks << quote << ppt1.ZIP << ", ";
        qse_edgar_ciks << bsID << ", ";
        qse_edgar_ciks << quote << ppt1.MStreet1 << ", ";
        qse_edgar_ciks << quote << ppt1.MStreet2 << ", ";
        qse_edgar_ciks << quote << ppt1.MCity << ", ";
        qse_edgar_ciks << quote << ppt1.MZIP << ", ";
        qse_edgar_ciks << msID << ", ";
        qse_edgar_ciks << quote << ppt1.MPhone;
        qse_edgar_ciks << ")";

        // LOGEP(qse_edgar_ciks.str());

        // if a duplicate exception occurs, ignore.
        qse_edgar_ciks.execute();
#ifdef HASEXCHANDLER    
    }
    catch (const std::exception& ex)
    {
        BERR << "Error: " << ex.what();
    }
    catch (...)
    {
        // LOGEP("CIKMissingEdgarHtmlData: Failed Unknown error.");
    }
#endif
    return S_Success;
}

long unsigned int CProcessEdgarStatements::IdentifyXbrlFiles(CSmartConnection& conn, const char* path, const char* Fpath, properties& ppt1)
{
#ifdef HASEXCHANDLER    
    try
    {
#endif
        ifstream fin;
        fin.open(path);
        //string pattern = "<a href=\"([\\w\\W]+)-(\\d{8})([_pre|_lab|_def|_cal]*)(\\.)(xsd|xml)\">";
        string pattern = "<a href=\"dako-20110731_lab.xml\">";
        regex rpattern(pattern, regex::icase);
        string line;
        while (getline(fin, line))
        {
            smatch mlist;
            if (boost::regex_match(line, mlist, rpattern))
            {
                BDBG << mlist[0];
            }
        }
        fin.close();
        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (const std::exception& ex)
    {
        BERR << "Error: " << ex.what();
        return E_XMLParseError;
    }
    catch (...)
    {
        // LOGEP("CIKMissingEdgarHtmlData: Failed Unknown error.");
        return E_XMLParseError;
    }
#endif
}

ErrorId CProcessEdgarStatements::ParseEdgarHtmlData(CSmartConnection &conn, CSTR path, properties &ppt1, CSTR Fpath)
{
#ifdef HASEXCHANDLER    
    try
    {
#endif
        ifstream fin;
        fin.open(path);
        string line;
        I32 Flag = 0;
        I32 DFlag = 0;
        string desc;
        string sic;
        while (getline(fin, line))
        {

            regex regexPattern(Fdate, regex::extended);
            smatch what;
            bool isMatchFound = boost::regex_match(line, what, regexPattern);
            if (isMatchFound)
            {
                for (unsigned int i = 0; i < what.size(); i++)
                {
                    if (i == 1)
                    {

                        ppt1.fieldDate = what.str(2).c_str();



                    }
                }
            }
            regex regexPattern1(Cdate, regex::extended);
            smatch what1;
            bool isMatchFound1 = boost::regex_match(line, what1, regexPattern1);
            if (isMatchFound1)
            {
                for (unsigned int i = 0; i < what1.size(); i++)
                {
                    if (i == 1)
                    {

                        ppt1.ChangeDate = what1.str(2).c_str();


                    }
                }
            }
            regex regexPattern2(CName, regex::extended);
            smatch what2;
            bool isMatchFound2 = boost::regex_match(line, what2, regexPattern2);
            if (isMatchFound2)
            {
                for (unsigned int i = 0; i < what2.size(); i++)
                {
                    if (i == 1)
                    {

                        ppt1.CompanyName = what2.str(2).c_str();


                    }
                }
            }

            //            regex regexPattern3(cik, regex::extended);
            //            smatch what3;
            //            bool isMatchFound3 = boost::regex_match(line, what3, regexPattern3);
            //            if (isMatchFound3)
            //            {
            //                for (unsigned int i = 0; i < what3.size(); i++)
            //                {
            //                    if (i == 1)
            //                    {
            //
            //                        ppt1.cikValue = what3.str(2).c_str();
            //
            //
            //                    }
            //                }
            //            }


            regex regexPattern4(Sicnumber, regex::extended);
            smatch what4;
            bool isMatchFound4 = boost::regex_match(line, what4, regexPattern4);
            if (isMatchFound4)
            {
                for (unsigned int i = 0; i < what4.size(); i++)
                {
                    if (i == 1)
                    {

                        sic = what4.str(2).c_str();



                    }
                }
            }


            regex regexPattern5(Ipsnumber, regex::extended);
            smatch what5;
            bool isMatchFound5 = boost::regex_match(line, what5, regexPattern5);
            if (isMatchFound5)
            {
                for (unsigned int i = 0; i < what5.size(); i++)
                {
                    if (i == 1)
                    {

                        ppt1.taxid = what5.str(2).c_str();


                    }
                }
            }


            regex regexPattern6(Fyear, regex::extended);
            smatch what6;
            bool isMatchFound6 = boost::regex_match(line, what6, regexPattern6);
            if (isMatchFound6)
            {
                for (unsigned int i = 0; i < what6.size(); i++)
                {
                    if (i == 1)
                    {

                        ppt1.FYearEnd = what6.str(2).c_str();


                    }
                }
            }


            regex regexPattern7(FType, regex::extended);
            smatch what7;
            bool isMatchFound7 = boost::regex_match(line, what7, regexPattern7);
            if (isMatchFound7)
            {
                for (unsigned int i = 0; i < what7.size(); i++)
                {
                    if (i == 1)
                    {

                        ppt1.TYPE = what7.str(2).c_str();


                    }
                }
            }


            regex regexPattern8(BS1, regex::extended);
            smatch what8;

            bool isMatchFound8 = boost::regex_match(line, what8, regexPattern8);
            if (isMatchFound8)
            {
                for (unsigned int i = 0; i < what8.size(); i++)
                {
                    if (i == 1 && Flag == 0)
                    {

                        ppt1.BStreet1 = what8.str(2).c_str();


                    }
                    else if (i == 1 && Flag == 1)
                    {

                        ppt1.MStreet1 = what8.str(2).c_str();


                    }
                }
            }


            regex regexPattern9(BS2, regex::extended);
            smatch what9;
            bool isMatchFound9 = boost::regex_match(line, what9, regexPattern9);
            if (isMatchFound9)
            {
                for (unsigned int i = 0; i < what9.size(); i++)
                {
                    if (i == 1 && Flag == 0)
                    {

                        ppt1.BStreet2 = what9.str(2).c_str();


                    }
                    else if (i == 1 && Flag == 1)
                    {

                        ppt1.MStreet2 = what9.str(2).c_str();


                    }
                }
            }


            regex regexPattern10(BC, regex::extended);
            smatch what10;
            bool isMatchFound10 = boost::regex_match(line, what10, regexPattern10);
            if (isMatchFound10)
            {
                for (unsigned int i = 0; i < what10.size(); i++)
                {
                    if (i == 1 && Flag == 0)
                    {

                        ppt1.City = what10.str(2).c_str();

                    }
                    else if (i == 1 && Flag == 1)
                    {

                        ppt1.MCity = what10.str(2).c_str();

                    }
                }
            }


            regex regexPattern11(BS, regex::extended);
            smatch what11;
            bool isMatchFound11 = boost::regex_match(line, what11, regexPattern11);
            if (isMatchFound11)
            {
                for (unsigned int i = 0; i < what11.size(); i++)
                {
                    if (i == 1 && Flag == 0)
                    {

                        ppt1.State = what11.str(2).c_str();

                    }
                    else if (i == 1 && Flag == 1)
                    {

                        ppt1.MState = what11.str(2).c_str();

                    }
                }
            }


            regex regexPattern12(BZ, regex::extended);
            smatch what12;
            bool isMatchFound12 = boost::regex_match(line, what12, regexPattern12);
            if (isMatchFound12)
            {
                for (unsigned int i = 0; i < what12.size(); i++)
                {
                    if (i == 1 && Flag == 0)
                    {
                        ppt1.ZIP = what12.str(2).c_str();
                    }
                    else if (i == 1 && Flag == 1)
                    {

                        ppt1.MZIP = what12.str(2).c_str();

                    }
                }
            }


            regex regexPattern13(PH, regex::extended);
            smatch what13;
            bool isMatchFound13 = boost::regex_match(line, what13, regexPattern13);
            if (isMatchFound13)
            {
                for (unsigned int i = 0; i < what13.size(); i++)
                {
                    if (i == 1 && Flag == 0)
                    {

                        ppt1.Phone = what13.str(2).c_str();


                    }
                    else if (i == 1 && Flag == 1)
                    {
                        ppt1.MPhone = what13.str(2).c_str();

                    }
                }
            }

            /*
     
            CSTR xsd2 = "(.*)(Document)(.*)(- file:)(.*)(cal.xml)(.*)";
            regex regexPattern15(xsd2, regex::extended);  
            smatch what15;
            bool isMatchFound15 = boost::regex_match(line, what15, regexPattern15);
            if (isMatchFound15) 
            { 
                 for (unsigned int i=0; i < what15.size(); i++) 
                 { 
                     if(i==1)
                     {
                         string xml2= what15.str(5).c_str();
                         trim(xml2);
                         string url2 = string(Fpath)+"/"+xml2+"cal.xml";
                          ppt1.xbrlDocs[CALCULATION]=xmlReadFile(url2.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                          ppt1.nsDocs[CALCULATION] = XmlNs();
                          RegisterNamespaces(ppt1.xbrlDocs[CALCULATION], ppt1.nsDocs[CALCULATION]);
                     }
                 } 
            } 
          
            CSTR xsd = "(.*)(Document)(.*)(- file:)(.*)(pre.xml)(.*)";
            //CSTR xsd ="(.*)(Document)(.*)(- file:)(.*)(_)?(pre|cal|lab|def|ref)?(.xml|.xsd)";
            regex regexPattern14(xsd, regex::extended);  
            smatch what14;
            bool isMatchFound14 = boost::regex_match(line, what14, regexPattern14);
            if (isMatchFound14) 
            { 
                 for (unsigned int i=0; i < what14.size(); i++) 
                 { 
                     if(i==1)
                     {
                          string xml= what14.str(5).c_str();
                          trim(xml);
                          string url = string(Fpath)+"/"+xml+"pre.xml";
                          ppt1.xbrlDocs[PRESENTATION]=xmlReadFile(url.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                          ppt1.nsDocs[PRESENTATION] = XmlNs();
                          RegisterNamespaces(ppt1.xbrlDocs[PRESENTATION], ppt1.nsDocs[PRESENTATION]);
                     }
                 } 
            } 
            
            CSTR xsd1 = "(.*)(Document)(.*)(- file:)(.*)(.xsd)(.*)";
            regex regexPattern16(xsd1, regex::extended);  
            smatch what16;
            bool isMatchFound16 = boost::regex_match(line, what16, regexPattern16);
            if (isMatchFound16) 
            { 
                  for (unsigned int i=0; i < what16.size(); i++) 
                  { 
                       if(i==1)
                       {
                          string xml1= what16.str(5).c_str();
                          trim(xml1);
                          if (ppt1.XSD_Fname.empty())
                          {
                              ppt1.XSD_Fname =xml1+".xsd";
                              string url1 = string(Fpath)+"/"+xml1+".xsd";
                              ppt1.xbrlDocs[SCHEMA]=xmlReadFile(url1.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                              ppt1.nsDocs[SCHEMA] = XmlNs();
                              RegisterNamespaces(ppt1.xbrlDocs[SCHEMA], ppt1.nsDocs[SCHEMA]);
                              auto ret =CacheSchema(conn,url1, ppt1);
                              {
                                  if (FAILED(ret))
                                      return ret;
                              }
                          }
                          else
                          {
                              ppt1.XSD_Fname1 =xml1+".xsd";
                              string url1 = string(Fpath)+"/"+xml1+".xsd";
                              //ppt1.xbrlDocs[SCHEMA]=xmlReadFile(url1.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                              //ppt1.nsDocs[SCHEMA] = XmlNs();
                              //RegisterNamespaces(ppt1.xbrlDocs[SCHEMA], ppt1.nsDocs[SCHEMA]);
                              auto ret =CacheSchema(conn,url1, ppt1);
                              {
                                  if (FAILED(ret))
                                      return ret;
                              }
                          }
                       }
                   } 
            } 
            CSTR xsd3 = "(.*)(Document)(.*)(- file:)(.*)(lab.xml)(.*)";
            regex regexPattern19(xsd3, regex::extended);  
            smatch what19;
            bool isMatchFound19 = boost::regex_match(line, what19, regexPattern19);
            if (isMatchFound19) 
            { 
              for (unsigned int i=0; i < what19.size(); i++) 
              { 
                  if(i==1)
                  {
                      string xml1= what19.str(5).c_str();
                      trim(xml1);
                      string url1 = string(Fpath)+"/"+xml1+"lab.xml";
                      ppt1.xbrlDocs[LABEL]=xmlReadFile(url1.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                      ppt1.nsDocs[LABEL] = XmlNs();
                      RegisterNamespaces(ppt1.xbrlDocs[LABEL], ppt1.nsDocs[LABEL]);
                  }
               } 
            }
            CSTR xsd4 = "(.*)(Document)(.*)(- file:)(.*)(def.xml)(.*)";
            regex regexPattern20(xsd4, regex::extended);  
            smatch what20;
            bool isMatchFound20 = boost::regex_match(line, what20, regexPattern20);
            if (isMatchFound20) 
            { 
              for (unsigned int i=0; i < what20.size(); i++) 
              { 
                   if(i==1)
                   {
                      string xml1= what20.str(5).c_str();
                      trim(xml1);
                      string url1 = string(Fpath)+"/"+xml1+"def.xml";
                      ppt1.xbrlDocs[DEFINITION]=xmlReadFile(url1.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                      ppt1.nsDocs[DEFINITION] = XmlNs();
                      RegisterNamespaces(ppt1.xbrlDocs[DEFINITION], ppt1.nsDocs[DEFINITION]);
                   }
               } 
            } 

            ifstream fin;
              CSTR xsd5 = "(.*)(Document)(.*)(- file:)(.*)(-)([\\d]{8})(.xml)(.*)";
              regex regexPattern21(xsd5, regex::extended);  
              smatch what21;
              bool isMatchFound21 = boost::regex_match(line, what21, regexPattern21);
              if (isMatchFound21) 
              { 
                for (unsigned int i=0; i < what21.size(); i++) 
                { 
                     if(i==1)
                     {
                       string xml1= what21.str(5).c_str();
                       trim(xml1);
                       string xml2= what21.str(7).c_str();
                       string url1 = string(Fpath)+"/"+xml1+"-"+xml2+".xml";
                       ppt1.XML_Instance = xml1+"-"+xml2+".xml";
                       ppt1.xbrlDocs[INSTANCE]=xmlReadFile(url1.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                       ppt1.nsDocs[INSTANCE] = XmlNs();
                       RegisterNamespaces(ppt1.xbrlDocs[INSTANCE], ppt1.nsDocs[INSTANCE]);
                     }
                 } 
              } 
              CSTR xsd51 = "(.*)(Document)(.*)(- file:)(.*)(_)([\\d]{8})(.xml)(.*)";
              regex regexPattern51(xsd51, regex::extended);  
              smatch what51;
              bool isMatchFound51 = boost::regex_match(line, what51, regexPattern51);
              if (isMatchFound51) 
              { 
                for (unsigned int i=0; i < what51.size(); i++) 
                { 
                     if(i==1)
                     {
                       string xml1= what51.str(5).c_str();
                       trim(xml1);
                       string xml2= what51.str(7).c_str();
                       string url1 = string(Fpath)+"/"+xml1+"_"+xml2+".xml";
                       ppt1.XML_Instance = xml1;
                       ppt1.xbrlDocs[INSTANCE]=xmlReadFile(url1.c_str(), NULL, XML_PARSE_IGNORE_ENC);
                       ppt1.nsDocs[INSTANCE] = XmlNs();
                       RegisterNamespaces(ppt1.xbrlDocs[INSTANCE], ppt1.nsDocs[INSTANCE]);
                     }
                 } 
              } 
             */

            regex regexPattern17(SC, regex::extended);
            smatch what17;
            bool isMatchFound17 = boost::regex_match(line, what17, regexPattern17);
            if (isMatchFound17)
            {
                for (unsigned int i = 0; i < what17.size(); i++)
                {
                    if (i == 1)
                    {
                        ppt1.IncState = what17.str(2).c_str();
                    }
                }
            }

            regex regexPattern18(SCN, regex::extended);
            smatch what18;
            bool isMatchFound18 = boost::regex_match(line, what18, regexPattern18);
            if (isMatchFound18)
            {
                for (unsigned int i = 0; i < what18.size(); i++)
                {
                    if (i == 1)
                    {
                        string sicname = what18.str(3);
                        ppt1.sic = std::make_pair(atoi(sic.c_str()), sicname.c_str());

                    }
                }
            }

            if (line == "<MAIL-ADDRESS>")
                Flag = 1;
            if (line == "&lt;DOCUMENT&gt;")
                DFlag = 1;
            if (line == "&lt;/DOCUMENT&gt;")
            {
                DFlag = 0;
            }
        }//end while loop
        fin.close();

        /*
      
      if (!ppt1.XSD_Fname.empty() && !ppt1.XSD_Fname1.empty())
      {
          string fname1 = ppt1.XSD_Fname;
          replace_all(fname1, ".xsd", "");
          string fname2 = ppt1.XSD_Fname1;
          replace_all(fname2, ".xsd", "");
          string fxml = ppt1.XML_Instance;
          replace_all(fxml, ".xml", "");
        
          if (fname2 == fxml)
          {
              xmlFreeDoc(ppt1.xbrlDocs[SCHEMA]);
              string url1 = string(Fpath)+"/"+ppt1.XSD_Fname1;
              ppt1.xbrlDocs[SCHEMA]=xmlReadFile(url1.c_str(), NULL, XML_PARSE_IGNORE_ENC);
              RegisterNamespaces(ppt1.xbrlDocs[SCHEMA], ppt1.nsDocs[SCHEMA]);
          }        
      }

         */

        return S_Success;
#ifdef HASEXCHANDLER    
    }
    catch (...)
    {
        // LOGEP("ParseEdgarHtmlData: Failed Unknown error.");
        return E_UnknownError;
    }
#endif
}
