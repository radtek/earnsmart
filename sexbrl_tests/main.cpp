/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on July 8, 2012, 11:02 PM
 */
#include <string.h>
#include <fstream>
#include <fstream>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <libxml/nanohttp.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlstring.h>

#include <cstdlib>
#include <boost/filesystem.hpp>
#include "../secommon.h"
#include "CXbrlListener.h"
//#include <log4cxx/log4cxx.h>
//#include <log4cxx/logger.h>
//#include <log4cxx/basicconfigurator.h>
//#include <log4cxx/propertyconfigurator.h>
//#include <log4cxx/helpers/exception.h>

#include <../semysql/CSEConnections.h>
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"

#include "../semain/semain.h"

#include "../seglobal/CJsonSerializedData.h"

#include <../semysql/CSEConnections.h>
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/properties.hpp>
#include <map>
#include "../seglobal/CStringUtility.h"
#include<boost/regex.hpp>
#include <libxml2/libxml/xmlreader.h>
#include <sstream>
#include <iostream>
#include <boost/date_time.hpp>
#include "../seglobal/CQuarter.h"
#include "../sesecurities/sesecuritiesapi.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "xbrldatawork.h"
#include "../seglobal/exceptions.h"
#include "../seglobal/CFinancialPeriod.h"
#include "../seeconomy/seeconomapi.h"


//log4cxx:://LoggerPtr logger;

//using namespace log4cxx;
using namespace se::securities;

//log4cxx:://LoggerPtr logger(log4cxx::Logger::getLogger("sexbrltests"));

void make_tree();
void performanceRatio();
void  DebtAssetRatio();
void CurrentRatio();
void HistoricalRatio();

#define SEDBCONN        CSEConnections::GetMain()
#define SEDBBKP         CSEConnections::GetBackup()

//#define LOGI    LOG4CXX_INFO
//#define LOGW    LOG4CXX_WARN
//#define LOGE    LOG4CXX_ERROR
//#define LOGF    LOG4CXX_FATAL
//
//
//#define LOGIP(str)    LOG4CXX_INFO(logger, str)
//#define LOGWP(str)    LOG4CXX_WARN(logger, str)
//#define LOGEP(str)    LOG4CXX_ERROR(logger, str)
//#define LOGFP(str)    LOG4CXX_FATAL(logger, str)


using namespace std;
using namespace se;
using namespace se::xbrl;
using namespace se::data;
using namespace se::securities;
using namespace mysqlpp;
using namespace boost;	

void PopulateXbrlHierarchy();
void DisplayFinStatement();
void DisplayFinStatement(UID statementId);
void ProcessStatement(UID);
void TestConsolidatedFinancials(CIK cik, UID xbrlid = 0 , UID segmid = 0);
void TestDateType();
void TestEdgarLookup(CSTR cikOrSymbl);
void TestCompanySearch();
void TestFinancialRatios(CIK cik);
void TestFinancialRatios1(CIK cik);
void TestFormulaParser();
void TestStatements(CIK cik);
void TestPerformanceRatioPersistance(CIK cik = 0, UID PerfId = 0);
void TestCachedFinancialRatios(CIK cik);
void TestPopulateCachedPerformanceOfAll();
void TestWebScreener();
void TestRSSFeedReadWrite();
void TestRevenueProspects();
void TestGicsRatioCalculationAndSaving(UID gicId);
void TestEconomicSourcingData();
void TestProductKeyStats(UID prodId = 0);
void TestLeaks();

