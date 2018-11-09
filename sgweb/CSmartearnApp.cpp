/* 
 * File:   CSmartgazeApp.cpp
 * Author: santony
 * 
 * Created on August 18, 2013, 12:22 PM
 */

#include <boost/algorithm/string/replace.hpp>


#include "sgwebinternal.h"
#include "CSmartearnApp.h"
#include "CAdminApp.h"
#include "CSmartearnApp.h"
#include "models.h"
#include "CSearchController.h"
#include <fstream>
//#include <log4cxx/logger.h>
#include <cppcms/http_cookie.h>
#include <iomanip>
#include "../seglobal/CJsonSerializedData.h"
#include "../seglobal/exceptions.h"
#include "../semain/semain.h"
#include "../semysql/CSEConnections.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include "../external/mariadbpp/connection.h"
#include "../external/mariadbpp/query.h"
#include "../seglobal/CStringUtility.h"
#include "../senet/senetapi.h"
#include "../senet/CSmtpClient.h"
#include "../external/mariadbpp/ssqls.h"
#include "../semath/CCommon.h"
#include <boost/regex/v4/regex.hpp>

using namespace std;
using namespace sg::web::models;
using namespace se;
using namespace se::data;
using namespace Json;
using namespace cppcms::http;
using namespace mysqlpp;

#define SEDBCONN CSEConnections::GetMain()

namespace sg
{
    namespace web
    {
        
        const string PRODPAGE = "PRPG";

        CSmartearnApp::CSmartearnApp(cppcms::service& srv) : 
            CBaseApplication(srv)
        {
            //CBaseApplication::_logger = Logger::getLogger("sg.web.smartearnapp"); 
            SP<IApp> papp;
            SEOpenApp(&papp);
            THROWIFFAILED(papp->get_Configurator(&_pconfig), "Failed to open configuration instance");
            Init(srv);
        }

        CSmartearnApp::~CSmartearnApp() 
        {
            
        }
        
        void CSmartearnApp::Init(cppcms::service& srv)
        {
            CBaseApplication::Init(srv);

            // if maintenance mode is enabled, display that page.
            if (_pconfig->get_IsInMaintenanceMode())
            {
                // maintenance page.
                dispatcher().assign("", &CSmartearnApp::DisplayMaintenancePage, this);
                mapper().assign("");
                mapper().root("/");
                return;
            }

            dispatcher().assign("^/searchlist$", &CSmartearnApp::ProcessSearchList, this);
            mapper().assign("searchlist", "/searchlist/");
            
            dispatcher().assign("^/economy$", &CSmartearnApp::DisplayEconomyView, this, 1);
            mapper().assign("economy", "/economy");

            dispatcher().assign("^/economy/(\\d+)$", &CSmartearnApp::DisplayEconomyView, this, 1);
            mapper().assign("economy", "/economy/{1}");

            dispatcher().assign("^/about$", &CSmartearnApp::DisplayAboutView, this);
            mapper().assign("about", "/about/");
            
            dispatcher().assign("^/sitemap$", &CSmartearnApp::DisplaySitemap, this);
            mapper().assign("sitemap", "/sitemap/");
            
            dispatcher().assign("^/fundamentals/p/(\\d+)$", &CSmartearnApp::DisplayFundamentals, this, 1);
            mapper().assign("f_p", "/fundamentals/p/{1}");

            dispatcher().assign("^/fundamentals/p/(summary|financials|performance|prospects|news)/([\\w\\W]*)$", &CSmartearnApp::DisplayFundamentals, this, 1, 2);
            mapper().assign("f_p_s", "/fundamentals/p/{1}/{2}");
            
            dispatcher().assign("^/fundamentals/list/si/(\\d+)$", &CSmartearnApp::DisplayListSameIndustries, this, 1);
            mapper().assign("f_list_si", "/fundamentals/list/si/{1}");

            dispatcher().assign("^/fundamentals/list/pf/(\\d+)$", &CSmartearnApp::DisplayListSimilarPerformances, this, 1);
            mapper().assign("f_list_pf", "/fundamentals/list/pf/{1}");

            dispatcher().assign("^/fundamentals/p/json/mp/(\\d+)$", &CSmartearnApp::OutputJsonMonthlyPriceHistory, this, 1);
            mapper().assign("f_p_json_mp", "/fundamentals/p/json/mp/{1}");

            dispatcher().assign("^/fundamentals/p/json/if/(\\d+)$", &CSmartearnApp::OutputJsonCompanyInfo, this, 1);
            mapper().assign("f_p_json_if", "/fundamentals/p/json/if/{1}");
            
            dispatcher().assign("^/fundamentals$", &CSmartearnApp::DisplayCompanyPicks, this);
            mapper().assign("fundamentals", "/fundamentals/");

            dispatcher().assign("^/fundamentals/screener$", &CSmartearnApp::DisplayCompanyScreener, this);
            mapper().assign("fund_screener", "/fundamentals/screener");

            dispatcher().assign("^/fundamentals/portfolio$", &CSmartearnApp::DisplayPortfolioBuilder, this);
            mapper().assign("fund_pf", "/fundamentals/portfolio");
            
            dispatcher().assign("^/home$", &CSmartearnApp::Default, this);
            mapper().assign("home", "/home/");
            
            dispatcher().assign("^/([a-zA-Z]{1,5}$)", &CSmartearnApp::DisplayFundamentals, this, 1);
            mapper().assign("symbolsearch", "/{1}");
            
            // default page.
            dispatcher().assign("", &CSmartearnApp::Default, this);
            mapper().assign("");

            mapper().root("/");
            
        }
        
        
        void CSmartearnApp::init()
        {
            CBaseApplication::init();
            
            if (!_pSecApi)
                THROWIFFAILED(SecOpen(&_pSecApi), "Failed to open security api. Insuffcient memory.");

            if (!_pxbrl)
                THROWIFFAILED(XbrlOpen(&_pxbrl), "Failed to open fin api. Insuffcient memory.");
        }
                
        void CSmartearnApp::GetFile(string filename)
        {
            //LOG4CXX_INFO(_logger,"Blob download request for file name " << filename);
            GetFileBase(filename);
        }
        
        
        
