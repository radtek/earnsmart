/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on September 15, 2012, 7:42 AM
 */

#include <cstdlib>
#include "../sesecurities/sesecuritiesapi.h"

#include <iostream>

#include "../seglobal/CROList.h"
#include "../semysql/CSEConnections.h"
#include "../semain/semain.h"

#include "../seglobal/CJsonSerializedData.h"

#include "../seeconomy/seeconomapi.h"

#include "../sexbrl/sexbrlapi.h"


using namespace std;
using namespace se;
using namespace se::securities;
using namespace se::economy;
using namespace se::xbrl;


void PrintCompanySummary(int productid);
void TestDailySecurityPrice();
void DateExperiments();
void MathTests();
void ANNTest_InflationPrediction();
void TestCompanySearch();
void TestCubist();
void TestGicsListing();
void TestEconomyAPI();
void TestRevenuePrediction();
void TestEarnCalendarProcessing();
void TestEconomicDataDownloadAndUpdate();
void TestSecurityNews();
void TestFileMonitor();
void TestEodDataOps();
void TestPredictor();


/*
 * 
 */
int main(int argc, char** argv) 
{
     
    SP<se::IApp> pApp;
    string progname = argv[0];
    SEGetApp(progname.c_str(), &pApp);
    
    cout << "Starting test client..." << endl;
    
    //TestEodDataOps();
    
    //TestFileMonitor();
    
    //TestSecurityNews();
    
    
    //TestEconomicDataDownloadAndUpdate();
    
    //TestEarnCalendarProcessing();
    
    //TestRevenuePrediction();
    
    //TestGicsListing();
    
    //TestEconomyAPI();
    
    // uncomment below to test cubist library
    //TestCubist();
    
    // uncomment code below for testing search feature on the website
    //TestCompanySearch ();
    
    // uncomment below to to test ANN methods
    //ANNTest_InflationPrediction();
    
    //uncomment below to test math functions
    //MathTests();

    // uncomment below to test printing company summary.
    //PrintCompanySummary(5165);
    
    // uncomment below to test daily security price updating function.
    //TestDailySecurityPrice();
    
    // uncomment for date experiments
    //DateExperiments();
    
//    SP<IAddress> pAddr ;
//    CreateAddress(&pAddr);
//    pAddr->set_City("Kochi");
//    pAddr->set_State("Kerala");
//    pAddr->set_Country("India");
//    pAddr->set_Street("Maradu St.");
//    
//    SP<ISerializedData<>> pJsonData ;
//    CJsonSerializedData::Create(&pJsonData);
//    pAddr->Serialize(pJsonData);
//    
//    SP<IString> pstr;
//    pJsonData->get_Output(&pstr);
//    
//    cout << pstr->get_Buffer() << endl;
    
    //SP<ISecurityAPI> api;
    
    
    //if (S_Success == se::securitiepCatLists::SecOpen(&api))
    //{
        
        
        //api->ProcessSecDailyPrice();
        
        //string criteria ="business";
        //SP<IROSPList<ISecurityDescription> > pList;
        
//        SP<ISecurityDescription> pSecDesc ;
//        api->CreateSecurityDescription(&pSecDesc);
//        pSecDesc->set_Symbol("IAF");
//        SP<ISecurityPrice>pSecurityPrice;
//        CDate from, to;
//        from.set_Year(2011);
//        from.set_Month(12);
//        from.set_Day(12);
//        to.set_Year(2012);
//        to.set_Month(1);
//        to.set_Day(12);select a.month as input0, a.lnratio as input1, (0.1156-b.pricechg) as input2, c.Nominal as input3, (2.40000000000001-d.monthlydelta) as output from se_eco_money_velocity a inner join sa.se_eco_commodity_price_index b on (a.year = b.year and a.month = b.month) inner join sa.se_eco_UnemploymentRate c on (a.year = c.year and a.month = c.month)inner join sa.se_eco_inflation_cpi d on (a.year = d.year and a.month = d.month)	where a.country_id = 1 and a.state_id = 0  and a.year >= (year(now())-10) order by a.year desc, a.month desc;
//        
//        api->ProcessSecurityDailyPpCatListrice(pSecDesc);
//        SP<IROSPList<ISecurityPriceEx> > PriceList;
//        api->get_SecurityHistoricalPrices(pSecDesc,from,to,&PriceList);

//        SP<ISecurityPriceEx>pPriceEx;
//        api->get_SecurityPrice(pSecDesc,&pPriceEx);
//        cout<<pPriceEx->get_High()<<endl;
//        SP<ISecurity>pSec;
//        ErrorId retVal = api->get_Security(pSecDesc ,&pSec); 
//        retVal = pSec->get_AvgHistoricalPriceAndVolume(from,to,&pSecurityPrice);
        
         
//        cout<<pSecurityPrice->get_Price()<<"\t"<<pSecurityPrice->get_Volume()<<endl;
//        retVal = api->SearchSecurities(criteria.c_str(), &pList);
//        if (FAILED(retVal))    cube.FeedAttrValues("Month", 9);

//        {
//            return 0;
//        }

//        SP<ISerializedData<>> pJsonSerializer;
//        CJsonSerializedData::Create(&pJsonSerializer, true);
//        pList->Serialize(pJsonSerializer);
//        SP<IString> pStrOutput;
//        pJsonSerializer->get_Output(&pStrOutput);
//        cout<<pStrOutput->get_Buffer()<<endl;
        
  
    
//        SP<ISecurityDescription> pSecDesc ;
//        if (S_Success == api->CreateSecurityDescription(&pSecDesc))
//        {
//            pSecDesc->set_Country(COUNTRY_USA);
//            pSecDesc->set_Symbol("dit");
//            
//            SP<ISecurity> pSecurity ;    cube.FeedAttrValues("Month", 9);

//            if (S_Success == api->get_Security(pSecDesc, &pSecurity))
//            {
//                SP<IPublicCompanyInfo> pCo;
//                auto retVal = pSecurity->get_Company(&pCo);
//                auto name = pCo->get_Name();
                //pCo->get_
                //wcout << name << endl;
                
                
//                
//                SP<IString> szPerf;
//                retVal = pCo->get_Performance(&szPerf);
                
               // cout << szPerf->get_Buffer() << endl;
//                
//            }
//        }
        
        //se::securities::SecClose(api);
    //}

    //TestEodDataOps();
    TestPredictor();
    
    return 0;
}

