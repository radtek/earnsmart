/* 
 * File:   CXbrl.cpp
 * Author: santony
 * 
 * Created on July 1, 2012, 9:41 PM
 */

#include "sexbrlinternal.h"
#include "CXbrl.h"
#include "../seglobal/CFunctionCache.h"
#include "CProcessEdgarStatementsjson.h"
#include "CCIKToSymbolMapper.h"
#include "CListEdgarForms.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "CXbrlDataTree.h"

#include <stdlib.h>
#include <string>
#include <cstring>

#include "../semysql/CSEConnections.h"
#include "CXbrlDocType.h"
#include "CXbrlStatement.h"
#include "CStatementUtilities.h"
#include "../seglobal/exceptions.h"
#include "CFinancialsConsolidated.h"
#include "CXbrlJsonOutputs.h"
#include "../seglobal/CString.h"
#include <boost/algorithm/string.hpp>
#include <libxml/nanohttp.h>
#include <libxml2/libxml/HTMLparser.h>
#include "../seglobal/CDirectory.h"
#include "../senet/CHttpClient.h"
#include <fstream>
#include <sstream>
#include "../senet/senetapi.h"
#include <boost/regex.hpp>
#include "CMyqlUtilities.h"
#include "CXbrlStatementList.h"
#include "CPersistPerformanceData.h"
#include "CFinancialPerformance.h"
#include "../semain/semain.h"
#include <Poco/ScopedLock.h>
#include "CFinancialPerformanceList.h"
#include "CXbrlIdList.h"
#include "../seglobal/CQuarter.h"
#include "CXbrlAccountMatcher.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/date_clock_device.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

#include <../setraining/setrainingapi.h>
#include "../secubist/CCubist.h"

#include "../semath/CStatistics.h"
#include "CFinPeriodList.h"
#include "../semain/semain.h"
#include "CSpecialPerformanceTrainingData.h"
#include "CCaptureFiscalPeriods.h"
#include <Poco/Condition.h>
#include "../semysql/CMysqlTable.h"

using namespace std;
using namespace se::xbrl;
using namespace boost;
using namespace se::securities;
using namespace se::training;

//NamedObjectNamingType IXbrl::Name = 0xE;

CXbrl::CXbrl(ISession* ps) : CObjectRoot(ps), statementProcessor(nullptr)
{
    SEDBCONN = CSEConnections::GetMain();
}

CXbrl::~CXbrl()
{
    delete (CProcessEdgarStatements*)statementProcessor;
}

NamedObjectNamingType CXbrl::get_TypeId()
{
    return se::xbrl::IXbrlName;
}

unsigned long CXbrl::GetProductId(CIK cik)
{
    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q << "select get_ProductId(" << cik << ")";
    auto use = q.use();
    if (use)
    {
        const Row& r = use.fetch_row();
        if (r)
            return r[0];
    }
    return 0;
}

SE_CODE CXbrl::GetCIK(CSTR symbol, IString** ppOut)
{

    // new method that reads it from db itself.
    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q << "call get_SymbolToCIK('" << symbol << "')";
    auto store = q.store();
    if (store.size() > 0)
    {
        sql_varchar_null sym = store[0][0];
        if (!sym.is_null)
        {
            const string& ssym = sym.data;
            return CObject<CString>::Create(get_Session(), ssym.c_str(), ppOut);
        }
    }
    while (q.more_results())
        q.store_next();
    return E_NotFound;
    /* DEPRECATED
    // create instance
    CCIKToSymbolMapper mapper ;
    return mapper.GetCIKFromSymbol(symbol, ppOut);
     */
}

SE_CODE CXbrl::get_CIK(unsigned long ProdId, IString** ppOut)
{
    RETURNIFFAILED(CString::Create("", ppOut));
    try
    {
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q << "select cik from se_edgar_productid_cik where product_id = " << ProdId;
        auto qr = q.store();
        if (qr.size())
            (*ppOut)->set_Buffer(((string) qr[0][0]).c_str());
        return qr.size() ? S_OK : S_False;
    }
    catch (const std::exception& ex)
    {
        // LOGE(_logger, ex.what());
        return S_False;
    }
}

bool CXbrl::GetSymbol(CIK inCIK, IString** outSymbol)
{
    CCIKToSymbolMapper mapper;
    return mapper.GetSymbolFromCIK(inCIK, outSymbol);
}

ErrorId CXbrl::UpdateCIKs(IXbrlCompletionEvent<int>* listener)
{
    //LOG4CXX_INFOINFO(_logger, "In UpdateCIKs method.");
    CCIKToSymbolMapper mapper;
    mapper.UpdateCIKForAllSymbols();
    //LOG4CXX_INFOINFO(_logger, "Exited UpdateCIKs method.");
}

//ErrorId CXbrl::ProcessEdgarFormIdx(const YEARQUARTER& period, CSTR formType)
//{
//    CListEdgarForms formsParser(period.Year, period.Quarter, formType);
//    return formsParser.Process(0);
//}

ErrorId CXbrl::ProcessStatements()
{
    if (!statementProcessor)
    {
        Poco::ScopedLock<Poco::Mutex> l(_lock);
        if (!statementProcessor)
        {
            statementProcessor = new CProcessEdgarStatements(get_Session());
        }
    }
    return static_cast<CProcessEdgarStatements*>(statementProcessor)->Process();
}

ErrorId CXbrl::ProcessStatement(UID recid)
{
    if (!statementProcessor)
    {
        Poco::ScopedLock<Poco::Mutex> l(_lock);
        if (!statementProcessor)
        {
            statementProcessor = new CProcessEdgarStatements(get_Session());
        }
    }
    return static_cast<CProcessEdgarStatements*>(statementProcessor)->Process(recid);
}


// Returns a edgar financial reports for the given security description.

ErrorId CXbrl::GetStatements(se::securities::ISecurityDescription* pIn, DATAPERIOD& period, XFT formType, XDC xbrltype, IXbrlDataTree** ppOut)
{
    return CObject<CXbrlDataTree>::Create(get_Session(), pIn, period, formType, xbrltype, ppOut);
}

// Returns a edgar financial reports for the given Edgar CIK.

ErrorId CXbrl::GetStatements(CIK cik, DATAPERIOD& period, XFT formType, XDC xbrltype, IXbrlDataTree** ppOut)
{
    return CObject<CXbrlDataTree>::Create(get_Session(), cik, period, formType, xbrltype, ppOut);
}

ErrorId CXbrl::GetDocumentTypes(IROSPList<IXbrlDocType>** ppOut)
{
    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q << " SELECT * FROM se_edgar_doctypes where doc_isenabled = 1";
    try
    {
        auto qRes = q.store();
        SP<CROSPList < IXbrlDocType>> pTypes;
        RETURNIFFAILED(CObject< CROSPList<IXbrlDocType> >::Create(get_Session(), &pTypes));
        for (const Row& row : qRes)
        {
            SP<IXbrlDocType> pXbrlDocType;
            ErrorId retCode = CObject<CXbrlDocType>::Create(get_Session(), &pXbrlDocType);
            if (FAILED(retCode))
            {
                return retCode;
            }
            pXbrlDocType->set_Id((UID) row["doc_id"]);
            pXbrlDocType->set_Name(row["doc_name"]);
            pXbrlDocType->set_Desc(row["doc_desc"]);
            pXbrlDocType->set_PeriodType((PERIOD) (I16) row["doc_reportperiod"]);
            pTypes->Add(pXbrlDocType);
        }
        pTypes.CopyTo(ppOut);
        return S_Success;
    }
    CATCHCLAUSE("GetDocumentTypes");
}

ErrorId CXbrl::GetPeriods(UID uidProdId, IROList<I16>** ppOut)
{
    const string funcName = "se.xbrl.GetPeriods";
    CROI16List* p;
    if (!FC.TrySPGetFValue(funcName, uidProdId, p))
    {
        Poco::ScopedLock<Poco::Mutex> l(_lock);
        if (!FC.TrySPGetFValue(funcName, uidProdId, p))
        {
            CSmartConnection conn(SEDBCONN);
            string cik;
            {
                Query q = conn->query();
                q << "select cik from se_edgar_productid_cik  where product_id =" << uidProdId;
                auto qRes = q.store();
                if (qRes.size())
                    cik = (string) qRes[0]["cik"];
                else
                    return E_NotFound;
            }

            Query q = conn->query();
            q << "select distinct YEAR(filedate) as periods from se_edgar_formrecs where cik = " << cik << " order by periods desc";
            auto qRes = q.store();
            RETURNIFFAILED(CObject<CROI16List>::Create(get_Session(), &p));
            for (const Row& row : qRes)
            {
                p->Add((I16) row[0]);
            }
            FC.SPSetFValue(funcName, uidProdId, p);
            *ppOut = p;
            return S_Success;
        }
    }
    p->AddRef();
    *ppOut = p;
    return S_Success;
}

ErrorId CXbrl::GetStatementNames(UID uidProdId, DATAPERIOD& period, XFT formType, IROSPList<IXbrlStatement>** ppOut)
{
    try
    {
        const string funcName = "se.xbrl.GetStatementNames";
        IROSPList<IXbrlStatement>* p;
        if (!FC.TrySPGetFValue(funcName, uidProdId, period.Quarter, period.Year, (int) formType, p))
        {
            Poco::ScopedLock<Poco::Mutex> l(_lock);
            if (!FC.TrySPGetFValue(funcName, uidProdId, period.Quarter, period.Year, (int) formType, p))
            {
                CSmartConnection conn(SEDBCONN);
                CStatementUtilities utils(get_Session());
                SP<IString> cik;
                RETURNIFFAILED(utils.GetCikFromProductId(conn, uidProdId, &cik));
                RETURNIFFAILED(utils.GetStatementNames(conn, cik->get_Buffer(), period, formType, &p));
                FC.SPSetFValue(funcName, uidProdId, period.Quarter, period.Year, (int) formType, p);
                *ppOut = p;
                return S_OK;
            }
        }
        p->AddRef();
        *ppOut = p;
        return S_OK;
    }
    catch (std::exception ex)
    {
        // LOGE(_logger, "Unhandled error : " << ex.what());
        return E_UnknownError;
    }
}

