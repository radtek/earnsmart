/* 
 * File:   CResetPwd.cpp
 * Author: santony
 * 
 * Created on July 30, 2015, 11:39 AM
 */

#include "CResetPwd.h"
#include "../seadmin/seadminapi.h"
#include "CValidationMethods.h"

using namespace se;
using namespace se::admin;

CResetPwd::CResetPwd() {
}

CResetPwd::CResetPwd(const CResetPwd& orig) {
}

CResetPwd::~CResetPwd() {
}

const string& CResetPwd::EmailValidationText() 
{
    const string& s = get_Error("EmailAddress");
    return s;
}

//TODO: The auto generated password randomization is not perfect. Occasionally same
// password (previously emailed) is generated and emailed.
bool CResetPwd::Check() 
{
    if (_checked)
    
    if (!CValidationMethods::ValidateEmail(EmailAddress))
    {
        set_Error("EmailAddress", "Requires a valid email address");
        return false;
    }
    SP<IAdminAPI> papi;
    AdminOpen(&papi);
    SP<IString> resetPwd;
    auto rcode = papi->ResetGetTemporaryPassword(EmailAddress.c_str(), &resetPwd);
    if (rcode == E_NotFound)
    {
        set_CommonError("This email address is not registered with us. A previously registered email address is required.");
        return false;
    }
    else if (FAILED(rcode))
    {
        string str = "Sorry, resetting password failed due to an unknown error. The site admin has been notified. Please retry.";
        set_CommonError(str);
        NotifyError(str);
        return false;
    }
    // sent email
    ostringstream ostr;
    ostr << "A temporary password " ;
    ostr << resetPwd->get_Buffer() ;
    ostr << " is created for you."; 
    ostr << " Please <a href='http://" ;
    ostr << app().request().http_host() << "/pwd/";
    ostr << EmailAddress;
    ostr << "'>click</a> to change password.";
    
    Notify(EmailAddress, "Password Reset", ostr.str());
    
    set_CommonError("Password reset instructions are emailed to you. Click the button above to reset password again.");
    
    return true;
}

