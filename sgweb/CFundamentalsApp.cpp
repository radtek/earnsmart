/* 
 * File:   CFundamentalsApp.cpp
 * Author: santony
 * 
 * Created on August 18, 2013, 12:31 PM
 */

#include <curl/curl.h>
#include <libxml2/libxml/SAX.h>
#include "../seglobal/CJsonSerializedData.h"
#include "sgwebinternal.h"

#include "../seglobal/CDate.h"
#include "../seglobal/CQuarter.h"

#include "CFundamentalsApp.h"
#include "../sesecurities/sesecuritiesapi.h"

using namespace cppcms;

namespace sg
{
    namespace web
    {


        CFundamentalsApp::CFundamentalsApp(cppcms::service& srv) : CBaseApplication(srv)
        {

            CBaseApplication::_logger = Logger::getLogger("sg.web.fundamentalapp");

            dispatcher().assign("^/p/(\\d*)$", &CFundamentalsApp::DisplayFundamentals, this, 1);
            mapper().assign("p", "/p/{1}");

            dispatcher().assign("^/p/json/mp/(\\d*)$", &CFundamentalsApp::OutputJsonMonthlyPriceHistory, this, 1);
            mapper().assign("p_json_mp", "/p/json/mp/{1}");

            dispatcher().assign("^/p/json/if/(\\d*)$", &CFundamentalsApp::OutputJsonCompanyInfo, this, 1);
            mapper().assign("p_json_if", "/p/json/if/{1}");
            
            dispatcher().assign("", &CFundamentalsApp::DisplayStockPicks, this);
            mapper().assign("");
            
        }

        CFundamentalsApp::~CFundamentalsApp() 
        {
        }
        
        void CFundamentalsApp::init()
        {
            CBaseApplication::init();
        }
        
        void CFundamentalsApp::DisplayStockPicks()
        {
            StockPicks stockPick(context());
            Master m(context(), stockPick);
            m.Title = "Fundamentals";
            m.Init();
            BuildHeaderMenus(m.header, "Fundamentals");
            render("MasterView", m);
            
        }
        
        void CFundamentalsApp::DisplayCompanySummary(string productId)
        {
            // capture the company
            UID uid = (UID)atoi(productId.c_str());
            CompanySummary summary(context());
            summary.Init(uid);
            Fundamentals fdm(context(), summary);
            fdm.Init(uid);
            Master m(context(), fdm);
            m.Title = "Stock - Summary";
            m.Init();
            BuildHeaderMenus(m.header, "Fundamentals");
            render("MasterView", m);
        }
        
        void CFundamentalsApp::DisplayFinancials(string productId)
        {
            UID uid = (UID)atoi(productId.c_str());
            CompanyFinancials fin(context());
            Fundamentals fdm(context(), fin);
            fdm.Init(uid);
            Master m(context(), fdm);
            m.Title = "Stock - Fundamentals";
            m.Init();
            BuildHeaderMenus(m.header, "Fundamentals");
            render("MasterView", m);
        }
        
        void CFundamentalsApp::DisplayPerformance(string productId)
        {
            UID uid = (UID)atoi(productId.c_str());
            CompanyPerformance perf(context());
            Fundamentals fdm(context(), perf);
            fdm.Init(uid);
            Master m(context(), fdm);
            m.Title = "Stock - Performance";
            m.Init();
            BuildHeaderMenus(m.header, "Fundamentals");
            render("MasterView", m);
        }
        
        void CFundamentalsApp::DisplayProspects(string productId)
        {
            UID uid = (UID)atoi(productId.c_str());
            CompanyProspects prospects(context());
            Fundamentals fdm(context(), prospects);
            fdm.Init(uid);
            Master m(context(), fdm);
            m.Title = "Stock - Prospects";
            m.Init();
            BuildHeaderMenus(m.header, "Fundamentals");
            render("MasterView", m);
        }
        