#include "../sepredict/sepredictapi.h"
void TestPredictor()
{
    using namespace se::predict;
    // a prod desc is necessary to start with a predictor.
    SP<IProductDescription> pdesc;
    SE_CODE r = IProductDescription::Create(23, ProductDescriptionType::Industry, TrainingDataContext::FirmPerformance, &pdesc);
    if (FAILED(r)) return ;

    /// CREATE PREDICTOR.
    SP<IPredictor> pr;
    r = se::predict::Create(pdesc, &pr);
    if (FAILED(r)) return ;
    
    if (r != S_DataExists)
    {
        pr->set_Name("Basic Industries/Steel/Iron Ore Industry");
        pr->set_Description("Basic Industries/Steel/Iron Ore industry analysis using MVLR.");


        auto FSetTSetAttribs = [](ITrainingSet* tset, std::initializer_list<tuple<I32, TrainingDataContext>> attribids)
        {
            for (auto tpl : attribids)
            {
                SP<IAttributeDescription> pattrib;
                IAttributeDescription::Create(get<0>(tpl), get<1>(tpl), &pattrib);
                tset->add_Attribute(pattrib);
            }
        };

        SP<ITrainingSet> ptset[3];
        r = ITrainingSet::CreateorOpen("Trainingset Test 1", &ptset[0]);
        if (FAILED(r)) return ;
        if (r != S_DataExists)
        {
            FSetTSetAttribs(ptset[0], { make_tuple(1, (TrainingDataContext)(TrainingDataContext)'E'), make_tuple(2, (TrainingDataContext)(TrainingDataContext)'E'), make_tuple(3, (TrainingDataContext)(TrainingDataContext)'E'), make_tuple(4, (TrainingDataContext)(TrainingDataContext)'E'), make_tuple(5, (TrainingDataContext)(TrainingDataContext)'E') });
            ptset[0]->set_Algo(PredictionAlgos::MultiVarLinearRegression);

            SP<IResultAttribDescription> pResult0;
            IResultAttribDescription::Create(1, TrainingDataContext::FirmPerformance, &pResult0);

            ptset[0]->set_ResultContext(pResult0);
        }
        r = ITrainingSet::CreateorOpen("Trainingset Test 2", &ptset[1]);
        if (FAILED(r)) return ;
        if (r != S_DataExists)
        {
            FSetTSetAttribs(ptset[1], { make_tuple(1, (TrainingDataContext)(TrainingDataContext)'E'), make_tuple(2, (TrainingDataContext)'E'), make_tuple(3, (TrainingDataContext)'E'), make_tuple(4, (TrainingDataContext)'E'), make_tuple(5, (TrainingDataContext)'E') });
            ptset[1]->set_Algo(PredictionAlgos::MultiVarLinearRegression);

            SP<IResultAttribDescription> pResult1;
            IResultAttribDescription::Create(26, TrainingDataContext::FirmPerformance, &pResult1);

            ptset[1]->set_ResultContext(pResult1);
        }
        r = ITrainingSet::CreateorOpen("Trainingset Test 3", &ptset[2]);
        if (FAILED(r)) return ;
        if (r != S_DataExists)
        {
            FSetTSetAttribs(ptset[2], { make_tuple(1, (TrainingDataContext)'E'), make_tuple(2, (TrainingDataContext)'E'), make_tuple(3, (TrainingDataContext)'E'), make_tuple(4, (TrainingDataContext)'E'), make_tuple(5, (TrainingDataContext)'E') });
            ptset[2]->set_Algo(PredictionAlgos::MultiVarLinearRegression);

            SP<IResultAttribDescription> pResult2;
            IResultAttribDescription::Create(27, TrainingDataContext::FirmPerformance, &pResult2);
            ptset[2]->set_ResultContext(pResult2);

        }
        
        ptset[0]->AddDependent(ptset[1]);
        ptset[0]->AddDependent(ptset[2]);

        // Populate attributes for all the training sets
        // NOTE: ADD NETINCOME GROWTH AND EXPENSES GROWTH into perf ratios
        // Net Indome Growth <- [ Revenue Growth, Expense Growth ]
        // Add similar attributes for all the three.maria

        r = pr->SetTrainingSet(ptset[0])
                .Store();
        if (FAILED(r)) return ;

    }
    else
    {
        /*
        IResult& result = pr->BuildEquation()
                .Result();    
        cout << result.Value();
         * */
        
        r = pr->Remove();
        if (FAILED(r))
            cout << "Failed to remove.";
    }
    return;
}


