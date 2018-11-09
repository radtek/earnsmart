/* 
 * File:   CBaseApplication.h
 * Author: santony
 *
 * Created on August 31, 2013, 10:23 AM
 */

#ifndef CBASEAPPLICATION_H
#define	CBASEAPPLICATION_H

#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include <cppcms/application.h>
#include "CSession.h"
#include <string>
#include "models.h"
//#include <log4cxx/logger.h>
#include <unordered_map>
#include <zthread/Mutex.h>
#include <zthread/Guard.h>

//using namespace log4cxx;
using namespace std;
using namespace cppcms;
using namespace sg::web::models;
using namespace ZThread;

namespace sg
{
    namespace web
    {
        class CBaseApplication : public application
        {
        public:
            CBaseApplication(cppcms::service& srv);
            virtual ~CBaseApplication();
            
            virtual void init();        // inherited
            virtual void clear();       // inherited

            virtual void main(std::string url);

        protected:
            virtual void Init(cppcms::service& srv);
            SE_CODE get_Session(se::admin::IWebSession**);
            bool IsAuthenticated();

            void GetFileBase(string filename);
            void InitModel(BaseContent& content);
            void ProcessSearch();
            void BuildHeaderMenus(Header& header, string selectedMenuHeader);
            void DisplayBase();
            virtual void GetJson(string p1, string p2, string p3);
            void DisplaySignIn();
            void DisplaySignout();
            void DisplayRegister();
            void DisplayPPApproved();
            void DisplayPPCanceled();
            void DisplayMyAccount();
            void CheckUser(string userId);
            void ListPaypalCountries();
            void ListStates(const string& countryid);
            virtual void ListEconIndicatorCategories(); 
            virtual void ListEconIndicators(UID catId = 0);
            void ListEconIndicatorDataList(unsigned long eid);

            void GetCompanySummaryBalanceSheetData(UID prodid);
            void GetCompanySummaryIncomeData(UID prodid);
            void GetCompanySummaryCashflowData(UID prodid);

            void GetYearlyFinancials(UID prodId);
            //void GetYearlyFinancialsFlipped(UID prodId);
            
            // screener related json outputs.
            void GetExchanges();
            void GetGICHierarchy(UID sectorId = 0); // to get sectors only set sectorId = 0 (default);
            virtual void GetRatioGroups();  // ratio groups
            virtual void GetRatios(UID groupid = 0);    // ratios
            void GetRatioValueCount(UID rid);
            
            void GetSecurityDetails();  // parameters are posted
            virtual void GetLatestAnnualAccountValue(); // get most recent annual account financial number (cik and xbrlid given)
            void GetSecurityProspectsData();
            
            void GetGicsComparedPerformanceData();
            
            void GetCurrentPriceRSSFeed(string prodID);
            
        private:
            void InvalidateExpiredSessions();
            
        protected:
            //LoggerPtr _logger;
            string _rootPath;
            static Mutex _mutex;
            SP<se::admin::IAdminAPI> _pAdm;
        };

    }
}

#endif	/* CBASEAPPLICATION_H */