ErrorId CXbrl::GetStatement(UID statementId, XDC xdc, IXbrlDataNode** ppOut)
{
    // LOGI(_logger, "Fetching statement with id " << statementId);
    // if it is in cache
    const string funcName = "se.xbrl.GetStatement";
    IXbrlDataNode* p;
    if (!FC.TrySPGetFValue(funcName, statementId, p))
    {
        Poco::ScopedLock<Poco::Mutex> l(_lock);
        if (!FC.TrySPGetFValue(funcName, statementId, p))
        {
            SP<IXbrlDataTree> pTree;
            RETURNIFFAILED(CObject<CXbrlDataTree>::Create(get_Session(), statementId, xdc, &pTree));
            SP<IROSPList < IXbrlDataNode>> pStmts;
            RETURNIFFAILED(pTree->get_Statements(&pStmts));
            if (!pStmts->get_Count())
                return S_False;
            RETURNIFFAILED(pStmts->get_Item(0, &p));
            FC.SPSetFValue(funcName, statementId, p);
            *ppOut = p;
            return S_OK;
        }
    }
    p->AddRef();
    *ppOut = p;
    return S_OK;
}

long unsigned int CXbrl::GetConsolidatedFinancials(CIK cik, IFinancialsConsolidated** ppOut, bool fillemptyValues)
{
    //    try
    //    {
    SP<IApp> papp;
    RETURNIFFAILED(SEOpenApp(&papp));
    const string fname = "se.xbrl.GetConsolidatedFinancials";
    IFinancialsConsolidated* p;
    if (!FC.TrySPGetFValue(fname, (string) cik, p))
    {
        Poco::ScopedLock<Poco::Mutex> l(_lock);
        if (!FC.TrySPGetFValue(fname, (string) cik, p))
        {
            RETURNIFFAILED(CObject<CFinancialsConsolidated>::Create(get_Session(), cik, fillemptyValues, &p));
            FC.SPSetFValue(fname, (string) cik, p);
            *ppOut = p;
            return S_OK;
        }
    }
    *ppOut = p;
    p->AddRef();
    return S_OK;
    //    }
    //    CATCHCLAUSE("GetConsolidatedFinancials");
}

long unsigned int CXbrl::get_Label(CSmartConnection& conn, IString** ppOut, unsigned long xbrlId, CIK cik)
{
    StoreQueryResult qr;
    Query q = conn->query();
    if (!cik || !strlen(cik))
    {
        q << "select get_XbrlLabel(" << xbrlId << ", null);";
        qr = q.store();
    }
    else
    {
        q << "select get_XbrlLabel(" << xbrlId << ", " << cik << ");";
        qr = q.store();
    }
    if (qr.size())
    {
        return IString::Create((CSTR) qr[0][0], ppOut);
    }
    else
        return E_NotFound;
}

SE_CODE CXbrl::get_Label(IString** ppOut, unsigned long xbrlId, CIK cik)
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        return get_Label(conn, ppOut, xbrlId, cik);
    }
    catch (const seexception& ex)
    {
        // LOGE(_logger, "Reported error : " << ex.what());
        return E_NotFound;
    }
    catch (const std::exception& ex)
    {
        // LOGE(_logger, "Reported error : " << ex.what());
        return E_NotFound;
    }
    catch (...)
    {
        // LOGE(_logger, "Reported unknown error.");
        return E_UnknownError;
    }
}

long unsigned int CXbrl::GetJsonOutputController(IJsonOutputController** ppOut)
{
    const string funcName = "se.xbrl.json";
    IJsonOutputController* p;
    if (!FC.TrySPGetFValue(funcName, p))
    {
        Poco::ScopedLock<Poco::Mutex> l(_lock);
        if (!FC.TrySPGetFValue(funcName, p))
        {
            RETURNIFFAILED(CObject<CXbrlJsonOutputs>::Create(get_Session(), &p));
            FC.SPSetFValue(funcName, p);
            *ppOut = p;
            return S_OK;
        }
    }
    p->AddRef();
    *ppOut = p;
    return S_OK;
}

long unsigned int CXbrl::GetEdgarCompanyInfo(const char* symbolOrCIK, IPublicCompanyInfo** ppOut)
{
    string strUrl = "http://www.sec.gov/cgi-bin/browse-edgar?CIK=~~&Find=Search&owner=exclude&action=getcompany";
    replace_all(strUrl, "~~", symbolOrCIK);

    // download and save the file.
    char* contentType = 0;
    void* httpCtxt = xmlNanoHTTPOpen(strUrl.c_str(), &contentType);
    if (httpCtxt == 0)
        return E_HttpError;

    ostringstream os;
    char buffer[0x800];
    int count;
    while ((count = xmlNanoHTTPRead(httpCtxt, buffer, 0x800)))
    {
        os << buffer;
    }
    //xmlFree(contentType);
    xmlNanoHTTPClose(httpCtxt);
    xmlNanoHTTPCleanup();


    /*
     * <!-- START FILER DIV -->
<div style="margin: 15px 0 10px 0; padding: 3px; overflow: hidden; background-color: #BCD6F8;">
        <div class="mailer">
                Mailing Address <span class="mailerAddress">550 SOUTH TRYON STREET</span>
                <span class="mailerAddress">DEC45</span>
                <span class="mailerAddress"> CHARLOTTE NC 28202 </span>
        </div>
        <div class="mailer">
                Business Address <span class="mailerAddress">550 SOUTH TRYON STREET</span>
                <span class="mailerAddress">DEC45</span>
                <span class="mailerAddress"> CHARLOTTE NC 28202 </span>
                <span class="mailerAddress">704-382-3853</span>
        </div>
        <div class="companyInfo">
                <span class="companyName">Duke Energy CORP <acronym title="Central Index Key">CIK</acronym>#: <a href="/cgi-bin/browse-edgar?action=getcompany&amp;CIK=0001326160&amp;owner=exclude&amp;count=40">0001326160 (see all company filings)</a></span>
                <p class="identInfo">
                        <acronym title="Standard Industrial Code">SIC</acronym>: <a href="/cgi-bin/browse-edgar?action=getcompany&amp;SIC=4931&amp;owner=exclude&amp;count=40">4931</a> - ELECTRIC &amp; OTHER SERVICES COMBINED
                        <br />
                        State location: <a href="/cgi-bin/browse-edgar?action=getcompany&amp;State=NC&amp;owner=exclude&amp;count=40">NC</a> | State of Inc.: <strong>DE</strong> | Fiscal Year End: 1231
                        <br />
                        formerly: Deer Holding Corp. (filings through 2005-06-22)
                        <br />
                        formerly: Duke Energy Holding Corp. (filings through 2006-04-03)
                        <br />
                        (Assistant Director Office: 2)
                        <br />
                        Get <a href="/cgi-bin/own-disp?action=getissuer&amp;CIK=0001326160"><b>insider transactions</b></a> for this <b>issuer</b>.
                        <br />
                        Get <a href="/cgi-bin/own-disp?action=getowner&amp;CIK=0001326160"><b>insider transactions</b></a> for this <b>reporting owner</b>.
                </p>
        </div>
</div>
     */

    htmlParserCtxtPtr parser = htmlCreatePushParserCtxt(NULL, NULL, NULL, 0, NULL, XML_CHAR_ENCODING_UTF8);
    htmlCtxtUseOptions(parser, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_RECOVER | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
    htmlParseChunk(parser, os.str().c_str(), os.str().size(), 0);
    xmlNodePtr ns = xmlDocGetRootElement(parser->myDoc);
    std::function<void(xmlNodePtr) > func;
    func = [this, &ns, &func](xmlNodePtr node)
    {
        xmlNode *cur_node = NULL;
        xmlAttr *cur_attr = NULL;
        for (cur_node = node; cur_node; cur_node = cur_node->next)
        {
            // do something with that node information, like… printing the tag’s name and attributes
            //printf(“Got tag : %s\n”, cur_node->name);
            for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next)
            {
                //printf(“  -> with attribute : %s\n”, cur_attre->name);
                // LOGI(_logger, "Name: " << (CSTR)cur_node->name << " | " << (CSTR)xmlNodeGetContent(cur_node));
            }
            func(cur_node->children);
        }
    };

    func(ns);
    return S_OK;
}

SE_CODE CXbrl::FindCompany(CSTR criteria, ISecurityDescriptionList** ppOut)
{
    try
    {
        if (!criteria)
            return E_InvalidArg;
        CSmartConnection conn(SEDBCONN);
        SP<ISecurityDescriptionList> pList;
        RETURNIFFAILED(ISecurityDescriptionList::Create(get_Session(), &pList));
        int strLen = strlen(criteria);
        //const string sQuery = "select a.product_id, a.product_symbol, a.product_name, b.cik, c.exchange_name from se_sec_products a inner join se_edgar_productid_cik b on a.product_id = b.product_id inner join se_exchanges c on a.product_exchange_id = c.exchange_id ";
        const string sQuery = "select * from list_ListedCompanies ";
        if (strLen > 0 && strLen < 11) // could be a cik. Check cik table first.
        {
            boost::regex ciktype("(\\d{2,10})");
            if (boost::regex_match(criteria, ciktype))
            {
                //check cik table.
                Query qcik = conn->query();
                qcik << sQuery << " where cik = " << criteria << " order by product_symbol";
                StoreQueryResult qrcik = qcik.store();
                if (qrcik.size())
                {
                    // fill as much info and add
                    SP<ISecurityDescription> psecdesc;
                    RETURNIFFAILED(CreateSecurityDescription(get_Session(), &psecdesc));
                    psecdesc->set_Id((UID) qrcik[0]["product_id"]);
                    //psecdesc->set_Name((CSTR)qrcik[0]["product_name"]);
                    psecdesc->set_Symbol((CSTR) qrcik[0]["product_symbol"]);
                    //psecdesc->set_Exchange((CSTR)qrcik[0]["exchange_name"]);
                    pList->Add(psecdesc);
                }
            }
            else if (strLen > 0 && strLen < 6)
            {
                Query qcik = conn->query();
                string crt = criteria;
                boost::algorithm::to_lower(crt);
                qcik << sQuery << " where lower(product_symbol) like '" << crt << "%'";
                qcik << " order by product_symbol";
                StoreQueryResult qrcik = qcik.store();
                // match company name in se_sec_products table.
                for (const Row& row : qrcik)
                {
                    SP<ISecurityDescription> psecdesc;
                    RETURNIFFAILED(CreateSecurityDescription(get_Session(), &psecdesc));
                    psecdesc->set_Id((UID) row["product_id"]);
                    //psecdesc->set_Name((CSTR)row["product_name"]);
                    psecdesc->set_Symbol((CSTR) row["product_symbol"]);
                    //psecdesc->set_Exchange((CSTR)row["exchange_name"]);
                    pList->Add(psecdesc);
                }
            }
        }
        if (!pList->get_Count() or ( strLen > 5))
        {
            // now search company name or symbol.
            Query qcik = conn->query();
            string crt = criteria;
            boost::algorithm::to_lower(crt);
            qcik << sQuery << " where lower(product_name) like '%" << crt << "%'";
            qcik << " order by product_symbol";
            StoreQueryResult qrcik = qcik.store();
            // match company name in se_sec_products table.
            for (const Row& row : qrcik)
            {
                SP<ISecurityDescription> psecdesc;
                RETURNIFFAILED(CreateSecurityDescription(get_Session(), &psecdesc));
                psecdesc->set_Id((UID) row["product_id"]);
                //psecdesc->set_Name((CSTR)row["product_name"]);
                psecdesc->set_Symbol((CSTR) row["product_symbol"]);
                //psecdesc->set_Exchange((CSTR)row["exchange_name"]);
                pList->Add(psecdesc);
            }
        }
        pList.CopyTo(ppOut);
        return S_OK;
    }
    CATCHCLAUSE("FindCompany");
}