#include "../seeoddata/seeoddata.h"

void TestEodDataOps()
{
    using namespace se::eoddata;
    SP<IEODData> peod;
    SE_CODE ret;
    ret = EodDataOpen(&peod);
    if (FAILED(ret))
    {
        return;
    }
    SP<IWSClientSession> psession;
    ret = peod->OpenDefault(&psession);
    if (FAILED(ret))
    {
        return;
    }
    
    // check if still logged in
    cout << "Is Logged In : " << psession->IsLoggedIn() << endl;
    
    if (!psession->IsLoggedIn())
        return;
    
    cout << "Is Logged In : " << psession->get_Version() << endl;


    ret = psession->PublishEodData();

    
//    SP<IExchangeList> pexchl;
//    ret = psession->GetExchanges(&pexchl);
//    
//    if (FAILED(ret))
//        return;
//    
//    cout << "How many exchanges ? " << pexchl->get_Size() << endl;
//    
//
//    SP<IEnumerator1<se::eoddata::IExchange>> penum;
//    ret = pexchl->get_Enumerator(&penum);
//    int i=1;
//    while (penum->MoveNext())
//    {
//        SP<se::eoddata::IExchange> pexch = penum->get_Current();
//        cout << i++ << ": " << pexch->get_Name() << endl;
//    } 
//    
//    SP<INewsList> pnewslist;
//    ret = psession->GetNews("A", "NYSE", &pnewslist);
//    SP<IEnumerator1<INews>> penews;
//    ret = pnewslist->get_Enumerator(&penews);
//    while (penews->MoveNext())
//    {
//        SP<INews> pnews = penews->get_Current();
//        cout << 
//                "Title: " << pnews->get_Title() << endl <<
//                "Story: " << pnews->get_Story() << endl << 
//                "Date: " << pnews->get_When() << endl;
//    }
    
//    SP<ISecurityDetails> psdet;
//    SP<ISecurityDescription> pdesc;
//    ret = CreateSecurityDescription(&pdesc);
//    pdesc->set_Symbol("IBM");
//    pdesc->set_Exchange("NYSE");
//    ret = psession->GetSecurityDetails(pdesc, &psdet);
//    
//    if (!psdet)
//    {
//        cout << "Error : security details is null" << endl;
//        return;
//    }
//    
//    SP<ISecurityQuote> pqt = psdet->get_Quote();
//    if (!pqt)
//    {
//        cout << "Error : security quote is null" << endl;
//        return;
//    }
//    cout << "Security " << pdesc->get_Symbol() << " close price is " << pqt->get_Close() << endl;
//    
//    SP<ISymbolChangeList> psymchgs;
//    ret = psession->GetSymbolChanges("NYSE", &psymchgs);
//    if (FAILED(ret))
//    {
//        cout << "Failed to get symbol change list" << endl;
//        return;
//    }
//    
//    SP<IEnumerator1<ISymbolChange>> penum1;
//    ret = psymchgs->get_Enumerator(&penum1);
//    while (penum1->MoveNext())
//    {
//        SP<ISymbolChange> psymchg = penum1->get_Current();
//        cout << "New Sym: " << psymchg->get_NewSymbol() << endl;
//        cout << "Old Sym: " << psymchg->get_OldSymbol() << endl;
//        cout << "New Exch: " << psymchg->get_NewExchangeCode() << endl;
//        cout << "Old Exch: " << psymchg->get_OldExchangeCode() << endl;
//     }
//    
//    SP<ISecurityQuoteList> pqlist;
//    CDate start(2013, 1, 2);
//    CDate end = CDate::Now();
//    ret = psession->GetSymbolHistoryMonthly(pdesc, start, end, &pqlist);
//    if (FAILED(ret))
//    {
//        cout << "Failed to get symbol history list" << endl;
//        return;
//    }
//    SP<IEnumerator1<ISecurityQuote>> penum2;
//    ret = pqlist->get_Enumerator(&penum2);
//    while (penum2->MoveNext())
//    {
//        SP<ISecurityQuote> pq = penum2->get_Current();
//        cout << pq->get_DateTime() << "-> " << pq->get_High() << "/ " << pq->get_Low() << "/ "  << pq->get_Close() << endl;
//    }
    
    return;
}