int main(int argc, char** argv) 
{
    {
    SP<se::IApp> pApp;
    SEGetApp(argv[0], &pApp);
    
    ////LoggerPtr logger = Logger::getLogger("sexbrltests");
    
    
    /*
    CSmartConnection conn(SEDBBKP);
    Query q = conn->query();
    q << "SELECT COUNT(*) FROM sa.se_edgar_formrecs;";
    for (const Row& r : q.store())
    {
        cout << r[0] << endl;
    }
     */
    
//    TestLeaks();
    
//    TestEconomicSourcingData();
    
    
//    CFinancialPeriod per;
//    DATAPERIOD dpFYE;
//    dpFYE.Month = 6;
//    dpFYE.Day = 29;
//    DATAPERIOD dpN, dpC;
//    dpN.Year = 2014;
//    dpN.Quarter = 3;
//    per.get_CompanyPeriod(dpFYE, dpN, &dpC);
//    
//    cout << "CoPeriod: " << dpC << endl;
    
    
    
//    TestGicsRatioCalculationAndSaving(27);    
    
    //TestRevenueProspects();
    
    
    //UploadSecurityWebLinks("/home/santony/Documents/smartearn/Documents/dumps/securitylist-newspages.csv");
    
    
    //TestRSSFeedReadWrite();
    
    
    
    //TestWebScreener();
    //TestPopulateCachedPerformanceOfAll();
    
    
//    RepopulateListedCompanies(
//        {"/home/santony/Documents/smartearn/Documents/fundamentals/2014-06-16/amex.csv", 
//                "/home/santony/Documents/smartearn/Documents/fundamentals/2014-06-16/nasdaq.csv", 
//                "/home/santony/Documents/smartearn/Documents/fundamentals/2014-06-16/nyse.csv"});
//    
//    
    //TestDateType();
    
    //ProcessStatement(110429);
    
    //ProcessStatement(9);
    
    //ProcessStatement(87190);
    
    //TestEdgarLookup("AAPL");
    
//    SP<IString> str;
//    CDate::Now().AsDayString(&str);
//    cout << str->get_Buffer() << endl;
    
    
    //PopulateXbrlHierarchy();
    
    //HistoricalRatio();
//    DebtAssetRatio();
    //CurrentRatio();
    
//    if (argc == 2)
//    {
//        TestStatements(argv[1]);
//        return 0;
//    }
    
    //TestStatements("0000789019");
    //TestStatements("0001578732");
    
    //TestCompanySearch();
    
    
    
    //TestConsolidatedFinancials("1018840"); //aapl
    //TestConsolidatedFinancials("0000858877", 28901); //csco
    //TestConsolidatedFinancials("0000858877", 16637); //csco
    //TestConsolidatedFinancials("0000858877", 2661679); //csco
    //TestConsolidatedFinancials("0000858877", 23787); //csco
    //TestConsolidatedFinancials("0000858877", 28901, 25445); //csco
    //TestConsolidatedFinancials("0001341439"); //orcl
    //TestConsolidatedFinancials("0000796343");     // adbe
    //TestConsolidatedFinancials("0001444275");
    //TestConsolidatedFinancials("0000789019");  //msft
    //TestConsolidatedFinancials("0000072971"); //wfc
    
    //TestFinancialRatios("1018840");
    //TestFinancialRatios("0000066740");
    
    //TestFinancialRatios("0000066740");
    //TestFinancialRatios("0000200406");
    //TestFinancialRatios("0000789019");
    
    //TestFormulaParser();
    
    
    //TestCachedFinancialRatios("0001528903");
    

    //TestPerformanceRatioPersistance();
    //TestPerformanceRatioPersistance("789019", 0);
    //TestPerformanceRatioPersistance("0000789019", 22);
    
    TestProductKeyStats();
    
/*
    else if (argc == 3)
    {
        UID xbrlid = atoi(argv[2]);
        TestConsolidatedFinancials(argv[1], xbrlid);
    }
    else
    {
        TestConsolidatedFinancials("0000858877"); //csco
        //TestConsolidatedFinancials("0001341439"); //orcl
        //TestConsolidatedFinancials("0000796343");     // adbe
        //TestConsolidatedFinancials("0001444275");
        //TestConsolidatedFinancials("0000789019");  //msft
        //TestConsolidatedFinancials("0000072971"); //wfc
    }
  */  
    //DisplayFinStatement();
    //DisplayFinStatement(4846199);
    
    //CopyInstanceSegmentData();
    
    
    
//    SP<ISecurityAPI> api;
//    se::securities::SecOpen(&api);
//    
//    DATAPERIOD dt;
//    dt.Quarter = 2;
//    dt.Year = 2010;
//    UID uidProdId = 1509;
//    
//    
//    /***** testing statement list and populating a single statement */
//    SP<IROSPList<IXbrlStatement>> pStatementNames;
//    auto r1 = pXbrl->GetStatementNames(uidProdId, dt, XFT_10Q, &pStatementNames);
//    if (r1 == S_Success)
//    {
//        for (IXbrlStatement* r : *pStatementNames._ptr)
//        {
//            LOGIP( r->get_Name() );
//            
//            // get statement
//            SP<IXbrlDataNode> pStatement;
//            r1 = pXbrl->GetStatement(r->get_Id(), &pStatement);
//            if (r1 == S_Success)
//            {
//                LOGI(logger, "Total : " << pStatement->get_Total())
//            }
//            
//        }
//    }    
//    
//    SP<ISecurityDescription>Psec;
//    
//    
//    api->CreateSecurityDescription(&Psec);
//    Psec->set_Id(uidProdId);
//    XFT formType = XFT_10Q;
//    SP<IXbrlDataTree> pStatementTree;
//    
//    ErrorId ret = pXbrl->GetStatements(Psec, dt, formType, &pStatementTree);
//    
//    SP<IROSPList<IXbrlDataNode> > pDataNode;
//    pStatementTree->get_Statements(&pDataNode);
//     
//    SP<IXbrlDataNode> pNdoe;
//    
//    
////    SP<IROSPList<IXbrlStatement> > statement;
////    ErrorId ret = pXbrl->GetStatementNames(uidProdId, dt, XFT_10Q, &statement);
////    
////    
////    SP<IROSPList<IXbrlPeriod> > period;
////    ret = pXbrl->GetPeriod(1143,&period);
////    
////    SP<IROSPList<IXbrlDocType> > pList;
////    ret = pXbrl->GetDocumentTypes(&pList);
////            
//    15328
//    
//    
//    UID stmentid =7408;
//    
//    CIK cik = "791519";
//    
//    
//    
//    cout<<pDataNode<<endl;
//    ErrorId retCode = pStatementTree->get_Statements(&pDataNode);
//    if (FAILED(retCode))
//    {
//        
//        return E_NotFound;
//    }       
//    cout<<pDataNode<<endl;
//    
//    //make_tree();
//    
    }
    return 0;
}