        /***************************************************/

        
        void CSmartearnApp::Default()
        {
            if (cache().fetch_page("home"))
                return;
            
            DisplayBase();
            Home home(context());
            CSmartConnection conn(SEDBCONN);
            Query q = conn->query();
            q << "call get_RandomProductId()";
            UID prodId = 4642;  //IBM
            try
            {
                auto sqr = q.store();
                if (sqr.size())
                    prodId = sqr[0][0];
                while (q.more_results())
                    q.store_next();
            }
            catch(...)
            {
                while (q.more_results())
                    q.store_next();
                throw;
            }
            home.Init(prodId);
            Master master(context(), home);
            master.Title = "Smartearn | Home Page";
            master.Init();
            BuildHeaderMenus(master.header, "Home");
            render("MasterView", master);
            
            cache().store_page("home", 900);
        }

        void CSmartearnApp::DisplayMaintenancePage() 
        {
            if (cache().fetch_page("maintenancepage"))
                return;
            
            DisplayBase();
            Home home(context());
            Master master(context(), home);
            master.Title = "Smartearn | Home Page";
            master.Init();
            //BuildHeaderMenus(master.header, "Home");
            master.set_Error("Sorry, the site is currently on a maintenance schedule. We will be back soon. Thank you for your patience.", false);
            render("MaintainView", master);
            
            cache().store_page("maintenancepage", 3600);
        }

        
        void CSmartearnApp::ViewSubscriptions()
        {
            DisplayBase();
            Subscriptions subs(context());
            Master master(context(), subs);
            master.Title = "Smartearn | Subscriptions";
            master.Init();
            render("MasterView", master);
        }
        
        void CSmartearnApp::ProcessSearchList()
        {
            DisplayBase();
            if (session().is_set("searchlist"))
            {
                IROSPList<ISecurityDescription>* pList = (IROSPList<ISecurityDescription>*)session().get<LPVOID>("searchlist");
                session().erase("searchlist");
                Search search(context());
                search.SetList(pList);
                Master master(context(), search);
                master.Title = "Smartearn | Search Results";
                master.Init();
                BuildHeaderMenus(master.header, "Fundamentals");
                render("MasterView", master);
                pList->DecRef();
            }
            else
                response().set_redirect_header("/");
        }
        
        
        void CSmartearnApp::DisplayEconomyView(string catid)
        {
            
            string cachename = "DisplayEconomyView" + (catid.empty() ? "0" : catid);
            if (cache().fetch_page(cachename))
                return;

           // LOGIP("Going to display economy page.");

            DisplayBase();
            Economy eco(context());
            Master master(context(), eco);
            try
            {
                master.Init();
                BuildHeaderMenus(master.header, "Economy");
                if (catid.empty() || catid == "0")
                {
                    // show news section . see model class.
                    eco.setSelectedCategoryId(0);
                    eco.CategoryName = "News";
                    master.Title = "Smartearn | Economic Data | " + eco.CategoryName;
                }
                else
                {
                    UID categoryId = 0;
                    categoryId = strtol(catid.c_str(), nullptr, 10);
                    eco.setSelectedCategoryId(categoryId);
                    SP<IIndicatorCategory> pCat;
                    THROWIFFAILED(eco.getCategories()->get_ItemById(categoryId, &pCat), "Category for id not found.");
                    eco.CategoryName = pCat->get_Name();
                    master.Title = "Smartearn | Economic Data | " + eco.CategoryName;
                }
                
                goto TOPAGE;

            }
            catch(const seexception& ex)
            {
                master.set_Error("Website processing error.");
               // LOGE(_logger, "Economy: SessionId: " << session().get_session_cookie() << ". Page error: " << ex.what());
                goto TOERROR;
            }
            catch(const std::exception& ex)
            {
                master.set_Error("Website processing error.");
               // LOGE(_logger, "Economy: SessionId: " << session().get_session_cookie() << ". Page error: " << ex.what());
                goto TOERROR;
            }
            catch(...)
            {
                master.set_Error("Website processing error.");
               // LOGE(_logger, "Economy: SessionId: " << session().get_session_cookie() << ". Page fatal error.");
                goto TOERROR;
            }
            
        TOPAGE:
           // LOGIP("Now going to render of economy page.");
            render("MasterView", master);
           // LOGIP("Done with display of economy page.");
            
            cache().store_page(cachename, 1800);
            
            return;
            
        TOERROR:
            render("ErrorView", master);
            return;       
            
            
        }
        
        void CSmartearnApp::DisplayAboutView()
        {
            DisplayBase();
            About about(context());
            Master master(context(), about);
            master.Title = "Smartearn | About this site";
            master.Init();
            BuildHeaderMenus(master.header, "About");
            
            try
            {
            
            if (request().request_method()== "POST")
            {
                const std::string& name = request().post("contact_txt_name");
                const std::string& email = request().post("contact_txt_email");
                const std::string& org = request().post("contact_txt_org");
                const std::string& street = request().post("contact_txt_street");
                const std::string& city = request().post("contact_txt_city");
                const std::string& state = request().post("contact_txt_state");
                const std::string& zip = request().post("contact_txt_zip");
                const std::string& country = request().post("contact_txt_country");
                const std::string& sub = request().post("contact_txt_subject");
                const std::string& msg = request().post("contact_txt_message");
                
                if (name.empty() || email.empty() || street.empty() || 
                        state.empty() || city.empty() || 
                        sub.empty() || msg.empty())
                {
                    about.set_Error("Name, email, street, state, city, subject and message are required fields.");
                    about.StatusLabelClass = "statuslabel";
                }
                else
                {
                    //se::net::CSmtpClient smtp;
                    std::ostringstream ostr;
                    ostr << "Contact Message from " << endl;
                    if (!org.empty())
                        ostr << org << "," << endl;
                    ostr << street << "," << endl;
                    ostr << city << "," << endl;
                    ostr << state << ", Zip: " << zip << "," << endl;
                    ostr << country << "." << endl;
                    ostr << "Message:" << endl;
                    ostr << msg << endl;
                    
                    //smtp.SendMail("info@smart-earn.com", email.c_str(), sub.c_str(), ostr.str().c_str());
                    
                    SP<se::net::CMailSenderTask> sender;
                    se::net::CMailSenderTask::Create(&sender);
                    sender->set_From(email.c_str());
                    sender->set_ReceiptTo("info@smart-earn.com");
                    sender->set_Subject(sub.c_str());
                    sender->set_Message(ostr.str().c_str());

                    SP<IApp> papp;
                    se::SEOpenApp(&papp);
                    SP<se::threading::IThreadPool> ppool;
                    if (ISOK(papp->get_ThreadPool(se::threading::TPT_SynchronousPool, &ppool)) && ppool)
                        ppool->RunTask(sender);
                    assert(ppool != nullptr);
                    

                    about.Done = true;
                }
            }

            render("MasterView", master);
            
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "Failed to deliver message from the contact (about page). Error: " << ex.what());
                master.set_Error("Sorry. Failed to submit your message. Please retry.");
                render("ErrorView", master);
            }
        }