#include "../seglobal/FileSystemMonitor.h"

void TestFileMonitor()
{
    using namespace se::io;
    
    FileSystemMonitor mon("/home/santony/Documents");
    NotifyFunction func = [](const string& name, FSNotifyType nf)
    {
        cout << "File " << name << " is " << (char)nf << endl;
    };
    int funcId ;
    SE_CODE ret = mon.AddListener(func, funcId);
    
    if (ISOK(ret))
    {
        mon.Begin();
        
        auto inval = cin.get();
        
        mon.Stop();
    }
    
    
    
    
}


void TestSecurityNews()
{
    SP<ISecurityAPI> papi;
    if (ISOK(SecOpen(&papi)))
    {
        SP<ISecurityNewsList> pNews;
        if (ISOK(papi->get_SecurityNews(6181, &pNews)))
        {
            for (ISecurityNews* p : *pNews)
            {
                cout << "Title: " << p->get_Title() ;
                cout << "Content: " << p->get_NewsContent();
                cout << "NewsPrecis: " << p->get_NewsPrecis();
                cout << "Link: " << p->get_Link();
                cout << endl;
            }
        }
        else
        {
            cout << "Failed to get any news for this company.";
        }
    }
}

void TestEconomicDataDownloadAndUpdate()
{
    SP<IEconomyApi> papi;
    EconOpen(&papi);
    papi->SourceEconomicData();
    return;
}

void TestGicsListing()
{
    SP<ISecurityAPI> papi;
    if (ISOK(SecOpen(&papi)))
    {
        SP<IGICList> pgics;
        if (ISOK(papi->get_GICS(&pgics, 
                [](IGICInfo::QUERYFLDS fld, LGX& l, BOOL& b)
                {
                    if (fld == IGICInfo::Sector)
                    {
                        b = true;
                        l = LGX_And;
                        return "Basic Industries";
                    }
                    if (fld == IGICInfo::Industry)
                    {
                        b = true;
                        l = LGX_And;
                        return "Forest Products";
                    }
                    return "";
                }
                )))
                {
                    for(IGICInfo* pgic : *pgics)
                    {
                        cout << "Gic: " << pgic->get_Sector()  << "/" << pgic->get_Industry() << endl;
                        SP<IPublicCompanyInfoList> pcos;
                        if (ISOK(pgic->get_Companies(&pcos)))
                        {
                            for (IPublicCompanyInfo* pco : *pcos)
                            {
                                cout << "Company: " << pco->get_Name();
                                cout << endl;
                            }
                        }
                    }
                }
                
    }
}

void TestEconomyAPI()
{
    SP<IEconomyApi> papi;
    EconOpen(&papi);
    
    SP<IIndicatorCategoryList> pCatList;
    SE_CODE ret = papi->get_IndicatorCategories(&pCatList);

    SP<ISerializedData<>> pSerStr;
    CJsonSerializedData::Create(&pSerStr, true);
    
    if (SUCCEEDED(ret))
    {
        pCatList->Serialize(pSerStr);
        SP<IString> str;
        pSerStr->get_Output(&str);
        cout << str->get_Buffer();
        
    }
    
    SP<IEconomyIndicatorList> pIndicators;
    ret = papi->get_EconomicIndicators(
            &pIndicators,
            [](IEconomyIndicator::QUERYFLDS f, LGX& l, BOOL& i)
            {
                if (f == IEconomyIndicator::CategoryId)
                {
                    i = true;
                    return "1";
                }
                return "";
            }
            );
    if (SUCCEEDED(ret))
    {
        pSerStr->Reset();
        pIndicators->Serialize(pSerStr);
        SP<IString> str;
        pSerStr->get_Output(&str);
        cout << str->get_Buffer();
    }
    
    return;
}





#include "../setraining/setrainingapi.h"
#include "../secubist/CCubist.h"

using namespace se::math;
using namespace se::training;




