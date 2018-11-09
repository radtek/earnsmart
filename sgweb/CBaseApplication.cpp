/* 
 * File:   CBaseApplication.cpp
 * Author: santony
 * 
 * Created on August 31, 2013, 10:23 AM
 */

#include <cppcms/session_interface.h>
#include "CBaseApplication.h"
#include <cppcms/http_response.h>
#include <fstream>
#include <mysql/mysql.h>
#include <boost/concept_check.hpp>
#include "../seglobal/CDirectory.h"
#include "sgwebinternal.h"
#include "models.h"
#include "../seglobal/exceptions.h"
#include "../seadmin/seadminapi.h"
#include "../semain/semain.h"
#include "../seglobal/CJsonSerializedData.h"
#include "../seeconomy/seeconomapi.h"
#include <string>
#include <jsoncpp/json/json.h>
#include "../sesecurities/sesecuritiesapi.h"
#include "../semysql/CSEConnections.h"
#include "CSmartearnApp.h"
#include "../seglobal/CMisc.h"
#include "../seglobal/CFinancialPeriod.h"

using namespace sg::web::models;
using namespace se::economy;
using namespace se::securities;
using namespace Json;
using namespace se::data;

#define SEDBCONN CSEConnections::GetMain()

namespace sg
{
    namespace web
    {
        Mutex CBaseApplication::_mutex;
        
        const string USERID = "USERID";
        const string PRODPAGE = "PRPG";
        
        CBaseApplication::CBaseApplication(cppcms::service& srv) :
                application(srv)
        {
            SP<IString> str;
            CDirectory::GetExePath(&str);
            _rootPath = str->get_Buffer();
        }

        CBaseApplication::~CBaseApplication() 
        {
        }
        
        void CBaseApplication::Init(cppcms::service& srv)
        {
            
            // blob downloads
            dispatcher().assign("^/blob/(([\\w\\W]+)(\\.)(css|js|jpg|ico|png|bmp|gif|txt|json))$", &CBaseApplication::GetFileBase, this, 1);
            mapper().assign("blob", "/blob/{1}");
            
#ifdef __PAID__
            dispatcher().assign("^/register$", &CBaseApplication::DisplayRegister, this);
            mapper().assign("register", "/register/");
            dispatcher().assign("^/ppapproved/$", &CBaseApplication::DisplayPPApproved, this);
            mapper().assign("ppapproved", "/ppapproved/");
            dispatcher().assign("^/ppcanceled$", &CBaseApplication::DisplayPPCanceled, this);
            mapper().assign("ppcanceled", "/ppcanceled/");
            dispatcher().assign("^/signin$", &CBaseApplication::DisplaySignIn, this);
            mapper().assign("signin", "/signin/");
            dispatcher().assign("^/signout$", &CBaseApplication::DisplaySignout, this);
            mapper().assign("signout", "/signout/");
            dispatcher().assign("^/myaccount$", &CBaseApplication::DisplayMyAccount, this);
            mapper().assign("myaccount", "/myaccount/");
#endif
            
            dispatcher().assign("^/gj/([\\d]+)(/?)([\\w\\W]*)$", &CBaseApplication::GetJson, this, 1, 2, 3);
            mapper().assign("gj", "/gj/{1}/{3}");
            
            dispatcher().assign("^/search$", &CBaseApplication::ProcessSearch, this);
            mapper().assign("search", "/search");
            
            dispatcher().assign("^/checkuser/(([\\w\\W]*)(@{1})([\\w\\W]*)(\\.{1})([\\w\\W]*))$", &CBaseApplication::CheckUser, this, 1);
            mapper().assign("checkuser", "/checkuser/{1}");
        }

        void CBaseApplication::main(std::string url) 
        {
            application::main(url);
        }

        
        void CBaseApplication::init()
        {
            if (!_pAdm)
            {
                THROWIFFAILED(AdminOpen(&_pAdm), "Failed to open admin gateway.");
            }
            
            SP<IWebSession> pws;
            get_Session(&pws);
            if (!session().is_set(USERID) || !pws)
            {
                const string sid = session().get_session_cookie();
                if (!sid.empty())
                {
                    UID userId = 0;
                    if (session().is_set(USERID))
                        userId = session().get<UID>(USERID);
                    
                    if (!pws)
                    {
                        SP<IUserSession> pus;
                        IUserSession::Create(&pus);
                        pus->set_UserId(userId);
                        CSession::BuildUserSession(context(), pus._ptr);
                        pus->set_Activity(UST_SessionInit);
                        _pAdm->RegisterSessionActivity(pus._ptr, &pws);
                        session().set(USERID, pws->get_UserId());
                    }
                }
            }
        }

        void CBaseApplication::clear()
        {
            
        }

        bool CBaseApplication::IsAuthenticated() 
        {
            return session().is_set(USERID) && session().get<UID>(USERID) > 0;
        }


        long unsigned int CBaseApplication::get_Session(IWebSession** ppOut) 
        {
            return _pAdm->get_WebSession(session().get_session_cookie().c_str(), ppOut);
        }
        
        void CBaseApplication::GetJson(string p1, string p2, string p3)
        {
            UID id = 0;
            int cat = strtol(p1.c_str(), 0, 10);
            
            switch(cat)
            {
                case 1:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("ListPaypalCountries", data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    ListPaypalCountries();
                    break;
                }
                case 2:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("ListStates" + p3, data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    ListStates(p3);     // note that p2 is a slash between such as /gj/2/1 (between 2 and 1)
                    break;
                }
                case 3:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("ListEconIndicatorCategories", data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    ListEconIndicatorCategories();
                    break;
                }
                case 4:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("ListEconIndicators" + p3, data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    id = strtol(p3.c_str(), 0, 10);
                    ListEconIndicators(id);
                    break;
                }
                case 5:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("ListEconIndicatorDataList" + p3, data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    id = strtol(p3.c_str(), 0, 10);
                    ListEconIndicatorDataList(id);
                    break;
                }
                case 6:
                    id = strtol(p3.c_str(), 0, 10);
                    GetYearlyFinancials(id);
                    break;
                case 7:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("GetCompanySummaryBalanceSheetData" + p3, data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    id = strtol(p3.c_str(), 0, 10);
                    GetCompanySummaryBalanceSheetData(id);
                    break;
                }
                case 8:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("GetCompanySummaryIncomeData" + p3, data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    id = strtol(p3.c_str(), 0, 10);
                    GetCompanySummaryIncomeData(id);
                    break;
                }
                case 9:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("GetCompanySummaryCashflowData" + p3, data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    id = strtol(p3.c_str(), 0, 10);
                    GetCompanySummaryCashflowData(id);
                    break;
                }
                case 10:    // get exchanges
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("GetExchanges", data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    GetExchanges();
                    break;
                }
                case 11:    // get gics
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("GetGICHierarchy" + (p3.empty() || p3=="0" ? "" : p3), data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    if (p3 != "")
                    {
                        id = strtol(p3.c_str(), 0, 10);
                        GetGICHierarchy(id);
                    }
                    else
                        GetGICHierarchy();
                    break;
                }
                case 12:    // ratio groups;
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("GetRatioGroups", data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    GetRatioGroups();
                    break;
                }
                case 13:    // ratios;
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("GetRatios" + (p3.empty() || p3=="0" ? "" : p3), data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    if (p3 != "")
                    {
                        id = strtol(p3.c_str(), 0, 10);
                        GetRatios(id);
                    }
                    else
                        GetRatios();
                    break;
                }
                case 14:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("GetRatioValueCount" + p3, data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    id = strtol(p3.c_str(), 0, 10);
                    GetRatioValueCount(id);
                    break;
                }
                case 15:    // get security details
                {
                    GetSecurityDetails();
                    break;
                }
                case 16:
                {
                    GetLatestAnnualAccountValue();
                    break;
                }
                case 17:    // returns a combined view of current revenue and predicated value on quarterly basis
                {
                    GetSecurityProspectsData();
                    break;
                }
                case 18:
                    GetGicsComparedPerformanceData();
                    break;
                case 19:    // stock current price rss feed html
                    GetCurrentPriceRSSFeed(p3);
                    break;
                    
                default:
                    response().out() << "{\"Status\":0}";
            }
        }
        
