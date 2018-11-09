/* 
 * File:   CPwd.cpp
 * Author: santony
 * 
 * Created on August 12, 2015, 2:30 PM
 */

#include "CPwd.h"
#include "../seadmin/seadminapi.h"

using namespace se;
using namespace se::admin;

CPwd::CPwd() {
}

CPwd::CPwd(const CPwd& orig) {
}

CPwd::~CPwd() {
}

void CPwd::Check() 
{
    // verity email
    SP<IAdminAPI> api;
    if (SUCCEEDED(AdminOpen(&api)))
    {
        if (SUCCEEDED(api->VerifyUser(EmailAddress.c_str(), Pwd.c_str())))
        {
            // compare passwrod
            if (NewPwd == NewPwdConfirm)
            {
                // Update password.
                auto rcode = api->UpdateAuthentication(EmailAddress.c_str(), 
                        Pwd.c_str(), 
                        NewPwd.c_str());
                if (FAILED(rcode))
                {
                    set_CommonError("Failed to save new password. System error. Site admin has been notified.");
                    std::ostringstream ostr;
                    ostr << (EmailAddress.empty() ? "Unknown user" : EmailAddress.c_str()) << "attempted to update new password and failed. Site error.";
                    NotifyError(ostr.str());
                }
            }
            else
            {
                set_CommonError("New and confirmation passwords do not match.");
            }
        }
        else
        {
            set_CommonError("Email address or password does not match.");
        }
    }
    else
    {
        set_CommonError("Invalid Operation. Site admin has been notified.");
        std::ostringstream ostr;
        ostr << (EmailAddress.empty() ? "Unknown user" : EmailAddress.c_str()) << "attempted to change password and failed. Site error.";
        NotifyError(ostr.str());
    }
}

void CPwd::Login(IUserSession*& us) 
{
    SP<IAdminAPI> api;
    AdminOpen(&api);
    auto rcode = api->AuthenticateUser(EmailAddress.c_str(), NewPwd.c_str(), us);
    if (FAILED(rcode))
    {
        string error;
        if (E_SecurityAuthorization == rcode)
        {
            error = "Invalid username or password.";
        }
        else
        {
            error = "An internal error while attempting to sign in. Site administrator is been notified. Sorry for any inconvenience.";
        }
        set_CommonError(error);
        std::ostringstream ostr;
        ostr << EmailAddress << " has failed to login. The reported error was: " << error;
        NotifyError(ostr.str());
    }
 
}