void TestCompanySearch()
{
    // search can be a symbol, cik, company name or gics/sics
    CSTR searchtext = "0000016160";
    SP<ISecurityAPI> pSecApi;
    ErrorId retCode ;
    retCode = SecOpen(&pSecApi);
    if (FAILED(retCode))
    {
        cout << "Error: " << retCode << endl;
        return;
    }
    SP<IROSPList<ISecurityDescription>> pResults;
    retCode = pSecApi->SearchSecurities(searchtext, &pResults);
    if (SUCCEEDED(retCode))
    {
        // found the following.
        for (auto it = pResults->begin(); it != pResults->end(); ++it)
        {
            ISecurityDescription* pSecDesc = *it;
            cout << pSecDesc->get_Symbol() << endl;
        }
    }    
}

void TestDailySecurityPrice()
{
    SP<ISecurityAPI> pApi;
    SecOpen(&pApi);
    pApi->ProcessSecDailyPrice();
}

void PrintCompanySummary(int productid)
{
    SP<ISecurityAPI> pApi;
    SecOpen(&pApi);
    
    SP<ISecurity> pSecurity;
    pApi->get_Security(productid, &pSecurity);
    
            
    SP<ISerializedData<>> pJsonSerializer;
    CJsonSerializedData::Create(&pJsonSerializer, true);
    pSecurity->Serialize(pJsonSerializer);
    SP<IString> pStrOutput;
    pJsonSerializer->get_Output(&pStrOutput);
    cout<<pStrOutput->get_Buffer()<<endl;
            
    
    
}

#include <ctime>
#include <iomanip>
#include <chrono>
#include <boost/operators.hpp>

using namespace std::chrono;
//using namespace std::chrono::_V2;

void DateExperiments()
{
    CDate now = CDate::Now();
    tm t;
    memset(&t, 0, sizeof(tm));
    t.tm_year = now.get_Year();
    t.tm_mon  = now.get_Month() -1 ;
    t.tm_mday = now.get_Day();
    
    time_t tnow = mktime(&t);
    
    auto tp = system_clock::from_time_t(tnow);

    tp -= std::chrono::hours(24);
    
    
}

#include "../semath/CCommon.h"

using namespace se::math;

void MathTests()
{
    CCommon semath;
    SP<ISecurityAPI> pApi;
    SecOpen(&pApi);
    SP<ISecurityDescription> pSec;
    pApi->get_SecurityDescription(1509, &pSec);
    SP<IROList<PeriodPriceAdjustedPair>> pList;
    pApi->get_SecurityHistoricalPrices(pSec, CDate::Empty(), CDate::Empty(), EAM_Inflation, HPAM_Monthly, &pList);
    cout << "Count : " << pList->get_Count() << endl;
    SP<IEnumerator<PeriodPriceAdjustedPair>> pEnum;
    pList->get_Enumerator(&pEnum);
    while (pEnum->MoveNext())
    {
        auto v0 = get<0>(pEnum->get_Current());
        SP<IEconomicallyAdjustedPrices> v1 = get<1>(pEnum->get_Current());
        cout << v0.Month << "/" << v0.Year << " = " << v1->get_Value(EAM_None) << "/" << v1->get_Value(EAM_Inflation) << endl;
    }
    return ;
}