long unsigned int CXbrl::GetFinancialPerformance(CIK cik, IFinancialPerformance** ppOut)
{
    SP<IFinancialsConsolidated> pCon;
    RETURNIFFAILED(GetConsolidatedFinancials(cik, &pCon));
    return _GetFinancialPerformance(pCon, ppOut);
}

long unsigned int CXbrl::GetFinancialPerformance(CIK cik, unsigned long perfId, IFinancialRatio** ppOut)
{
    return CObject<CFinancialRatio>::Create(get_Session(), perfId, cik, ppOut);
}

long unsigned int CXbrl::_GetFinancialPerformance(IFinancialsConsolidated* pcon, IFinancialPerformance** ppOut)
{
    return pcon->get_Performance(ppOut);
}

long unsigned int CXbrl::GetCachedFinancialPerformance(CIK cik, unsigned long perfId, IFinancialRatio** ppOut)
{
    return CObject<CFinancialRatio>::Create(get_Session(), perfId, cik, ppOut);
}

long unsigned int CXbrl::GetCachedFinancialPerformance(CIK cik, bool OnlyPubliclyDisplayed, IFinancialPerformance** ppOut)
{
    return CObject<CFinancialPerformance>::Create(get_Session(), cik, OnlyPubliclyDisplayed, ppOut);
}

long unsigned int CXbrl::GetCachedFinancialPerformances(IFinancialPerformanceList** ppOut)
{
    try
    {
        // LOGIP("Starting to building a the collection of cached financial performances of all companies (or based on criteria");
        CFinancialPerformanceList *pl;
        if (!FC.TrySPGetFValue("se.xbrl.GetCachedFinancialPerformances", pl))
        {
            Poco::ScopedLock<Poco::Mutex> g(_lock1);
            if (!FC.TrySPGetFValue("se.xbrl.GetCachedFinancialPerformances", pl))
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "SELECT * FROM sa.list_ListedCompanies";
                RETURNIFFAILED(CObject<CFinancialPerformanceList>::Create(get_Session(), &pl));
                for (const Row& r : q.store())
                {
                    SP<IFinancialPerformance> p;
                    if (FAILED(GetCachedFinancialPerformance(r["cik"], false, &p)))
                    {
                        // LOGE(_logger, "Failed to create cached performance instance for CIK " << r["cik"]);
                        continue;
                    }
                    pl->Add(p);
                }
                FC.SPSetFValue("se.xbrl.GetCachedFinancialPerformances", pl, 86400);
                *ppOut = pl;
                return S_OK;
            }
        }
        pl->AddRef();
        // LOGIP("Done with building a the collection of cached financial performances of all companies (or based on criteria");
        *ppOut = pl;
        return S_OK;
    }
    CATCHCLAUSE("GetCachedFinancialPerformances");
}

long unsigned int CXbrl::PersistPerformanceData()
{
    try
    {
        CPersistPerformanceData perf(get_Session());
        return perf.Run();
    }
    CATCHCLAUSE("PersistPerformanceData");
}

long unsigned int CXbrl::PersistPerformanceData(CIK cik, UID perfId)
{
    try
    {
        CPersistPerformanceData perf(get_Session());
        return perf.RunForCik(cik, perfId);
    }
    CATCHCLAUSE("PersistPerformanceData");
}

long unsigned int CXbrl::_PersistPerformanceData(IFinancialsConsolidated* pcon, unsigned long PerfId)
{
    RTRY
    CPersistPerformanceData perf(get_Session());
    return perf.RunForCik(pcon, PerfId);
    RCATCH("_PersistPerformanceData")
}

long unsigned int CXbrl::GetFinancialStatements(CIK cik, IXbrlStatementList** ppOut)
{
    try
    {
        const string funcName = "se.xbrl.finstatements";
        IXbrlStatementList* p;
        if (!FC.TrySPGetFValue(funcName, (string) cik, p))
        {
            Poco::ScopedLock<Poco::Mutex> l(_lock);
            if (!FC.TrySPGetFValue(funcName, (string) cik, p))
            {
                RETURNIFFAILED(CObject<CXbrlStatementList>::Create(get_Session(), cik, &p));
                FC.SPSetFValue(funcName, (string) cik, p, 3600);
                *ppOut = p;
                return S_OK;
            }
        }
        p->AddRef();
        *ppOut = p;
        return S_OK;
    }
    CATCHCLAUSE("GetFinancialStatements");
}

SE_CODE CXbrl::GetLatestFilingDates(CIK cik, CDate* pDateAnnual, CDate* pDateQuarterly)
{
    try
    {
        if (!pDateAnnual && !pDateQuarterly)
            return E_InvalidPointer;

        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q << "call get_LatestFilingDates(" << cik << ")";

        StoreQueryResult sqr = q.store();
        bool hasq = false, hasa = false;
        for (const Row& r : sqr)
        {
            if (hasq && hasa) break;
            PERIOD per = (PERIOD) (int) r["doc_reportperiod"];
            if (!hasa && per == PERIOD_Annually)
            {
                if (pDateAnnual)
                {
                    sql_date dt = r["filedate"];
                    pDateAnnual->set_Day(dt.day());
                    pDateAnnual->set_Month(dt.month());
                    pDateAnnual->set_Year(dt.year());
                    hasa = true;
                    continue;
                }
            }
            if (!hasq && per == PERIOD_Quarterly)
            {
                if (pDateQuarterly)
                {
                    sql_date dt = r["filedate"];
                    pDateQuarterly->set_Day(dt.day());
                    pDateQuarterly->set_Month(dt.month());
                    pDateQuarterly->set_Year(dt.year());
                    hasq = true;
                    continue;
                }
            }
        }
        while (q.more_results())
            q.store_next();
        return S_OK;
    }
    CATCHCLAUSE("GetLatestFilingDates");
}

long unsigned int CXbrl::MapFinancialQuartersToMonths(const CDate& finyearEnd, QuarterMonthMap* map)
{
    if (finyearEnd.IsEmpty())
        return E_InvalidArg;

    if (!map)
        return E_InvalidPointer;

    try
    {


        CDate dttemp = finyearEnd;
        CDate dt4qm1, dt4qm2;
        CDate dt3qm1, dt3qm2;
        CDate dt2qm1, dt2qm2;
        CDate dt1qm1, dt1qm2;

        dt4qm2 = finyearEnd;
        dt3qm2 = dt4qm2;
        dt3qm2.AddSubMonths(-3);
        dt2qm2 = dt3qm2;
        dt2qm2.AddSubMonths(-3);
        dt1qm2 = dt2qm2;
        dt1qm2.AddSubMonths(-3);


        dt1qm1 = finyearEnd;
        dt1qm1 = dt1qm1.AddSubYears(-1);
        dt1qm1 = dt1qm1.AddSubDays(1);

        dt2qm1 = dt1qm2;
        dt2qm1 = dt2qm1.AddSubDays(1);

        dt3qm1 = dt2qm2;
        dt3qm1 = dt3qm1.AddSubDays(1);

        dt4qm1 = dt3qm2;
        dt4qm1 = dt4qm1.AddSubDays(1);

        // LOGI(_logger, "Q1 begin on " << dt1qm1 << " and end on " << dt1qm2);
        // LOGI(_logger, "Q2 begin on " << dt2qm1 << " and end on " << dt2qm2);
        // LOGI(_logger, "Q3 begin on " << dt3qm1 << " and end on " << dt3qm2);
        // LOGI(_logger, "Q4 begin on " << dt4qm1 << " and end on " << dt4qm2);

        //numdays = (dt4qm2 - dt4qm1).Day;
        map->insert(make_pair(1, make_pair(dt1qm1, dt1qm2)));
        map->insert(make_pair(2, make_pair(dt2qm1, dt2qm2)));
        map->insert(make_pair(3, make_pair(dt3qm1, dt3qm2)));
        map->insert(make_pair(4, make_pair(dt4qm1, dt4qm2)));


        return S_OK;
    }
    CATCHCLAUSE("MapFinancialQuartersToMonths");

}