        void CSmartearnApp::DisplaySitemap() 
        {
            if (cache().fetch_page("sitemap"))
                return;

            DisplayBase();
            Sitemap sm(context());
            Master master(context(), sm);
            master.Title = "Smartearn | Sitemap";
            master.Init();
            BuildHeaderMenus(master.header, "About");
            render("MasterView", master);

            cache().store_page("sitemap");
        }

        
        /////////////////////fundamentals/////////////////////////////////

        void CSmartearnApp::DisplayCompanySummary(string productId)
        {
            
            session().set(PRODPAGE, "summary");
            
            if (cache().fetch_page("DisplayCompanySummary-" + productId))
                return;
            
            // capture the company
            UID uid = (UID)atoi(productId.c_str());
            CompanySummary summary(context());
            summary.Init(uid);
            Master m(context(), summary);
            m.Init();
            m.relatedLinks.insert(
                make_pair("Company Screener", "/fundamentals")
            );
            SP<IGICInfo> pgic;
            if (ISOK(summary.get_CompanyInfo()->get_GICInfo(&pgic)))
            {
                ostringstream ostr;
                ostr << "/fundamentals/list/si/";
                ostr << pgic->get_Id();
                m.relatedLinks.insert(make_pair("Same-sector Companies", ostr.str()));
            }
            BuildHeaderMenus(m.header, "Fundamentals");
            m.Title = (string)summary.get_SecurityInfo()->get_Symbol() + " - Summary";
            render("MasterView", m);
            
            cache().store_page("DisplayCompanySummary" + productId, 3600);
        }
        
        void CSmartearnApp::DisplayFinancials(string productId)
        {
            
            session().set(PRODPAGE, "financials");
            
            //
            string cachedpagename;
            string statementSelection;
            string tabSelection;
            if (request().request_method() == "POST")
            {
                statementSelection = request().post("fdm_fin_statements_select");
                tabSelection = request().post("fdm_allyears_tabselection");
                // store in session to reuse
                session().set("fdmfin_statementSelection", statementSelection);
                session().set("fdmfin_tabSelection", tabSelection);
                cachedpagename = "DisplayFinancials-" + productId + "-" + statementSelection + "-"+ tabSelection;
            }
            else
            {
                if (session().is_set("fdmfin_statementSelection"))
                    statementSelection = session().get("fdmfin_statementSelection");
                if (session().is_set("fdmfin_tabSelection"))
                    tabSelection = session().get("fdmfin_tabSelection");
            }

            
            
            UID uid = (UID)atoi(productId.c_str());
            CompanyFinancials fin(context());
            fin.Init(uid);
            if ( !statementSelection.empty())
            {
                UID sid = (UID)strtol(statementSelection.c_str(), nullptr, 10);
                if (fin._pstmts && fin._pstmts->HasItem(sid))
                {
                    fin.SetCurrentStatement(sid);
                    cachedpagename = "DisplayFinancials-" + productId + "-" + statementSelection + "-" + tabSelection;
                    //if ( cache().fetch_page( cachedpagename ) )
                     //   return;            
                }
            }

            if (fin.get_CurrentStatement())
            {
                ostringstream ostr;
                ostr << fin.get_CurrentStatement()->get_Id();
                session().set("fdmfin_statementSelection", ostr.str());
            }            
            
            Master m(context(), fin);
            m.Init();
            m.relatedLinks.insert(
                make_pair("Company Screener", "/fundamentals")
            );

            SP<IGICInfo> pgic;
            if (ISOK(fin.get_CompanyInfo()->get_GICInfo(&pgic)))
            {
                ostringstream ostr;
                ostr << "/fundamentals/list/si/";
                ostr << pgic->get_Id();
                m.relatedLinks.insert(make_pair("Same-sector Companies", ostr.str()));
            }
            
            BuildHeaderMenus(m.header, "Fundamentals");
            try
            {
                fin.Init(uid);
                fin.TabSelection = tabSelection==""?"S":tabSelection;
                if (request().request_method() == "POST")
                {
                    if (tabSelection == "S")
                    {
                       // LOGI(_logger, "Selected Statement : " << statementSelection);
                        if (!statementSelection.empty())
                            fin.SetCurrentStatement(strtol(statementSelection.c_str(), nullptr, 10));
                    }
                    else    // account lookup
                    {
                        
                    }
                }
                m.Title = (string)fin.get_SecurityInfo()->get_Symbol() + " - Financials";
                render("MasterView", m);
                
                //cache().store_page( cachedpagename , 3600);
                
            }
            catch(const std::exception& ex)
            {
                m.set_Error(ex.what());
                render("ErrorView", m);
            }
        }
        