void ANNTest_InflationPrediction()
{
    /* we need datatable of following timeseries data which are 
     * aligned month to month basis for 10 years
     * 1. Unemployment Rate
     * 2. Commodity Price Index
     * 3. Money Stock (M2)
     * Then flaten them into a single record in a datatable with year and month as keys
     * Pass into the fann cascade trainer
     * Once training is completed, provide future month, year, predicted unemployment rate, commodity price index and money stock
     * to get predicted inflation:
     * However this means, we have to predict Unemployment Rate, Commodity Price Index and Money Stock
     */
    
    SP<ITrainingAPI> pApi;
    const UID trainingId = 100;
    TrainingOpen(&pApi);
    ErrorId retCode;

    R4 average = 0.;
    //for (int i=0; i<10; ++i)
    {
        retCode = pApi->AnnTrainData(trainingId);
        if (FAILED(retCode))
            return;
        R4 inputs1[4] = {-0.0152, -0.3, -11.0, 11};
        retCode = pApi->AnnPredictResult(trainingId, 
                [&inputs1](int inputindex)
                {
                    return inputs1[inputindex];
                },
                [&average, &inputs1](int length, R4 const* outputs)
                {
                    R4 outputvalue = outputs[0];
                    cout << "Predicted inflation: ";
                    cout.precision(6);
                    cout << inputs1[0] << "|" << inputs1[1] << "|" << inputs1[2] << "|" << inputs1[3] << "=";
                    cout << outputvalue ;
                    cout << endl;
                    average += outputvalue;
                }
            );

    }
    
    //average = average/10.0;
    
    cout << "Average of ANN predicted value : " << average << endl << endl;
    
//    //retCode = pApi->AnnTrainData(trainingId);
//    R4 inputs2[4] = {10.0, 1.09, 0.0110, 4.3};
//    retCode = pApi->AnnPredictResult(trainingId, 
//            [&inputs2](int inputindex)
//            {
//                return inputs2[inputindex];
//            },
//            [&inputs2](int length, R4 const* outputs)
//            {
//                R4 outputvalue = outputs[0];
//                cout << "Predicted inflation: ";
//                cout.precision(6);
//                cout << inputs2[0] << "|" << inputs2[1] << "|" << inputs2[2] << "|" << inputs2[3] << "=";
//                cout << outputvalue ;
//                cout << endl;
//            }
//        );
//        
//    //retCode = pApi->AnnTrainData(trainingId);
//    R4 inputs3[4] = {9.0, 1.88464, -0.0036, 7.2};
//    retCode = pApi->AnnPredictResult(trainingId, 
//            [&inputs3](int inputindex)
//            {
//                return inputs3[inputindex];
//            },
//            [&inputs3](int length, R4 const* outputs)
//            {
//                R4 outputvalue = outputs[0];
//                cout << "Predicted inflation: ";
//                cout.precision(6);
//                cout << inputs3[0] << "|" << inputs3[1] << "|" << inputs3[2] << "|" << inputs3[3] << "=";
//                cout << outputvalue ;
//                cout << endl;
//            }
//        );
        
        return;
}


void TestCubist()
{
    SP<ITrainingAPI> pApi;
    const UID trainingId = 100;
    TrainingOpen(&pApi);
    ErrorId retCode;
    SP<IStoreQueryResult> pResult;
    pApi->get_TrainingRecords(trainingId, &pResult);
    
    CCubist cube;
    COptions& options = cube.get_Options();
    options.set_Option(OPT_AllowUnbiasedRules);
    options.set_Option(OPT_RulesInstanceAuto);
    options.set_NumberOfNearestNeighbors(9);
    options.set_MaximumNumOfRules(500);
    options.set_ExtrapolationLimit(5);
    
    // add attributes
    CAttributeNumeric attrM3("CPI");
    cube.AddAttribute(attrM3);
    CAttributeNumeric attrM4("UER");
    cube.AddAttribute(attrM4);
    CAttributeNumeric attrM2("MV");
    cube.AddAttribute(attrM2);
    CAttributeNumeric attrMonth("Month");
    cube.AddAttribute(attrMonth);
    CAttributeNumeric attrTarget("Inflation");
    cube.AddAttribute(attrTarget, true);
    
    cube.BeginLoadTrainData();
    for (Row& row : pResult->get_Result())
    //for (int i=0; i<10; i++)
    {
        //Row& row = pResult->get_Result()[i];
        cube.BeginAddRecord();
        double rval = row["input0"];
        cube.FeedAttrValues("CPI", rval);
        rval = row["input1"];
        cube.FeedAttrValues("UER", rval);
        rval = row["input2"];
        cube.FeedAttrValues("MV", rval);
        rval = row["input3"];
        cube.FeedAttrValues("Month", rval);
        rval = row["output"];
        cube.FeedAttrValues("Inflation", rval);
        cube.EndAddRecord();
    }
    cube.EndLoadTrainData();

    cout << "Predication Rules/Formulae : " << cube.get_RuleText() << endl;    

    cube.BeginPredict();
    cube.FeedAttrValues("CPI", 0.0428);
    cube.FeedAttrValues("UER", -0.3);
    cube.FeedAttrValues("MV", -11.40);
    cube.FeedAttrValues("Month", 12);
    cube.FeedAttrValues("Inflation", 0);
    R8 pred = cube.EndPredict();

    cout << "Predicted result for Inflation (Month: 12)" << pred << endl;

//    for (double i=1.; i <=12.; ++i)
//    {
//        cube.BeginPredict();
//        cube.FeedAttrValues("CPI", 0.0);
//        cube.FeedAttrValues("UER", "?");
//        cube.FeedAttrValues("MV", "?");
//        cube.FeedAttrValues("Month", i);
//        cube.FeedAttrValues("Inflation", 0);
//        pred = cube.EndPredict();
//        cout << "Predicted result for Inflation (Month: " << i << ")" << pred << endl;
//    }

    
    return;
}