void TestProductKeyStats(UID ProdId)
{
    SP<IXbrl> papi;
    XbrlOpen(&papi);
    if (FAILED(papi->ProcessProductKeyStats(ProdId)))
    {
        cout << "Error processing stats for product id " << ProdId << endl;
    }
    return;
}


void TestLeaks()
{
    for (int i=0; i != 100000000; ++i)
    {
        SP<IString> pstr;
        IString::Create("this is a test string to check memory leak.", &pstr);
        OmniType ot(pstr->get_Buffer());
        cout << "OT1: " << (CSTR)ot << endl;
        OmniType ot2 = ot;
        cout << "OT2: " << (CSTR)ot2 << endl;
    }
    string s;
    cout << "Enter to exit:" << endl;
    cin >> s;
    return;
}

void TestEconomicSourcingData()
{
    using namespace se::economy;
    SP<IEconomyApi> papi;
    EconOpen(&papi);
    //papi->SourceEconomicData(58);
    //papi->SourceEconomicData(60);
    papi->SourceEconomicData(62);
    //papi->SourceEconomicData();
    
}

void TestGicsRatioCalculationAndSaving(UID gicId)
{
    
    SP<IXbrl> pxbrl;
    THROWIFFAILED(XbrlOpen(&pxbrl), "Failed to open xbrl api");
    SP<IFinPeriodList> plist;
    OmniType avg;
    THROWIFFAILED( pxbrl->GetGicsRevenueGrowth(true, gicId, PERIOD_Annually, &plist, &avg) , "Failed to gics rev growth");
    THROWIFFAILED( pxbrl->GetGicsRevenueGrowth(true, gicId, PERIOD_Quarterly, &plist, &avg) , "Failed to gics rev growth");
    
    for (FINPRDDATA d : *plist)
    {
        cout << get<0>(d) << " : " << get<1>(d) << endl;
    }
    cout << "Geomean: " << avg << endl;
    return;
}

void TestRevenueProspects()
{
    SP<IXbrl> pxbrl;
    THROWIFFAILED(XbrlOpen(&pxbrl), "Failed to open xbrl api");
    SP<se::training::ITrainingTable> ptbl;
    THROWIFFAILED(pxbrl->GetProspectsView(9000, 25, PERIOD_Quarterly, &ptbl), "Failed to get prospects view");
    
    SP<ITrainingRecord> prec;
    THROWIFFAILED(ptbl->get_FutureRecord(&prec), "Failed to get future record.");
    
    cout << prec->get_Value();
    
    return;
}


