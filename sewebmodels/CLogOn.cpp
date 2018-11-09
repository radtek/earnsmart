/* 
 * File:   CLogOn.cpp
 * Author: santony
 * 
 * Created on July 19, 2015, 7:17 PM
 */

#include "CLogOn.h"
#include "CValidationMethods.h"

#define TEXTEMAIL       "EmailAddress"
#define TEXTPWD         "Password"

CLogOn::CLogOn() 
{
}

CLogOn::CLogOn(const CLogOn& orig) :
    EmailAddress(orig.EmailAddress), 
        Password(orig.Password),
        Remember(orig.Remember)
{
    
}

CLogOn::~CLogOn() 
{
    
}

void CLogOn::Validate(IUserSession*& us) 
{
    if (!CValidationMethods::ValidateEmail(EmailAddress))
        set_Error(TEXTEMAIL, "Requires a valid email address");
    if (!CValidationMethods::ValidatePassword(Password))
        set_Error(TEXTPWD, CValidationMethods::PwdValidationText);
    
    if (get_IsValid())
    {
        SP<IAdminAPI> api;
        AdminOpen(&api);
        auto rcode = api->AuthenticateUser(EmailAddress.c_str(), Password.c_str(), us);
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
}

const string& CLogOn::EmailValidationText() 
{
    return get_Error(TEXTEMAIL);
}

const string& CLogOn::PwdValidationText() 
{
    return get_Error(TEXTPWD);
}