// take a given quarter/year and then convert to a company's quarter/year based on the company financial year end.
//long unsigned int CXbrl::MapGivenQuarterToCompanyQuarter(CDate finyearEnd, DATAPERIOD dp, DATAPERIOD* pout) 
//{
//    try
//    {
//        int coq = finyearEnd.get_Quarter();
//        // we assume that company reports 
//        int aq = 4 - coq; //quarters ahead.
//
//        int adjq = dp.Quarter - aq;
//        int year = dp.Year;
//        
//        if (adjq == 0)
//        {
//            adjq = 4;
//            year-=1;
//        }
//        else if (adjq < 0)
//        {
//            adjq = 4+adjq;
//            year-=1;
//        }
//
//        DATAPERIOD& dp = *pout;
//        switch (adjq)
//        {
//            case 1:
//                dp.Month = 1;
//                break;
//            case 2:
//                dp.Month = 4;
//                break;
//            case 3:
//                dp.Month = 7;
//                break;
//            case 4:
//                dp.Month = 10;
//                break;
//        }
//        
//        dp.Day = 1;
//        dp.Year = year;
//        dp.Quarter = adjq;
//        
//        return S_OK;
//    }
//    CATCHCLAUSE("MapGivenQuarterToCompanyQuarter");
//}

//long unsigned int CXbrl::MapCompanyQuarterToRegularQuarter(CDate finyearEnd, DATAPERIOD dp, DATAPERIOD* pout) 
//{
//    try
//    {   
//        int coq = finyearEnd.get_Quarter();
//        int qresult, year;
//        // substract from 4
//        int qsub = 4 - coq;
//        //add it to given quarter
//        int rq = dp.Quarter;
//        int qsum = rq + qsub;
//        // if qsum is greater than 4 then +year
//        year = dp.Year;
//        qresult = qsum;
//        if (finyearEnd.get_Month() < 6)
//        {
//            if (qsum > 4)
//            {
//                year = year;
//                qresult = qsum - 4;        
//            }
//            else
//            {
//                year =  year - 1;
//            }
//        }
//        else
//        {
//            if (qsum > 4)
//            {
//                year = year + 1;
//                qresult = qsum - 4;        
//            }
//        }        
//        
//        pout->Day = 1;
//        pout->Year = year;
//        pout->Quarter = qresult;
//        
//        switch (qresult)
//        {
//            case 1:
//                pout->Month = 1;
//                break;
//            case 2:
//                pout->Month = 4;
//                break;
//            case 3:
//                pout->Month = 7;
//                break;
//            case 4:
//                pout->Month = 10;
//                break;
//        }
//        
//        return S_OK;
//    }
//    CATCHCLAUSE("MapCompanyQuarterToRegularQuarter");
//}

/* used equation for below function (MapFinancialQuartersToRealMonths)

    Company reports 1st quarter
    4 - 1 = 3

    4 - 3 = 1				current year
    3 - 3 = 0 	> 4			previous year
    2 - 3 = -1	> 3			previous year
    1 - 3 = -2	> 2			previoius year

    Company reports 2nd quarter
    4 - 2 = 2

    3 - 2 = 1				current year
    4 - 2 = 2				current year
    1 - 2 = -1 	> 3			previous year
    2 - 2 = 0 	> 4			previous year

    Company reports 3rd quarter
    4 - 3 = 1

    2-1 = 1
    3-1 = 2
    4-1 = 3
    1-1 = 0 	> 4			previous year

    Company reports 4th quarter of a year, nothing to do.

    var result = company quarter - (company quarter - real quarter)
    if result is 0 then return 4
    if result is less than 0 then return 4 + (result)
    else return result 
  
 
 */


long unsigned int CXbrl::MapFinancialQuartersToRealMonths(const CDate& finyearEnd, QuarterMonthMap* map)
{
    if (finyearEnd.IsEmpty())
        return E_InvalidArg;

    if (!map)
        return E_InvalidPointer;

    try
    {
        auto quarteradjuster = [](int companyquarter)
        {
            int adjquarter;
            bool pastyear = false;
            if (companyquarter == 4)
            {
                adjquarter = 4;
            }
            else
            {
                int qdiff = 4 - companyquarter;
                qdiff = 4 - qdiff;
                if (qdiff == 0)
                {
                    adjquarter = 4;
                    pastyear = true;
                }
                else if (qdiff < 0)
                {
                    adjquarter = 4 + qdiff;
                    pastyear = true;
                }
                else
                {
                    adjquarter = qdiff;
                }
            }
            return std::make_tuple(adjquarter, pastyear);
        };

        CDate last = finyearEnd;
        int coq = finyearEnd.get_Quarter();
        auto tpl = quarteradjuster(coq);

        CDate newfinend;
        switch (std::get<0>(tpl))
        {
        case 1:
            newfinend.set_Month(3);
            break;
        case 2:
            newfinend.set_Month(6);
            break;
        case 3:
            newfinend.set_Month(9);
            break;
        case 4:
            newfinend.set_Month(12);
            break;
        }

        newfinend.set_Year(std::get<1>(tpl) ? finyearEnd.get_Year() : finyearEnd.get_Year() - 1);
        newfinend.set_Day(31);
        // stream line
        return MapFinancialQuartersToMonths(newfinend, map);

    }
    CATCHCLAUSE("MapFinancialQuartersToRealMonths");
}

SE_CODE CXbrl::GetFinancialYearEnd(CIK cik, CDate* pdate)
{
    try
    {
        /*
        CDate _latestAnnualFilingDate, _latestQuarterlyFilingDate;
        THROWIFFAILED(GetLatestFilingDates(cik, &_latestAnnualFilingDate, &_latestQuarterlyFilingDate), "Failed to get latest filing dates.");
        
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q << "select get_FinYearEnd(" << quote << cik << ")";
        StoreQueryResult sqr = q.store();
        string fye = (string)sqr[0][0];
        // split string
        string fym = fye.substr(0, 2);
        string fyd = fye.substr(2);
        CDate _finyr;
        _finyr.set_Month(strtol(fym.c_str(), nullptr, 10));
        _finyr.set_Day(strtol(fyd.c_str(), nullptr, 10));
        CDate tmp;
        if (_latestAnnualFilingDate.IsEmpty() || _latestAnnualFilingDate.IsInvalid())
        {
                tmp.set_Day(_finyr.get_Day());
                tmp.set_Month(_finyr.get_Month());
                tmp.set_Year(_latestQuarterlyFilingDate.get_Year());
                if (tmp > _latestQuarterlyFilingDate)
                    tmp = tmp.AddSubYears(-1);
                else
                    tmp = tmp.AddSubYears(1);
        }
        else
        {
                _finyr.set_Year(_latestAnnualFilingDate.get_Year());
                if (_latestAnnualFilingDate < _finyr)
                {
                    tmp = _finyr.AddSubYears(-1);
                }
                else
                {
                    int numdays = abs((_finyr - _latestAnnualFilingDate).Day);
                    tmp = _latestAnnualFilingDate;
                    tmp = tmp.AddSubDays(0-numdays);
                }
        }
        _finyr.set_Year(tmp.get_Year());
         *pdate = _finyr;
         * 
         */
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q << "call get_FinYearEndCompleted(" << cik << ")";
        StoreQueryResult sqr = q.store();
        if (sqr.size())
        {
            sql_date dt = sqr[0][0];
            pdate->set_Day(dt.day());
            pdate->set_Month(dt.month());
            pdate->set_Year(dt.year());
            while (q.more_results())
                q.store_next();
            return S_OK;
        }
        else
        {
            while (q.more_results())
                q.store_next();
            return E_NotFound;
        }
    }
    CATCHCLAUSE("GetFinancialYearEnd");
}

long unsigned int CXbrl::GetSelectedAccounts(CIK cik, const XbrlMatchId& mids, XbrlMatchId& availableMids, IXbrlRecordList** ppOut)
{
    try
    {
        if (!mids.size())
            return E_InvalidArg;

        CXbrlRecordList* plist;
        SP<IFinancialsConsolidated> pcon;
        RETURNIFFAILED(GetConsolidatedFinancials(cik, &pcon, true));
        StoreQueryResult sqr;
        CSmartConnection conn(SEDBCONN);
        RETURNIFFAILED(CObject<CXbrlRecordList>::Create(get_Session(), pcon, &plist));
        for (U32 id : mids)
        {
            SP<IXbrlIdList> xidlist;
            RETURNIFFAILED(GetMatchAccounts(id, &xidlist, cik));
            for (UID xbrlId : *xidlist)
            {
                SP<IXbrlRecord> prec;
                // LOGIP(xbrlId);
                if (ISOK(pcon->get_Record(xbrlId, &prec)))
                {
                    SP<IXbrlField> pfld;
                    // LOGI(_logger, "Getting financial record for the year " << pcon->get_FinancialYearEnd().get_Year() << " for xbrl id " << xbrlId);
                    if (ISOK(prec->get_Financials(pcon->get_FinancialYearEnd().get_Year(), &pfld)) && !pfld->get_FY().IsEmpty())
                    {
                        plist->Add(prec);
                        availableMids.push_back(id);
                        // LOGI(_logger, "Financial record found for the year " << pcon->get_FinancialYearEnd().get_Year() << " for xbrl id " << xbrlId);
                        break;
                    }
                    else
                    {
                        // LOGW(_logger, "Financial record not found for the year " << pcon->get_FinancialYearEnd().get_Year() << " for xbrl id " << xbrlId);
                    }
                }
            }
        }
        plist->AddRef();
        *ppOut = plist;
        return S_OK;
    }
    CATCHCLAUSE("GetSelectedAccounts");
}

long unsigned int CXbrl::GetRatioGroups(IFinancialRatioGroupList** ppOut)
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q << "select * from se_fin_perf_ratio_groups order by group_name";
        SP<CFinancialRatioGroupList> plist;
        RETURNIFFAILED(CObject<CFinancialRatioGroupList>::Create(get_Session(), &plist));
        for (const Row& r : q.store())
        {
            SP<CFinancialRatioGroup> grp;
            RETURNIFFAILED(CObject<CFinancialRatioGroup>::Create(get_Session(), &grp));
            grp->set_Id(r["group_id"]);
            grp->set_Name(r["group_name"]);
            grp->set_IsHidden(r["group_hidden"] == "1" ? true : false);
            plist->Add(grp);
        }
        plist.CopyTo(ppOut);
        return S_OK;
    }
    CATCHCLAUSE("GetRatioGroups");
}

