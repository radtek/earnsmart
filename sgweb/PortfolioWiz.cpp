/* 
 * File:   PortfolioWiz.cpp
 * Author: santony
 * 
 * Created on June 29, 2014, 2:04 PM
 */

#include "models.h"
#include "sgwebinternal.h"
#include "../seglobal/exceptions.h"

namespace sg
{
    namespace web
    {
        namespace models
        {

            PortfolioWiz::PortfolioWiz(cppcms::http::context& ctxt) : CompanyPicks(ctxt, "PortfolioWizView")
            {
                
            }

            PortfolioWiz::~PortfolioWiz() 
            {
            }

            void PortfolioWiz::Init() 
            {
                CompanyPicks::Init();
            }

            void PortfolioWiz::Init(cppcms::http::context& httpctxt) 
            {
                CompanyPicks::Init(httpctxt);
                
                // Populate exchanges
                SP<ISecurityAPI> psecapi;
                THROWIFFAILED(SecOpen(&psecapi), "Failed to open security api gw.");
                THROWIFFAILED(psecapi->get_Exchanges(&_pExchanges), "Failed to get exchange list.");
            }

            ISecurityExchangeList* PortfolioWiz::get_Exchanges() 
            {
                return _pExchanges._ptr;
            }

        }
    }
}