        void CBaseApplication::GetCurrentPriceRSSFeed(string prodID) 
        {
            SP<ISecurityAPI> papi;
            THROWIFFAILED(SecOpen(&papi), "Failed to open security api.");
            SP<ISecurityPriceRSSFeed> pfeed;
            UID pid = strtol(prodID.c_str(), nullptr, 10);
            THROWIFFAILED(papi->get_SecurityPriceRSSFeedCurrent( pid , SPS::Nasdaq, &pfeed ), "Failed to get rss price feed");
            // response is not a json, but an html. replace content header.
            response().set_content_header("text/html");
            
            // inject _blank.
            string feed = pfeed->get_Feed();
            boost::replace_all(feed, " href=", " target='_blank' href=");
//            boost::replace_all(
//                feed, 
//                "<a href=\"http://www.nasdaq.com/symbol/irm\">IRM</a>", 
//                "<text class=\"se_div_box_title\">Stock Quote</text>");
            response().out() << feed;
            return;
        }

        
        void CBaseApplication::GetGicsComparedPerformanceData()
        {
            if (request().request_method() != "POST")
              throw seexception("Invalid operation. Only post methods are accepted.");
            
            string productid = request().post("productid");
            string perfid = request().post("perfid");
            
            UID iProdId = strtol(productid.c_str(), nullptr, 10);
            UID iPerfId = strtol(perfid.c_str(), nullptr, 10);
            bool bAnnual = true;
            
            string scachename = "GetGicsComparedPerformanceData-" + productid + "-" + perfid;
            MakeSerializable<string> data;
            if (cache().fetch_data(scachename, data))
            {
                response().out() << data.get();
                return;
            }
            

            /*
             * 1. Get performance of company
             * 2. get performance of industry
             * 3. get performance of sector.
             */

            CSmartConnection conn(SEDBCONN);
            Query q = conn->query();
            q << "SELECT parent_gics_id, product_gic_id FROM sa.list_ListedCompaniesExtended where product_id = " << iProdId;
            auto sqr = q.store();
            if (!sqr.size())
                throw seexception("Product not found.");
            
            SP<ISecurityAPI> psecapi;
            SP<IPublicCompanyInfo> pco;
            THROWIFFAILED(SecOpen(&psecapi), "Failed to open security api.");
            THROWIFFAILED(psecapi->get_Company(iProdId, &pco), "No company found");
            SP<IGICInfo> pgicsinfo;
            THROWIFFAILED(pco->get_GICInfo(&pgicsinfo), "No gics info available.");
            
            SP<IXbrl> pxbrl;
            THROWIFFAILED(XbrlOpen(&pxbrl), "Failed to open xbrl api.");
            
            SP<IString> pCik;
            THROWIFFAILED(pxbrl->get_CIK(iProdId, &pCik), "Failed to find the cik for the product.");
            
            SP<IFinancialRatio> pCoFin;
            THROWIFFAILED(pxbrl->GetCachedFinancialPerformance(
                    pCik->get_Buffer(), 
                    iPerfId, 
                    &pCoFin), "Failed to get company financial performance data.");

            SP<IFinPeriodList> pGicsFin0, pGicsFin1;
            THROWIFFAILED(pxbrl->GetGicsRevenueGrowth(
                    false, 
                    (UID)sqr[0][0], 
                    bAnnual ? PERIOD_Annually : PERIOD_Quarterly, 
                    &pGicsFin0, 
                    nullptr
                    ), "Failed to get gics performance data");
            
            THROWIFFAILED(pxbrl->GetGicsRevenueGrowth(
                    false, 
                    (UID)sqr[0][1], 
                    bAnnual ? PERIOD_Annually : PERIOD_Quarterly, 
                    &pGicsFin1, 
                    nullptr
                    ), "Failed to get gics performance data");
            
            CDate dtFinEnd;
            pxbrl->GetFinancialYearEnd(pCik->get_Buffer(), &dtFinEnd);
            
            // lowest year.
            Value root;
            Value& header = root[0];
            header[0] = "Period";
            header[1] = pco->get_Name();
            header[2] = pgicsinfo->get_Industry();
            header[3] = pgicsinfo->get_Sector();

            CDate dtNow = CDate::Now();
            I16 lowestyear = (I16)std::max((int)dtNow.get_Year() - 3, (int)pCoFin->get_FloorYear());
            I16 highestyear = dtNow.get_Year();
            I16 upperQ = dtNow.get_Quarter();
            int i=1;
            ostringstream ostr;
            
            SP<IFinancialPeriod> fper;
            THROWIFFAILED(pxbrl->CaptureFinancialPeriods(pCik->get_Buffer(), &fper), "Error to create financial period type.");
            
            for (I16 yr = lowestyear; yr <= highestyear; ++yr)
            {
                if (!bAnnual)
                {
                    for (I16 q = 1; q <= 4; ++q)
                    {
                        if (highestyear == yr && q > upperQ)
                            goto EXIT;

                        Value& row = root[i++];

                        DATAPERIOD dp;
                        dp.Quarter = q;
                        dp.Year = yr;
                        
                        DATAPERIOD dtNatPeriod;
                        fper->get_NaturalPeriod(dp, &dtNatPeriod);
                        ostr.str("");
                        ostr << std::setw(2) << std::setfill('0') << (dp.Year%2000) << "Q" << dp.Quarter;
                        row[0] = ostr.str();

                        const OmniType& coval = pCoFin->get_Value(dtNatPeriod.Year, dtNatPeriod.Quarter);
                        if (coval.IsEmpty())
                            row[1] = Json::nullValue;
                        else
                            row[1] = (R8)coval;

                        OmniType val0;
                        if (ISOK(pGicsFin0->get(dp, &val0)) && !val0.IsEmpty())
                        {
                            row[2] = (R8)val0;
                        }
                        else
                        {
                            row[2] = Json::nullValue;
                        }

                        OmniType val1;
                        if (ISOK(pGicsFin1->get(dp, &val1)) && !val1.IsEmpty())
                        {
                            row[3] = (R8)val1;
                        }
                        else
                        {
                            row[3] = Json::nullValue;
                        }
                    }
                }
                else
                {
                    Value& row = root[i++];

                    DATAPERIOD dp;
                    dp.Year = yr;
                    dp.Quarter = 0;

                    ostr.str("");
                    ostr << dp.Year;
                    row[0] = ostr.str();

                    const OmniType& coval = pCoFin->get_Value(yr);
                    if (coval.IsEmpty())
                        row[1] = Json::nullValue;
                    else
                        row[1] = (R8)coval;

                    OmniType val0;
                    if (ISOK(pGicsFin0->get(dp, &val0)) && !val0.IsEmpty())
                    {
                        row[2] = (R8)val0;
                    }
                    else
                    {
                        row[2] = Json::nullValue;
                    }

                    OmniType val1;
                    if (ISOK(pGicsFin1->get(dp, &val1)) && !val1.IsEmpty())
                    {
                        row[3] = (R8)val1;
                    }
                    else
                    {
                        row[3] = Json::nullValue;
                    }                    
                }
            }
            
        EXIT:

            FastWriter writer;
            MakeSerializable<string> ser(writer.write(root));
            cache().store_data(scachename, ser);
            response().out() << ser.get() ;

        }

        void CBaseApplication::GetSecurityProspectsData() 
        {
            if (request().request_method() != "POST")
              throw seexception("Invalid operation. Only post methods are accepted.");

            string productid = request().post("productid");
            //string accounts = request().post("accounts");
            // ignoring accounts value for now.
            int pid = strtol(productid.c_str(), nullptr, 10);
            //int pid = 9000;
            
            string scachename = "GetSecurityProspectsData-" + productid ;
            MakeSerializable<string> data;
            if (cache().fetch_data(scachename, data))
            {
                response().out() << data.get();
                return;
            }
            
            
            SP<IXbrl> pxbrl;
            THROWIFFAILED(XbrlOpen(&pxbrl), "Failed to open financials api.");
            
            SP<se::training::ITrainingTable> ptbl;
            THROWIFFAILED(pxbrl->GetProspectsView(pid, 25, PERIOD_Quarterly, &ptbl), "Failed to get prospects view");
            
            Value root;
            Value& cols = root["cols"];
            // x label
            Value& col0 = cols[0];
            col0["id"] = "";
            col0["label"] = "Qtr Yr";
            col0["pattern"] = "";
            col0["type"] = "string";

            // y label
            Value& col1 = cols[1];
            col1["id"] = "";
            col1["label"] = "Revenue";
            col1["pattern"] = "";
            col1["type"] = "number";

            // certainty
            Value& col2 = cols[2];
            col2["id"] = "";
            col2["label"] = "";
            col2["pattern"] = "";
            col2["type"] = "boolean";
            col2["p"]["role"] = "certainty";

            // tooltip
            Value& col3 = cols[3];
            col3["id"] = "";
            col3["label"] = "";
            col3["pattern"] = "";
            col3["type"] = "string";
            col3["p"]["role"] = "tooltip";

            // annotation
            Value& col4 = cols[4];
            col4["id"] = "";
            col4["label"] = "";
            col4["pattern"] = "";
            col4["type"] = "string";
            col4["p"]["role"] = "annotation";
            
//            { c : 
//             * [ 
//             *  { v : "Q1'09", f : null }, 
//             *  { v : 1213121, f : null }, 
//             *  { v : false, f : null }, 
//             *  { v : null, f : null }, 
//             *  { v : null, f : null } 
//             * ]
//             * }            
            
            CDate dtToday = CDate::Now();
            auto startYear = dtToday.get_Year() - 3;
            Value& rows = root["rows"];
            int i = 0;
            stringstream ostr;
            for (ITrainingRecord* prec : *ptbl)
            {
                const DATAPERIOD& dp = prec->get_Period();
                if (dp.Year < startYear)
                    continue;
                
                Value& row = rows[i++];
                Value& c = row["c"];
                
                ostr.str("");
                Value& c0 = c[0];
                ostr << std::setw(2) << std::setfill('0') << (dp.Year%2000) << "Q" << dp.Quarter;
                c0["v"] = ostr.str();
                c0["f"] = Json::nullValue;

                Value& c1 = c[1];
                c1["v"] = (R8)prec->get_Value();
                c1["f"] = Json::nullValue;

                Value& c2 = c[2];
                c2["v"] = true;
                c2["f"] = Json::nullValue;

                Value& c3 = c[3];
                ostr.str("");
                ostr << (dp.Year%2000) << "Q" << dp.Quarter << " revenue: " << (Json::Int64)(R8)prec->get_Value();
                c3["v"] = ostr.str();
                c3["f"] = Json::nullValue;
                

                Value& c4 = c[4];
                c4["v"] = Json::nullValue;
                c4["f"] = Json::nullValue;
                
            }
            
            SP<ITrainingRecord> prec;
            THROWIFFAILED(ptbl->get_FutureRecord(&prec), "Failed to get future record.");
            
            Value& row = rows[i];
            Value& c = row["c"];

            ostr.str("");
            Value& c0 = c[0];
            const DATAPERIOD& dp = prec->get_Period();
            ostr << std::setw(2) << std::setfill('0') << (dp.Year%2000) << "Q" << dp.Quarter;
            c0["v"] = ostr.str();
            c0["f"] = Json::nullValue;

            Value& c1 = c[1];
            c1["v"] = (Json::Int64)(R8)prec->get_Value();
            c1["f"] = Json::nullValue;

            Value& c2 = c[2];
            c2["v"] = false;
            c2["f"] = Json::nullValue;

            Value& c3 = c[3];
            ostr.str("");
            ostr << "Expected " 
                    << (dp.Year%2000) 
                    << "Q" << dp.Quarter 
                    << " revenue: " 
                    << (Json::Int64)(R8)prec->get_Value();
            ostr << endl;
            ostr << "Expected Revenue Growth: " ;
            ostr << setprecision(2) ;
            ostr << fixed ;
            ostr << (R8)prec->get_Change();
            c3["v"] = ostr.str();
            c3["f"] = Json::nullValue;
            
            Value& c4 = c[4];
            ostr.str("");
            ostr << "Expected " 
                    << (dp.Year%2000) 
                    << "Q" << dp.Quarter 
                    << " revenue: " ;
            CMisc::FormatFinancialNumber(ostr, (R8)prec->get_Value() ) ;
            c4["v"] = ostr.str();
            c4["f"] = Json::nullValue;
            
            
            root["p"] = Json::nullValue;
            
            FastWriter writer;
            MakeSerializable<string> ser(writer.write(root));

            cache().store_data(scachename, ser);
            response().out() << ser.get() ;
            
        }