long unsigned int CXbrl::GetRatiosForGroup(unsigned long groupId, IFinancialRatioList** ppOut)
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        SP<CFinancialRatioList> pl;
        RETURNIFFAILED(CObject<CFinancialRatioList>::Create(get_Session(), &pl));
        if (groupId)
            q << "select * from se_fin_perf_ratios where group_id = " << groupId << " order by perf_name";
        else
            q << "select * from se_fin_perf_ratios order by perf_name";
        for (const Row& row : q.store())
        {
            SP<CFinancialRatio> pcfr;
            if (ISOK(CObject<CFinancialRatio>::Create(get_Session(), &pcfr)))
            {
                pcfr->set_Id(row["perf_id"]);
                pcfr->set_Name(row["perf_name"]);
                pcfr->set_Description(row["perf_desc"]);
                pcfr->set_Importance(row["perf_imp"]);
                sql_double_null dval = row["perf_middlepoint"];
                pcfr->set_MiddlePoint(dval.is_null ? ER8 : dval.data);
                sql_double_null dval1 = row["perf_maxpoint"];
                pcfr->set_MaximumPoint(dval1.is_null ? MAXR8 : dval1.data);
                sql_double_null dval2 = row["perf_minpoint"];
                pcfr->set_MinimumPoint(dval2.is_null ? MINR8 : dval2.data);
                pcfr->set_IsFree(row["perf_isfree"] == "1" ? true : false);
                pcfr->set_Property("group_id", (UID) row["group_id"]);
                pl->Add(pcfr);
            }
        }
        pl.CopyTo(ppOut);
        return S_OK;
    }
    CATCHCLAUSE("GetRatiosForGroup");
}

long unsigned int CXbrl::GetMatchedAccountValue(unsigned long productId, unsigned long matchId, IXbrlRecord** ppOut)
{
    SP<IString> pstr;
    RETURNIFFAILED(get_CIK(productId, &pstr));
    return GetMatchedAccountValue(pstr->get_Buffer(), matchId, ppOut);
}

long unsigned int CXbrl::GetMatchedAccountValue(CIK cik, unsigned long matchId, IXbrlRecord** ppOut)
{
    try
    {
        SP<IFinancialsConsolidated> pcon;
        RETURNIFFAILED(GetConsolidatedFinancials(cik, &pcon, false));
        return _GetMatchedAccountValue(pcon, matchId, ppOut);
    }
    CATCHCLAUSE("GetMatchedAccountValue");
}

// private method

long unsigned int CXbrl::_GetMatchedAccountValue(IFinancialsConsolidated* pCon, unsigned long matchId, IXbrlRecord** ppOut)
{
    CXbrlAccountMatcher matcher(get_Session(), pCon, matchId);
    return matcher.GetAccountRecord(ppOut);
}

long unsigned int CXbrl::_GetMatchAccounts(unsigned long matchId, IXbrlIdList** ppOut)
{
    try
    {
        const string funcname = "GetMatchAccounts";
        IXbrlIdList* p;
        if (!FC.TrySPGetFValue(funcname, matchId, p))
        {
            std::lock_guard<std::mutex> g(_mtx);
            if (!FC.TrySPGetFValue(funcname, matchId, p))
            {
                RETURNIFFAILED(CObject<CXbrlIdList>::Create(get_Session(), &p));
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                try
                {
                    q << "call get_MatchedAccount(" << matchId << ", 0)";
                    for (const Row& row : q.store())
                    {
                        UID xid = row["xbrl_id"];
                        if (!(p)->HasItem(xid))
                            (p)->Add(xid);
                    }
                    while (q.more_results())
                    {
                        for (const Row& row : q.store_next())
                        {
                            UID xid = row["xbrl_id"];
                            if (!(p)->HasItem(xid))
                                (p)->Add(xid);
                        }
                    }
                    FC.SPSetFValue(funcname, matchId, p);
                    *ppOut = p;
                    return S_OK;
                }
                catch (...)
                {
                    while (q.more_results())
                        q.store_next();
                    throw;
                }
            }
        }
        p->AddRef();
        *ppOut = p;
        return S_OK;
    }
    CATCHCLAUSE("_GetMatchAccounts");
}

long unsigned int CXbrl::GetMatchAccounts(unsigned long matchId, IXbrlIdList** ppOut, CIK cik)
{
    try
    {
        if (!cik or !strlen(cik))
            return _GetMatchAccounts(matchId, ppOut);

        const string funcname = "GetMatchAccounts";
        IXbrlIdList* p;
        if (!FC.TrySPGetFValue(funcname, matchId, (string) cik, p))
        {
            std::lock_guard<std::mutex> g(_mtx);
            if (!FC.TrySPGetFValue(funcname, matchId, (string) cik, p))
            {
                RETURNIFFAILED(CObject<CXbrlIdList>::Create(get_Session(), &p));
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                try
                {
                    q << "call get_MatchedAccount(" << matchId << ", " << cik << ")";
                    for (const Row& row : q.store())
                    {
                        UID xid = row["xbrl_id"];
                        if (!(p)->HasItem(xid))
                            (p)->Add(xid);
                    }
                    while (q.more_results())
                    {
                        for (const Row& row : q.store_next())
                        {
                            UID xid = row["xbrl_id"];
                            if (!(p)->HasItem(xid))
                                (p)->Add(xid);
                        }
                    }
                    while (q.more_results())
                        q.store_next();

                    FC.SPSetFValue(funcname, matchId, (string) cik, p, 3600);
                    *ppOut = p;
                    return S_OK;
                }
                catch (...)
                {
                    while (q.more_results())
                        q.store_next();
                    throw;
                }
            }
        }
        p->AddRef();
        *ppOut = p;
        return S_OK;
    }
    CATCHCLAUSE("GetMatchAccounts");
}

long unsigned int CXbrl::GetPerformanceTrainingData(
                                                    unsigned long securityId,
                                                    unsigned long perfratioid,
                                                    DATAPERIOD from,
                                                    DATAPERIOD to,
                                                    se::training::ITrainingTable** ppOut)
{
    try
    {
        if (!securityId)
            return E_InvalidArg;
        SP<IString> pcik;
        RETURNIFFAILED(get_CIK(securityId, &pcik));
        return GetPerformanceTrainingData(
                                          pcik->get_Buffer(),
                                          perfratioid,
                                          from,
                                          to,
                                          ppOut);
    }
    CATCHCLAUSE("GetPerformanceTrainingData");
}

long unsigned int CXbrl::GetPerformanceTrainingData(
                                                    unsigned long securityId,
                                                    unsigned long perfratioid,
                                                    DATAPERIOD from,
                                                    DATAPERIOD to,
                                                    se::training::ITrainingTableList** ppOut)
{
    SP<IString> pcik;
    RETURNIFFAILED(get_CIK(securityId, &pcik));
    return CSpecialPerformanceTrainingData(get_Session(), perfratioid).Build(pcik->get_Buffer(), ppOut);
}

long unsigned int CXbrl::GetPerformanceTrainingData(
                                                    CIK cik,
                                                    unsigned long perfratioid,
                                                    DATAPERIOD from,
                                                    DATAPERIOD to,
                                                    se::training::ITrainingTable** ppOut)
{
    RTRY
            const string funcName = "se.xbrl.GetPerformanceTrainingData";
    ITrainingTable* pTable;
    if (!FC.TrySPGetFValue(funcName, (string) cik, perfratioid, pTable))
    {
        Poco::ScopedLock<Poco::Mutex> g(_lock1);
        if (!FC.TrySPGetFValue(funcName, (string) cik, perfratioid, pTable))
        {

            //SP<IFinancialsConsolidated> pCon;
            //RETURNIFFAILED(GetConsolidatedFinancials(cik, &pCon, true));

            // update performance tables.
            SP<IFinancialRatio> pRatio;
            //RETURNIFFAILED(CObject<CFinancialRatio>::Create (perfratioid, pCon, &pRatio));
            //pRatio->Persist();
            RETURNIFFAILED(CObject<CFinancialRatio>::Create(get_Session(), perfratioid, cik, &pRatio));

            // the company reports on quarter
            SP<IFinancialPeriod> fper;
            //RETURNIFFAILED(pCon->get_FiscalPeriodMappings(&fper));
            RETURNIFFAILED(CCaptureFiscalPeriods::Capture(cik, &fper));

            CDate dtNow = CDate::Now();

            RETURNIFFAILED(CreateTrainingTable(get_Session(), PERIOD_Monthly, PERIOD_Monthly, &pTable));
            CSmartConnection conn(SEDBCONN);

            Query q = conn->query();
            q << "SELECT * FROM sa.list_PerfRatioValuesSlim where";
            q << " cik = " << cik << " and perf_id = " << perfratioid;
            if (!from.IsEmpty())
                q << " and year >= " << from.Year;
            if (!to.IsEmpty())
                q << " and year <= " << to.Year;
            q << " and quarter > 0 ";
            q << " order by year, quarter";

            // LOGIP(q.str());

            StoreQueryResult sqr = q.store();

            OmniType prevVal;


            for (const Row& r : sqr)
            {
                // create three rows for each quarterly month.
                I16 q = r["quarter"];
                I16 y = r["year"];

                sql_double_null sqlval = r["perf_value"];
                OmniType val;
                OmniType chg;
                if (!sqlval.is_null)
                {
                    val = sqlval.data;
                    if (!prevVal.IsEmpty())
                    {
                        R8 rprev = (R8) prevVal;
                        chg = sqlval.data - rprev;
                    }
                }

                DATAPERIOD dp;
                dp.Quarter = q;
                dp.Year = y;
                fper->get_NaturalPeriod(dp, &dp);

                switch (dp.Quarter)
                {
                case 1:
                    dp.Month = 1;
                    break;
                case 2:
                    dp.Month = 4;
                    break;
                case 3:
                    dp.Month = 7;
                    break;
                case 4:
                    dp.Month = 10;
                    break;
                }
                dp.Day = 0;
                //dp.Year = year;

                // LOGI(_logger, " Ratio " << perfratioid << "'s performance value for cik = " << cik << " for period " << q << "/" << y << " is " << val);
                // LOGI(_logger, " Ratio " << perfratioid << "'s performance value for cik = " << cik << " for real period " << dp << " is " << val);

                pTable->AddRecord(dp, val, chg);

                prevVal = val;
            }

        }

        std::ostringstream ostr;
        ostr << "P_" << perfratioid << "_CIK_" << cik;
        pTable->set_Name(ostr.str().c_str());

        FC.SPSetFValue(funcName, (string) cik, perfratioid, pTable, 3600);
        *ppOut = pTable;
        return S_OK;
    }
    pTable->AddRef();
    *ppOut = pTable;
    return S_OK;

    RCATCH("GetPerformanceTrainingData");
}

