/* 
 * File:   modelmaster.h
 * Author: santony
 *
 * Created on August 17, 2013, 11:26 AM
 */

#ifndef MODELMASTER_H
#define	MODELMASTER_H

#include <cppcms/view.h>
#include <cppcms/application.h>
#include <string>
#include <cppcms/form.h>
#include "commontypes.h"
#include <vector>
#include "../sesecurities/sesecuritiesapi.h"
#include "../seadmin/seadminapi.h"
#include "../sexbrl/sexbrlapi.h"
#include "CSession.h"
#include "CBaseContent.h"
#include "../seeconomy/seeconomapi.h"
//#include <log4cxx/logger.h>
#include <unordered_map>
#include <jsoncpp/json/json.h>

using namespace cppcms;
using namespace std;
using namespace se;
using namespace se::securities;
using namespace se::admin;
using namespace se::xbrl;
using namespace se::economy;
//using namespace log4cxx;
using namespace Json;

namespace sg
{
    namespace web
    {
        namespace models
        {
            
            
            class Footer : public BaseContent
            {
            public:
                Footer();
                Footer(cppcms::http::context& httpctxt);

            };
            
            // menu header, href, isselected 
            struct MenuItem
            {
                string Header;
                string Href;
                bool Selected;
            };
            
            typedef std::unordered_map<string, string> RelatedLinks;

            class RecentFinds : public BaseContent
            {
            public:
                RecentFinds();
                RecentFinds(cppcms::http::context& httpctxt);

                void operator+=(int productid);
                void Clear();   // clear RECENTFINDS session data
                ErrorId get_RecentFinds(ISPList<ISecurityDescription>** ppOut) ;
                string get_RecentFinds();
                bool get_HasRecentFinds();
            };
            
            
            class CompSearch : public BaseContent
            {
            public:
                std::string name;
                
            public:
                CompSearch();
                CompSearch(cppcms::http::context& httpctxt);
                virtual void Init();
                virtual void Init(cppcms::http::context& httpctxt);
                
                ISecurityDescriptionList* getRecentPicks(IAdminAPI* papi = nullptr);
                
            };
            
            class Header : public BaseContent
            {
            public:
                std::list<MenuItem> appMenus;
                std::list<MenuItem> userMenus;
                CompSearch compSearch;  // search may be used from any page, so moving to master model.

            public:
                Header();
                Header(cppcms::http::context& httpctxt);
                virtual void Init();
                virtual void Init(cppcms::http::context& httpctxt);
                
            };
            

            class Master : public BaseContent
            {
            private:
                BaseContent* _ppageModel;
             
            public:
                string Title;
                string CName;
                Footer footer;
                Header header;
                PID pid;
                RecentFinds recentFinds;
                RelatedLinks relatedLinks;

            public:
                Master();
                Master(cppcms::http::context& ctxt) ;
                Master(cppcms::http::context& ctxt, BaseContent& pageModel);
                
                virtual void Init();
                virtual void Init(BaseContent& pageModel);
                
                void Init(cppcms::http::context& httpctxt, BaseContent& pageModel);
                void Init(cppcms::http::context& httpctxt, BaseContent& pageModel, const string& viewName);
                
                BaseContent& pageModel();
                
            };
            
            class Sitemap : public BaseContent
            {
            private:
                SP<IStoreQueryResult> _pProducts;
            public:
                Sitemap(cppcms::http::context& httpctxt);

                virtual void Init();
                
            public:
                IStoreQueryResult* get_Products();

            };
            
            class Register : public BaseContent
            {
            public:
                Register(cppcms::http::context& httpctxt);
                virtual void Init();
                virtual void Init(IUser* pUser);

                IUser*  get_User();
                IAddress* get_UserAddress();
                
                void ClearRegSessionData();
                
                const string get_Agreement();
                const string get_Login();
                const string get_Password();
                const string get_PayPalToken();
                const string get_PayPalPayerId();
                const string get_PayPalPayerProfileId();
                
                const string get_PaypalProfileStatus(); // after creating trial profile, returns either PendingProfile or ActiveProfile
                
                IPaypalExpressCheckout* get_PaypalInstance();
                bool get_HasAgreed();

                void set_Login(const string&);
                void set_Password(const string&);
                void set_PayPalToken(const string&);
                void set_PayPalPayerId(const string& );
                void set_PayPalPayerProfileId(const string& );
                void set_PaypalInstance(IPaypalExpressCheckout*);
                void set_HasAgreed(bool);
                void set_PaypalProfileStatus(const string&);
                
            private:
                SP<IAddress> pAddr;
            };
            
            class SignIn : public BaseContent
            {
            public:
                SignIn(cppcms::http::context& httpctxt);
                virtual void Init();
                string Login;
                string DisplayError;    // set to "none" or "inline"
                string ErrorMessage;
            };

