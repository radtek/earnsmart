/* 
 * File:   CSessionManageMethods.h
 * Author: santony
 *
 * Created on August 9, 2015, 11:25 AM
 */

#ifndef CSESSIONMANAGEMETHODS_H
#define	CSESSIONMANAGEMETHODS_H

#include "../seadmin/seadminapi.h"
#include <cppcms/http_context.h>

using namespace cppcms;
using namespace se::admin;

class CSessionManageMethods 
{
public:
    static SE_CODE BuildUserSession(http::context& cctxt, IUserSession*& pUserSession) ;
};

#endif	/* CSESSIONMANAGEMETHODS_H */