        void CBaseApplication::GetLatestAnnualAccountValue() 
        {
            if (request().request_method() != "POST")
                throw seexception("Invalid operation. Only post methods are accepted.");
            
            string cik = request().post("cik");
            string xbrlid = request().post("xid");
            
            if (cik.empty() || xbrlid.empty())
                throw seexception("Invalid posted parameters.");
            
            string scachename = "GetLatestAnnualAccountValue-" + cik + "-" + xbrlid ;
            MakeSerializable<string> data;
            if (cache().fetch_data(scachename, data))
            {
                response().out() << data.get();
                return;
            }
            
            
            // load consolidated financials
            SP<IXbrl> pxbrl;
            THROWIFFAILED(XbrlOpen(&pxbrl), "Failed to open xbrl api");
            
            SP<IFinancialsConsolidated> pcon;
            THROWIFFAILED(pxbrl->GetConsolidatedFinancials(cik.c_str(), &pcon), "No consolidated view definied.");

            UID xid = strtol(xbrlid.c_str(), 0, 10);
            SP<IXbrlRecord> prec;
            THROWIFFAILED( pcon->get_MatchedAccountRecord(xid, &prec), "Failed to get xbrl record from consolidated view");
            
            SP<IXbrlField> pfld;
            THROWIFFAILED(prec->get_Financials(pcon->get_FinancialYearEnd().get_Year(), &pfld), "Failed to get financial field for given year from consolidated view");
            
            const OmniType& val = pfld->get_FY();
            
            Value root;
            root["cik"] = cik;
            root["matchid"] = (U32)xid;
            root["year"] = pcon->get_FinancialYearEnd().get_Year();
            Value& v = root["value"];
            if (val.dt == DT_R8)
                v["FY"] = (R8)val;
            else if (val.dt == DT_I64)
                v["FY"] = (Json::Int64)(I64)val;
            else
            {
                std::ostringstream ostr;
                ostr << val;
                v["FY"] = ostr.str();
            }
            
            for (int q=1; q<=4; ++q)
            {
                std::ostringstream oqstr;
                oqstr << "Q" << q;
                const OmniType& qval = pfld->get_Q(q);
                if (val.dt == DT_R8)
                    v[oqstr.str()] = (R8)qval;
                else if (val.dt == DT_I64)
                    v[oqstr.str()] = (Json::Int64)(I64)qval;
                else
                {
                    std::ostringstream ostr;
                    ostr << qval;
                    v[oqstr.str()] = ostr.str();
                }
            }
            
            FastWriter writer;
            MakeSerializable<string> ser(writer.write(root));
            cache().store_data(scachename, ser);
            response().out() << ser.get() ;
            
        }
        
        
        void CBaseApplication::GetSecurityDetails()
        {
            if (request().request_method() != "POST")
                throw seexception("Invalid operation. Only post methods are accepted.");
            
            // expected json
            // { xchange : 'NYSE', country : 'US', symbol : 'MSFT' }
            Json::Reader reader;
            Json::Value param;
            reader.parse(request().post("param"), param, false);
            
            const string sym = param["symbol"].asString();
            const string country = param["country"].asString();
            const string xchange = param["xchange"].asString();
            string scachename = "GetSecurityDetails-" + sym + "-" + country + "-" + xchange;
            MakeSerializable<string> data;
            if (cache().fetch_data(scachename, data))
            {
                response().out() << data.get();
                return;
            }

            
            SP<ISecurityAPI> pSecApi;
            SP<ISecurity> pSec;
            THROWIFFAILED(SecOpen(&pSecApi), "Error to open security gateway.");
            SP<ISecurityDescription> pSecDesc;
            THROWIFFAILED(CreateSecurityDescription(&pSecDesc), "Error to create security desc");
            pSecDesc->set_Symbol(sym.c_str());
            pSecDesc->set_Country(country.c_str());
            pSecDesc->set_Exchange(xchange.c_str());
            THROWIFFAILED(pSecApi->get_Security(pSecDesc, &pSec), "Failed to get security details");
            
            SP<IPublicCompanyInfo> pCo;
            THROWIFFAILED(pSec->get_Company(&pCo), "Failed to get company info");
            Json::Value respdata;
            respdata["cik"] = pCo->get_CIK();
            respdata["name"] = pCo->get_Name();
            respdata["id"] = (U32)pCo->get_Id();
            
            FastWriter writer;
            MakeSerializable<string> ser(writer.write(respdata));
            cache().store_data(scachename, ser);
            response().out() << ser.get() ;
            
        }

        void CBaseApplication::GetRatioValueCount(unsigned long rid) 
        {
            try
            {
                // breaking rule of isolation mysql for this method.
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "call list_PerfRatioValuesCurrentYear(" << rid << ")";
                Value root;
                Value& header = root[0];
                header[0] = "value";
                header[1] = "count";
                int i=1;
                for (const Row& r : q.store())
                {
                    Value& row = root[i++];
                    row[0] = (R8)r["val"]; //val
                    row[1] = (U32)r["cnt"]; //count
                }
                
                while(q.more_results())
                    q.store_next();
                
                FastWriter writer;
                MakeSerializable<string> ser(writer.write(root));
                ostringstream ostr ;
                ostr << "GetRatioValueCount" << rid;
                cache().store_data(ostr.str(), ser);
                response().out() << ser.get();

            }
            catch(const std::exception ex)
            {
                throw ex;
            }
        }


        void CBaseApplication::GetRatioGroups() 
        {
            try
            {
                SP<IXbrl> papi;
                THROWIFFAILED(XbrlOpen(&papi), "Failed to open fin gateway.");
                SP<IFinancialRatioGroupList> pl;
                THROWIFFAILED(papi->GetRatioGroups(&pl), "Failed to get json output for ratio groups.");
                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                int i = 1;
                for (IFinancialRatioGroup* g : *pl)
                {
                    if (g->get_IsHidden())
                        continue;
                    
                    Value& row = root[i++];
                    row[0] = (U32)g->get_Id();
                    row[1] = g->get_Name();
                }
                
                FastWriter writer;
                MakeSerializable<string> ser(writer.write(root));
                cache().store_data("GetRatioGroups", ser);
                response().out() << ser.get();
            }
            catch(const std::exception ex)
            {
                throw ex;
            }
        }

        void CBaseApplication::GetRatios(unsigned long groupid) 
        {
            try
            {
                
                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                if (groupid == 255)
                    response().out() << root;
                    
                SP<IXbrl> papi;
                THROWIFFAILED(XbrlOpen(&papi), "Failed to open fin gateway.");
                SP<IFinancialRatioList> pl;
                THROWIFFAILED(papi->GetRatiosForGroup(groupid, &pl), "Failed to get json output for ratios.");
                int i = 1;
                ostringstream ostr;
                SP<IWebSession> pws;
                get_Session(&pws);
                for (IFinancialRatio* p : *pl)
                {
                    if (!p->get_IsFree() && !IsAuthenticated() )
                        continue;
                        
                    Value& row = root[i++];
                    row[0] = (U32)p->get_Id();
                    row[1] = p->get_Name();
                    ostr.str("");
                    ostr << "Description: " << p->get_Description() << std::endl;
                    if (p->get_MinimumPoint() != MINR8)
                    {
                        ostr << "Valid Range is between ";
                        ostr << p->get_MinimumPoint();
                        ostr << " and ";
                        ostr << p->get_MaximumPoint() << ". ";
                    }
                    if (p->get_MiddlePoint() != ER8)
                    {
                        ostr << "The value of a performing company is close to ";
                        ostr << p->get_MiddlePoint() << ".";
                    }
                    row[2] = ostr.str();
                }

                FastWriter writer;
                MakeSerializable<string> ser(writer.write(root));
                if (groupid)
                {
                    ostringstream ostr;
                    ostr << "GetRatios" << groupid;
                    cache().store_data(ostr.str(), ser, 86400);
                }
                else
                {
                    cache().store_data("GetRatios", ser, 86400);
                }
                response().out() << ser.get();
                
                
            }
            catch(const std::exception ex)
            {
                throw ex;
            }

        }


