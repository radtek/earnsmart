/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEmailNotifications.cpp
 * Author: santony
 * 
 * Created on April 26, 2016, 5:49 AM
 */

#include "../senet/CSmtpClient.h"
#include "../seglobal/CFile.h"
#include "CEmailNotifications.h"


using namespace se::net;

const string sendEmailAddress = "saji.antony@smartgazellc.com";

CEmailNotifications::CEmailNotifications(ISessionInterface* pSession)
{
    this->_pSession = pSession;
}

CEmailNotifications::~CEmailNotifications()
{
}

long unsigned int CEmailNotifications::ApplyTemplateAndEmail(const string& emailtosend, const string& tmplFileName, const se::net::TemplateVaribles& vars)
{
    string emailFileIn = (string)_pSession->get_App()->get_Configurator()->get_ExecutableDir() + "/emailtemplates/" + tmplFileName;
    CEmailTemplate tmpl(_pSession, emailFileIn, vars);
    string outFile = tmpl.Apply();
    if (!CFile::Exists(outFile.c_str()))
    {
        BERR << "Failed to create email template file. File name is empty.";
        return E_FileNotFound;
    }
    
    string contents ;
    try
    {
        contents = CFile::ReadFile(outFile.c_str());
    }
    catch(size_t errnumber)
    {
        BERR << "Failed to read the contents of the templated file with error " << strerror(errnumber);
        return E_ReadError;
    }
    
    CSmtpClient smtp;
    return smtp.SendMail(emailtosend.c_str(), sendEmailAddress.c_str(), "earn-smart: Password Reset", contents.c_str());

}


SE_CODE CEmailNotifications::NotifyResetPassword
(
    const string& emailAddress, 
    const string& tempPassword,
    const string& signinUrl
)
{
    // Utilize a email template to email out a reset message.
    se::net::TemplateVaribles vars;
    vars.insert(make_pair("emailAddress", emailAddress));
    vars.insert(make_pair("TempPassword", tempPassword ));
    vars.insert(make_pair("signInUrl",  signinUrl));

    return ApplyTemplateAndEmail(emailAddress, "resetpassword.html", vars);

}

long unsigned int CEmailNotifications::NotifyToConfirmEmailOnRegistration(const string& emailAddress, const string& firstName, const string& confirmUrl, const UserRole& role)
{

    se::net::TemplateVaribles vars;
    vars.insert(make_pair("FirstName", firstName));
    vars.insert(make_pair("ConfirmUrl",  confirmUrl));

    return ApplyTemplateAndEmail(emailAddress, role == UserRole::Licensed ? "licensedregistration.html" : "standardregistration.html", vars);
}


