/* 
 * File:   CSessionActivity.cpp
 * Author: aashok
 * 
 * Created on August 31, 2013, 4:06 PM
 */

#include "semaininternal.h"
#include "CSessionActivity.h"


namespace se
{
    CSessionActivity::CSessionActivity(ISession* ps) : CObjectRoot(ps)
    {

    }

    CSessionActivity::~CSessionActivity() 
    {
        //Save();
    }

    DATETIME& CSessionActivity::get_Time()
    {
        return _time;
    }

    CSTR CSessionActivity::get_UserAgent()
    {
        return _useragent.c_str();
    }

    void CSessionActivity::set_Time(DATETIME& ts)
    {
        _time = ts;
    }

    void CSessionActivity::set_UserAgent(const char* val)
    {
        _useragent = val;
    }

    CSTR CSessionActivity::get_HttpReferer()
    {
        return _httpReferer.c_str();
    }

    void CSessionActivity::set_HttpReferer(CSTR val)
    {
        _httpReferer = val;
    }

    CSTR CSessionActivity::get_RemoteAddress()
    {
        return _remoteAddr.c_str();
    }

    void CSessionActivity::set_RemoteAddress(CSTR val)
    {
        _remoteAddr = val;
    }

    CSTR CSessionActivity::get_RemoteHost()
    {
        return _remoteHost.c_str();
    }

    void CSessionActivity::set_RemoteHost(CSTR val)
    {
        _remoteHost = val;
    }

    CSTR CSessionActivity::get_RemoteIdent()
    {
        return _remoteIdent.c_str();
    }

    void CSessionActivity::set_RemoteIdent(CSTR val)
    {
        _remoteIdent = val;
    }

    CSTR CSessionActivity::get_RemoteUser()
    {
        return _remoteUser.c_str();
    }

    void CSessionActivity::set_RemoteUser(CSTR val)
    {
        _remoteUser = val;
    }

}

