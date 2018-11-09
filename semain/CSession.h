/* 
 * File:   CSession.h
 * Author: santony
 *
 * Created on October 30, 2012, 8:05 PM
 */

#ifndef CSESSION_H
#define	CSESSION_H

#include <string>
#include "../semysql/CSEConnections.h"
#include "ISessionInterface.h"


using namespace std;
using namespace se;

class CSession : public CObjectRoot, public ISessionInterface
{
    
    friend class CApplication;
    
protected:
    SE_CODE InitApis();
    
#ifdef __DEBUG__
    void OnAdd(U16 rc) override
    {
        BINF << rc;
    }
    
    void OnDec(U16 rc) override
    {
        BINF << rc;
    }

#endif
    
public:
    CSession(ISession*);
    CSession(const CSession& orig) = delete;
    virtual ~CSession();

    RP<se::config::IConfigurator> get_Configs() override;

    
    virtual UID get_UserId() ;
    const char* get_UserName() override;

    virtual ErrorId get_Data(I32 key, LPVOID** outVal);
    /* internal */
public:
    virtual ErrorId Init(IApp*, CSTR userId, CSTR password);
    
    long unsigned int Init(IApp* pApp);
    
    SE_CODE Init(IApp*, CSTR sessionId);
    
    SE_CODE Reset(IUser* pUser); // resets the session to represent new user.
    
    ErrorId Close();

    RP<IApp> get_App() override;
    
    
    RP<IAdminAPI> get_AdminApi() override;
    RP<IEconomyApi> get_EconomyApi() override;
    RP<IEODData> get_EodDataApi() override;
    RP<ISecurityAPI> get_SecurityApi() override;
    RP<IXbrl> get_XbrlApi() override;
   
    UserRole get_UserRole() override;
    bool IsUserHasRole(UserRole) override;
    bool IsUserRoleHigherThan(UserRole) override;


    RP<INamedObject> get_Object(NamedObjectNamingType) override;
    
    void set_Object(NamedObjectNamingType typeName, INamedObject* pObject) override;

    std::recursive_mutex& get_LockObject() override;

    long unsigned int Logout() override;

    bool get_Remember() override;
    
    void set_Remember(bool value) override;

    const char* get_UniqueId() override;
    
    void set_UniqueId(CSTR) override;

    bool get_IsAuthenticated() override;

    void* get_Tag() override;
    
    void set_Tag(void* tag) override;
    
    RP<IUser> get_User() override;


    RP<IString> get_UserRoleStr() override;

    
private:
    SP<IApp> _pApp;
    std::recursive_mutex _lock;
    SP<INamedObjectList> _pObjects;
    SP<IUser> _pUser;
    bool bRemember = false;
    string _uniqueId;
    POINTER _pTag = nullptr;
    SP<ISessionActivity> _pActivity;
    REGSTAGES _stage = REGSTAGES::UserTypeAndAddress;
};

#endif	/* CSESSION_H */

