/* 
 * File:   CSessionManageMethods.cpp
 * Author: santony
 * 
 * Created on August 9, 2015, 11:25 AM
 */

#include "CSessionManageMethods.h"
#include <cppcms/session_interface.h>
#include <cppcms/http_request.h>


SE_CODE CSessionManageMethods::BuildUserSession(http::context& cctxt, IUserSession*& pUserSession) 
{
    http::context& ctxt = cctxt;
    http::request& req = cctxt.request();
    pUserSession->set_SessionId(ctxt.session().get_session_cookie().c_str());
    pUserSession->set_UserAgent(req.http_user_agent().c_str());
    pUserSession->set_RemoteAddress(req.remote_addr().c_str());
    pUserSession->set_RemoteHost(req.remote_host().c_str());
    pUserSession->set_RemoteUser(req.remote_user().c_str());
    pUserSession->set_RemoteIdent(req.remote_ident().c_str());
    pUserSession->set_HttpReferer(req.http_referer().c_str());
}