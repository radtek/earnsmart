/* 
 * File:   CAdminApp.h
 * Author: santony
 *
 * Created on August 18, 2013, 12:34 PM
 */

#ifndef CADMINAPP_H
#define	CADMINAPP_H

#include "CBaseApplication.h"
#include <cppcms/http_response.h>
#include "../semysql/CSEConnections.h"
#include "CSmartearnApp.h"

using namespace cppcms;

namespace sg
{
    namespace web
    {


        class CAdminApp  : public CBaseApplication
        {
        private:
            //se::data::CSmartConnection conn;
            
        public:
            CAdminApp(cppcms::service& srv);
            virtual ~CAdminApp();

            virtual void init();
            
        protected:
            // all admin json function identifiers starts from 10000;
            virtual void GetJson(string p1, string p2, string p3);

        private:
            ////////////////////Json Routines///////////////////////////////
            void ListGeneralAccounts();
            void ListUsgaapAccounts();
            void ListCommonlyMappedAccounts(string matchid);
            void ListCommonlyMappedAccounts(string matchid, string cik);
            void ListCompanyAccounts(string cik);
            void ListTrainingSets();
            void ListTrainingSetDetail(string setId);
            
            virtual void ListEconIndicatorCategories();

            virtual void ListEconIndicators(unsigned long catId);
            virtual void ListEconIndicator(unsigned long ecoId);

            virtual void GetRatioGroups();
            virtual void GetRatios(unsigned long groupid);


            

            
            ///////////////// Page Methods ///////////////////////////
            void Default();
            void CommonAccounts();
            void CommonAccountsAdd();

            void MapCommonAccounts();
            void MapCommonAccountsAdd();
            void MapCommonAccountsRemove();
            void MapCommonAccountsSort();

            void MapSpecificAccounts();
            void MapSpecificAccountsAdd();
            void MapSpecificAccountsRemove();
            void MapSpecificAccountsSort();

            void PredictionSetup(string pagename);
            void SetupPrediction();
            void DeletePrediction();
            void ListAllPredictionSetup();
            void ListGicsPredictionSetup();
            void ListProductPredictionSetup();
            void SaveGicsPredictionSetup();
            void SaveProductPredictionSetup();
            void SaveAllPredictionSetup();
            void StepThruModelTesting();
            
            void EconomyManageAdd();
            void EconomyManageEdit();
            
            void EconomyGetPredictedValue();
            
            void RecursiveOutputGicHierarchy(ISecurityAPI* papi, std::ostringstream& strm, UID gicid, CSmartConnection& conn);
            
            R8 RunCubeModel(int step, IExTrainingTable *pTable, Json::Value& ruleset);
            
            void ManageEconomicIndicators();
            

            virtual void GetLatestAnnualAccountValue();

            
        };

    }
}
#endif	/* CADMINAPP_H */