long unsigned int CXbrl::SavePredictedFinancialPerformance(CIK cik, unsigned long perfId, DATAPERIOD dp, const OmniType& value)
{
    try
    {
        //        // map quarter
        //        CDate dd;
        //        RETURNIFFAILED(GetFinancialYearEnd(cik, &dd));
        //        
        //        int coq = dd.get_Quarter();
        //        // substract from 4
        //        int qsub = 4 - coq;
        //        
        //        //add it to given quarter
        //        int rq = dp.Quarter;
        //        int qsum = rq + qsub;
        //        // if qsum is greater than 4 then +year
        //        int year = dp.Year;
        //        if (qsum > 4)
        //            year = year + 1;
        //        
        //        int qresult = qsum;
        //        if (qsum > 4)
        //            qresult = qsum - 4;

        CSmartConnection conn(SEDBCONN);
        UID key;
RETRY:
        Query q1 = conn->query();
        q1 << "select cik_perf_id from se_fin_perf_cik_ratio where cik = " << cik << " and perf_id = " << perfId;
        StoreQueryResult sqr = q1.store();
        if (sqr.size())
        {
            key = sqr[0][0];
            goto COMPLETE;
        }
        else
        {
            Query q2 = conn->query();
            q2 << "insert into se_fin_perf_cik_ratio (cik, perf_id) values (" << cik << ", " << perfId << ")";
            q2.exec();
            key = q2.insert_id();
            goto RETRY;
        }

COMPLETE:
        if (!key)
        {
            throw seexception(E_DBUpdateError, "Failed to create a new cik_perf_id");
        }
        CPersistPerformanceData::UpdatePerfPredValue(conn, key, dp.Year, dp.Quarter, value.IsEmpty() ? ER8 : (R8) value);
        return S_OK;
    }
    CATCHCLAUSE("SavePredictedFinancialPerformance");

}

long unsigned int CXbrl::_GetProspectsViewFast
(
 IFinancialsConsolidated* pcon,
 unsigned long matchid,
 PERIOD interval,
 se::training::ITrainingTable*& pt
 )
{
    try
    {

        SP<IXbrlRecord> prec;
        THROWIFFAILED(_GetMatchedAccountValue(pcon, matchid, &prec), "_GetMatchedAccountValue");
        // smallest year and largest year
        I16 firstyear = prec->get_LowerYear();
        I16 recentyear = prec->get_HighestYear();

        // for the purpose of chart smoothening, we will set previous year's value when
        // a intervening year is missing.
        R8 prevval = ER8;
        DATAPERIOD finaldp;

        SP<se::training::ITrainingTable> ptbl;
        THROWIFFAILED(ITrainingTable::Create(get_Session(), &ptbl), "ITrainingTable::Create");

        for (I16 year = firstyear; year <= recentyear; year++)
        {
            R8 thisval;
            SP<IXbrlField> pfld;
            if (ISOK(prec->get_Financials(year, &pfld)))
            {
                switch (interval)
                {
                case PERIOD_Annually:
                {
                    DATAPERIOD dp;
                    dp.Year = year;
                    const OmniType& val = pfld->get_FY();
                    if (val.IsEmpty())
                    {
                        if (prevval != ER8)
                        {
                            thisval = prevval;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        thisval = val;
                    }
                    ptbl->AddRecord(dp, thisval);
                    prevval = thisval;
                    finaldp = dp;
                    break;
                }
                case PERIOD_Quarterly:
                {
                    I16 qlast = prec->get_LatestQuarter();
                    for (I16 q = 1; q <= 4; q++)
                    {
                        if (year == recentyear && q > qlast)
                            break;

                        DATAPERIOD dp;
                        dp.Year = year;
                        dp.Quarter = q;
                        const OmniType& val = pfld->get_Q(q);
                        if (val.IsEmpty())
                        {
                            if (recentyear == year)
                            {
                                // LOGIP("Completed");
                                goto COMPLETE;
                            }
                            if (prevval != ER8)
                            {
                                thisval = prevval;
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            thisval = val;
                        }
                        //DATAPERIOD realdp;
                        //MapCompanyQuarterToRegularQuarter(pcon->get_FinancialYearEnd(), dp, &realdp);
                        ptbl->AddRecord(dp, thisval);
                        prevval = thisval;
                        finaldp = dp;
                    }
                    break;
                }
                }
            }
        }

COMPLETE:

        if (!ptbl->get_Count())
        {
            // LOGEP("Not data to return.");
            THROWIFFAILED(E_NoDataToReturn, "No data to return");
        }

        // get all predictions setup for this product.
        SP<ISecurityAPI> psecapi;
        THROWIFFAILED(SecOpen(get_Session(), get_Session()->get_App()->get_Configurator()->get_DefaultMarketDataProvider(), &psecapi), "Failed to open security api.");

        UID productId = GetProductId(pcon->get_Cik());
        SP<IPredictionSetupList> pSetups;
        THROWIFFAILED(
                      psecapi->get_SecurityPredictionSetups(productId, matchid, &pSetups),
                      "Failed to obtain trainingset data for the security.");

        if (!pSetups->get_Count())
        {
            // LOGEP("No prediction setups for this product.");
            THROWIFFAILED(E_NoDataToReturn, "No data to return");
        }

        if (PERIOD_Quarterly == interval)
        {
            switch (finaldp.Quarter)
            {
            case 4:
                finaldp.Quarter = 1;
                finaldp.Year += 1;
                break;
            default:
                finaldp.Quarter += 1;
                break;
            }
        }
        else
        {
            finaldp.Year += 1;
        }

        SP<IPredictionSetup> pSetup;

        // the ratio id for revenue growth is 21.

        for (IPredictionSetup* p : *pSetups)
        {
            const OmniType& mid = p->get_Property("MatchId");
            if ((UID) mid == matchid)
            {
                pSetup = p;
                break;
            }
        }

        if (!pSetup)
        {
            // LOGEP("No prediction setup assigned to match id this product.");
            THROWIFFAILED(E_NoDataToReturn, "No data to return");
        }

        const TRAINING_RESULT& result = pSetup->get_ResultInfo();

        THROWIFFAILED(pSetups->FindByResult(result, &pSetup), "Error getting extended training dataset");
        //
        SP<IExTrainingTable> pExtTable;
        pSetup->set_Property("ProductId", this->GetProductId(pcon->get_Cik()));
        THROWIFFAILED(pSetup->get_TrainingData(DATAPERIOD(), DATAPERIOD(), &pExtTable), "Failed to obtain extended table.");

        THROWIFFAILED(RunRegressionTree(pExtTable, finaldp), "Failed while running the regression tree.");

        SP<IExTrainingRecord> pfut;
        THROWIFFAILED(pExtTable->get_FutureRecord(&pfut), "Failed to get future record from extended training table.");

        const OmniType& predictedValue = pfut->get_Result();

        SE_CODE retcode = SavePredictedFinancialPerformance(
                                                            pcon->get_Cik(),
                                                            result.Identifier,
                                                            finaldp,
                                                            predictedValue
                                                            );

        THROWIFFAILED(retcode, "SavePredictedFinancialPerformance");

        R8 newval = prevval + (prevval * (R8) predictedValue / 100);
        ptbl->set_FutureRecord(finaldp, newval, predictedValue);

        ostringstream ostr;
        ostr << "predtables-" << pcon->get_Cik();
        pExtTable->Write(ostr.str().c_str());

        ptbl.CopyTo(&pt);

        return S_OK;
    }
    CATCHCLAUSE("_GetProspectsViewFast")
}

long unsigned int CXbrl::_GetProspectsView(unsigned long productId, unsigned long matchid, PERIOD interval, se::training::ITrainingTable*& pt)
{
    try
    {

        SP<IString> pCik;
        SP<IFinancialsConsolidated> pcon;
        THROWIFFAILED(get_CIK(productId, &pCik), "get_CIK");
        THROWIFFAILED(GetConsolidatedFinancials(pCik->get_Buffer(), &pcon, true), "GetConsolidatedFinancials");

        auto rCode = _GetProspectsViewFast(pcon, matchid, interval, pt);
        return rCode;
    }
    CATCHCLAUSE("GetProspectsView");
}

SE_CODE CXbrl::GetProspectsView
(
 unsigned long productId,
 unsigned long matchid,
 PERIOD interval,
 se::training::ITrainingTable** ppOut
 )
{
    try
    {

        if (matchid != 25)
            return E_NotImpl;

        if (interval != PERIOD_Quarterly)
            return E_NotImpl;

        SP<se::config::IConfigurator> pCfg;
        THROWIFFAILED(GetConfigurator(&pCfg), "GetConfigurator");
        ITrainingTable* ptbl = nullptr;
        const string funcName = "se.xbrl.GetProspectsView";
        if (!FC.TrySPGetFValue(funcName, productId, matchid, (I32) interval, ptbl))
        {
            Poco::ScopedLock<Poco::Mutex> g(_lock1);
            if (!FC.TrySPGetFValue(funcName, productId, matchid, (I32) interval, ptbl))
            {
                THROWIFFAILED(_GetProspectsView(productId, matchid, interval, ptbl), "Failed at _GetProspectsView.");
                // save it.
                FC.SPSetFValue(funcName, productId, matchid, (I32) interval, ptbl, 3600);
                *ppOut = ptbl;
                return S_OK;
            }
        }
        ptbl->AddRef();
        *ppOut = ptbl;
        return S_OK;
    }
    CATCHCLAUSE("GetProspectsView");
}

SE_CODE CXbrl::RunRegressionTree(IExTrainingTable* pExTable, const DATAPERIOD& newper)
{
    try
    {
        using namespace se::math;

        for (int i = 0; i != 4; ++i)
        {
            OPT settings;
            switch (i)
            {
            case 0:
                settings = OPT_RulesInstanceAuto;
                break;
            case 1:
                settings = OPT_AllowUnbiasedRules;
                break;
            case 2:
                settings = OPT_UseRulesAndInstances;
                break;
            case 3:
                settings = OPT_UseRulesOnly;
                break;
            }

            CCubist cube;
            COptions& options = cube.get_Options();

            options.set_Option(OPT_AllowUnbiasedRules);
            options.set_Option(settings);
            options.set_NumberOfNearestNeighbors(9);
            //options.set_MaximumNumOfRules(500);
            options.set_ExtrapolationLimit(5);
            options.set_CrossValidationFolds(10);
            //options.set_Sampling(50);



            CAttributeDiscrete attr("QTR");
            attr.AddItem("Q1");
            attr.AddItem("Q2");
            attr.AddItem("Q3");
            attr.AddItem("Q4");
            cube.AddAttribute(attr);


            int attrCount = pExTable->get_AttributeFieldsCount();
            for (int i = 0; i != attrCount; ++i)
            {
                // add attributes
                CAttributeNumeric attr(pExTable->get_AttributeFieldName(i));
                cube.AddAttribute(attr);
            }

            CAttributeNumeric attrTarget("RESULT");
            cube.AddAttribute(attrTarget, true);

            cube.BeginLoadTrainData();
            R8 predictedvalue;
            R8 lastKnownresult;

            R8 qrevDir[4];
            /* Incremental averaging formula
             * Mn = Mn-1 + (An - Mn-1)/n
             */
            R8 qCnt[4] = {1.0, 1.0, 1.0, 1.0};

            for (IExTrainingRecord* prec : *pExTable)
            {
                const DATAPERIOD& dp = prec->get_Period();
                const DATAPERIOD& dpRecent = pExTable->get_RecentResultPeriod();

                CDate dt(dp.Year, dp.Month, dp.Day);
                //CDate dtTmp = dt;
                //dtTmp = dtTmp.AddSubQuarters(3);
                CQuarter qtr(dt);
                ostringstream qtrStr;
                qtrStr << "Q" << qtr.get_Quarter();
                CSTR szQtr = qtrStr.str().c_str();

                cube.BeginAddRecord();
                cube.FeedAttrValues("QTR", szQtr);
                for (int i = 0; i != pExTable->get_AttributeFieldsCount(); ++i)
                {
                    OmniType ot = prec->get_Value(i);
                    if (ot.IsEmpty())
                        cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), "?");
                    else
                        cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), (R8) prec->get_Value(i));
                }
                const OmniType& oresult = prec->get_Result();
                if (oresult.IsEmpty())
                    cube.FeedAttrValues("RESULT", "?");
                else
                {
                    lastKnownresult = (R8) oresult;
                    cube.FeedAttrValues("RESULT", lastKnownresult);

                }
                cube.EndAddRecord();
                int qindex = qtr.get_Quarter() - 1;
                auto prevMean = qrevDir[qindex];
                qrevDir[qindex] = prevMean + (lastKnownresult - prevMean) / qCnt[qindex];
                qCnt[qindex]++;

            }

            if (cube.EndLoadTrainData())
            {
                cube.BeginPredict();

                ostringstream qtrStr;
                qtrStr << "Q" << newper.Quarter;
                CSTR szQtr = qtrStr.str().c_str();
                cube.FeedAttrValues("QTR", szQtr);

                SP<IExTrainingRecord> pfut;
                if (ISOK(pExTable->get_FutureRecord(&pfut)))
                {
                    for (int i = 0; i != pExTable->get_AttributeFieldsCount(); ++i)
                    {
                        const auto& fv = pfut->get_Value(i);
                        if (fv.IsEmpty())
                            cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), "?");
                        else
                            cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), (R8) pfut->get_Value(i));
                    }
                }
                else
                {
                    for (int i = 0; i != pExTable->get_AttributeFieldsCount(); ++i)
                    {
                        cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), "?");
                    }
                }

                predictedvalue = cube.EndPredict();

                if (pfut)
                {
                    //                    auto direction = qrevDir[newper.Quarter-1];
                    //                    if (direction < 0 && predictedvalue > 0)
                    //                        pfut->set_Result(0-predictedvalue);
                    //                    else
                    pfut->set_Result(predictedvalue);
                }
                return S_OK;
            }

        }

        throw seexception(E_AlgorithmError, "Regression tree algorithm failed to compute.");

    }
    CATCHCLAUSE("RunRegressionTree");
}