#include "../seeconomy/seeconomapi.h"

using namespace se::economy;

void TestRSSFeedReadWrite()
{
    //        //"http://www.adp.com/rss/esp.xml";

    //LoggerPtr _logger = Logger::getLogger("TestRSSFeedReadWrite");
    SP<se::economy::IEconomyApi> api;
    if (ISOK(se::economy::EconOpen(&api)))
    {
        api->ProcessEconomicNewsFeeds();
        
//        SP<IEconomicNewsFeeds> plist;
//        if (ISOK(api->get_EconomicNewsFeeds(&plist)))
//        {
//            SP<ISerializedData<>> pJsonSerializer;
//            CJsonSerializedData::Create(&pJsonSerializer, true);
//            plist->Serialize(pJsonSerializer);
//            SP<IString> pOut;
//            pJsonSerializer->get_Output(&pOut);
//            LOGIP(pOut->get_Buffer());
//        }
    }
    return;    
}

void TestWebScreener()
{
    /*  A sample message received.
    {
        "exchanges" : [ "3", "2" ],
        "industries" : [ "6", "7" ],
        "pratios" : 
        {
            "1" : 
            {
                "end" : 11.33839285714286,
                "start" : 1.778571428571429
            },
            "2" : 
            {
                "end" : 40.71093750,
                "start" : 21.54017857142857
            }
        }
    }
    */
    // first async cache performance ratios
    using namespace se::threading;
    SP<IApp> papp;
    SEOpenApp(&papp);
    SP<IThreadPool> ptp;
    papp->get_ThreadPool(TPT_ConcurrentExecution, &ptp);
    SP<IXbrl> xbrl;
    SE_CODE ret = XbrlOpen(&xbrl);
    SP<IFinancialPerformanceList> pl;
    if (SUCCEEDED(ret))
    {
        ptp->ExecuteFunction([&pl](void* x)
        {
            SP<IXbrl> pxbrl = static_cast<IXbrl*>(x);
            pxbrl->GetCachedFinancialPerformances(&pl);
        }, xbrl);
        
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q << "SELECT * FROM sa.list_ListedCompaniesExtended;";
        for (const Row& r  : q.store())
        {
        }
    }
    return;
}



void TestPopulateCachedPerformanceOfAll()
{
    //LoggerPtr _logger = Logger::getLogger("sexbrltests.TestPopulateCachedPerformanceOfAll");
    SP<IXbrl> xbrl;
    SE_CODE ret = XbrlOpen(&xbrl);
    if (SUCCEEDED(ret))
    {
        SP<IFinancialPerformanceList> pl;
        ret = xbrl->GetCachedFinancialPerformances(&pl);
        if (SUCCEEDED(ret))
        {
           // LOGI(_logger, "Loaded financial performances of " << pl->get_Count() << " companies.");
            
            // filter ratios
            SP<IFinancialPerformanceList> plfiltered;
            ret = pl->FilterRatioByRange(2013, 0, 1, 1.78, 11.34, &plfiltered );
            if (ISOK(ret))
            {
                SP<ISerializedData<>> pJsonSerializer;
                CJsonSerializedData::Create(&pJsonSerializer, true);
                plfiltered->Serialize(pJsonSerializer);
                SP<IString> pOut;
                pJsonSerializer->get_Output(&pOut);
               // LOGIP(pOut->get_Buffer());
            }
            plfiltered.Clear();
            ret = pl->FilterRatioByValue(2013, 0, 1, 10, 5, &plfiltered );
            if (ISOK(ret))
            {
                SP<ISerializedData<>> pJsonSerializer;
                CJsonSerializedData::Create(&pJsonSerializer, true);
                plfiltered->Serialize(pJsonSerializer);
                SP<IString> pOut;
                pJsonSerializer->get_Output(&pOut);
               // LOGIP(pOut->get_Buffer());
            }
        }
    }    
    return;
}

void TestPerformanceRatioPersistance(CIK cik, UID perfid)
{
    SP<IXbrl> xbrl;
    SE_CODE ret = XbrlOpen(&xbrl);
    if (SUCCEEDED(ret))
    {
        if (!cik)
        {
            ret = xbrl->PersistPerformanceData();
            if (SUCCEEDED(ret))
            {
                cout << "Saved performance ratios." << endl;
            }
        }
        else
        {
            ret = xbrl->PersistPerformanceData(cik, perfid);
            if (SUCCEEDED(ret))
            {
                cout << "Saved performance ratios." << endl;
            }
        }
    }    
    
    return;
}

