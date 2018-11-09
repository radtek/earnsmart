/* 
 * File:   CMaster.cpp
 * Author: santony
 * 
 * Created on July 3, 2015, 9:00 PM
 */

#include "common.h"
#include "CMaster.h"

CHeader CMaster::Header;
CFooter CMaster::Footer;
CToolbar CMaster::Toolbar;

CMaster::CMaster(const string& title) : 
    CModelBase("MasterView", title)
{
    Toolbar.InitMenus();
}

CMaster::~CMaster() 
{
}

void CMaster::serialize(archive& a) 
{
    CModelBase::serialize(a);
    a & cppcms::as_pod(Toolbar);
}


const se::admin::IUserSession* CMaster::get_UserSession() 
{
    return _pUserSession;
}

void CMaster::set_UserSession(IUserSession* p) 
{
    _pUserSession = p;
}