        void CFundamentalsApp::DisplayFundamentals(string productId)
        {
            
            http::request& req = context().request();
            if (req.request_method() == "POST")
            {
                string fdm_sel = req.post("fdm_sel");
                if (fdm_sel == "0")
                {
                    DisplayCompanySummary(productId);
                }
                else if (fdm_sel == "1")
                {
                    DisplayFinancials(productId);
                }
                else if (fdm_sel == "2")
                {
                    DisplayPerformance(productId);
                }
                else if (fdm_sel == "3")
                {
                    DisplayProspects(productId);
                }
                else
                {
                    DisplayCompanySummary(productId);
                }
            }
            else
                DisplayCompanySummary(productId);
            
            //DisplayPerformance(productId);
            
            
//            /* DocType (combobox)   Statement Name (comboxbox)   [ [Quarter-]Year ](combobox)
//             * 
//             * Totalling Label              Don't show total
//             *      Sublabel1               Amount
//             *      SubLabel2               Amount
//             *          Sublevel21          Amount  
//             * Total                        TotalAmount
//
//             * Totalling Label              Don't show total
//             *      Sublabel1               Amount
//             *      SubLabel2               Amount
//             *          Sublevel21          Amount  
//             * Total                        TotalAmount
//             * 
//             */
//            
//            auto& req = request();
//            auto& res = response();
//            auto& ses = session();
//            SP<IXbrl> pXbrl;
//            SP<ISecurityAPI> pSecApi;
//            
//            try
//            {
//                if (productId.empty())
//                {
//
//                    LOGEP("Product id is missing. Returning to home page.");
//                    // redirect to home page. // we need an error page for this purpose.
//                    res.set_redirect_header("/");
//                    
//                    return;
//                }
//
//                XbrlOpen(&pXbrl);
//                SecOpen(&pSecApi);
//
//                UID uidProdId = (UID)atoi(productId.c_str());
//                SP<IROSPList<IXbrlDocType> > pDocList;
//                ErrorId retVal = pXbrl->GetDocumentTypes(&pDocList);
//                if (FAILED(retVal))
//                {
//                    LOGEP("Failed to create a list of financial document types..");
//                    res.set_redirect_header("/");
//                    return ;
//                }
//                
//                SP<IROList<I16> > pPeriods;
//                retVal = pXbrl->GetPeriods(uidProdId,&pPeriods);
//                if (FAILED(retVal))
//                {
//                    LOGEP("Failed to get a list of financial periods for the given product.");
//                    res.set_redirect_header("/");
//                    return;
//                }
//
//                SP<ISecurityDescription> pSecDesc;
//                retVal = pSecApi->get_SecurityDescription(uidProdId, &pSecDesc);
//                if (FAILED(retVal))
//                {
//                    LOGEP("Failed to get product security description.");
//                    res.set_redirect_header("/");
//                    return;
//                }
//                // above details are required during pre-post and post events. Ideally they should be saved
//                // in session temporarily.
//                 // create the fundamentals page model.
//                Fundamentals f(context());
//                f.Init((UID)uidProdId);
//                // create master model with page model.
//                Master p(context(), f);
//                p.Title = "Smartearn - Fundamentals";
//                p.Init();
//                BuildHeaderMenus(p.header, "Fundamentals");
//                
//                if (req.request_method() == "POST")
//                {
//                    // get values from form elements
//                    f.qtr = (I16)atoi(req.post("ddQuarters").c_str());
//                    f.year = (I16)atoi(req.post("ddYears").c_str());
//                    f.dtype = (I32)atoi(req.post("ddDocTypes").c_str());
//                    f.stId = (I64)atoi(req.post("ddStatements").c_str());
//                    
//                    
//                    //price history
//                    SP<ISecurityPrice>currentSecurityPrice;
//                    SP<ISecurityPrice>curSecurityPrice;
//                    SP<ISecurityPrice>prevSecurityPrice;
//                    SP<ISecurity>pSec;
//                    retVal = pSecApi->get_Security(pSecDesc ,&pSec); 
//                    if (FAILED(retVal))
//                    {
//                        LOGEP("Failed to open sec");
//                        // set an error message on the screen
//                    }
//                    retVal = f.get_HistoricalPrice(pSec, f.year, f.qtr, f.dtype, &curSecurityPrice);
//                    if (FAILED(retVal))
//                    {
//                        LOGEP("Failed to get Historical Price");
//                        // set an error message on the screen
//                    }
//                    if(f.dtype == 1)
//                    {
//                        
//                        if(f.qtr == 4)
//                            retVal = f.get_HistoricalPrice(pSec, (f.year)-1, 1, f.dtype, &prevSecurityPrice);
//                        else
//                            retVal = f.get_HistoricalPrice(pSec, f.year, (f.qtr)-1, f.dtype, &prevSecurityPrice);
//                            
//                    }
//                        
//                    else if(f.dtype == 3)
//                        retVal = f.get_HistoricalPrice(pSec, (f.year)-1, f.qtr, f.dtype, &prevSecurityPrice);
//                    //end price history   
//                    stringstream str;
//                    f.HistoricalPrice(prevSecurityPrice,curSecurityPrice,f.dtype);
//                    
//                }
//                else
//                {
//                    CDate dtNow = CDate::Now();
//                    CQuarter quarter(dtNow);
//                    
//                    // update recent finds list.
//                    RecentFinds rf(context());                
//                    rf += uidProdId;
//                    if (quarter.get_Quarter() == 1)
//                    {
//                        f.qtr = 4;
//                        f.year = dtNow.get_Year() - 1;
//                    }
//                    else
//                    {
//                        f.qtr = quarter.get_Quarter()-1;if (context().request())
//                        f.year = dtNow.get_Year();
//                    }
//                    f.dtype = 1;
//                    f.stId = 0;
//                }
//                                                                             
//                f.subTitle = pSecDesc->get_Name();
//
//                f.SetList(pDocList);
//                f.SetPeriod(pPeriods);
//                
//                // get statement titles
//                SP<IROSPList<IXbrlStatement>> pStmtTitles;
//                SP<IXbrlDataNode> pStatement ;
//                DATAPERIOD dp = { 0, f.qtr, f.year };
//                retVal = pXbrl->GetStatementNames(uidProdId, dp, (XFT)f.dtype, &pStmtTitles);
//                if (FAILED(retVal))
//                {
//                    LOGEP("Failed to get statement titles.");
//                    // set an error message on the screen
//                }
//                else
//                {
//                    if (pStmtTitles->get_Count())
//                    {
//                        f.SetStatementTitles(pStmtTitles);
//                        if (f.stId == 0)
//                        {
//                            SP<IXbrlStatement> pTitle;
//                            pStmtTitles->get_Item(0, &pTitle);
//                            f.stId = pTitle->get_Id();
//                        }
//                        if (f.stId)
//                        {
//                            retVal = pXbrl->GetStatement(f.stId, &pStatement);
//                            if (FAILED(retVal))
//                            {
//                                LOGE(_logger, "Failed to get statement " << f.stId);
//                            }
//                            else
//                            {
//                                f.SetStatement(pStatement);
//                            }
//                        }
//                    }
//                }
//
//                
//                render("MasterView", p);
//
//                
//            }
//            catch(std::exception const& ex)
//            {
//
//
//                LOGE(_logger, "Product id is missing. Returning to home page." << ex.what());
//                // redirect to home page. // we need an error page for this purpose.
//                res.set_redirect_header("/");
//
//                return;
//            }

        }
        
        
        void CFundamentalsApp::OutputJsonCompanyInfo(string productId)
        {
            try
            {
                SP<ISecurityAPI> pApi;
                SecOpen(&pApi);
                SP<IPublicCompanyInfo> pCompany;
                pApi->get_Company(atoi(productId.c_str()), &pCompany);
                SP<ISerializedData<>> pSerialized;
                CJsonSerializedData::Create(&pSerialized);
                pCompany->Serialize(pSerialized);
                SP<IString> pJsonStr;
                pSerialized->get_Output(&pJsonStr);
                context().response().set_content_header("application/json");
                context().response().out() << pJsonStr->get_Buffer();
            }
            catch(std::exception& ex)
            {
                context().response().out() << "{}";
            }
        }
        