        void CBaseApplication::GetExchanges() 
        {
            try
            {
                SP<ISecurityAPI> papi;
                THROWIFFAILED(SecOpen(&papi), "Failed to open security gateway.");
                SP<ISecurityExchangeList> pl;
                THROWIFFAILED(papi->get_Exchanges(&pl), "Failed to get json out put exchanges.");
                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                int i = 1;
                for (ISecurityExchange* s : *pl)
                {
                    Value& row = root[i++];
                    row[0] = (U32)s->get_Id();
                    row[1] = s->get_Name();
                }
                FastWriter writer;
                MakeSerializable<string> ser(writer.write(root));
                cache().store_data("GetExchanges", ser, 86400);
                response().out() << ser.get();
            }
            catch(const std::exception ex)
            {
                throw ex;
            }
        }

        void CBaseApplication::GetGICHierarchy(unsigned long sectorId) 
        {
            try
            {
                SP<ISecurityAPI> papi;
                THROWIFFAILED(SecOpen(&papi), "Failed to open security gateway.");
                SP<IIdNameList> pl;
                THROWIFFAILED(papi->get_GICHierarchy(sectorId, &pl), "Failed to get GIC list.");
                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                int i = 1;
                for(IIdName* s : *pl)                
                {
                    Value& row = root[i++];
                    row[0] = (U32)s->get_Id();
                    row[1] = s->get_Name();
                }
                FastWriter writer;
                MakeSerializable<string> ser(writer.write(root));
                if (sectorId)
                {
                    ostringstream ostr;
                    ostr << "GetGICHierarchy" << sectorId;
                    cache().store_data(ostr.str(), ser, 86400);
                }
                else
                {
                    cache().store_data("GetGICHierarchy", ser, 86400);
                }
                response().out() << ser.get();
            }
            catch(const std::exception ex)
            {
                throw ex;
            }

        }

        

        void CBaseApplication::DisplayBase() 
        {
//            if (session().is_set("registrationstage"))
//            {
//                Register reg(this->context());
//                reg.ClearRegSessionData();
//                session().erase("registrationstage");
//            }
        }

//        void CBaseApplication::GetYearlyFinancialsFlipped(unsigned long prodId) 
//        {
//            try
//            {
//                std::initializer_list<UID> xids = {27111, 30042, 20629, 16979, 20293, 22542, 23477};
//                SP<IXbrl> papi;
//                THROWIFFAILED(XbrlOpen(&papi), "xbrl api cannot be obtained.");
//                SP<IString> pcik;
//                if (S_False == papi->get_CIK(prodId, &pcik))
//                    throw seexception(E_NotFound, "CIK not found.");
//                SP<IFinancialsConsolidated> pcon;
//                THROWIFFAILED(papi->GetConsolidatedFinancials(pcik->get_Buffer(), &pcon), "Failed to get to consolidated financials.");
//                SP<IXbrlRecordList> pxrl;
//                THROWIFFAILED(pcon->get_Records(xids, &pxrl), "Failed to get financial records.");
//            }
//            catch(const seexception& ex)
//            {
//               // LOGE(_logger, "GetYearlyFinancialsFlipped failed. Unknown error. " << ex.what());
//                response().out() << "{\"Status\":0}";
//            }
//            catch(const std::exception& ex)
//            {
//               // LOGE(_logger, "GetYearlyFinancialsFlipped failed. Unknown error. " << ex.what());
//                response().out() << "{\"Status\":0}";
//            }
//        }

