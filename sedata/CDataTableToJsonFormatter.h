/* 
 * File:   CDataTableToJsonFormatter.h
 * Author: santony
 *
 * Created on January 19, 2013, 10:01 PM
 */

#ifndef CDATATABLETOJSONFORMATTER_H
#define	CDATATABLETOJSONFORMATTER_H

#include "IDataFormatter.h"
#include "../seglobal/CObjectRoot.h"
//#include <log4cxx/logger.h>

//using namespace log4cxx;

namespace se
{
    namespace data
    {
        class CDataTableToJsonFormatter : 
            public CObjectRoot, 
            public IDataFormatter
        {
        public:
            CDataTableToJsonFormatter(ISession*);
            CDataTableToJsonFormatter(const CDataTableToJsonFormatter& orig);
            virtual ~CDataTableToJsonFormatter();
            
            virtual ErrorId Convert(IDataProvider* input, IDataFormattedOutput** output);
            
        private:
           // LoggerPtr _logger;

        };
        
    }
}


#endif	/* CDATATABLETOJSONFORMATTER_H */

