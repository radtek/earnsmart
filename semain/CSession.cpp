/* 
 * File:   CSession.cpp
 * Author: santony
 * 
 * Created on October 30, 2012, 8:05 PM
 */

#include "semaininternal.h"
#include "CSession.h"
#include "CNamedObjectList.h"
#include "CSessionActivity.h"
#include "CApplication.h"
#include "../seglobal/CStringUtility.h"

using namespace se::admin;

CSession::CSession(ISession* ps) : CObjectRoot(ps) {

}

CSession::~CSession() {

}

UserRole CSession::get_UserRole() 
{
    if (!_pUser)
    {
        return UserRole::Anonymous;
    }
    return (UserRole)_pUser->get_Roles();
}

bool CSession::IsUserHasRole(UserRole ur) 
{
    U32 r = (U32) ur;
    U32 roles = (U32)get_UserRole();
    return r == (roles & r);
}

bool CSession::IsUserRoleHigherThan(UserRole ur) 
{
    auto roles = (U32)get_UserRole();
    U32 r = (roles & (U32)ur) ;
    return roles > r;
}


RP<INamedObject> CSession::get_Object(NamedObjectNamingType name) 
{
    return _pObjects->get(name);
}

void CSession::set_Object(NamedObjectNamingType typeName, INamedObject* pObject) 
{
    _pObjects->set(typeName, pObject);
}

UID CSession::get_UserId() 
{
    if (!_pUser)
    {
        return 0;
    }
    return _pUser->get_Id();
}

CSTR CSession::get_UserName()
{
    if (!_pUser)
    {
        return nullptr;
    }
    return _pUser->get_UserId();
}

ErrorId CSession::get_Data(I32 key, LPVOID** outVal) 
{
    return E_NotImpl;
}

SE_CODE CSession::InitApis() 
{
    SP<IAdminAPI> papi1;
    RETURNIFFAILED(AdminOpen(this, &papi1));
    set_Object(IAdminAPIName, papi1);

    SP<IEconomyApi> papi2;
    RETURNIFFAILED(EconOpen(this, &papi2));
    set_Object(IEconomyApiName, papi2);
    
    SP<IEODData> papi3;
    RETURNIFFAILED(EodDataOpen(this, &papi3));
    set_Object(IEODDataName, papi3);
    
    
    SP<ISecurityAPI> papi4;
    MARKETDATAPROVIDERS provider = this->get_Configs()->get_DefaultMarketDataProvider();
    RETURNIFFAILED(SecOpen(this, provider, &papi4));
    set_Object(ISecurityAPIName, papi4);
    
    SP<IXbrl> papi5;
    RETURNIFFAILED(XbrlOpen(this, &papi5));
    set_Object(IXbrlName, papi5);

    return S_OK;
    
}


SE_CODE CSession::Init(IApp* pApp)
{
    if (!pApp)
        return E_InvalidArg;

    auto ret = CObject<CNamedObjectList>::Create(this, &this->_pObjects);
    if (FAILED(ret))
        return ret;
    _pApp = pApp;
    // initialize apis
    RETURNIFFAILED(InitApis());
    RETURNIFFAILED(get_AdminApi()->RegisterUser(&_pUser)); // creates a new user as anonymous and populates session with a session id.
    return S_Success;

}

ErrorId CSession::Init(IApp* pApp, const char* userId, const char* password) {
    if (!pApp)
        return E_InvalidArg;

    auto ret = CObject<CNamedObjectList>::Create(this, &this->_pObjects);
    if (FAILED(ret))
        return ret;
    _pApp = pApp;
    // initialize apis
    RETURNIFFAILED(InitApis());
    // is email address ?
    bool isemail = se::utils::CStringUtility::ValidateEmail(userId);
    // others goes here...
    SP<IAdminAPI> pAdmin ;
    RETURNIFFAILED(AdminOpen(this, &pAdmin));
    auto retCode = pAdmin->AuthenticateUser(userId, password, isemail);
    if (ISOK(retCode))
    {
        return pAdmin->CheckUserLogin(userId, &_pUser);
    }
    return retCode;
}

