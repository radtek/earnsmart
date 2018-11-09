/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEmailNotifications.h
 * Author: santony
 *
 * Created on April 26, 2016, 5:49 AM
 */

#ifndef CEMAILNOTIFICATIONS_H
#define CEMAILNOTIFICATIONS_H

#include "internal.h"
#include <string>
#include "../senet/CEmailTemplate.h"

using namespace std;

class CEmailNotifications 
{
public:
    CEmailNotifications(ISessionInterface*);
    CEmailNotifications(const CEmailNotifications& orig) = delete;
    virtual ~CEmailNotifications();
    
    
    SE_CODE NotifyResetPassword(
        const string& emailAddress, 
        const string& tempPassword,
        const string& signinUrl
    );
    
    SE_CODE NotifyToConfirmEmailOnRegistration(
        const string& emailAddress, 
        const string& firstName,
        const string& confirmUrl,
        const UserRole& role
    );
    
private:
    SE_CODE ApplyTemplateAndEmail(const string& emailtosend, const string& tmplFileName, const se::net::TemplateVaribles& vars);
    
private:
    SP<ISessionInterface> _pSession;

};

#endif /* CEMAILNOTIFICATIONS_H */