        void CFundamentalsApp::OutputJsonMonthlyPriceHistory(string productId)
        {
            context().response().set_content_header("application/json");
            try
            {
                SP<ISecurityAPI> pApi;
                SecOpen(&pApi);
                SP<ISecurityDescription> pSec;
                pApi->get_SecurityDescription(atoi(productId.c_str()), &pSec);
                SP<IROList<PeriodPriceAdjustedPair>> pList;
                CDate dtNow = CDate::Now();
                CDate dtFrom(dtNow.get_Year() - 10, 1, 1);
                pApi->get_SecurityHistoricalPrices(pSec, dtFrom, dtNow, EAM_Inflation, HPAM_Monthly, &pList);
                auto& strm = context().response().out();
                /*
                 * [
                    ['Year', 'Sales', 'Expenses'],
                    ['2004',  1000,      400],
                    ['2005',  1170,      460],
                    ['2006',  660,       1120],
                    ['2007',  1030,      540]
                  ]
                 */
                strm << "[";
                strm << "[\"M/Y\", \"Normal Price($)\", \"Inflation Adjusted Price($)*\"],";
                auto size = pList->get_Count() -1 ;
                SIZE inc = 0;
                for (auto it = pList->begin(); it != pList->end(); ++it)
                {
                    auto v0 = get<0>(*it);
                    SP<se::securities::IEconomicallyAdjustedPrices> v1 = get<1>(*it);
                    R8 normalprice = v1->get_Value(EAM_None);
                    R8 adjprice = v1->get_Value(EAM_Inflation);
                    strm << "[\"" << v0.Month << "-" 
                            << v0.Year << "\", " 
                            << normalprice 
                            << ", " 
                            << adjprice
                            << "]" ;
                    if (inc < size)
                        strm << ",";
                    inc++;
                }
                strm << "]";
            }
            catch(std::exception& ex)
            {
                context().response().out() << "{}";
            }
        }
    }
}