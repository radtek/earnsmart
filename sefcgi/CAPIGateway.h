/* 
 * File:   CAPIGateway.h
 * Author: santony
 *
 * Created on August 31, 2012, 1:42 AM
 */


#ifndef CAPIGATEWAY_H
#define	CAPIGATEWAY_H

#include <iostream>
#include "../seglobaltypedefs.h"
#include <vector>

#include "FCgiIO.h"
#include "CJsonWrapper.h"

#include <log4cxx/logger.h>

using namespace std;
using namespace log4cxx;

typedef vector<string> PARAMS;

namespace se
{
    namespace gw
    {
        
        /*
         * The gateway class accepts a request, submits to a threadpool queue.
         * The queue will process the request, and write the output to the request's output stream.
         * Then calls fastcgi's request completion call.
         */

        class CAPIGateway 
        {
        public:
            CAPIGateway();
            CAPIGateway(const CAPIGateway& orig);
            virtual ~CAPIGateway();
            void InvokeMethod(const std::string& url, std::ostream& io);

        private:
            CJsonWrapper _jsonWrapper;
            LoggerPtr _logger;
        };
        
    }
}


#endif	/* CAPIGATEWAY_H */