            class SignOut : public BaseContent
            {
            public:
                SignOut(cppcms::http::context& httpctxt);
                virtual void Init();
            };            

            class MyAccount : public BaseContent
            {
            public:
                MyAccount(cppcms::http::context& httpctxt);
                virtual void Init();
            };            
            
            class Home : public BaseContent
            {
            public:
                UID ProductId;
            public:
                Home(cppcms::http::context& httpctxt);
                virtual void Init(UID);
            };
            

            class Search : public BaseContent
            {
            private:
                SP<IROSPList<ISecurityDescription>> _pList;
                
            public:
                
                Search(cppcms::http::context& httpctxt);
                virtual void Init();
                void SetList(IROSPList<ISecurityDescription>* pList);
                
                IROSPList<ISecurityDescription>* get_List();
            };
            
            class Subscriptions : public BaseContent
            {
            public:
                Subscriptions(cppcms::http::context& httpctxt);
                virtual void Init();
                
            };
            
            class Fundamentals : public BaseContent
            {
            public:
                string SubViewName;
                UID ProductId;

            protected:
                //LoggerPtr _logger;

            private:
                SP<ISecurity> _pSec;
                SP<IPublicCompanyInfo> _pCo;
                SP<ISecurityDescription> _pSecDesc;
                SP<IGICInfo> _pgic;
                
            public:
                Fundamentals(
                        cppcms::http::context& httpctxt, 
                        string const& subViewName);
                
                ISecurity* get_Security();
                ISecurityDescription * get_SecurityInfo();
                IPublicCompanyInfo * get_CompanyInfo();
                IGICInfo* get_GicInfo();
                
                virtual void Init(UID productId);
                
            };

            class CompanySummary : public Fundamentals
            {
                
            public:
                CompanySummary(cppcms::http::context& httpctxt);
                void Init(UID productId);
            };
            
            class CompanyFinancials : public Fundamentals
            {
            public:
                SP<IXbrlStatementList> _pstmts;
                SP<IXbrlStatement> _pcurstmt;
                bool HasStatements;
                string TabSelection;
                
            public:
                CompanyFinancials(cppcms::http::context& httpctxt);
                
                //IXbrlRecordList* get_IncomeRecords();
                
                void Init(UID prodId);
                
                IXbrlStatementList& get_Statements() const;
                IXbrlStatement* get_CurrentStatement() const;
                
                void SetCurrentStatement(UID statementId);
                
                void WriteNode(std::ostream& os, int level, IXbrlAbstractRecord* node, bool isLastItem, I16 sy, I16 ey) const;
                
                
                friend std::ostream& operator<<(std::ostream& os, const CompanyFinancials& cfin);
                
            };
            
            class CompanyPerformance : public Fundamentals
            {
            public:
                I16 FinYearEnd;
            public:
                CompanyPerformance(cppcms::http::context& httpctxt);
                IFinancialRatioList* get_Ratios();
                void Init(UID prodId);

            private:
                SP<IFinancialRatioList> _pratiolist;
            };
            
            class CompanyProspects;
            
            class CProspectChart : public BaseContent
            {
            public:
                CProspectChart(
                        cppcms::http::context& httpctxt, 
                        const string& viewName,
                        CompanyProspects& prospects
                        );
                
            public:
                string title;
                SIZE chartid;
                UID ProductId;
                UID RefId;
                CompanyProspects& Prospects;
                
            };
            
            class CProspectChartSector : public CProspectChart
            {
            public:
                CProspectChartSector(cppcms::http::context& httpctxt, CompanyProspects& prospects);
                virtual void Init();
                
               
            };

            class CProspectChartFuture : public CProspectChart
            {
            public:
                CProspectChartFuture(cppcms::http::context& httpctxt, CompanyProspects& prospects);
                virtual void Init();
            public:
                R8 RevAvg;
                R8 Risk;
                R8 PredRevGrowth;
            };
            
            typedef vector<CProspectChart*> ProspectCharts;
            
            
            class CompanyProspects : public Fundamentals
            {
            public:
                CompanyProspects(cppcms::http::context& httpctxt);
                ~CompanyProspects();
                void Init(UID prodId);
                
                void AddChart(CProspectChart*);
                
            public:
                ProspectCharts charts;
                const string CompanyProspectsChartView;
            };
            
            
            
            class FinancialStatement : public BaseContent
            {
            private:
                SP<IROSPList<IXbrlDocType> > _pList;
                SP<IROList<I16> > _period;
                SP<IROSPList<IXbrlStatement> > _statement;
                SP<IXbrlDataNode> _pStatementNode;
                void RenderChildren(ostream& out, IROSPList<IXbrlDataNode>*  child, int tabcount = 0);
                stringstream _str,_hPrice;
                
