/* 
 * File:   CFundamentalsApp.h
 * Author: santony
 *
 * Created on August 18, 2013, 12:31 PM
 */

#ifndef CFUNDAMENTALSAPP_H
#define	CFUNDAMENTALSAPP_H

#include "CBaseApplication.h"
#include "../sexbrl/sexbrlapi.h"
#include "../sesecurities/sesecuritiesapi.h"

using namespace se;
using namespace se::xbrl;
using namespace cppcms;
using namespace se;
using namespace boost;	
using namespace se::securities;

namespace sg
{
    namespace web
    {

        class CFundamentalsApp  : public CBaseApplication
        {
        public:
            CFundamentalsApp(cppcms::service& srv);
            virtual ~CFundamentalsApp();
            
            virtual void init();
            
        private:
            void DisplayStockPicks();
            void DisplayCompanySummary(string productId);
            void DisplayFinancials(string productId);
            void DisplayPerformance(string productId);
            void DisplayProspects(string productId);
            void DisplayFundamentals(string productId);
           
            /** json data */
            void OutputJsonCompanyInfo(string productId);
            void OutputJsonMonthlyPriceHistory(string productId);
            

        };
    }
}

#endif	/* CFUNDAMENTALSAPP_H */