        void CSmartearnApp::DisplayPerformance(string productId)
        {
            
            session().set(PRODPAGE, "performance");
            
            if (cache().fetch_page("DisplayPerformance-" + productId))
                return;            
            
            UID uid = (UID)atoi(productId.c_str());
            CompanyPerformance perf(context());
            perf.Init(uid);
            Master m(context(), perf);
            m.Init();
            ostringstream ostr;
            ostr << "/fundamentals/list/pf/";
            ostr << uid;
            m.relatedLinks.insert(make_pair("Companies with similar performance", ostr.str()));
            m.relatedLinks.insert(make_pair("Company Screener", "/fundamentals"));
            SP<IGICInfo> pgic;
            if (ISOK(perf.get_CompanyInfo()->get_GICInfo(&pgic)))
            {
                ostr.str("");
                ostr << "/fundamentals/list/si/";
                ostr << pgic->get_Id();
                m.relatedLinks.insert(make_pair("Same-sector Companies", ostr.str()));
            }
            BuildHeaderMenus(m.header, "Fundamentals");
            m.Title = (string)perf.get_SecurityInfo()->get_Symbol() + " - Performance";
            render("MasterView", m);
            
            cache().store_page("DisplayPerformance" + productId, 3600);
            
        }
        
        void CSmartearnApp::DisplayProspects(string productId)
        {
            
            session().set(PRODPAGE, "prospects");
            
            if (cache().fetch_page("DisplayProspects-" + productId))
                return;

            UID uid = (UID)atoi(productId.c_str());
            CompanyProspects prospects(context());
            prospects.Init(uid);

            Master m(context(), prospects);
            m.Init();
            m.relatedLinks.insert(
                make_pair("Company Screener", "/fundamentals")
            );
            SP<IGICInfo> pgic;
            if (ISOK(prospects.get_CompanyInfo()->get_GICInfo(&pgic)))
            {
                ostringstream ostr;
                ostr << "/fundamentals/list/si/";
                ostr << pgic->get_Id();
                m.relatedLinks.insert(make_pair("Same-sector Companies", ostr.str()));
            }
            BuildHeaderMenus(m.header, "Fundamentals");
            m.Title = (string)prospects.get_SecurityInfo()->get_Symbol() + " - Prospects";
            
            render("MasterView", m);
            cache().store_page("DisplayProspects" + productId, 3600);
            
        }

        void CSmartearnApp::DisplayProductNews(string productId, int newsIndex) 
        {
            
            // if newsIndex is provided, then news content is requested
            if ( newsIndex == 0)
            {
                int newsId = strtol(request().post("newsId").c_str(), nullptr, 10);
                SP<ISecurityNewsList> pNews;
                SP<ISecurityAPI> papi;
                SP<ISecurityNews> p;
                THROWIFFAILED(SecOpen(&papi), "Failed to get the security api interface.");
                THROWIFFAILED(papi->get_SecurityNews(strtol(productId.c_str(), nullptr, 10), &pNews), "Failed to get the security news list.");
                if (SUCCEEDED(pNews->get_Item(newsId, &p)))
                {
                    string nc = p->get_NewsContent();
                    boost::replace_all(nc, "<a href=", "<a target='_blank' href=");
                    response().out() << nc;
                }
                return;
            }
            
            session().set(PRODPAGE, "news");
            
            if (cache().fetch_page("DisplayProductNews-" + productId))
                return;

            UID uid = (UID)atoi(productId.c_str());
            CompanyNews news(context());
            news.Init(uid);

            Master m(context(), news);
            m.Init();
            m.relatedLinks.insert(
                make_pair("Company Screener", "/fundamentals")
            );
            SP<IGICInfo> pgic;
            if (ISOK(news.get_CompanyInfo()->get_GICInfo(&pgic)))
            {
                ostringstream ostr;
                ostr << "/fundamentals/list/si/";
                ostr << pgic->get_Id();
                m.relatedLinks.insert(make_pair("Same-sector Companies", ostr.str()));
            }
            BuildHeaderMenus(m.header, "Fundamentals");
            m.Title = (string)news.get_SecurityInfo()->get_Symbol() + " - News";
            
            render("MasterView", m);
            cache().store_page("DisplayProductNews" + productId, 3600);
        }


        void CSmartearnApp::DisplayFundamentals(string pagename, string productId) 
        {
            string prodNum;
            try
            {
                boost::regex regcheck("[a-zA-Z]{1,5}", boost::regex_constants::icase);
                if (boost::regex_search(productId, regcheck))
                {
                    // find the product id of the symbol
                    SP<ISecurityDescriptionList> psecdescs;
                    THROWIFFAILED(_pSecApi->get_SecurityDescriptions(productId.c_str(), &psecdescs), "Failed to get any security matching given symbol.");
                    if (!psecdescs->get_Count())
                        throw seexception(E_NotFound, "Failed to get any security matching given symbol.");

                    // first item
                    ostringstream ostr;
                    SP<ISecurityDescription> pSecDesc;
                    psecdescs->get_Item(0, &pSecDesc);
                    ostr << pSecDesc->get_Id();
                    prodNum = ostr.str();
                }
                else
                    prodNum = productId;
            }
            catch(const std::exception& ex)            
            {
               // LOGE(_logger, "Regex exception : " << ex.what());
                CompanySummary summ(context());
                Master m(context(), summ);
                m.Init();
                m.set_Error("URL format is incorrect or unknown security symbol. As a security symbol, only alphabets and numbers are allowed and must not exceed 5 characters (alpha or number).");
                render("ErrorView", m);
                return;
            }

            SP<IWebSession> pws;
            _pAdm->get_WebSession(session().get_session_cookie().c_str(), &pws);
            if (pws)
                pws->set_NewProductFound(strtol(prodNum.c_str(), nullptr, 10));
            
            if (pagename == "summary")
            {
                DisplayCompanySummary(prodNum);
                session().set("fdm_sel", "0");
            }
            else if (pagename == "financials")
            {
                DisplayFinancials(prodNum);
                session().set("fdm_sel", "1");
            }
            else if (pagename == "performance")
            {
                DisplayPerformance(prodNum);
                session().set("fdm_sel", "2");
            }
            else if (pagename == "prospects")
            {
                DisplayProspects(prodNum);
                session().set("fdm_sel", "3");
            }
            else if (pagename == "news")
            {
                DisplayProductNews(prodNum);
                session().set("fdm_sel", "4");
            }
            else
            {
                DisplayCompanySummary(prodNum);
                session().set("fdm_sel", "0");
            }
            
            
        }

        
        void CSmartearnApp::DisplayFundamentals(string productId)
        {
            
            // check if productid is a symbol
            try
            {
                boost::regex regcheck("[a-zA-Z]{1,5}", boost::regex_constants::icase);
                if (boost::regex_search(productId, regcheck))
                {
                    // find the product id of the symbol
                    SP<ISecurityDescriptionList> psecdescs;
                    THROWIFFAILED(_pSecApi->get_SecurityDescriptions(productId.c_str(), &psecdescs), "Failed to get any security matching given symbol.");
                    if (!psecdescs->get_Count())
                        throw seexception(E_NotFound, "Failed to get any security matching given symbol.");

                    // first item
                    ostringstream ostr;
                    SP<ISecurityDescription> pSecDesc;
                    psecdescs->get_Item(0, &pSecDesc);
                    ostr << pSecDesc->get_Id();
                    
                    productId = ostr.str();
                }
            }
            catch(const std::exception& ex)            
            {
               // LOGE(_logger, "Regex exception : " << ex.what());

                CompanySummary summ(context());
                Master m(context(), summ);
                m.Init();
                m.set_Error("URL format is incorrect or unknown security symbol. As a security symbol, only alphabets and numbers are allowed and must not exceed 5 characters (alpha or number).");
                render("ErrorView", m);
                return;
            }
            
            
            string fdm_sel;
            http::request& req = context().request();
            if (req.request_method() == "POST")
            {
                fdm_sel = req.post("fdm_sel");
                if (fdm_sel.empty())
                {
                    // check if session already has 
                    fdm_sel = session().get("fdm_sel", "0");
                }
                else
                {
                    // store in session
                    session().set("fdm_sel", fdm_sel);
                }
            }
            else
            {
                fdm_sel = session().get("fdm_sel", "0");
            }
            
            SP<IWebSession> pws;
            _pAdm->get_WebSession(session().get_session_cookie().c_str(), &pws);
            if (pws)
                pws->set_NewProductFound(strtol(productId.c_str(), nullptr, 10));
            
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
            else if (fdm_sel == "4")
            {
                DisplayProductNews(productId);
            }
            else
            {
                DisplayCompanySummary(productId);
            }
            
        }
        
