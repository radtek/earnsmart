/* 
 * File:   admmodels.cpp
 * Author: santony
 * 
 * Created on September 18, 2013, 9:16 PM
 */

#include "admmodels.h"


namespace sg
{
    namespace web
    {
        namespace adm
        {
            AdminModel::AdminModel() : 
                PageModel(nullptr)
            {
                viewName = "AdminHomeView";
            }

            AdminModel::~AdminModel() 
            {
            }

            void AdminModel::Init(cppcms::http::context& httpctxt) 
            {
                BaseContent::Init(httpctxt);
            }

            void AdminModel::Init(cppcms::http::context& httpctxt, BaseContent* pageModel) 
            {
                Init(httpctxt);
                PageModel = pageModel;
                PageModel->Init(httpctxt);
            }
            
            BaseContent& AdminModel::getPageModel()
            {
                if (!PageModel)
                    return *this;
                
                return *PageModel;
            }
            
            ////////////////////////////////// AccountMapmodel///////////////////////////////

            AccountMapModel::AccountMapModel() : PageSelection(CommonAccounts)
            {
            }

            AccountMapModel::AccountMapModel(const AccountMapModel& cp) : BaseContent(cp)
            {

            }


            void AccountMapModel::Init(cppcms::http::context& httpctxt) 
            {
                BaseContent::Init(httpctxt, "AccountMapView");
            }

            
            ////////////////////////////////PredictionSetupModel/////////////////////////////

            PredictionSetupModel::PredictionSetupModel() : PageSelection(GeneralSetup)
            {

            }
            
            PredictionSetupModel::PredictionSetupModel(const PredictionSetupModel& cp) 
            {

            }

            void PredictionSetupModel::Init(cppcms::http::context& httpctxt)
            {
                BaseContent::Init(httpctxt, "PredictionSetupView");
                
            }
            
            /////////////////////////////////Economic Indicator Management MOdel////////////////////
            EconomicIndicatorManagementModel::EconomicIndicatorManagementModel()
            {
                
            }
            
            void EconomicIndicatorManagementModel::Init(cppcms::http::context& httpctxt)
            {
                BaseContent::Init(httpctxt, "EconomicIndicatorManageView");
            }
        }
    }
}