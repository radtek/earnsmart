/* 
 * File:   CSmartgazeApp.h
 * Author: santony
 *
 * Created on August 18, 2013, 12:22 PM
 */

#ifndef CSMARTGAZEAPP_H
#define	CSMARTGAZEAPP_H

#include "CBaseApplication.h"
#include <cppcms/http_response.h>
#include <jsoncpp/json/json.h>
#include "CSPSerializable.h"

using namespace std;
using namespace cppcms;

namespace sg
{
    namespace web
    {
        class CSmartearnApp : public CBaseApplication
        {
        public:
            CSmartearnApp(cppcms::service& srv);
            virtual ~CSmartearnApp();

        protected:            
            virtual void Init(cppcms::service& srv);

        private:
            
            virtual void init();
            
            // Used for downloading (by client) any file from the webserver such as 
            // js, css or images
            void GetFile(string filename);
            
            // our home page.
            void Default();

            void DisplayMaintenancePage();
            
            // subscription page.
            void ViewSubscriptions();
            
            // search results
            void ProcessSearchList();
            
            // economy page for first release.
            void DisplayEconomyView(string catid = "");
            
            // about view
            void DisplayAboutView();
            
            // sitemap
            void DisplaySitemap();
            
            // list same sector/industry companies
            void DisplayListSameIndustries(string gicid);
            // list companies with similar performances
            void DisplayListSimilarPerformances(string productid);            
            
            ///////////////////fundamentals///////////////////////////
            void DisplayCompanyPicks();
            void DisplayCompanyScreener();
            void GenerateScreenerResults(const Json::Value& json);
            void DisplayCompanySummary(string productId);
            void DisplayFinancials(string productId);
            void DisplayPerformance(string productId);
            void DisplayProspects(string productId);
            void DisplayProductNews(string productId, int newsIndex = -1);
            void DisplayFundamentals(string productId);
            void DisplayFundamentals(string pagename, string productId);
            void DisplayPortfolioBuilder();
           
            /** json data */
            void OutputJsonCompanyInfo(string productId);
            void OutputJsonMonthlyPriceHistory(string productId);
            virtual void GetJson(string p1, string p2, string p3);
            
            void BuildPortfolio();
            

            
        private:
            SP<IXbrl> _pxbrl;   // this is required for async caching such as when calling GetCachedFinancialPerformances
            SP<ISecurityAPI> _pSecApi;
            SGSP<IFinancialPerformanceList> _pperformances;
            SP<se::config::IConfigurator> _pconfig;    
        };
        
    }
}


#endif	/* CSMARTGAZEAPP_H */