        struct RowComparer
        {
            bool operator()(const Row& x, const Row& y) const
            { 
                return strcmp(x["cik"], y["cik"]) < 0;
            }
        };
        
        typedef std::set<Row, RowComparer> Records;
        
        sql_create_2(se_fin_perf_ratios, 1, 2, sql_bigint, perf_id, sql_varchar, perf_name);
        
        typedef std::set<se_fin_perf_ratios> PerfRatioNames;
        typedef std::unordered_map<U64, int> PerfColumnPositions;

        void CSmartearnApp::GenerateScreenerResults(const Json::Value& json) 
        {
            /*  A sample message received.
            {
                "exchanges" : [ 3, 2 ],
                "industries" : [ 5, 49 ],
                "pratios" : 
                [
                    {
                        "id" : "12",
                        "range" : 
                        {
                            "end" : 34.15178571428571,
                            "start" : 12.94642857142857
                        }
                    }
                ]
            }
             */
            try
            {
               // LOGI(_logger, json);
                
                bool bIsWhereIncluded = false;
                const Value& ratios = json["pratios"];
                bool hasRatios = ratios.size();
                ostringstream sperfIds;
                const auto& exchanges = json["exchanges"];
                const auto& industries = json["industries"];
                
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                if (hasRatios)
                {
                    q << "SELECT a.product_id, a.cik, a.product_symbol, a.product_name, a.exchange_name, a.sector, a.industry, b.perf_id, b.perf_name, b.val ";
                    q << "FROM sa.list_ListedCompaniesExtended a join ";
                    q << "(select y.cik, x.perf_id, x.perf_name, y.val from se_fin_perf_ratios x join  ";
                    q << "(select * from  ";
                    q << "( ";
                    for(int i=0; i != ratios.size(); ++i)
                    {
                        UID ratioId = ratios[i]["id"].asInt64();
                        if (i+1 == ratios.size())
                            sperfIds << ratioId;
                        else
                            sperfIds << ratioId << ", ";
                                    
                        R8 start = ratios[i]["range"]["start"].asDouble();
                        R8 end = ratios[i]["range"]["end"].asDouble();
                        if (i > 0)
                        {
                            q << " union ";
                        }
                        q << "SELECT * FROM sa.se_fin_perf_latest where (perf_id = " << ratioId << " and val between " << start << " and " << end << " ) ";
                    }
                    q << ") z ";
                    q << ") y on x.perf_id = y.perf_id) b ";
                    q << "on a.cik= b.cik ";
                }
                else
                {
                    q << "SELECT * FROM sa.list_ListedCompaniesExtended a ";
                }
                if (exchanges.size())
                {
                    bIsWhereIncluded = true;
                    q << " where a.product_exchange_id in (";
                    for (int i=0; i != exchanges.size(); ++i)
                    {
                        if (i != exchanges.size() - 1)
                            q << exchanges[i].asInt() << ", ";
                        else
                            q << exchanges[i].asInt() << ") ";
                    }
                }
                if (industries.size() )
                {
                    if (bIsWhereIncluded)
                        q << " and a.product_gic_id in (";
                    else
                        q << " where a.product_gic_id in (";
                    for (int i=0; i != industries.size(); ++i)
                    {
                        if (i != industries.size() - 1)
                            q << industries[i].asInt() << ", ";
                        else
                            q << industries[i].asInt() << ") ";
                    }
                }
                if (hasRatios)
                {
                    q << "order by a.product_symbol, b.perf_id";
                }
                else
                {
                    q << "order by a.product_symbol";
                }

               // LOGIP(q.str());
                const StoreQueryResult& records = q.store();
                
                // run a query to get perf ratio names
                PerfRatioNames pnames;
                if (hasRatios)
                {
                    q = conn->query();
                    q << "SELECT perf_id, perf_name FROM se_fin_perf_ratios where perf_id in (" << sperfIds.str() << ")";
                    q.storein(pnames);
                }
                
                Value root;
                Value& cols = root["cols"];
                    
                auto createCol = [this](Value& cols, int colIndex, string colName, string colLabel, string type)
                {
                    Value& col = cols[colIndex];
                    col["id"] = colName;
                    col["label"] = colLabel;
                    col["type"] = type;
                };
                    
                createCol(cols, 0, "Ticker", "Ticker", "string");
                createCol(cols, 1, "Company", "Company", "string");
                createCol(cols, 2, "Exchange", "Exchange", "string");
                createCol(cols, 3, "Sector", "Sector", "string");
                createCol(cols, 4, "Industry", "Industry", "string");
                PerfColumnPositions perfPos;
                for (auto& pf : pnames)
                {
                    int c = cols.size();
                    createCol(cols, c, pf.perf_name, pf.perf_name, "number");
                    perfPos.insert(make_pair(pf.perf_id, c));
                }
                    
                Value& rows = root["rows"];
                int len = -1;
                
                std::string slastsym = "";
                Value* prow;
                int cellincr;
                for (const Row& r  : records)
                {
                    const string& sym = (std::string)r["product_symbol"];
                    if (sym != slastsym)
                    {
                        Value row;
                        row["c"][0]["v"] = (CSTR)r["product_symbol"];
                        row["c"][0]["f"] = "<a href='/fundamentals/p/summary/" + (string)r["product_id"] + "' target='_blank'>" + (string)r["product_symbol"] + "</a>";
                        row["c"][1]["v"] = "<a href='/fundamentals/p/summary/" + (string)r["product_id"] + "' target='_blank'>" + (string)r["product_name"] + "</a>";
                        row["c"][2]["v"] = (CSTR)r["exchange_name"];
                        row["c"][3]["v"] = (CSTR)r["sector"];
                        row["c"][4]["v"] = (CSTR)r["industry"];
                        rows[++len] = row;
                        if (!hasRatios)
                        {
                            continue;
                        }
                        cellincr = 4;
                        prow = &rows[len];
                    }
                    sql_decimal_null val = r["val"];
                    (*prow)["c"][perfPos[(UID)r["perf_id"]]]["v"] = val.is_null ? Json::nullValue : val.data;
                    slastsym = sym;
                }                    

                FastWriter jsonwriter;
                //LOGIP(jsonwriter.write(root));
                response().set_content_header("application/json");
                response().out() << jsonwriter.write(root);
                
            }
            catch(...)
            {
                throw;
            }
        }

        
        void CSmartearnApp::DisplayCompanyPicks()
        {
            DisplayCompanyScreener();
        }