void TestStatements(CIK cik)
{
    ////LoggerPtr _logger = Logger::getLogger("sexbrltests.TestStatements");
    SP<IXbrl> xbrl;
    SE_CODE ret = XbrlOpen(&xbrl);
    if (SUCCEEDED(ret))
    {
        SP<IXbrlStatementList> pstmts;
        ret = xbrl->GetFinancialStatements(cik, &pstmts);
        if (SUCCEEDED(ret))
        {
            SP<ISerializedData<>> pJsonSerializer;
            CJsonSerializedData::Create(&pJsonSerializer, true);
            pstmts->Serialize(pJsonSerializer);
            SP<IString> pOut;
            pJsonSerializer->get_Output(&pOut);
           // LOGIP(pOut->get_Buffer());
        }
    }
}

#include "../semath/CFormulaParser.h"
using namespace se::math;

void TestFormulaParser()
{
    CFormulaParser parser;
    //cout << "Formula Result : " << parser.Run("234.23 + loge(2344) * 90 / 19") << endl;
    //cout << "Formula Result : " << parser.Run("abs(-2)-(loge(2)+1.12*90/19)") << endl;
    //cout << "Formula Result : " << parser.Run("ln(1+2)") << endl;
    cout << "Formula Result : " << parser.Run("ln(2)/(abs(2)+1.12*90/19)") << endl;
}

void TestCompanySearch()
{
    SP<IXbrl> xbrl;
    XbrlOpen(&xbrl);
    SP<ISecurityDescriptionList> plist;
    xbrl->FindCompany("0000001800", &plist);
    for(IROSPList<ISecurityDescription>::VECTOR::const_iterator it = plist->begin(); 
            it != plist->end(); ++it)
    {
        cout << (*it)->get_Id()  << " | ";
        cout << (*it)->get_Name() ;
        cout << " | " << (*it)->get_Symbol() ;
        cout << " | " << (*it)->get_Exchange() << endl;
    }
}

void TestEdgarLookup(CSTR cikOrSymbl)
{
    SP<IXbrl> xbrl;
    XbrlOpen(&xbrl);
    SP<IPublicCompanyInfo> pco;
    xbrl->GetEdgarCompanyInfo(cikOrSymbl, &pco);
}


void TestDateType()
{
    CDate today = CDate::Now();
    CDate d = today;
    d -= 30;
    cout << d.get_Year() << "/" << d.get_Month() << "/" << d.get_Day() << endl;
    d.AddSubMonths(-12);
    cout << d.get_Year() << "/" << d.get_Month() << "/" << d.get_Day() << endl;
    
    DATAPERIOD duration = d.get_Duration(today);
    cout << "Number of months = " << duration.Month << endl;
    
    
    return;
    
}

void TestConsolidatedFinancials(CIK cik, UID xbrlid , UID segmid )
{
    SP<IXbrl> xbrl;
    XbrlOpen(&xbrl);
    SP<IFinancialsConsolidated> pc;
    SE_CODE ret = xbrl->GetConsolidatedFinancials(cik, &pc);
    if (SUCCEEDED(ret))
    {
        if (xbrlid)
        {
            SP<IXbrlRecord> pRec;
            if (segmid)
            {
                ret = pc->get_Record(xbrlid, segmid, &pRec);
            }
            else
            {
                ret = pc->get_Record(xbrlid, &pRec);
            }
            if (SUCCEEDED(ret))
            {
                SP<ISerializedData<>> pJsonSerializer;
                CJsonSerializedData::Create(&pJsonSerializer, true);
                pRec->Serialize(pJsonSerializer);
                SP<IString> pOut;
                pJsonSerializer->get_Output(&pOut);
                cout << pOut->get_Buffer() << endl;
            }
        }
        else
        {
            SP<IXbrlRecordList> pRecs;
            ret = pc->get_Records({20627, 15328, 15446, 17505, 15447, 15448}, &pRecs);
            if (SUCCEEDED(ret))
            {
                pRecs->FillEmptyFields();
                SP<ISerializedData<>> pJsonSerializer;
                CJsonSerializedData::Create(&pJsonSerializer, true);
                pRecs->Serialize(pJsonSerializer);
                SP<IString> pOut;
                pJsonSerializer->get_Output(&pOut);
                cout << pOut->get_Buffer() << endl;
            }
        }
    }    
    return;
}