        void CBaseApplication::GetCompanySummaryBalanceSheetData(UID prodid) 
        {
            XbrlMatchId mids;
            mids.push_back(8);  // assets
            mids.push_back(17); // liabilities
            mids.push_back(9);  // cash/equivalents
            SP<IXbrl> pxbrl;
            if (ISOK(XbrlOpen(&pxbrl)))
            {
                SP<IString> pcik;
                if (ISOK(pxbrl->get_CIK(prodid, &pcik)))
                {
                    ostringstream ostr;
                    Value jsonroot;
                    SP<IXbrlRecordList> pxids;
                    XbrlMatchId availableIds;
                    SE_CODE ret ;
                    if (ISOK(pxbrl->GetSelectedAccounts(pcik->get_Buffer(), mids, availableIds, &pxids)))
                    {
                        Value& xval = jsonroot[0];
                        xval[0] = "YrQtr";
                        if (availableIds.size() != mids.size())
                        {
                            availableIds.clear();
                            mids.clear();
                            pxids.Clear();
                            mids.push_back(7);  // assets
                            mids.push_back(16); // liabilities
                            mids.push_back(9);  // cash/equivalents
                            ret = pxbrl->GetSelectedAccounts(pcik->get_Buffer(), mids, availableIds, &pxids);
                            if (ISOK(ret))
                            {
                                for (int i = 0; i != availableIds.size(); ++i)
                                {
                                    U32 id = availableIds[i];
                                   // LOGIP(id);
                                    switch(id)
                                    {
                                    case 7:
                                        xval[i+1] = "Assets";
                                        break;
                                    case 16:
                                        xval[i+1] = "Liabilities";
                                        break;
                                    case 9:
                                        xval[i+1] = "Cash/Equivalents";
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (int i = 0; i != availableIds.size(); ++i)
                            {
                                U32 id = availableIds[i];
                               // LOGIP(id);
                                switch(id)
                                {
                                case 8:
                                    xval[i+1] = "Assets(Current)";
                                    break;
                                case 17:
                                    xval[i+1] = "Liabilities(Current)";
                                    break;
                                case 9:
                                    xval[i+1] = "Cash/Equivalents";
                                    break;
                                }
                            }
                            ret = S_OK;
                        }
                        
                        if (FAILED(ret))
                        {
                            response().out() << jsonroot;
                            return;
                        }
                        
                        
                        typedef vector<R8> LastValues;
                        int accountnum = availableIds.size();
                        LastValues lastval(accountnum);  // this number should match # of accounts index.
                        for (int i=0; i!=accountnum; ++i)
                            lastval[i] = 0;
                        
                        int i = 1;
                        for (I16 year = pxids->get_HighestYear() - 5; 
                                year <= pxids->get_HighestYear(); 
                                ++year)
                        {
                            for (I16 q = 1; q <= 4; ++q)
                            {
                                int goodcounter = 0;
                                Value xval1 ;
                                ostr.str("");
                                ostr << "'" << (int)(year % 2000) << "Q" << q;
                                xval1[0] = ostr.str();
                                int j = 1;
                                for (IXbrlAbstractRecord* ptr : *pxids._ptr)
                                {
                                    IXbrlRecord* p = dynamic_cast<IXbrlRecord*>(ptr);
                                    SP<IXbrlField> pfld;
                                    if (ISOK(p->get_Financials(year, &pfld)))
                                    {
                                        OmniType ot = pfld->get_Q(q);
                                        if (ot.IsEmpty())
                                        {
                                            xval1[j] = lastval[j-1];
                                        }
                                        else
                                        {
                                            xval1[j] = lastval[j-1] = (R8)ot;
                                            goodcounter += 1;
                                        }
                                            
                                    }
                                    else
                                    {
                                        xval1[j] = lastval[j-1];
                                    }
                                    j++;
                                }
                                if (goodcounter == accountnum)
                                {
                                    jsonroot[i] = xval1;
                                    i++;
                                }
                            }
                        }
                        ostr.str("");
                        ostr << "GetCompanySummaryBalanceSheetData" << prodid;
                        FastWriter writer;
                        MakeSerializable<string> ser(writer.write(jsonroot));
                        cache().store_data(ostr.str(), ser, 3600);
                        response().out() << ser.get();
                    }
                }
            }
        }
        void CBaseApplication::GetCompanySummaryIncomeData(UID prodid) 
        {
            XbrlMatchId mids;
            mids.push_back(25);  // revenue
            mids.push_back(33); // cost of revenue
            mids.push_back(13);  // gross profit
            SP<IXbrl> pxbrl;
            if (ISOK(XbrlOpen(&pxbrl)))
            {
                SP<IString> pcik;
                if (ISOK(pxbrl->get_CIK(prodid, &pcik)))
                {
                    ostringstream ostr;
                    Value jsonroot;
                    SP<IXbrlRecordList> pxids;
                    XbrlMatchId availableIds;
                    if (ISOK(pxbrl->GetSelectedAccounts(pcik->get_Buffer(), mids, availableIds, &pxids)))
                    {
                        Value& xval = jsonroot[0];
                        xval[0] = "YrQtr";
                        for (int i = 0; i != availableIds.size(); ++i)
                        {
                            U32 id = availableIds[i];
                           // LOGIP(id);
                            switch(id)
                            {
                            case 25:
                                xval[i+1] = "Revenues";
                                break;
                            case 33:
                                xval[i+1] = "Cost";
                                break;
                            case 13:
                                xval[i+1] = "Gross Profit";
                                break;
                            }
                        }
                        
                        typedef vector<R8> LastValues;
                        int accountnum = availableIds.size();
                        LastValues lastval(accountnum);  // this number should match # of accounts index.
                        for (int i=0; i!=accountnum; ++i)
                            lastval[i] = 0;
                        
                        int i = 1;
                        for (I16 year = pxids->get_HighestYear() - 5; 
                                year <= pxids->get_HighestYear(); 
                                ++year)
                        {
                            for (I16 q = 1; q <= 4; ++q)
                            {
                                int goodcounter = 0;
                                Value xval1;
                                ostr.str("");
                                ostr << "'" << (int)(year % 2000) << "Q" << q;
                                xval1[0] = ostr.str();
                                int j = 1;
                                for (IXbrlAbstractRecord* ptr : *pxids._ptr)
                                {
                                    IXbrlRecord* p = dynamic_cast<IXbrlRecord*>(ptr);
                                   // LOGIP(p->get_Name());
                                    SP<IXbrlField> pfld;
                                    if (ISOK(p->get_Financials(year, &pfld)))
                                    {
                                        OmniType ot = pfld->get_Q(q);
                                        if (ot.IsEmpty())
                                        {
                                            xval1[j] = lastval[j-1];
                                        }
                                        else
                                        {
                                            xval1[j] = lastval[j-1] = (R8)ot;
                                            goodcounter += 1;
                                        }
                                            
                                    }
                                    else
                                    {
                                        xval1[j] = lastval[j-1];
                                    }
                                    j++;
                                }
                                if (goodcounter == accountnum)
                                {
                                    jsonroot[i] = xval1;
                                    i++;
                                }
                            }
                        }
                        ostr.str("");
                        ostr << "GetCompanySummaryIncomeData" << prodid;
                        FastWriter writer;
                        MakeSerializable<string> ser(writer.write(jsonroot));
                        cache().store_data(ostr.str(), ser, 3600);
                        response().out() << ser.get();
                        
                    }
                }
            }
        }

        void CBaseApplication::GetCompanySummaryCashflowData(unsigned long prodid) 
        {
            XbrlMatchId mids;
            mids.push_back(18);  // net financiing
            mids.push_back(19); // net investing
            mids.push_back(20);  // net operating.
            mids.push_back(2);  // # of shares
            mids.push_back(1);  // eps
            SP<IXbrl> pxbrl;
            if (ISOK(XbrlOpen(&pxbrl)))
            {
                SP<IString> pcik;
                if (ISOK(pxbrl->get_CIK(prodid, &pcik)))
                {
                    ostringstream ostr;
                    Value jsonroot;
                    SP<IXbrlRecordList> pxids;
                    XbrlMatchId availableIds;
                    if (ISOK(pxbrl->GetSelectedAccounts(pcik->get_Buffer(), mids, availableIds, &pxids)))
                    {
                        Value& xval = jsonroot[0];
                        xval[0] = "YrQtr";
                        for (int i = 0; i != availableIds.size(); ++i)
                        {
                            U32 id = availableIds[i];
                           // LOGIP(id);
                            switch(id)
                            {
                            case 18:
                                xval[i+1] = "Financing";
                                break;
                            case 19:
                                xval[i+1] = "Investing";
                                break;
                            case 20:
                                xval[i+1] = "Operating";
                                break;
                            case 2:
                                xval[i+1] = "Shares Outstanding";
                                break;
                            case 1:
                                xval[i+1] = "EPS";
                                break;
                            }
                        }        
                        typedef vector<R8> LastValues;
                        int accountnum = availableIds.size();
                        LastValues lastval(accountnum);  // this number should match # of accounts index.
                        for (int i=0; i!=accountnum; ++i)
                            lastval[i] = 0;
                        
                        int i = 1;
                        for (I16 year = pxids->get_HighestYear() - 5; 
                                year <= pxids->get_HighestYear(); 
                                ++year)
                        {
                            for (I16 q = 1; q <= 4; ++q)
                            {
                                int goodcounter = 0;
                                Value xval1 ;
                                ostr.str("");
                                ostr << "'" << (int)(year % 2000) << "Q" << q;
                                xval1[0] = ostr.str();
                                int j = 1;
                                for (IXbrlAbstractRecord* ptr : *pxids._ptr)
                                {
                                    IXbrlRecord* p = dynamic_cast<IXbrlRecord*>(ptr);
                                    SP<IXbrlField> pfld;
                                    if (ISOK(p->get_Financials(year, &pfld)))
                                    {
                                        OmniType ot = pfld->get_Q(q);
                                        if (ot.IsEmpty())
                                        {
                                            xval1[j] = lastval[j-1];
                                        }
                                        else
                                        {
                                            switch (ot.dt)
                                            {
                                            case DT_R8:
                                                xval1[j] = lastval[j-1] = (R8)ot;
                                                break;
                                            case DT_I64:
                                                xval1[j] = lastval[j-1] = (I64)ot;
                                                break;
                                            case DT_CSTR:
                                                xval1[j] = lastval[j-1] = strtod(ot, nullptr);
                                                break;
                                            }
                                            goodcounter += 1;
                                        }
                                            
                                    }
                                    else
                                    {
                                        xval1[j] = lastval[j-1];
                                    }
                                    j++;
                                }
                                if (goodcounter == accountnum)
                                {
                                    jsonroot[i] = xval1;
                                    i++;
                                }
                            }
                        }
                        ostr.str("");
                        ostr << "GetCompanySummaryCashflowData" << prodid;
                        FastWriter writer;
                        MakeSerializable<string> ser(writer.write(jsonroot));
                        cache().store_data(ostr.str(), ser, 3600);
                        response().out() << ser.get();
                    }
                }
            }

        }

        
        
        void CBaseApplication::GetYearlyFinancials(UID prodId)
        {
            try
            {
                std::initializer_list<UID> xids = {27111, 30042, 20629, 16979, 20293, 22542, 23477};
                SP<IXbrl> pxbrl;
                THROWIFFAILED(XbrlOpen(&pxbrl), "Xbrl api can't be opened.");
                SP<IString> pcik;
                if (S_False == pxbrl->get_CIK(prodId, &pcik))
                    throw seexception(E_NotFound, "CIK not found.");
                SP<IFinancialsConsolidated> pcon;
                THROWIFFAILED(pxbrl->GetConsolidatedFinancials(pcik->get_Buffer(), &pcon), "Failed to get to consolidated financials.");
                SP<IXbrlRecordList> pxrl;
                THROWIFFAILED(pcon->get_Records(xids, &pxrl), "Failed to get financial records.");
                Value root;
                Value& inner = root[0];
                inner[0] = "year";
                int i=1;
                auto& strm = response().out();
                stringstream str;
                for (auto it = pxrl->begin(); it != pxrl->end(); ++it)
                {
                    IXbrlAbstractRecord* paxr = *it;
                    if (!paxr->get_IsAbstract())
                    {
                        IXbrlRecord* pxr = static_cast<IXbrlRecord*>(paxr);
                        string name = pxr->get_Name();
                        boost::replace_all(name, "us-gaap:", "");
                        inner[i] = name;
                        int j=1;
                        for (I16 year = pxrl->get_LowerYear(); year <= pxrl->get_HighestYear(); ++year)
                        {
                            {
                            SP<IXbrlField> pfld;
                            THROWIFFAILED(pxr->get_Financials(year, &pfld), "Financial field retrieval failed.");
                            }
                            {
                            SP<IXbrlField> pfld;
                            THROWIFFAILED(pxr->get_Financials(year, &pfld), "Financial field retrieval failed.");
                            if (year == pxrl->get_HighestYear() && (double)pfld->get_FY() == 0)
                                break;
                            Value& values = root[j++];
                            str.str("");
                            str.clear();
                            str << std::round(year);
                            values[0] = str.str();
                            values[i] = (double)pfld->get_FY();
                            }
                        }
                        i++;
                    }
                }
                strm << root;
                
                
            }
            catch(const seexception& ex)
            {
               // LOGE(_logger, "GetYearlyFinancials failed. Unknown error. " << ex.what());
                response().out() << "{\"Status\":0}";
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "GetYearlyFinancials failed. Unknown error. " << ex.what());
                response().out() << "{\"Status\":0}";
            }
        }

        void CBaseApplication::ListPaypalCountries() 
        {
            try
            {
                SP<IAdminAPI> papi;
                THROWIFFAILED(AdminOpen(&papi), "AdminOpen failed.");
                SP<ICountryList> pCountries;
                THROWIFFAILED(papi->get_Countries(
                    &pCountries, 
                    [this](ICountry::QUERYFLDS fld, LGX& lgx, BOOL& include)
                    {
                        if (fld == ICountry::QUERYFLDS::IsPaypalavailable)
                        {
                            include=true;
                            return "1";
                        }
                        return "";
                    }), "Failed to get a list of countries.");
                    
                SP<ISerializedData<>> ser;
                CJsonSerializedData::Create(&ser, true);
                pCountries->Serialize(ser);
                SP<IString> output;
                ser->get_Output(&output);

                ostringstream ostr;

                ostr << "{ \"Status\" : 1, ";
                ostr << "\"Countries\" : ";
                ostr << output->get_Buffer();
                ostr << " } ";
                
                MakeSerializable<string> serdata(ostr.str());
                cache().store_data("ListPaypalCountries", serdata, 86400);
                
                response().out() << ostr.str();
                
            }
            catch(const seexception& ex)
            {
               // LOGE(_logger, "ListPaypalCountries failed. Unknown error. " << ex.what());
                response().out() << "{\"Status\":0}";
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "ListPaypalCountries failed. Unknown error. " << ex.what());
                response().out() << "{\"Status\":0}";
            }
        }
     
        void CBaseApplication::ListStates(const string& countryid) 
        {
            try
            {
                if (countryid.empty())
                    throw seexception("Invalid country id.");
                
                SP<IAdminAPI> papi;
                THROWIFFAILED(AdminOpen(&papi), "AdminOpen failed.");
                SP<IStateList> pStates;
                if (TYPECHECK::IsNumber(countryid.c_str()))
                {
                    THROWIFFAILED(papi->get_States(
                        &pStates, 
                        [this, &countryid](IState::QUERYFLDS fld, LGX& lgx, BOOL& include)
                        {
                            if (fld == IState::QUERYFLDS::CountryId)
                            {
                                include=true;
                                return countryid.c_str();
                            }
                            return "";
                        }), "Failed to get a list of countries.");
                }
                else
                {
                    SP<ICountry> pcountry;
                    THROWIFFAILED(papi->get_Country(
                            &pcountry, 
                            [this, &countryid](ICountry::QUERYFLDS fld, LGX& lgx, BOOL& include)
                            {
                                if (fld == ICountry::QUERYFLDS::TwoChar)
                                {
                                    include = true;
                                    return countryid.c_str();
                                }
                                return "";
                            }), "Failed to get the country.");
                            
                    THROWIFFAILED(papi->get_States(
                        &pStates, 
                        [this, &pcountry](IState::QUERYFLDS fld, LGX& lgx, BOOL& include)
                        {
                            if (fld == IState::QUERYFLDS::CountryId)
                            {
                                include=true;
                                stringstream str;
                                str << pcountry->get_Id();
                                return  str.str().c_str();
                            }
                            return "";
                        }), "Failed to get a list of countries.");
                }
                SP<ISerializedData<>> ser;
                CJsonSerializedData::Create(&ser, true);
                pStates->Serialize(ser);
                SP<IString> output;
                ser->get_Output(&output);
                
                ostringstream ostr;
                ostr << "{ \"Status\" : 1, ";
                ostr << "\"States\" : ";
                ostr << output->get_Buffer();
                ostr << " } ";
                
                ostringstream ostr2 ;
                ostr2 << "ListStates" << countryid;
                
                MakeSerializable<string> serdata(ostr.str());
                cache().store_data(ostr2.str(), serdata, 86400);
                response().out() << ostr.str();
                
            }
            catch(const seexception& ex)
            {
               // LOGE(_logger, "ListStates failed for country id " << countryid << ". Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "ListStates failed for country id " << countryid << ". Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }
        }

        void CBaseApplication::ListEconIndicatorCategories() 
        {
            try
            {
                SP<IEconomyApi> papi;
                THROWIFFAILED(EconOpen(&papi), "Econ api failed to open.");
                SP<IIndicatorCategoryList> pCatList;
                THROWIFFAILED(papi->get_IndicatorCategories(&pCatList),"Failed to get indicator categories from library.");

                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                header[2] = "Description";
                header[3] = "IsDisplayed";
                int i=1;
                for (IIndicatorCategory* pcat : *pCatList)
                {
                    if (pcat->get_IsDisplayed())
                    {
                        Value& row = root[i++];
                        row[0] = (I32)pcat->get_Id();
                        row[1] = pcat->get_Name();
                        row[2] = pcat->get_Description();
                        row[3] = pcat->get_IsDisplayed();
                    }
                }
                FastWriter fw;
                MakeSerializable<string> dataser(fw.write(root));
                cache().store_data("ListEconIndicatorCategories", dataser, 7200);
                
                response().out() << dataser.get();
                
            }
            catch(const seexception& ex)
            {
               // LOGE(_logger, "ListEconIndicatorCategories failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "ListEconIndicatorCategories failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }

        }

        void CBaseApplication::ListEconIndicators(unsigned long catId) 
        {
            try
            {
                SP<IEconomyApi> papi;
                THROWIFFAILED(EconOpen(&papi), "Econ api failed to open.");
                SP<IEconomyIndicatorList> pIndicators;
                THROWIFFAILED(papi->get_EconomicIndicators(
                &pIndicators,
                [catId](IEconomyIndicator::QUERYFLDS f, LGX& l, BOOL& i)
                {
                    if (f == IEconomyIndicator::CategoryId)
                    {
                        i = true;
                        std::stringstream str;
                        str << catId;
                        return str.str().c_str();
                    }
                    return "";
                }
                ), "Failed to get economic indicators from library.");

                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                header[2] = "Description";
                header[3] = "IsDisplayed";
                header[4] = "PublishPeriod";
                header[5] = "RecentPeriod";
                header[6] = "RecentValue";
                int i=1;
                for (IEconomyIndicator * p : *pIndicators)
                {
                    if (p->get_IsDisplayed())
                    {
                        Value& row = root[i++];
                        row[0] = (I32)p->get_Id();
                        row[1] = p->get_Name();
                        row[2] = p->get_Description();
                        row[3] = p->get_IsDisplayed();
                        row[4] = p->get_PublishPeriod();

                        try
                        {
                            auto tpl = papi->get_EconomicIndicatorLatestValue(p->get_Id());
                            std::ostringstream ostr;
                            ostr << get<0>(tpl);
                            row[5] = ostr.str();
                            row[6] = get<1>(tpl);
                        }
                        catch(...)
                        {
                            row[5] = "";
                            row[6] = 0;
                        }
                    }
                }
                FastWriter fw;

                ostringstream ostr2;
                ostr2 << "ListEconIndicators" << catId;

                MakeSerializable<string> dataser(fw.write(root));
                cache().store_data(ostr2.str(), dataser, 7200);

                response().out() << dataser.get();

            }
            catch(const seexception& ex)
            {
               // LOGE(_logger, "ListEconIndicators failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "ListEconIndicators failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }
        }

        void CBaseApplication::ListEconIndicatorDataList(unsigned long eid) 
        {
            try
            {
                SP<IEconomyApi> papi;
                THROWIFFAILED(EconOpen(&papi), "Econ api failed to open.");
                SP<IStoreQueryResult> pQueryResult;
                THROWIFFAILED(papi->get_EconomicIndicatorDataList(eid, &pQueryResult, 100), 
                        "Failed to get economic indicators data list from library.");
                
                /*
                 * [
                    ['period', 'value'],
                    ['2004',  1000],
                    ['2005',  1170],
                    ['2006',  660],
                    ['2007',  1030]
                   ]
                 */
                
                Value root;
                Value& header = root[0];
                header[0] = "Period";
                header[1] = "Value";
                
                auto& qr = pQueryResult->get_Result();
                int i=1;
                for (const Row& r : qr)
                {
                    Value& row = root[i++];
                    row[0] = (string)r[0];
                    row[1] = (R8)r[1];
                }
                
                ostringstream ostr2;
                ostr2 << "ListEconIndicatorDataList" << eid;

                FastWriter fw;
                MakeSerializable<string> dataser(fw.write(root));
                cache().store_data(ostr2.str(), dataser, 7200);
                response().out() << dataser.get();
                
            }
            catch(const seexception& ex)
            {
               // LOGE(_logger, "ListEconIndicators failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "ListEconIndicators failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }
        }
        
        
        void CBaseApplication::CheckUser(string userId)
        {
            response().set_content_header("application/json");
            try
            {
                SP<IAdminAPI> papi;
                THROWIFFAILED(AdminOpen(&papi), "AdminOpen failed.");
                ErrorId ret = papi->CheckUserLogin(userId.c_str());
                if (SUCCEEDED(ret))
                    response().out() << "{ \"Status\" : 1 }";
                else
                    response().out() << "{ \"Status\" : 0 }";
            }
            catch(const seexception& ex)
            {
               // LOGE(_logger, "CheckUser failed. Unknown error. " << ex.what());
                response().out() << "{\"Status\":0}";
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "CheckUser failed. Unknown error. " << ex.what());
                response().out() << "{\"Status\":0}";
            }
        }
        
        void CBaseApplication::GetFileBase(string filename)
        {
            string f = _rootPath + "/blob/" + filename;
            try
            {
                //LOG4CXX_INFO(_logger, "Invoking GetFileBAse for file " << filename);
                ifstream strm;
                strm.open(f);
                if (strm.is_open())
                {
                    response().out() << strm.rdbuf();
                    strm.close();
                    //LOG4CXX_INFO(_logger, "Successfully outputted file " << f);
                }
                else
                {
                    //LOG4CXX_WARN(_logger, "GetFileBase failed to open file " << f);
                }
            }
            catch (seexception const& ex)
            {
                //LOG4CXX_ERROR(_logger, "GetFileBase failed to open file " << f << " with error " << ex.what());
                throw ex;
            }
            catch (std::exception const& ex)
            {
                //LOG4CXX_ERROR(_logger, "GetFileBase failed to open file " << f << " with error " << ex.what());
                throw ex;
            }
        }
        
        void CBaseApplication::BuildHeaderMenus(Header& header, string selectedMenuHeader)
        {
            MenuItem menu = {"Home", "/", selectedMenuHeader == "Home" ? true : false};
            header.appMenus.push_back(menu);
            menu = {"Fundamentals", "/fundamentals", selectedMenuHeader == "Fundamentals" ? true : false};
            header.appMenus.push_back(menu);
            menu = {"Economy", "/economy", selectedMenuHeader == "Economy" ? true : false};
            header.appMenus.push_back(menu);
            menu = {"About", "/about", selectedMenuHeader == "About" ? true : false};
            header.appMenus.push_back(menu);
#ifdef __PAID__            
            if (IsAuthenticated())
            {
                menu = {"My Account", "/myaccount", selectedMenuHeader == "My Account" ? true : false};
                header.userMenus.push_back(menu);
                menu = {"Sign Out", "/signout", selectedMenuHeader == "Sign Out" ? true : false};
                header.userMenus.push_back(menu);
            }
            else
            {
                menu = {"Sign In", "/signin", selectedMenuHeader == "Sign In" ? true : false};
                header.userMenus.push_back(menu);
                menu = {"Register", "/register", selectedMenuHeader == "Register" ? true : false};
                header.userMenus.push_back(menu);
            }
#endif
        }
        
        
        void CBaseApplication::InitModel(BaseContent& content)
        {
            content.Init();
        }
        
        void CBaseApplication::ProcessSearch()
        {
            if (request().request_method() != "POST")
                return;
            
            auto& ctxt = context();
            auto& req = ctxt.request();
            auto& res = ctxt.response();
            string prodPage = session().is_set(PRODPAGE) ? session().get(PRODPAGE) : "summary";
            string const& path = req.path_info();
            const string& searchquery = req.post("search_text");
            Value root;
            Value& cols = root["cols"];
            auto createCol = [this](Value& cols, int colIndex, string colName, string colLabel, string type)
            {
                Value& col = cols[colIndex];
                col["id"] = colName;
                col["label"] = colLabel;
                col["type"] = type;
            };
            createCol(cols, 0, "Symbol", "Symbol", "string");
            createCol(cols, 1, "Company", "Company", "string");
            createCol(cols, 2, "Exchange", "Exchange", "string");
            
            if (!searchquery.empty())
            {
                SP<IXbrl> pxbrl;
                XbrlOpen(&pxbrl);
                SP<ISecurityDescriptionList> pList;
                ErrorId retVal = pxbrl->FindCompany(searchquery.c_str(), &pList);
                if (SUCCEEDED(retVal) && pList->get_Count())
                {
                    int i = 0;
                    ostringstream ostr;
                    Value& rows = root["rows"];
                    for (ISecurityDescription* p : *pList)
                    {
                        Value& row = rows[i++];
                        ostr.str("");
                        ostr << "<a href='/fundamentals/p/" << prodPage << "/" << p->get_Id() << "'>" << p->get_Symbol() << "</a>";
                        row["c"][0]["v"] =  ostr.str();
                        ostr.str("");
                        ostr << "<a href='/fundamentals/p/" << prodPage << "/" << p->get_Id() << "'>" << p->get_Name() << "</a>";
                        row["c"][1]["v"] =  ostr.str();
                        row["c"][2]["v"] = p->get_Exchange();
                    }
                }
            }
            FastWriter writer;
            response().set_content_header("application/json");
            response().out() << writer.write(root);
        }
        
        void CBaseApplication::DisplayRegister()
        {
            Register reg(context());
            Master master(context(), reg);
            master.Title = "Smartearn | Register";
            master.Init();
            BuildHeaderMenus(master.header, "Register");
            session().set("registrationstage", "register");
            try
            {
                if (request().request_method()=="POST")
                {
                    // doublecheck form fields.
                    http::request& rq = request();
                    const string& sEmail = rq.post("reg_txtemail");
                    const string& sHasAgreed = rq.post("reg_rdaggree");

                    reg.set_Login(rq.post("reg_txtemail"));
                    reg.set_Password(rq.post("reg_txtpassword"));
                    reg.set_HasAgreed(sHasAgreed=="Yes");

                    // grab the user instance out from session and update properties.
                    IUser* pUser = reg.get_User();
                    pUser->set_UserId(sEmail.c_str());
                    SP<IAdminAPI> papi;
                    ErrorId retCode = AdminOpen(&papi);
                    if (FAILED(retCode))
                    {
                        master.set_Error("Internal API error. Please retry.");
                       // LOGE(_logger, "Register: SessionId: " << session().get_session_cookie() << ". Failed AdminOpen call.");
                        goto TOERROR;
                    }
                   
                    pUser->add_Role(UserRole::Standard);

                    IPaypalExpressCheckout* ppp;
                    retCode = papi->get_PaypalExpressCheckOut(&ppp);
                    if (FAILED(retCode))
                    {
                        master.set_Error("Express check out failed to link with Paypal service. Service unavailable.");
                        goto TOERROR;
                    }
                    reg.set_PaypalInstance(ppp);

                    SP<IString> token;
                    retCode = ppp->Step01_SetExpressCheckout(
                            "https://www.smart-earn.com", 
                            "https://www.smart-earn.com",
                            "https://www.smart-earn.com/smartearn.png",
                            &token);
                    if (FAILED(retCode))
                    {
                        master.set_Error("Express check out failed to link with Paypal service. Service unavailable.");
                       // LOGE(_logger, "Register: SessionId: " << session().get_session_cookie() << ". Failed Step01_SetExpressCheckout call.");
                        goto TOERROR;
                    }
                    // redirect user
                    SP<IApp> papp;
                    retCode = se::SEOpenApp(&papp);
                    if (FAILED(retCode))
                    {
                        master.set_Error("Internal API error. Please retry.");
                       // LOGE(_logger, "Register: SessionId: " << session().get_session_cookie() << ". Failed SEGetApp call.");
                        goto TOERROR;
                    }
                    SP<se::config::IConfigurator> pCfg;
                    retCode = papp->get_Configurator(&pCfg);
                    if (FAILED(retCode))
                    {
                        master.set_Error("Internal API error. Please retry.");
                       // LOGE(_logger, "Register: SessionId: " << session().get_session_cookie() << ". Failed get_Configurator call.");
                        goto TOERROR;
                    }
                    SP<se::config::IConfigItemList> pCfgList;
                    retCode = pCfg->get_Settings(GLOBALREGIONID, "Paypal", &pCfgList);
                    if (FAILED(retCode))
                    {
                        master.set_Error("Internal API error. Please retry.");
                       // LOGE(_logger, "Register: SessionId: " << session().get_session_cookie() << ". Cannot open IConfigItemList.");
                        goto TOERROR;
                    }
                    string paypalexpresscheckout = pCfgList->get_Item("ExpressCheckoutPage")->get_Value();
                    paypalexpresscheckout += token->get_Buffer();
                    response().set_redirect_header(paypalexpresscheckout);

                    // sample response 
                    // approved: https://www.smart-earn.com/ppapproved/?token=EC-3KF98176JF0547625
                    // canceled: https://www.smart-earn.com/ppcancel/?token=EC-23R098739D609324L
                    return;

                }
                else
                {
                    // create an save a user instance.
                    IUser* pUser ;
                    if (FAILED(IUser::Create(&pUser)))
                        goto TOERROR;
                    // save it session.
                    reg.Init(pUser);
                }
            
            }
            catch(const seexception& ex)
            {
                master.set_Error("Website processing error.");
               // LOGE(_logger, "Register: SessionId: " << session().get_session_cookie() << ". Page error: " << ex.what());
                goto TOERROR;
            }
            catch(const std::exception& ex)
            {
                master.set_Error("Website processing error.");
               // LOGE(_logger, "Register: SessionId: " << session().get_session_cookie() << ". Page error: " << ex.what());
                goto TOERROR;
            }
            catch(...)
            {
                master.set_Error("Website processing error.");
               // LOGE(_logger, "Register: SessionId: " << session().get_session_cookie() << ". Page fatal error.");
                goto TOERROR;
            }
            

            
        TOPAGE:
            render("MasterView", master);
            return;
            
        TOERROR:
            reg.ClearRegSessionData();
            session().reset_session();
            render("ErrorView", master);
            return;       
             
        }
        
        void CBaseApplication::DisplayPPApproved() 
        {
            Register reg(context());
            Master master(context(), reg);
            master.Init();
            try
            {
                if (request().request_method()=="POST")
                {
                    session().set("registrationstage", "postapproval");
                    master.Title = "Smartearn | Register | Thank you";
                    
                    http::request& rq = request();
//                    const string& sformname = rq.http_form();
//                    if (rq.http_form()!="ppapprove_form")
//                    {
//                        response().set_redirect_header("/");
//                        return;
//                    }
                    
                    IUser* pUser = reg.get_User();
                    IPaypalExpressCheckout* ppp = reg.get_PaypalInstance();
                    if (rq.post("reg_hdedit")=="1")
                    {
                        // capture form variables
                        pUser->set_FirstName(rq.post("ppa_txtfirstname").c_str());
                        pUser->set_LastName(rq.post("ppa_txtlastname").c_str());
                        SP<IAddress> pAddr;
                        pUser->get_Address(&pAddr);
                        pAddr->set_Street(rq.post("ppa_txtstreet1").c_str());
                        pAddr->set_Street2(rq.post("ppa_txtstreet2").c_str());
                        pAddr->set_City(rq.post("ppa_txtcity").c_str());
                        pAddr->set_Country(rq.post("ppa_cmbcountries").c_str());
                        pAddr->set_State(rq.post("ppa_cmbstates").c_str());
                        pAddr->set_PostalCode(rq.post("ppa_txtpostalcode").c_str());
                        
                    }
                    SP<IAdminAPI> pApi;
                    ErrorId retCode = AdminOpen(&pApi);
                    if (FAILED(retCode))
                    {
                       // LOGEP("DisplayPPApproved::Postback: Failed to open admin instance.");
                        master.set_Error("Sorry for the inconvenience. Due to an internal error, trial registration failed. No information will be saved in our system including any payment information. Please retry registration by clicking 'Register' link above.");
                        goto TOERROR;
                    }

                    // set up trial agreement with paypal
                    
                    // show a final page of success and thanks
                    // unload IUser and ppp instances from session.
                    
                    SP<IProduct> pProduct;
                    retCode =pApi->get_Product(1, &pProduct);
                    if (FAILED(retCode))
                    {
                       // LOGEP("DisplayPPApproved::Postback: Failed to get fundatamental product instance.");
                        master.set_Error("Sorry for the inconvenience. Due to an internal error, trial registration failed. No information will be saved in our system including any payment information. Please retry registration by clicking 'Register' link above.");
                        goto TOERROR;
                    }
                    
                    SP<IString> pProfileIdReturned;
                    SP<IString> pProfileStatus;
                    DATETIME dt = DATETIME::Now();
                    retCode = ppp->Step03_CreateRecurringPaymentsWithTrialProfile(
                            pProduct, 
                            reg.get_PayPalPayerId().c_str(),
                            dt,
                            &pProfileIdReturned,
                            &pProfileStatus);
                    if (FAILED(retCode))
                    {
                       // LOGEP("DisplayPPApproved::Postback: Failed to register user with paypal profile in the system.");
                        master.set_Error("Sorry for the inconvenience. Due to an internal error, trial registration failed. No information will be saved in our system including any payment information. Please retry registration by clicking 'Register' link above.");
                        goto TOERROR;
                    }
                    // save user info in db (login, address, payment type, etc)
                    SP<IUserSession> pu;
                    IUserSession::Create(&pu);
                    retCode = pApi->RegisterUser(pUser, reg.get_Password().c_str(), reg.get_PayPalPayerId().c_str(), pu._ptr);
                    if (FAILED(retCode))
                    {
                        pApi->UnregisterUser(pUser->get_Id());
                        ppp->Step0x1_CancelRecurringPaymentsProfileStatus(reg.get_PayPalPayerProfileId().c_str(), "System User Registration failed", &pProfileIdReturned);
                       // LOGEP("DisplayPPApproved::Postback: Paypal api failed to create a trial payment profile.");
                        master.set_Error("Sorry for the inconvenience. Due to an internal error, trial registration failed. No information will be saved in our system including any payment information. Please retry registration by clicking 'Register' link above.");
                        goto TOERROR;
                    }
                    session().set("registrationstage", "finalised");
                    reg.set_PaypalProfileStatus((string)pProfileStatus->get_Buffer());
                    SP<IUser> pCreatedUser;
                    pu->get_User(&pCreatedUser);
                    assert(pCreatedUser->get_Id() == pUser->get_Id());
                    reg.ModifyView("PPFinalView");
                    goto TOFINALPAGE;
                }
                else
                {
                    session().set("registrationstage", "approval");
                    reg.ModifyView("PPApprovedView");
                    master.Title = "Smartearn | Register | Thank you";
                    
                    // sample response 
                    // approved: https://www.smart-earn.com/ppapproved/?token=EC-3KF98176JF0547625
                    // canceled: https://www.smart-earn.com/ppcancel/?token=EC-23R098739D609324L
                    
                    ErrorId retCode;
                    // capture user info
                    // what is the token ?
                    string token = request().get("token");
                    if (token.empty())
                    {
                        master.set_Error("Paypal express check out failed. Token is missing. Cannot continue.");
                       // LOGE(_logger, "PPApproved: SessionId: " << session().get_session_cookie() << ". Missing token.");
                        goto TOERROR;
                    }
                    IUser* pUser = reg.get_User();
                    IPaypalExpressCheckout* ppp = reg.get_PaypalInstance();
                    BOOL hasAccepted;
                    SP<IString> payerId;
                    retCode = ppp->Step02_GetExpressCheckoutDetails(token.c_str(), &hasAccepted, pUser, &payerId);
                    if (FAILED(retCode))
                    {
                        master.set_Error("Express check out failed to link with Paypal service. Service unavailable.");
                       // LOGE(_logger, "PPApproved: SessionId: " << session().get_session_cookie() << ". Failed to execute Step02_GetExpressCheckoutDetails method.");
                        goto TOERROR;
                    }
                    reg.set_PayPalPayerId((string)payerId->get_Buffer());
                    
                    if (!hasAccepted)
                    {
                        // redirecto to cancel page.
                        response().set_redirect_header("/ppcancel");
                        return;
                    }
                    // now display approval info.
                    // display user's address 
                    // ask user to verify onces            render("MasterView", master);

                    // click "Accept" -> will create a recurring trial payment profile.
                    // click "Do not accept" -> will send to cancel page.
                }
            }
            catch(const seexception& ex)
            {
                master.set_Error("Website processing error.");
               // LOGE(_logger, "PPApproved: SessionId: " << session().get_session_cookie() << ". Page error: " << ex.what());
                goto TOERROR;
            }
            catch(const std::exception& ex)
            {
                master.set_Error("Website processing error.");
               // LOGE(_logger, "PPApproved: SessionId: " << session().get_session_cookie() << ". Page error: " << ex.what());
                goto TOERROR;
            }
            catch(...)
            {
                master.set_Error("Website processing error.");
               // LOGE(_logger, "PPApproved: SessionId: " << session().get_session_cookie() << ". Page fatal error.");
                goto TOERROR;
            }
            
        TOPAGE:
            BuildHeaderMenus(master.header, "Register");
            render("MasterView", master);
            return;
            
        TOFINALPAGE:
            BuildHeaderMenus(master.header, "My Account");
            render("MasterView", master);
            reg.ClearRegSessionData();
            return;
            
        TOERROR:
            reg.ClearRegSessionData();
            session().reset_session();
            BuildHeaderMenus(master.header, "Register");
            render("ErrorView", master);
            return;       
        }

        void CBaseApplication::DisplayPPCanceled() 
        {
            Register reg(context());
            Master master(context(), reg);
            master.Title = "Smartearn | Register | Cancel";
            master.Init();
            BuildHeaderMenus(master.header, "Register");
            session().set("registrationstage", "cancel");
            if (request().request_method()=="POST")
            {
                
            }
        }
        

        
        void CBaseApplication::DisplaySignIn()
        {
            SignIn mod(context());
            Master master(context(), mod);
            master.Title = "Smartearn | Sign In";
            master.Init();
            BuildHeaderMenus(master.header, "Sign In");
            http::request& req = request();
            try
            {
                if (req.request_method() == "POST")
                {
                    mod.Login = req.post("signin_txtemail");
                    if (mod.Login.empty() && 
                            req.post("signin_txtpassword").empty())
                    {
                        mod.DisplayError = "inline";
                        mod.ErrorMessage = "Please enter email address and password to sign in.";
                    }
                    else if (mod.Login.empty())
                    {
                        mod.DisplayError = "inline";
                        mod.ErrorMessage = "Please enter email address to sign in.";
                    }
                    else if (req.post("signin_txtpassword").empty())
                    {
                        mod.DisplayError = "inline";
                        mod.ErrorMessage = "Please enter password to sign in.";
                    }
                    else
                    {
                        SP<IUserSession> pUserSession;
                        IUserSession::Create(&pUserSession);
                        if (!_pAdm->AuthenticateUser(mod.Login.c_str(), req.post("signin_txtpassword").c_str(), pUserSession._ptr))
                        {
                            mod.DisplayError = "inline";
                            mod.ErrorMessage = "Sign in failed. Incorrect email address and/or password. Please retry.";
                        }
                        else
                        {
                            session().set(USERID, (UID)pUserSession->get_UserId());
                            // redirect
                            response().set_redirect_header("/myaccount");
                            return;
                        }
                    }
                }
                else
                {
                    if (session().is_set("registrationstage")) // check the stage. if completed, then display email address.
                    {
                        // finally clear the session data for registration
                        Register reg(context());
                        reg.ClearRegSessionData();
                    }
                }
                render("MasterView", master);
                return;
            }
            catch(const seexception& ex)
            {
                master.set_Error(ex.what());
                render("ErrorView", master);
            }
            catch(const std::exception& ex)
            {
                master.set_Error(ex.what());
                render("ErrorView", master);
            }
            catch(...)
            {
                master.set_Error("Unknown");
                render("ErrorView", master);
            }
        }
        
        void CBaseApplication::DisplaySignout()
        {
            cache().rise(session().get_session_cookie());
            session().clear();
            session().reset_session();
            response().set_redirect_header("/");
        }
        
        void CBaseApplication::DisplayMyAccount()
        {
            // if user hasn't logged, don't allow my account
            SP<IWebSession> pws;
            get_Session(&pws);
            if (!IsAuthenticated())
            {
                response().set_redirect_header("/");
                return;
            }
            MyAccount acc(context());
            Master master(context(), acc);
            master.Title = "Smartearn | My Account";
            master.Init();
            BuildHeaderMenus(master.header, "My Account");
            render("MasterView", master);
        }
        
    }
}