        void CSmartearnApp::DisplayCompanyScreener() 
        {
            
            http::request& req = context().request();
            if (req.request_method() == "POST")
            {
                if (req.post("type") == "screener")
                {
                    Value message;
                    Json::Reader reader;
                    const string& str = req.post("msg");
                    bool parseResult = reader.parse(req.post("msg"), message);
                    if (!parseResult)
                        throw seexception(E_ReadError, reader.getFormatedErrorMessages().c_str());
                    GenerateScreenerResults(message);
                    return;
                }
            }
            else
            {
                Master m(context());
                m.Title = "Fundamentals - Company Screener";
                BuildHeaderMenus(m.header, "Fundamentals");

                SP<IWebSession> pWebSession;
                get_Session(&pWebSession);
                CompanyScreener screener(context());
                screener.set_Session(pWebSession);
                screener.ScreeningMode = CompanyScreener::Default;
                m.Init(screener);
                render("MasterView", m);
            }            
        }

        void CSmartearnApp::DisplayListSimilarPerformances(string productid) 
        {
            
            if (cache().fetch_page("DisplayListSimilarPerformances" + productid))
                return;
            
            Master m(context());
            BuildHeaderMenus(m.header, "Fundamentals");
            SP<IWebSession> pWebSession;
            get_Session(&pWebSession);
            CompanyScreener lsi(context());
            lsi.set_Session(pWebSession);
            lsi.ScreeningMode = CompanyScreener::SimilarPerformances;
            m.Init(lsi);
            
            try
            {
                if (productid.empty())
                    throw seexception(E_InvalidArg, "Invalid performance identifier.");
                
                UID uid = (UID)atoi(productid.c_str());
                
                SP<IString> pcik;
                THROWIFFAILED(_pxbrl->get_CIK(uid, &pcik), "Failed to get CIK");
                
                THROWIFFAILED(_pSecApi->get_SecurityDescription(uid, &lsi.pSecurityDescription), "Failed to get security description.");
                
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                try
                {
                    q << "call get_PerfRatioValuesCurrentYear(" << pcik->get_Buffer() << ")";
                    StoreQueryResult sqr = q.store();
                    for (int i=0; i != sqr.size(); ++i)
                    {
                        const Row& r = sqr[i];
                        Value& j = lsi.Performances[i];
                        j["id"] = (I32)r["perf_id"];
                        const R8 val = r["val"];
                        j["range"]["start"] = std::floor(val);
                        j["range"]["end"] = std::ceil(val);
                    }
                    
                    while (q.more_results())
                        q.store_next();
                }
                catch(...)
                {
                    while (q.more_results())
                        q.store_next();
                    throw;
                }
                
                m.Title = "Fundamentals - Companies with similar performances";

                render("MasterView", m);
                
                cache().store_page("DisplayListSimilarPerformances" + productid, 86400);
                
            }
            catch(const std::exception& ex)
            {
               // LOGEP(ex.what());
                m.set_Error("Invalid parameters. Failed to render the page.");
                render("ErrorView", m);
            }
        }
        