void TestFinancialRatios(CIK cik)
{
    SP<IXbrl> xbrl;
    XbrlOpen(&xbrl);
    SP<IFinancialsConsolidated> pc;
    SE_CODE ret = xbrl->GetConsolidatedFinancials(cik, &pc);
    if (SUCCEEDED(ret))
    {
        SP<IFinancialPerformance> pperf;
        if (SUCCEEDED(pc->get_Performance(&pperf)))
        {
            SP<IFinancialRatioList> pratios;
            if (SUCCEEDED(pperf->get_Ratios(&pratios)))
            {
                for (auto it = pratios->begin(); it != pratios->end(); ++it)
                {
                    IFinancialRatio* pratio = *it;
                    for (I16 year = pratio->get_FloorYear(); year <= pratio->get_CeilYear(); ++year)
                    {
                        double d = pratio->get_Value(year);
                        ostringstream ostr;
                        ostr << d;
                        cout << "Ratio of " << pratio->get_Name() << " for year " 
                                << year << ": " << (d == ER8  ?  "-" : ostr.str().c_str()) << endl;
                        
                        for (auto q = 1; q <= 4; ++q)
                        {
                            switch (q)
                            {
                                    case 1:
                                        ostr.str("");
                                        d = pratio->get_Q1Value(year) ;
                                        ostr << d;
                                        cout << "and Quarter " << q << ": "  << (d == ER8  ?  "-" : ostr.str().c_str()) << endl;
                                        break;
                                    case 2:
                                        ostr.str("");
                                        d = pratio->get_Q2Value(year) ;
                                        ostr << d;
                                        cout << "and Quarter " << q << ": "  <<  (d == ER8  ?  "-" : ostr.str().c_str()) << endl;
                                        break;
                                    case 3:
                                        ostr.str("");
                                        d = pratio->get_Q3Value(year) ;
                                        ostr << d;
                                        cout << "and Quarter " << q << ": "  <<  (d == ER8  ?  "-" : ostr.str().c_str()) << endl;
                                        break; 
                                    case 4:
                                        ostr.str("");
                                        d = pratio->get_Q4Value(year) ;
                                        ostr << d;
                                        cout << "and Quarter " << q << ": "  <<  (d == ER8  ?  "-" : ostr.str().c_str()) << endl;
                                        break;
                            }
                        }
                    }
                }
                cout << endl;
            }
        }
    }    
    return;
}

void TestCachedFinancialRatios(CIK cik)
{
    SP<IXbrl> xbrl;
    XbrlOpen(&xbrl);
    SP<IFinancialPerformance> pperf;
    if (SUCCEEDED( xbrl->GetCachedFinancialPerformance(cik, 21, &pperf) ))
    {
        SP<IFinancialRatioList> pratios;
        if (SUCCEEDED(pperf->get_Ratios(&pratios)))
        {
            for (auto it = pratios->begin(); it != pratios->end(); ++it)
            {
                IFinancialRatio* pratio = *it;
                for (I16 year = pratio->get_FloorYear(); year <= pratio->get_CeilYear(); ++year)
                {
                    double d = pratio->get_Value(year);
                    ostringstream ostr;
                    ostr << d;
                    cout << "Ratio of " << pratio->get_Name() << " for year " 
                            << year << ": " << (d == ER8  ?  "-" : ostr.str().c_str()) << endl;

                    for (auto q = 1; q <= 4; ++q)
                    {
                        switch (q)
                        {
                                case 1:
                                    ostr.str("");
                                    d = pratio->get_Q1Value(year) ;
                                    ostr << d;
                                    cout << "and Quarter " << q << ": "  << (d == ER8  ?  "-" : ostr.str().c_str()) << endl;
                                    break;
                                case 2:
                                    ostr.str("");
                                    d = pratio->get_Q2Value(year) ;
                                    ostr << d;
                                    cout << "and Quarter " << q << ": "  <<  (d == ER8  ?  "-" : ostr.str().c_str()) << endl;
                                    break;
                                case 3:
                                    ostr.str("");
                                    d = pratio->get_Q3Value(year) ;
                                    ostr << d;
                                    cout << "and Quarter " << q << ": "  <<  (d == ER8  ?  "-" : ostr.str().c_str()) << endl;
                                    break; 
                                case 4:
                                    ostr.str("");
                                    d = pratio->get_Q4Value(year) ;
                                    ostr << d;
                                    cout << "and Quarter " << q << ": "  <<  (d == ER8  ?  "-" : ostr.str().c_str()) << endl;
                                    break;
                        }
                    }
                }
            }
            cout << endl;
        }
    }
    return;
}