void TestRevenuePrediction()
{
    SP<IEconomyApi> pecoapi;
    if (FAILED(EconOpen(&pecoapi)))
    {
        cout << "Error opening econ api" << endl;
        return ;
    }
    SP<ITrainingTableList> ptblList;
    ITrainingTableList::Create(&ptblList);
    
    SP<ITrainingTable> ptbl1;
    DATAPERIOD dpfrom, dpto;
    dpfrom.Day = 1;
    dpfrom.Month = 1;
    dpfrom.Year = 2010;
    if (FAILED(pecoapi->get_EconomicIndicatorTrainingTable(6, 1, 0, dpfrom, dpto, &ptbl1)))
    {
        cout << "Error getting table" << endl;
        return ;
    }    
    
    // get linear fitting equation.
    R8 c0, c1, cov0, cov1, cov2, chisq;
    if (FAILED(pecoapi->get_WeightedLinearFitEquation(
            ptbl1, &c0, &c1, &cov0, &cov1, &cov2, &chisq)))
    {
        cout << "Error computing linear fitting values." << endl;
        return ;
    }
    
    // data for period april
    DATAPERIOD dp;
    dp.Year = 2014;
    dp.Month = 4;
    dp.Day = 1;
    R8 dpVal = PERIODMASK(dp);
    R8 fittedValue = c0 + (dpVal * c1);
    cout << "Fitted value for april is " << fittedValue << endl;
    
    if (FAILED(pecoapi->get_LinearFitEquation(
            ptbl1, &c0, &c1, &cov0, &cov1, &cov2, &chisq)))
    {
        cout << "Error computing linear fitting values." << endl;
        return ;
    }
    fittedValue = c0 + (dpVal * c1);
    cout << "Non-weighted Fitted value for april is " << fittedValue << endl;    
    
    // test data
    for (ITrainingRecord* prec : *ptbl1)
    {
        cout << prec->get_Period() << ":" << prec->get_Value() << endl;
    }
    ptblList->Add(ptbl1);

    SP<ITrainingTable> ptbl2;
    if (FAILED(pecoapi->get_EconomicIndicatorTrainingTable(11, 1, 0, dpfrom, dpto, &ptbl2)))
    {
        cout << "Error getting table" << endl;
        return ;
    }    
    
    
    // test data
    for (ITrainingRecord* prec : *ptbl2)
    {
        cout << prec->get_Period() << ":" << prec->get_Value() << endl;
    }
    
    if (FAILED(pecoapi->get_WeightedLinearFitEquation(
            ptbl2, &c0, &c1, &cov0, &cov1, &cov2, &chisq)))
    {
        cout << "Error computing linear fitting values." << endl;
        return ;
    }
    
    dp.Year = 2014;
    dp.Month = 5;
    dp.Day = 1;
    dpVal = PERIODMASK(dp);
    fittedValue = c0 + (dpVal * c1);
    cout << "Fitted value for april is " << fittedValue << endl;    
    
    if (FAILED(pecoapi->get_LinearFitEquation(
            ptbl2, &c0, &c1, &cov0, &cov1, &cov2, &chisq)))
    {
        cout << "Error computing linear fitting values." << endl;
        return ;
    }
    fittedValue = c0 + (dpVal * c1);
    cout << "Non-weighted Fitted value for april is " << fittedValue << endl;    

    ptblList->Add(ptbl2);

    dp.Month = 4;

    SP<ITrainingTable> ptbl3;
    if (FAILED(pecoapi->get_EconomicIndicatorTrainingTable(24, 1, 0, dpfrom, dpto, &ptbl3)))
    {
        cout << "Error getting table" << endl;
        return ;
    }    
    
    // test data
    for (ITrainingRecord* prec : *ptbl3)
    {
        cout << prec->get_Period() << ":" << prec->get_Value() << endl;
    }
    ptblList->Add(ptbl3);

    SP<ITrainingTable> ptbl4;
    if (FAILED(pecoapi->get_EconomicIndicatorTrainingTable(25, 1, 0, dpfrom, dpto, &ptbl4)))
    {
        cout << "Error getting table" << endl;
        return ;
    }    
    
    // test data
    for (ITrainingRecord* prec : *ptbl4)
    {
        cout << prec->get_Period() << ":" << prec->get_Value() << endl;
    }
    ptblList->Add(ptbl4);
   
    SP<ITrainingTable> pRevenues;
    SP<IXbrl> pXbrl;
    if (FAILED(XbrlOpen(&pXbrl)))
    {
        cout << "Error xbrl open" << endl;
        return ;
    }        
    
    if (FAILED(pXbrl->GetPerformanceTrainingData("0000004281", 21, dpfrom, dpto, &pRevenues)))
    {
        cout << "Error opening perf training data." << endl;
        return ;
    }        
    
    // extend tables
    SP<IExTrainingTable> pExTable;
    if (ISOK(IExTrainingTable::ExtendTables(ptblList, pRevenues, &pExTable)))
    {
        for (IExTrainingRecord* prec : *pExTable)
        {
            cout << prec->get_Period() << " | ";
            for (int i=0; i != pExTable->get_AttributeFieldsCount(); ++i)
            {
                cout << prec->get_Value(i) << " | ";
            }
            cout << prec->get_Result();
            cout << endl;
        }
        cout << endl;
    }
    
    
    CCubist cube;
    COptions& options = cube.get_Options();
    options.set_Option(OPT_AllowUnbiasedRules);
    options.set_Option(OPT_RulesInstanceAuto);
    options.set_NumberOfNearestNeighbors(9);
    options.set_MaximumNumOfRules(500);
    options.set_ExtrapolationLimit(5);
    
    // add attributes
    CAttributeNumeric attrM1("ATTR_1");
    cube.AddAttribute(attrM1);
    CAttributeNumeric attrM2("ATTR_2");
    cube.AddAttribute(attrM2);
    CAttributeNumeric attrM3("ATTR_3");
    cube.AddAttribute(attrM3);
    CAttributeNumeric attrM4("ATTR_4");
    cube.AddAttribute(attrM4);
    CAttributeNumeric attrM5("ATTR_5");
    cube.AddAttribute(attrM5);
    CAttributeNumeric attrTarget("RESULT");
    cube.AddAttribute(attrTarget, true);
    
    cube.BeginLoadTrainData();
    std::ostringstream ostr;
    for (IExTrainingRecord* prec : *pExTable)
    {
        cube.BeginAddRecord();
        //cube.FeedAttrValues("ATTR_1", (R8)prec->get_Period().Month);
        for (int i=0; i != pExTable->get_AttributeFieldsCount(); ++i)
        {
            ostr.str("");
            ostr << "ATTR_" << i+1;
            OmniType ot = prec->get_Value(i);
            if (ot.IsEmpty())
                cube.FeedAttrValues(ostr.str(), "?");
            else
                cube.FeedAttrValues(ostr.str(), (R8)prec->get_Value(i));
        }
        cube.FeedAttrValues("RESULT", (R8)prec->get_Result());
        cube.EndAddRecord();
    }
   
    cube.EndLoadTrainData();

    cout << "Predication Rules/Formulae : " << cube.get_RuleText() << endl;    

    cube.BeginPredict();
    //cube.FeedAttrValues("ATTR_1", 4);
    cube.FeedAttrValues("ATTR_2", 0.3);
    cube.FeedAttrValues("ATTR_3", 11.40);
    cube.FeedAttrValues("ATTR_4", -180.0);
    cube.FeedAttrValues("ATTR_5", -190);
    cube.FeedAttrValues("RESULT", 0);
    R8 pred = cube.EndPredict();

    cout << "Predicted result " << pred << endl;    
    
    SP<IExTrainingRecord> prec;
    pExTable->CreateRecord(&prec);
    prec->set_Period(dp);
    prec->set_Value(0, .3);
    prec->set_Value(1, 11.4);
    prec->set_Value(2, -180);
    prec->set_Value(3, -190);
    prec->set_Result(pred);
    pExTable->Add(prec);
    
    cube.BeginLoadTrainData();
    for (IExTrainingRecord* prec : *pExTable)
    {
        cube.BeginAddRecord();
        cube.FeedAttrValues("ATTR_1", (R8)prec->get_Period().Month);
        for (int i=0; i != pExTable->get_AttributeFieldsCount(); ++i)
        {
            ostr.str("");
            ostr << "ATTR_" << i+1;
            OmniType ot = prec->get_Value(i);
            if (ot.IsEmpty())
                cube.FeedAttrValues(ostr.str(), "?");
            else
                cube.FeedAttrValues(ostr.str(), (R8)prec->get_Value(i));
        }
        cube.FeedAttrValues("RESULT", (R8)prec->get_Result());
        cube.EndAddRecord();
    }
   
    cube.EndLoadTrainData();

    cout << "Predication Rules/Formulae : " << cube.get_RuleText() << endl;    

    cube.BeginPredict();
    //cube.FeedAttrValues("ATTR_1", 5);
    cube.FeedAttrValues("ATTR_2", 1.3);
    cube.FeedAttrValues("ATTR_3", 1.40);
    cube.FeedAttrValues("ATTR_4", 102.0);
    cube.FeedAttrValues("ATTR_5", 123);
    cube.FeedAttrValues("RESULT", 0);
    pred = cube.EndPredict();

    cout << "Predicted result " << pred << endl;    
    
    
}


void TestEarnCalendarProcessing()
{
    SP<ISecurityAPI> papi;
    if (FAILED(SecOpen(&papi)))
    {
        cout << "Failed to open api";
    }
    if (FAILED(papi->ProcessEarningCalendar(PERIOD_Annually)))
    {
        cout << "Failed to process earning calendar.";
    }
    return;
}