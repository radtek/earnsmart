/* 
 * File:   CJsonWrapper.h
 * Author: santony
 *
 * Created on September 3, 2012, 1:24 PM
 */

#ifndef CJSONWRAPPER_H
#define	CJSONWRAPPER_H

#include <vector>
#include <unordered_map>
#include <string>

#include "../ReturnCodes.h"
#include "../seglobaltypedefs.h"

#include <cgicc/Cgicc.h>
#include "FCgiIO.h"

#include <log4cxx/logger.h>

using namespace cgicc;
using namespace std;
using namespace log4cxx;


namespace se
{
    namespace gw
    {
        #define FUNCPARAMS  std::vector<std::string>

        class CJsonWrapperBase
        {
        public:
            CJsonWrapperBase() {};
            virtual inline ~CJsonWrapperBase() {};
            virtual ErrorId ExecuteFunction(const std::string& url, std::ostream& io, I32 functionId, const FUNCPARAMS& parameters) = 0;
        };

        typedef unordered_map<I32, CJsonWrapperBase*> MODULEMAP;

        /* Contains a large set of functions that returns a json-formatted data from
         various smartearn libraries. */
        class CJsonWrapper 
        {
        public:
            CJsonWrapper();
            CJsonWrapper(const CJsonWrapper& orig);
            virtual ~CJsonWrapper();

            ErrorId ExecuteFunction(const std::string& url, std::ostream& io, I32 ModuleId, I32 FunctionId, const FUNCPARAMS& parameters);

        private:
            MODULEMAP moduleMap;
            LoggerPtr _logger;
        };
        
        
    }
}


#endif	/* CJSONWRAPPER_H */