void TestFinancialRatios1(CIK cik)
{
    SP<IXbrl> xbrl;
    XbrlOpen(&xbrl);
    SP<IFinancialsConsolidated> pc;
    SE_CODE ret = xbrl->GetConsolidatedFinancials(cik, &pc);
    if (SUCCEEDED(ret))
    {

        SP<IXbrlRecordList> pRecs;
        ret = pc->get_Records(
            {19164, 16979, 273593, 
                    26099, 28901, 25445, 
                    1667, 2661679, 23787,
                    26373, 24363, 30431
        }, &pRecs);
        if (SUCCEEDED(ret))
        {
            pRecs->FillEmptyFields();
            SP<ISerializedData<>> pJsonSerializer;
            CJsonSerializedData::Create(&pJsonSerializer, true);
            pRecs->Serialize(pJsonSerializer);
            SP<IString> pOut;
            pJsonSerializer->get_Output(&pOut);
            //cout << pOut->get_Buffer() << endl;
        }
        
        SP<IFinancialPerformance> pperf;
        if (SUCCEEDED(pc->get_Performance(&pperf)))
        {
            cout << "Borrowal Stress : " << pperf->get_Ratio(1) << endl;
            cout << "BPS : " << pperf->get_Ratio(2) << endl;
            cout << "Cash Flow to Assets : " << pperf->get_Ratio(3) << endl;
            cout << "EPS : " << pperf->get_Ratio(6) << endl;
            cout << "Dividends Payout ratio : " << pperf->get_Ratio(5) << endl;
            cout << "Profit Margin : " << pperf->get_Ratio(9) << endl;
            cout << "Gross Profit Margin : " << pperf->get_Ratio(7) << endl;
            cout << "ROA: " << pperf->get_Ratio(10) << endl;
            cout << "ROE: " << pperf->get_Ratio(11) << endl;
            cout << "Asset Turnover: " << pperf->get_Ratio(12) << endl;
            cout << "Annual Income Collection Ratio: " << pperf->get_Ratio(13) << endl;
            cout << "Inventory Turnover: " << pperf->get_Ratio(14) << endl;
            cout << "Debt-Asset Ratio: " << pperf->get_Ratio(15) << endl;
            cout << "Debt-Equity Ratio: " << pperf->get_Ratio(16) << endl;
            cout << "Interest Coverage: " << pperf->get_Ratio(18) << endl;
            cout << "Working capital ratio : " << pperf->get_Ratio(19) << endl;
            cout << "Liability Buster Ratio : " << pperf->get_Ratio(17) << endl;
            SP<IFinancialRatioList> pratios;
            if (SUCCEEDED(pperf->get_Ratios(&pratios)))
            {
                for (auto it = pratios->begin(); it != pratios->end(); ++it)
                {
                    IFinancialRatio* pratio = *it;
                    cout << "Ratio of " << pratio->get_Name() << ": " << pratio->get_Value() << endl;
                }
            }
        }
        
        
    }    
    return;
}


void ProcessStatement(unsigned long recId)
{
    SP<IXbrl> pXbrl;
    XbrlOpen(&pXbrl);
    pXbrl->ProcessStatement(recId);
}

void DisplayFinStatement(UID statementId)
{
    SP<IXbrl> pXbrl;
    XbrlOpen(&pXbrl);
    
    SP<IXbrlDataNode> pStatementTree;
    pXbrl->GetStatement(statementId, XDC_Presentation, &pStatementTree);
    
   // LOGIP("Now deserialize to json and print out the json text.");
    SP<ISerializedData<>> pJsonSerializer;
    CJsonSerializedData::Create(&pJsonSerializer, true);
    
    pStatementTree->Serialize(pJsonSerializer);
    
    SP<IString> pStrOutput;
    pJsonSerializer->get_Output(&pStrOutput);
   // LOGIP(pStrOutput->get_Buffer());
    
}

