/* 
 * File:   CDataTableToJsonFormatter.cpp
 * Author: santony
 * 
 * Created on January 19, 2013, 10:01 PM
 */

#include "CDataTableToJsonFormatter.h"
#include "CDataTable.h"
#include "../seglobal/Object.h"
#include "CJsonDataFormattedOutput.h"

namespace se
{
    namespace data
    {
        CDataTableToJsonFormatter::CDataTableToJsonFormatter(ISession* ps):CObjectRoot(ps)
        {
            
        }

        CDataTableToJsonFormatter::CDataTableToJsonFormatter(const CDataTableToJsonFormatter& orig) : CObjectRoot(orig)
        {
            
        }

        CDataTableToJsonFormatter::~CDataTableToJsonFormatter() 
        {
            
        }
        
        ErrorId CDataTableToJsonFormatter::Convert(IDataProvider* input, IDataFormattedOutput** output)
        {
            // dynamiccast and test if input is an instance of CDataTable
            CDataTable* dt = dynamic_cast<CDataTable*>(input);
            if (!dt)
            {
                //LOG4CXX_ERROR(_logger, "'input' argument is not a CDataTable* type or it is null.");
                return E_InvalidArg;
            }
            //LOG4CXX_INFO(_logger, "creating an instance of jsonformattedoutput and returning the status code.");
            return CObject<CJsonDataFormattedOutput>::Create(get_Session(), dt, output);
        }
    }
}

