/* 
 * File:   CLoginOrRegister.cpp
 * Author: santony
 * 
 * Created on July 19, 2015, 4:45 PM
 */

#include "common.h"
#include "CLoginOrRegister.h"

CLoginOrRegister::CLoginOrRegister()  : 
    CModelBase("LoginOrRegisterView")
{
}

CLoginOrRegister::CLoginOrRegister(const CLoginOrRegister& orig) {
}

CLoginOrRegister::~CLoginOrRegister() {
}

UserRole CLoginOrRegister::get_Roles() 
{
    if (_roles == UserRole::Anonymous)
    {
        if (UserSession && UserSession->get_UserId())
        {
            SP<IUser> pUser = UserSession->get_User();
            _roles = pUser->get_Roles();
        }
    }
    return _roles;
}


bool CLoginOrRegister::get_IsAdmin() 
{
    get_Roles();
    return (I32)(_roles) & (I32)UserRole::Admin == (I32)UserRole::Admin;
}

bool CLoginOrRegister::get_IsAnon() 
{
    get_Roles();
    return (_roles) == UserRole::Anonymous;
}

bool CLoginOrRegister::get_IsBasic() 
{
    get_Roles();
    return (_roles) == UserRole::Standard;
}

bool CLoginOrRegister::get_IsInternal() 
{
    get_Roles();
    return (_roles) == UserRole::Internal;
}

bool CLoginOrRegister::get_IsLicensed() 
{
    get_Roles();
    return (_roles) == UserRole::Licensed;
}

