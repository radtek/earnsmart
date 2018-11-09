/* 
 * File:   CJsonDataFormattedOutput.h
 * Author: santony
 *
 * Created on January 19, 2013, 9:33 PM
 */

#ifndef CJSONDATAFORMATTEDOUTPUT_H
#define	CJSONDATAFORMATTEDOUTPUT_H

#include "IDataFormattedOutput.h"
#include "../seglobal/CObjectRoot.h"
#include "CDataTable.h"
//#include <log4cxx/logger.h>
#include <jsoncpp/json/value.h>
#include <string>

using namespace std;


namespace se
{
    namespace data
    {
        class CJsonDataFormattedOutput : public CObjectRoot, public IDataFormattedOutput
        {
        public:
            CJsonDataFormattedOutput(ISession*);
            CJsonDataFormattedOutput(const CJsonDataFormattedOutput& orig);
            virtual ~CJsonDataFormattedOutput();
            
            virtual ErrorId Init(CDataTable* table);
            
            virtual ErrorId ToString(IString** outVal);
            
           
            
        private:  
            
            //log4cxx::LoggerPtr _logger;
            
            string jsonwriteoutput;
         
        private:
            void buildJson(CDataTable*,Json::Value&);
               
       
        };
       
    }
}

 
#endif	/* CJSONDATAFORMATTEDOUTPUT_H */

