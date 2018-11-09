/* 
 * File:   CMyqlUtilities.h
 * Author: santony
 *
 * Created on September 4, 2013, 6:34 AM
 */

#ifndef CMYQLUTILITIES_H
#define	CMYQLUTILITIES_H

#include "../seglobal/IObject.h"
#include "../seglobal/CDate.h"
#include "../seglobal/CTimeStamp.h"
#include "../external/mariadbpp/mysql++.h"
#include "semysqlapi.h"

using namespace mysqlpp;

namespace se
{
    namespace data
    {
        
        class MSQLEXP CMyqlUtilities 
        {
        public:
            static void PopulateDateTime(sql_datetime const& indt, se::DATETIME & outdt );
            static void PopulateDateTime(se::DATETIME const& indt, sql_datetime & outdt );
            static void PopulateDateTime(sql_datetime const& indt, se::CTimeStamp & outdt );
            static void PopulateDateTime(se::CTimeStamp & indt, sql_datetime & outdt );
            static void PopulateDateTime(se::CDate & indt, sql_date & outdt );
            static void PopulateDateTime( sql_date & indt, se::CDate & outdt );
                        
        };
        
    }
}


#endif	/* CMYQLUTILITIES_H */

