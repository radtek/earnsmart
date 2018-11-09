/* 
 * File:   CLoginOrRegister.h
 * Author: santony
 *
 * Created on July 19, 2015, 4:45 PM
 */

#ifndef CLOGINORREGISTER_H
#define	CLOGINORREGISTER_H

#include "CModelBase.h"
#include <string>

using namespace std;


class CLoginOrRegister  : public CModelBase
{
private:
    UserRole _roles = UserRole::Anonymous;
    
public:
    string LoginId;
    string Password;
    SP<IWebSession> UserSession ;
    
public:
    CLoginOrRegister();
    CLoginOrRegister(const CLoginOrRegister& orig);
    virtual ~CLoginOrRegister();
    
    UserRole get_Roles();
    
    bool get_IsAnon();
    bool get_IsBasic();
    bool get_IsLicensed();
    bool get_IsInternal();
    bool get_IsAdmin();
    
private:

};

#endif	/* CLOGINORREGISTER_H */