/* Table: se_fin_perf_gics_values
    Columns:
    perf_gics_id	bigint(20) PK
    year	smallint(6)
    quarter	smallint(6)
    value	decimal(12,4)
 */
sql_create_4(se_fin_perf_gics_values, 3, 4, sql_bigint, perf_gics_id, sql_smallint, year, sql_smallint, quarter, sql_decimal, value);

SE_CODE CXbrl::GetGicsRevenueGrowth(bool recompute, UID gicId, PERIOD interval, IFinPeriodList** ppOut, OmniType* poutAverage)
{
    try
    {
        // first get a list of products which are in the same gic id 

        const UID perfId = 21; //TODO eventually, perfid can be passed as a parameter and make this function generic.
        CSmartConnection conn(SEDBCONN);
        if (recompute)
        {
            Query q = conn->query();
            q << "select product_id, cik, product_marketcap ";
            q << "from list_ListedCompaniesExtended where product_gic_id = " << gicId;
            //q << " and product_marketcap > 0 order by product_marketcap desc" ;

            StoreQueryResult sqr = q.store();
            if (!sqr.size())
            {
                q = conn->query();
                q << "select product_id, cik, product_marketcap ";
                q << "from list_ListedCompaniesExtended where parent_gics_id = " << gicId;
                //q << " and product_marketcap > 0 order by product_marketcap desc" ;        
                sqr = q.store();
            }
            if (!sqr.size())
                return E_NoDataToReturn;



            typedef std::map< DATAPERIOD, pair<OmniType, OmniType> > PV;
            int numberOfProducts = 0;
            PV pv;
            typedef vector<se_fin_perf_gics_values> GicsValues;
            GicsValues gvals;

            // what is most recently completed quarter ?
            CDate dtNow = CDate::Now();
            QuarterMonthMap qmap;
            CDate dtLastDay(dtNow.get_Year(), 12, 31);
            MapFinancialQuartersToMonths(dtLastDay, &qmap);
            int quarter = dtNow.get_Quarter();
            const CDate& dtEnd = qmap[quarter].second;
            if (dtNow <= dtEnd)
                quarter--;
            int year = qmap[quarter].first.get_Year();

            // check how many companies where included in this calculation. The comparison is on
            // number of records returned by the above sql againsts num_of_cos in the table.
            q = conn->query();
            q << "select perf_gics_id, num_of_cos from se_fin_perf_gics_ratio where gics_id = " << gicId;
            q << " and perf_id = " << perfId;
            auto sqrnum = q.store();
            UID perfgicsid = 0;
            if (sqrnum.size())
            {
                perfgicsid = sqrnum[0][0];
                if ((I32) sqrnum[0][1] == sqr.size())
                {
                    // check for last quarter
                    q = conn->query();
                    q << "select * from se_fin_perf_gics_values where perf_gics_id = " << perfgicsid;
                    q << " order by year desc, quarter desc";
                    auto sqrq = q.store();
                    if (sqrq.size())
                    {
                        int lastq = sqrq[0]["quarter"];
                        int lasty = sqrq[0]["year"];
                        if (lastq == quarter && lasty == year)
                        {
                            // populate data and return.
                            goto COMPLETE;
                        }
                    }
                }
            }

            for (const Row& rp : sqr)
            {
                // now, for each product id, get the revenue (annual or quarterly - interval param)
                CIK cik = (CSTR) rp["cik"];
                q = conn->query();
                q << "SELECT year, quarter, ifnull(perf_value, perf_pred_value) ";
                q << "FROM list_PerfRatioValuesOnly where cik = " << cik << " and perf_id = " << perfId;
                if (interval == PERIOD_Annually)
                    q << " and quarter = 0 ";
                else
                    q << " and quarter > 0 ";

                // if revenue growth is available in the table, then increment numberOfProducts for this product.
                auto sqrpratio = q.store();
                if (!sqrpratio.size())
                    continue;


                SP<IFinancialPeriod> fper;
                if (FAILED(CCaptureFiscalPeriods::Capture(cik, &fper)))
                    continue;

                numberOfProducts++;

                // for each interval (time period), get the revenue growth
                // get the value from pv map for the same period and then add it.
                for (const Row& rv : sqrpratio)
                {
                    DATAPERIOD dpreg;
                    DATAPERIOD dp;
                    dp.Year = rv[0];
                    if (interval == PERIOD_Annually)
                    {
                        dp.Quarter = 0;
                        dpreg = dp;
                    }
                    else
                    {
                        dp.Quarter = rv[1];
                        if (FAILED(fper->get_NaturalPeriod(dp, &dpreg)))
                        {
                            continue;
                        }
                    }
                    sql_double_null dv = rv[2];
                    OmniType val;
                    if (dv.is_null)
                        continue;
                    val = dv.data;
                    auto dpfind = pv.find(dpreg);
                    if (dpfind == pv.end())
                        pv.insert(make_pair(dpreg, make_pair(val, (OmniType) (R8) 1)));
                    else
                    {
                        OmniType& v = dpfind->second.first;
                        v += val;
                        OmniType& c = dpfind->second.second;
                        c += (R8) 1.0;
                    }
                }
            }

            if (!pv.size())
            {
                // LOGE(_logger, "No performance data to return for gicsid " << gicId << " and perfid " << perfId);
                return E_NoDataToReturn;
            }

            // table structures
            /* 
             * Table: se_fin_perf_gics_ratio
                Columns:
                perf_gics_id	bigint(20) AI PK
                gics_id	int(11)
                perf_id	bigint(20)
                num_of_cos	int(11)
                lastupdated	timestamp
             *  
             * Table: se_fin_perf_gics_values
                Columns:
                perf_gics_id	bigint(20) PK
                year	smallint(6)
                quarter	smallint(6)
                value	decimal(12,4)
             * 
             */
            // insert and get a record for gics/perfid combination.
            if (!perfgicsid)
            {
                q = conn->query();
                q << "insert into se_fin_perf_gics_ratio (gics_id, perf_id, num_of_cos) values (";
                q << gicId << ", " << perfId << ", " << numberOfProducts << ")";
                q.exec();
                perfgicsid = q.insert_id();
            }

            q = conn->query();
            q << "delete from se_fin_perf_gics_values where perf_gics_id = " << perfgicsid;
            if (interval == PERIOD_Annually)
                q << " and quarter = 0";
            else
                q << " and quarter > 0";
            q.execute();

            // loop pv map, and then get the period and value
            // divide value by numberOfProducts
            for (auto& pr : pv)
            {
                OmniType v = pr.second.first;
                v /= pr.second.second;
                // save this value
                se_fin_perf_gics_values gval(perfgicsid, pr.first.Year, pr.first.Quarter, (R8) v);
                gvals.push_back(gval);
            }
            // now save in the table.
            conn->query().replace(gvals.begin(), gvals.end()).execute();
        }

COMPLETE:
        if (ppOut)
        {
            // get the records from the tables for perfId. also obtain a geometric average.
            // add this information to pGicsGrowthData;
            Query q = conn->query();
            if (interval == PERIOD_Annually)
                q << "select * from list_PerfGicsRatioValues where gics_id = " << gicId << " and perf_id = " << perfId << " and quarter = 0";
            else
                q << "select * from list_PerfGicsRatioValues where gics_id = " << gicId << " and perf_id = " << perfId << " and quarter > 0";
            auto sqrratio = q.store();
            SP<IFinPeriodList> pGicsGrowthData;
            RETURNIFFAILED(CObject<CFinPeriodList>::Create(get_Session(), &pGicsGrowthData));
            OmniType total = (R8) 0.0;
            for (const Row& r : sqrratio)
            {
                DATAPERIOD dp;
                dp.Year = r["year"];
                dp.Quarter = r["quarter"];
                OmniType value = (R8) r["value"];
                auto tpl = make_tuple(dp, value);
                pGicsGrowthData->Add(tpl);
                total += value;
            }

            if (poutAverage)
            {
                // also get geometric mean of all values and set that to poutAverage.
                OmniType& mean = *poutAverage;
                mean = total / (OmniType) (R8) sqrratio.size();
            }
            pGicsGrowthData.CopyTo(ppOut);
            // return both gics data and average.
        }
        return S_OK;

    }
    CATCHCLAUSE("GetGicsRevenueGrowth");
}

