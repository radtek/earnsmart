/* 
 * File:   JsonWrappers.h
 * Author: santony
 *
 * Created on September 3, 2012, 3:04 PM
 */

#ifndef JSONWRAPPERS_H
#define	JSONWRAPPERS_H

#include <iostream>
#include "CJsonWrapper.h"
#include <string>
#include <log4cxx/logger.h>


using namespace std;

namespace se
{

    namespace gw
    {
    
        class CXbrlJsonWrapper : public CJsonWrapperBase 
        {
        private:
           log4cxx::LoggerPtr _logger;
            
        public:
            CXbrlJsonWrapper();
            CXbrlJsonWrapper(const CXbrlJsonWrapper& orig);
            virtual ~CXbrlJsonWrapper();
            
            virtual ErrorId ExecuteFunction(const std::string& url, std::ostream& io, I32 functionid, const FUNCPARAMS& parameters);
            
            ErrorId GetCompanyInfo(CSTR ticker, string& jsonoutput); 
            
            ErrorId GetCompanyPerformance(CSTR ticker, string& jsonoutput);

        };
        
        // admin wrapper
        
        class CAdminJsonWrapper : public CJsonWrapperBase
        {
        private:
           log4cxx::LoggerPtr _logger;
           
        public:
            CAdminJsonWrapper();
            virtual ~CAdminJsonWrapper();
            virtual ErrorId ExecuteFunction(const std::string& url, std::ostream& io, I32 functionid, const FUNCPARAMS& parameters);
            
            /*Admin functions*/
            ErrorId RegisterUser(FCgiIO& io);
            
        };
    }

}

#endif	/* JSONWRAPPERS_H */

