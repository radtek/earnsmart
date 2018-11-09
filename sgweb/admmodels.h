/* 
 * File:   admmodels.h
 * Author: santony
 *
 * Created on September 18, 2013, 9:16 PM
 */

#ifndef ADMMODELS_H
#define	ADMMODELS_H

#include "CBaseContent.h"
#include "modelsupport.h"


using namespace sg::web::models;

namespace sg
{
    namespace web
    {
        namespace adm
        {
            class AdminModel : public BaseContent 
            {
            public:
                AdminModel();
                virtual ~AdminModel();
                virtual void Init(cppcms::http::context& httpctxt);
                virtual void Init(cppcms::http::context& httpctxt, BaseContent* pageModel);
                BaseContent& getPageModel();
                
            public:                
                string Title;        // page title
            private:
                BaseContent* PageModel;
            };
            

            class AccountMapModel : public BaseContent
            {
            public:
                enum PageModelSelection
                {
                    CommonAccounts,
                    MapCommonAccounts,
                    MapSpecificAccounts
                };
                
                AccountMapModel();
                AccountMapModel(const AccountMapModel& cp);
                virtual void Init(cppcms::http::context& httpctxt);
                
            public:
                PageModelSelection PageSelection;

            };

            class PredictionSetupModel : public BaseContent
            {
            public:
                enum PageModelSelection
                {
                    GeneralSetup,
                    Tie,
                    Test
                };
                
                PredictionSetupModel();
                PredictionSetupModel(const PredictionSetupModel& cp);
                virtual void Init(cppcms::http::context& httpctxt);
                
            public:
                PageModelSelection PageSelection;
                std::ostringstream GicsTree;

            };
            
            class EconomicIndicatorManagementModel : public BaseContent
            {
            public:
                EconomicIndicatorManagementModel();
                virtual void Init(cppcms::http::context& httpctxt);
            };
            
        }
    }
}


#endif	/* ADMMODELS_H */