long unsigned int CSession::Init(IApp* pApp, const char* sessionId)
{
    
    if (!pApp)
        return E_InvalidArg;

    auto ret = CObject<CNamedObjectList>::Create(this, &this->_pObjects);
    if (FAILED(ret))
        return ret;

    _pApp = pApp;
    
    _uniqueId = sessionId;
    
    // initialize apis
    RETURNIFFAILED(InitApis());

    SP<IAdminAPI> pAdmin ;
    RETURNIFFAILED(AdminOpen(this, &pAdmin));
    auto retCode = pAdmin->get_UserFromSessionUniqueID(sessionId, &_pUser);
    return retCode;
}

long unsigned int CSession::Reset(IUser* pUser)
{
    this->_pUser = pUser; // no need of calling set_UniqueId() since we are simply reassigning the new user to the session instance.
    if (_pUser)
    {
        this->_stage = pUser->get_RegistrationStage(); 
    }
    else
    {
        this->_stage = REGSTAGES::None;
    }
    return S_OK;
}


const char* CSession::get_UniqueId() 
{
    return _uniqueId.c_str();
}

void CSession::set_UniqueId(const char* value) 
{
    if (strcmp(_uniqueId.c_str(), value))
    {
        ((CApplication*)_pApp._ptr)->ResetSession(value, this);
    }
    _uniqueId = value;
}

RP<IApp> CSession::get_App() 
{
    return _pApp;
}

std::recursive_mutex& CSession::get_LockObject() {
    return _lock;
}

RP<se::config::IConfigurator> CSession::get_Configs() {
    return _pApp->get_Configurator();
}

ErrorId CSession::Close() {

    return S_Success;
}

RP<IAdminAPI> CSession::get_AdminApi() 
{
    auto ptr = get_Object(IAdminAPIName);
    return (IAdminAPI*)ptr._ptr;
}

RP<IEconomyApi> CSession::get_EconomyApi() {
    auto ptr = get_Object(IEconomyApiName);
    return (IEconomyApi*) ptr._ptr;
}

RP<IEODData> CSession::get_EodDataApi() {
    auto ptr = get_Object(IEODDataName);
    return (IEODData*) ptr._ptr;
}

RP<ISecurityAPI> CSession::get_SecurityApi() {
    auto ptr = get_Object(ISecurityAPIName);
    return (ISecurityAPI*) ptr._ptr;
}

RP<IXbrl> CSession::get_XbrlApi() {
    auto ptr = get_Object(IXbrlName);
    return (IXbrl*) ptr._ptr;
}

long unsigned int CSession::Logout()
{
    this->_pUser.Clear();
    return S_OK;
}

bool CSession::get_Remember() 
{
    return bRemember ;
}

void CSession::set_Remember(bool value) 
{
    bRemember = value;
}

bool CSession::get_IsAuthenticated() 
{
    RP<IUser> pUser = get_User();
    if (pUser)
    {
        bool bRet = ( pUser->get_RegistrationStage() == REGSTAGES::RegistrationCompleted && ( pUser->get_IsInRole(UserRole::Licensed) || pUser->get_IsInRole(UserRole::Standard ))) 
                || ( pUser->get_IsInRole(UserRole::Internal) || pUser->get_IsInRole(UserRole::Admin ) );
        return bRet;
    }
    return false;
}

void* CSession::get_Tag() 
{
    return _pTag;
}

void CSession::set_Tag(void* tag) 
{
    _pTag = tag;
}

RP<IUser> CSession::get_User()
{
    return _pUser;
}

RP<IString> CSession::get_UserRoleStr() 
{
    SP<IString> str;
    IString::Create("unknown", &str);
    switch(_pUser->get_Roles())
    {
        case UserRole::Anonymous:
            str->set_Buffer("anonymous");
            break;
        case UserRole::Admin:
            str->set_Buffer("admin");
            break;
        case UserRole::Standard:
            str->set_Buffer("standard");
            break;
        case UserRole::Licensed:
            str->set_Buffer("licensed");
            break;
        case UserRole::Internal:
            str->set_Buffer("internal");
            break;
    }
    return str;
}