        void CSmartearnApp::DisplayListSameIndustries(string gicid) 
        {
            
            if (cache().fetch_page("DisplayListSameIndustries" + gicid))
                return;
            
            Master m(context());
            BuildHeaderMenus(m.header, "Fundamentals");
            
            SP<IWebSession> pWebSession;
            get_Session(&pWebSession);
            CompanyScreener lsi(context());
            lsi.set_Session(pWebSession);
            m.Init(lsi);
            lsi.ScreeningMode = CompanyScreener::SameSector;
            
            try
            {
                if (gicid.empty())
                    throw seexception(E_InvalidArg, "Invalid industry identifier.");
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "select * from list_Gics where gic_id = " << gicid;
                StoreQueryResult sqr = q.store();
                if (sqr.size())
                {
                    lsi.SectorName = (CSTR)sqr[0]["sector"];
                    lsi.IndustryName = (CSTR)sqr[0]["industry"];
                    lsi.GicId = sqr[0]["gic_id"];
                }
                if (lsi.SectorName.empty())
                    throw seexception(E_InvalidArg, "Invalid industry identifier.");

                m.Title = "Fundamentals - Companies in " + lsi.SectorName + "/" + lsi.IndustryName;

                render("MasterView", m);
                
                cache().store_page("DisplayListSameIndustries" + gicid);
                
            }
            catch(const std::exception& ex)
            {
               // LOGEP(ex.what());
                m.set_Error("Invalid parameters. Failed to render the page.");
                render("ErrorView", m);
            }
        }

        
        
        void CSmartearnApp::OutputJsonCompanyInfo(string productId)
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
        
        void CSmartearnApp::OutputJsonMonthlyPriceHistory(string productId)
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
                pApi->get_SecurityHistoricalPrices(pSec, dtFrom, dtNow, EAM_None, HPAM_Monthly, &pList);
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
        
        sql_create_14(
            se_sec_product_keystats, 
                3, 
                4, 
                sql_bigint, product_id,
                sql_smallint, year,
                sql_smallint, quarter,
                sql_varchar, product_symbol,
                sql_varchar, product_name,
                sql_decimal_null, product_marketcap,
                sql_double_null, expected_return_value,
                sql_decimal_null, expected_return_rate,
                sql_decimal_null, return_gmean,
                sql_decimal_null, return_amean,
                sql_decimal_null, variance,
                sql_decimal_null, stddev,
                sql_decimal_null, cv,
                sql_timestamp_null, lastupdated);
        

        void CSmartearnApp::DisplayPortfolioBuilder() 
        {
            if (request().request_method() == "POST")
            {
                Value posteddata;
                Reader reader;
                if (!reader.parse(request().post("postdata"), posteddata, false))
                    return;
                
                if (posteddata["fn"] == "plot")
                {
                    const Value& params = posteddata["params"];
                    string seltype = params["seltype"].asString();
                    SIZE selSize = params["selsize"].asUInt64();
                    // check if session has instance.
                    SGSP<ISecuritySelectionStatisticsList> pList;
                    const string& exch = params["exch"].asString();
                    
                    if (seltype == "fresh")
                    {
                        if (session().is_set("PortfolioSecurityList"))
                            session().erase("PortfolioSecurityList");

                        SP<ISecurityAPI> papi;
                        THROWIFFAILED(SecOpen(&papi), "Failed to open security gateway");
                        THROWIFFAILED(papi->get_RandomListOfBestQuarterlySecurities(
                                &pList, 
                                selSize,
                                [&exch](ISecuritySelectionStatistics::QUERYFLDS qf, LGX& lgx, BOOL& yes)
                                {
                                    if (qf == ISecuritySelectionStatistics::ExchangeIds)
                                    {
                                        yes = true;
                                        return exch.c_str();
                                    }
                                }), "Failed to get security list.");
                        pList->AddRef();
                        session().store_data("PortfolioSecurityList", pList);
                    }
                    else if (seltype == "reset")
                    {
                        session().fetch_data("PortfolioSecurityList", pList);
                        pList->AddRef();
                        pList->Reset();
                    }
                    else if (seltype == "renew")
                    {
                        session().fetch_data("PortfolioSecurityList", pList);
                        pList->AddRef();
                        pList->RenewList();
                    }

                    bool isbubble = false;;
                    Value root;
                    Value& header = root[0];
                    if (isbubble)
                    {
                        header[0] = "Symbol";
                        header[1] = "Risk";
                        header[2] = "Return";
                        header[3] = "Asset";
                        header[4] = "Market Cap";
                    }
                    else
                    {
                        header[0] = "Risk";
                        header[1] = "Return";
                    }
                    int i=1;
                    for (ISecuritySelectionStatistics* p : *pList)
                    {
                        Value& row = root[i++];
                        if (isbubble)
                        {
                            SP<ISecurityDescription> pdesc;
                            if (ISOK(p->get_SecurityDescription(&pdesc)))
                            {
                                row[0] = pdesc->get_Symbol();
                                row[1] = p->get_ReturnRisk();
                                row[2] = p->get_ExpectedReturnRate();
                                row[3] = pdesc->get_Symbol();
                                row[4] = (I32)(1000000.0 * static_cast <double> (rand()) / static_cast <double> (RAND_MAX));   // market cap need work.
                            }
                        }
                        else
                        {
                            row[0] = p->get_ReturnRisk();
                            row[1] = p->get_ExpectedReturnRate();
                        }
                    }
                    FastWriter writer;
                    response().out() << writer.write(root);
                    return;
                }
                else if (posteddata["fn"] == "fill")
                {
                    const Value& params = posteddata["params"];
                    R8 x0, x1, y0, y1;
                    x0 = params["x0"].asDouble();
                    x1 = params["x1"].asDouble();
                    y0 = params["y0"].asDouble();
                    y1 = params["y1"].asDouble();
                    
                    // calculate slope
                    R8 slope = (y1 - y0) / (x1 - x0);
                    // now enumerate every value of x and find y.
                    // y = mx + c; here c is the y intersect of the line which is y0 already.
                    /*
                     * 1. Get all the records which has a risk value between x0 and x1
                     * 2. Enumerate these records. Take the risk value of these records and then
                     *      find y using above equation.
                     * 3. If y is greater or equal to record's "return rate" then select that record.
                     * 4. So on.... until all records are completed.
                     * 5. Return the json formatted output.
                     */
                    
                    // step 1 above.
                    typedef std::vector<se_sec_product_keystats> KeyStatList;
                    CSmartConnection conn(SEDBCONN);
                    Query qreturns = conn->query();
                    qreturns << "select * from list_CurrentProductKeyStats  a" ;
                    qreturns << " join list_ListedCompanies b using (product_id) " ;
                    qreturns << " where stddev between 0 and " << x1 ;
                    qreturns << " order by product_marketcap desc, cv desc;";
                    KeyStatList fullist;
                    qreturns.storein(fullist);
                    
                    // step 2, 3 and 4 above
                    KeyStatList filtered;
                    for (const se_sec_product_keystats& item : fullist)
                    {
                        if (item.expected_return_rate.is_null or item.stddev.is_null)
                            continue;
                        R8 y = slope * item.stddev.data + y0;  // c which is y intercept
                        if (item.expected_return_rate.data >= y)
                        {
                            filtered.push_back(item);
                        }
                    }
                    
                    // step 5.
                    // We need Google's table filling format with company symbol and names
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
                    createCol(cols, 2, "Expected Return (%)", "Expected Return (%)", "number");
                    createCol(cols, 3, "Return Risk (%)", "Return Risk (%)", "number");
                    createCol(cols, 4, "Mean Growth (%)", "Mean Growth (%)", "number");
                    createCol(cols, 5, "Expected Return ($)", "Expected Return ($)", "number");
                    createCol(cols, 6, "Expected Return/Return Risk", "Expected Return/Return Risk", "number");
                    createCol(cols, 7, "Marketcap ($)", "Marketcap ($)", "number");
                    createCol(cols, 8, "Exchange", "Exchange", "string");
                    int rowindex = 0;
                    Value& rows = root["rows"];
                    ostringstream ostr;
                    se::math::CCommon mfunc;
                    for (const se_sec_product_keystats& item : filtered)
                    {
                        ostr.str("");
                        Value& row = rows[rowindex++];
                        ostr << "<a href='/fundamentals/p/summary/" << item.product_id << "' target='_blank'>" << item.product_symbol << "</a>";
                        row["c"][0]["v"] = ostr.str();
                        ostr.str("");
                        ostr << "<a href='/fundamentals/p/summary/" << item.product_id << "' target='_blank'>" << item.product_name << "</a>";
                        row["c"][1]["v"] = ostr.str();
                        row["c"][2]["v"] = mfunc.RoundNear(item.expected_return_rate.data, 2);
                        row["c"][3]["v"] = mfunc.RoundNear(item.stddev.data, 2);
                        row["c"][4]["v"] = item.return_gmean.is_null ? Json::nullValue : mfunc.RoundNear(item.return_gmean.data, 2);
                        row["c"][5]["v"] = item.expected_return_value.is_null ? Json::nullValue : mfunc.RoundNear(item.expected_return_value.data, 0);
                        row["c"][6]["v"] = item.cv.is_null ? Json::nullValue : mfunc.RoundNear(item.cv.data * 100.0, 2);
                        row["c"][7]["v"] = item.product_marketcap.is_null ? Json::nullValue : mfunc.RoundNear(item.product_marketcap.data, 0);
                    }
                    FastWriter writer;
                    response().out() << writer.write(root);
                    return;
                }
            }            

            Master m(context());
            m.Title = "Fundamentals - Portfolios";
            BuildHeaderMenus(m.header, "Fundamentals");

            SP<IWebSession> pWebSession;
            get_Session(&pWebSession);

            PortfolioWiz wiz(context());
            wiz.set_Session(pWebSession);
            //se::math::CCommon mfunc;
            SP<se::economy::IEconomyApi> papi;
            THROWIFFAILED(EconOpen(&papi), "Failed to access economics api.");
            try
            {
                EIVAL latest;
                latest = papi->get_EconomicIndicatorLatestValue(140); // 140 is 10-year constant maturity rate.
                wiz.RiskFreeRate = get<1>(latest);
            }
            catch (const std::exception& ex)
            {
               // LOGE(_logger, "Error : " << ex.what());
                // failed. set to 2 as default.
                wiz.RiskFreeRate = 2.0;
            }
            m.Init(wiz);
            render("MasterView", m);
                

        }
        

