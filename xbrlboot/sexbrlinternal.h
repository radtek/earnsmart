/* 
 * File:   sexbrlinternal.h
 * Author: santony
 *
 * Created on July 1, 2012, 5:33 PM
 */

#ifndef SEXBRLINTERNAL_H
#define	SEXBRLINTERNAL_H

#include "../ReturnCodes.h"
#include "../seglobal/Object.h"
#include <log4cxx/logger.h>
#include "../seglobaltypedefs.h"

using namespace log4cxx;

extern log4cxx::LoggerPtr logger;

// temporary

namespace se
{
    namespace xbrl
    {
        typedef enum StatementType
        {
            ST_Unknown,
            ST_CashFlow,
            ST_Income,
            ST_BalanceSheet
        } STATEMENTTYPE, ST;
        
    }
}



#endif	/* SEXBRLINTERNAL_H */