            public:
                BaseContent& _content;
                string subTitle;
                I32 dtype;
                I16 year,qtr;
                I32 stId;
                string hdFundSel ;
                
                FinancialStatement(cppcms::http::context& httpctxt, BaseContent& content);
                virtual void Init();
                void Init(UID productId);
                void SetList(IROSPList<IXbrlDocType>* pList);
                IROSPList<IXbrlDocType>* get_List();
                void SetPeriod(IROList<I16>*  period);
                IROList<I16>* get_Period();
                void SetStatementTitles(IROSPList<IXbrlStatement>*  statement);       
                IROSPList<IXbrlStatement>* get_StatementTitles();
                void SetStatement(IXbrlDataNode*  pStatement); 
                void RenderStatement(ostream& out);
                ErrorId get_HistoricalPrice(ISecurity* pSec, I16 year, I16 qtr, I32 dtype, ISecurityPrice** ppOut);
                void getHistoricalPrice(ostream& out);
                void HistoricalPrice(ISecurityPrice* prev,ISecurityPrice* cuurent,I32 dtype);
                
                
            };            
            
            class CompanyNews : public Fundamentals
            {
            public:
                bool HasNews;
            private:
                SP<ISecurityNewsList> _pNews;
                
            public:
                CompanyNews(cppcms::http::context& httpctxt);
                void Init(UID productId);
                
                ISecurityNewsList* get_News();
            };            
            
            class CompanyPicks : public BaseContent
            {
            protected:
                UID _selectedExchangeId;
                SP<ISecurityExchangeList> _pexchanges;
                SP<IXbrl> _pxbrl;
                SP<ISecurityAPI> _psecapi;
                //typedef map<UID, tuple<R8, R8> > SelectedPerfRanges;
                //SelectedPerfRanges _selectedPerfRanges;

            public:
                string SubViewName;
                //LoggerPtr _logger;
                
            protected:
                void InternalInit();
                
            public:
                CompanyPicks();
                CompanyPicks(cppcms::http::context& httpctxt, string const& subViewName);
                virtual void Init();

                virtual void Init(cppcms::http::context& httpctxt);

                virtual void Init(cppcms::http::context& httpctxt, const string& viewName);
                
                ISecurityExchangeList* get_Exchanges();
                
                void set_SelectedExchange(UID id);
                
                UID get_SelectedExchange();

            };
            
            class CompanyScreener : public CompanyPicks
            {
            public:
                CompanyScreener();
                CompanyScreener(cppcms::http::context& httpctxt);
                virtual void Init();
                virtual void Init(cppcms::http::context& httpctxt);
                
            public:
                
                enum Mode
                {
                    Default,
                    SameSector,
                    SimilarPerformances
                };
                
                UID GicId;
                SP<ISecurityDescription> pSecurityDescription;
                string SectorName;
                string IndustryName;
                Value Performances;
                Mode ScreeningMode;
            };
            
            class EconomicNews : public BaseContent
            {
            private:
                SP<IEconomicNewsFeeds> _pnews;
                //LoggerPtr _logger;
                
            public:
                bool HasNews;
                
            public:
                EconomicNews(cppcms::http::context& httpctxt);
                virtual ~EconomicNews();
                
                IEconomicNewsFeeds* get_News();
                
                
            };

            class Economy : public BaseContent
            {
            public:
                Economy(cppcms::http::context& httpctxt);
                BaseContent& get_Content() ;
                
                virtual void Init();
                
                IIndicatorCategoryList* getCategories();
                UID getSelectedCategoryId();
                void setSelectedCategoryId(UID);

                IEconomyIndicatorList* getIndicators();

                IEconomicNewsFeeds* getNewsFeeds();
                
            public:
                string CategoryName;
                EconomicNews News;
                
            private:
                SP<IEconomyApi> _papi;
                UID _selectedCategoryId;
                SP<IIndicatorCategoryList> _pCategories;
                SP<IEconomyIndicatorList> _pIndicators;
                SP<IEconomicNewsFeeds> _pnewFeeds;
                //LoggerPtr _logger;
                
            };
            
            class About : public BaseContent
            {
            public:
                About(cppcms::http::context& httpctxt);
                
            public:
                bool Done;
                string StatusLabelClass;
            };
            
            class PortfolioWiz : public CompanyPicks
            {
            public:
                PortfolioWiz(cppcms::http::context& httpctxt);
                ~PortfolioWiz();
                virtual void Init();
                virtual void Init(cppcms::http::context& httpctxt);
                
                ISecurityExchangeList* get_Exchanges();
                
            public:
                R8 RiskFreeRate;
                
            private:
                SP<ISecurityExchangeList> _pExchanges;
            };
        }
    }
    
}



#endif	/* MODELMASTER_H */