        void CSmartearnApp::BuildPortfolio() 
        {
            if (request().request_method() != "POST")
                throw seexception("Invalid http method. Only http post allowed.");
            
            Reader jsreader;
            Json::Value posted;
            if (jsreader.parse(request().post("pfreq"), posted, false))
            {
                I32 pfid = posted["pfid"].asInt();
                R8 returnmin = posted["return"]["min"].asDouble();
                R8 returnmax = posted["return"]["max"].asDouble();
                R8 riskmin = posted["risk"]["min"].asDouble();
                R8 riskmax = posted["risk"]["max"].asDouble();
                
                Value root;
                Value& header = root[0];
                header[0] = "Symbol";
                header[1] = "Risk";
                header[2] = "Return";
                header[3] = "Asset";
                header[4] = "Market Cap";
                
                // following is a sampler
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                int size = 25;
                q << "SELECT product_id, product_symbol, product_name, product_marketcap FROM sa.list_ListedCompaniesExtended limit " << size;
                auto sqr = q.store();
                for (int i=0; i != size; ++i)
                {
                    const Row& r = sqr[i];
                    Value& row = root[i+1];
                    row[0] = (CSTR)r["product_symbol"];
                    row[1] = (I32)(100.0 * static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
                    row[2] = (I32)(100.0 * static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
                    row[3] = (string)r["product_symbol"] + "-" + (string)r["product_name"];
                    row[4] = (R8)r["product_marketcap"];
                }
                
                FastWriter writer;
                response().out() << writer.write(root);
                
            }
            else
                throw seexception("Posted parameter is in incorrect format.");
            
        }

        
        
        //////////////////////////////////////////// GetJson//////////////////////////////////

        void CSmartearnApp::GetJson(string p1, string p2, string p3) 
        {
            try
            {
                response().set_content_header("application/json");
                int ctxt = strtol(p1.c_str(), nullptr, 10);
                switch (ctxt)
                {
                case 11000:
                    DisplayPortfolioBuilder();
                    break;
                case 11001:
                    BuildPortfolio();
                    break;
                case 11002: // display news
                    response().set_content_header("text/html");
                    DisplayProductNews(p3, 0);
                    break;
                default:
                    CBaseApplication::GetJson(p1, p2, p3);
                    break;
                }
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, ex.what());
                throw ex;
            }
        }
    }
}
