/* 
 * File:   CXbrlElement.cpp
 * Author: santony
 * 
 * Created on December 10, 2012, 10:53 PM
 */

#include "CXbrlElement.h"

namespace se
{
    namespace xbrl
    {
        extern "C" STATEMENTTYPE ParseStatementType(const string& statementDesc)
        {
            ST ret = ST_Unknown;
            //TODO: Add a proper parsing of statementDesc and then return closing matching enum value.
            return ret;
        }
    }
}