void DisplayFinStatement()
{
    SP<IXbrl> pXbrl;
    XbrlOpen(&pXbrl);
    
    DATAPERIOD dt;
    dt.Quarter = 3;
    dt.Year = 2013;
    CIK cik = "0000066740";
    
    SP<IXbrlDataTree> pStatementTree;
    pXbrl->GetStatements(cik, dt, XFT_10Q, XDC_Calculation, &pStatementTree);
    
   // LOGIP("Now deserialize to json and print out the json text.");
    SP<ISerializedData<>> pJsonSerializer;
    CJsonSerializedData::Create(&pJsonSerializer, true);
    
    pStatementTree->Serialize(pJsonSerializer);
    
    SP<IString> pStrOutput;
    pJsonSerializer->get_Output(&pStrOutput);
   // LOGIP(pStrOutput->get_Buffer());

}

void PopulateXbrlHierarchy()
{
    CSmartConnection conn(SEDBCONN);
    ifstream fin;
    fin.open("../../../Design Documents/fundamentals/XBRL_ITC_CalculationHierarchySingle.csv");
    string line;
    char_separator<char> sep("~","" , keep_empty_tokens);
    typedef tokenizer< char_separator<char> > Tokenizer;
    typedef vector<string> TokenVector;
    TokenVector vec;
    
    Query q = conn->query();
    q << "truncate table se_xbrl_hierarchy";
    q.execute();
    
    while (getline(fin, line))
    {
        if (line.empty() || line.find("LinkRole") != string::npos)
            continue;
        // tokenize
        trim(line);
        Tokenizer tok(line, sep);
        vec.assign(tok.begin(), tok.end());
        if (vec.size() != 10)
            continue;
        if (vec[0].find("prefix") != string::npos)      // header
            continue;
        // find in xbrl_lookup table.
        trim(vec[0]);
        trim(vec[1]);
        trim(vec[7]);
        string parent = vec[7];
        auto pos = parent.find(":");
        string parprefix = parent.substr(0, pos);
        parent = parent.substr(pos+1);
        trim(parprefix);
        trim(parent);
        
        UID xbrlId = 0, xbrlpId = 0;
        
        {
            UseQueryResult qr;
            Query q = conn->query();
            q << "select xbrl_id from se_xbrl_lookup where xbrl_prefix = " ;
            q << quote << vec[0] << " and ";
            q << "xbrl_name = " ;
            q << quote << vec[1];

            qr = q.use();
            Row rw;
            if (qr && (rw = qr.fetch_row()))
                xbrlId = rw[0];
        }
        
        if (!parent.empty())
        {
            Query q = conn->query();
            q << "select xbrl_id from se_xbrl_lookup where xbrl_prefix = " ;
            q << quote << parprefix << " and ";
            q << "xbrl_name = " ;
            q << quote << parent;
            UseQueryResult qr = q.use();
            Row rw;
            if (qr && (rw = qr.fetch_row()))
                xbrlpId = rw[0];
        }
        
        {
            trim(vec[2]);       // label
            trim(vec[3]);       // depth
            trim(vec[4]);       // order
            trim(vec[5]);       // priority
            trim(vec[6]);       // weight
            trim(vec[8]);       // arcrole


            try
            {
                Query q = conn->query();
                q << "insert into se_xbrl_hierarchy (xbrl_id, xbrl_parent_id, xbrl_label, xbrl_depth, xbrl_order, xbrl_priority, xbrl_weight, xbrl_arcrole) ";
                q << "values (";
                q << xbrlId << ", ";
                q << xbrlpId << ", ";
                q << quote << vec[2] << ", ";
                q << (vec[3].empty()? 0: atoi(vec[3].c_str())) << ", ";
                q << (vec[4].empty()? 0.0: atof(vec[4].c_str())) << ", ";
                q << (vec[5].empty()? 0.0: atof(vec[5].c_str())) << ", ";
                q << (vec[6].empty()? 0.0: atof(vec[6].c_str())) << ", ";
                q << quote << vec[8] ;
                q << ")";
                cout << q.str() << endl;
                q.execute();
            }
            catch(const std::exception& ex)
            {
                cout << ex.what() << endl;
            }
        }
    }
    fin.close();
}