long unsigned int CXbrl::GetConfigurator(se::config::IConfigurator** ppOut)
{
    SP<IApp> papp;
    RETURNIFFAILED(SEOpenApp(&papp));
    return papp->get_Configurator(ppOut);
}

long unsigned int CXbrl::ProcessProductKeyStats()
{
    RTRY

            const string findexmarker = "ProcessProductKeyStats.txt";
    string line;
    std::ifstream fin(findexmarker);
    if (fin.is_open())
    {
        getline(fin, line);
        fin.close();
    }

    int startPosition = 0;
    if (!line.empty())
        startPosition = strtol(line.c_str(), nullptr, 10);

    std::ofstream fout(findexmarker);
    if (!fout.is_open())
    {
        // LOGEP("ProcessProductKeyStats.txt file cannot be opened. Cannot write last successful index.");
    }


    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q << "SELECT product_id FROM sa.list_ListedCompanies order by product_id";
    DATAPERIOD dp;
    CDate dtNow = CDate::Now();
    dp.Quarter = dtNow.get_Quarter();
    dp.Year = dtNow.get_Year();
    StoreQueryResult sqr = q.store();
    //for (const Row& r : sqr)
    for (int i = startPosition; i != sqr.size(); ++i)
    {
        // record current index
        fout.seekp(0, ios::beg);
        fout << i << endl;

        const Row& r = sqr[i];
        UID p = r["product_id"];
        if (FAILED(ProcessProductKeyStats(conn, p, dp)))
        {
            // LOGE(_logger, "Failed to process key stats for product " << p );
            continue;
        }
    }
    // delete the
    fout.close();
    remove(findexmarker.c_str());

    RCATCH("ProcessProductKeyStats");
}

long unsigned int CXbrl::ProcessProductKeyStats(CSmartConnection& conn, unsigned long prodId, IFinancialsConsolidated* pcon, const DATAPERIOD& dp)
{
    RTRY

    // only quarterly for now.
    ITrainingTable* ptbl = nullptr;
    SE_CODE retCode = _GetProspectsViewFast
            (
             pcon,
             25,
             PERIOD_Quarterly,
             ptbl
             );

    if (ISOK(retCode))
    {
        R8 variance, stddev;
        R8 amean, gmean;
        R8 expectedValue, expectedRate;
        SP<ITrainingRecord> pfut;
        if (FAILED(ptbl->get_FutureRecord(&pfut)))
        {
            // LOGE(_logger, "Failed to obtain future record for " << prodId);
            expectedValue = expectedRate = 0;
        }
        else
        {
            expectedValue = pfut->get_Value().IsEmpty() ? 0 : (R8) pfut->get_Value();
            expectedRate = pfut->get_Change().IsEmpty() ? 0 : (R8) pfut->get_Change();
        }

        if (FAILED(ptbl->get_AMean(true, &amean, true)))
        {
            // LOGE(_logger, "Failed to obtain amean for " << prodId);
            amean = 0;
        }
        if (FAILED(ptbl->get_GMean(true, &gmean, true)))
        {
            // LOGE(_logger, "Failed to obtain gmean for " << prodId);
            gmean = 0;
        }
        if (FAILED(ptbl->get_Variance(true, &variance)))
        {
            // LOGE(_logger, "Failed to obtain variance for " << prodId);
            variance = 0;
        }

        // std dev is sqrt of variance
        stddev = sqrt(variance);

        Query q = conn->query();
        q << "REPLACE se_sec_product_keystats (product_id, year, quarter, expected_return_value, expected_return_rate, return_gmean, return_amean, variance, stddev, lastupdated ) ";
        q << "VALUES ( " << prodId << ", " << dp.Year << ", " << dp.Quarter << ", ";
        if (std::isnan(expectedValue) || std::isinf(expectedValue))
            q << "NULL, ";
        else
            q << expectedValue << ", ";
        if (std::isnan(expectedRate) || std::isinf(expectedRate))
            q << "NULL, ";
        else
            q << expectedRate << ", ";
        if (std::isnan(gmean) || std::isinf(gmean))
            q << "NULL, ";
        else
            q << gmean << ", ";
        if (std::isnan(amean) || std::isinf(amean))
            q << "NULL, ";
        else
            q << amean << ", ";
        if (std::isnan(variance) || std::isinf(variance))
            q << "NULL, ";
        else
            q << variance << ", ";
        if (std::isnan(stddev) || std::isinf(stddev))
            q << "NULL, ";
        else
            q << stddev << ", ";
        q << " NOW() )";
        // LOGIP(q.str());
        try
        {
            q.exec();
        }
        catch (...)
        {
            throw;
        }

        if (ptbl)
            ptbl->DecRef();

        return S_OK;
    }
    return retCode;

    RCATCH("ProcessProductKeyStats(ifinancialconsolidated)")
}

long unsigned int CXbrl::ProcessProductKeyStats(CSmartConnection& conn, unsigned long prodId, const DATAPERIOD& dp)
{
    RTRY

    SP<IString> pCik;
    SP<IFinancialsConsolidated> pcon;
    RETURNIFFAILED(get_CIK(prodId, &pCik));
    RETURNIFFAILED(GetConsolidatedFinancials(pCik->get_Buffer(), &pcon, true));

    return ProcessProductKeyStats(conn, prodId, pcon, dp);

    RCATCH("ProcessProductKeyStats(conn, prodid)");
}

long unsigned int CXbrl::ProcessProductKeyStats(unsigned long prodId)
{

    if (!prodId)
        return ProcessProductKeyStats();

    try
    {


        CSmartConnection conn(SEDBCONN);
        DATAPERIOD dp;
        CDate dtNow = CDate::Now();
        dp.Quarter = dtNow.get_Quarter();
        dp.Year = dtNow.get_Year();
        return ProcessProductKeyStats(conn, prodId, dp);
    }
    CATCHCLAUSE("ProcessProductKeyStats(prodid)");
}

long unsigned int CXbrl::CaptureFinancialPeriods(unsigned long prodId, IFinancialPeriod** ppOut)
{
    SP<IString> pstr;
    RETURNIFFAILED(get_CIK(prodId, &pstr));
    return CCaptureFiscalPeriods::Capture(pstr->get_Buffer(), ppOut);
}

long unsigned int CXbrl::CaptureFinancialPeriods(CIK cik, IFinancialPeriod** ppOut)
{
    return CCaptureFiscalPeriods::Capture(cik, ppOut);
}

sql_create_2(recCompProfile, 2, 1, sql_bigint, secondary, sql_bool, IsComplement);

class CCompetitionProfile : public ICompetitionProfile
{
private:
    recCompProfile _profile;
public:

    CCompetitionProfile(const recCompProfile& profile)
    {
        _profile = std::move(profile);
    }

    virtual const char* get_Cik()
    {
        return (CSTR) _profile.secondary;
    }

    virtual bool get_IsComplementor()
    {
        return _profile.IsComplement;
    }

};

long unsigned int CXbrl::GetCompetitionProfiles(CIK primary, ICompetionProfileList** ppOut)
{
    RTRY

    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q << "SELECT secondary_cik, case ComplOrSuppl  when 'C' then true else false end as IsComplement FROM sa.se_fin_perf_competionprofiles where primary_cik = " << primary;
    typedef vector<recCompProfile> records;
    records recs;
    q.storein(recs);
    //SP<se::data::CMysqlTable<records, CCompetitionProfile>> ptbl;
    //RETURNIFFAILED(se::data::CMysqlTable<records, CCompetitionProfile>::Create(recs, &ptbl));
    //ptbl.CopyTo(ppOut);
    return S_OK;

    RCATCH("GetCompetitionProfiles")

}
