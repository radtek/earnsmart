/* 
 * File:   CSearchController.h
 * Author: santony
 *
 * Created on August 25, 2013, 7:58 AM
 */

#ifndef CSEARCHCONTROLLER_H
#define	CSEARCHCONTROLLER_H

#include "CBaseApplication.h"
#include "../sesecurities/sesecuritiesapi.h"

using namespace std;
using namespace se;
using namespace se::securities;

namespace sg
{
    namespace web
    {
        class CSearchController : public CBaseApplication
        {
        public:
            CSearchController(cppcms::service&);

            virtual ~CSearchController();
        
        private:

            /* Method used to get a company list in json form for jqueries*/
            void GetMatchingCompanies();
            void getMatchCompanies();
        
        private:
            SP<ISecurityAPI> _pSecApi;
            
            
        };

        
    }
}

#endif	/* CSEARCHCONTROLLER_H */

