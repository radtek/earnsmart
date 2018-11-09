/* 
 * File:   seadminapi.cpp
 * Author: santony
 * 
 * Created on March 17, 2013, 1:12 PM
 */

#include "internal.h"
#include "../seglobal/CList.h"
#include "CAdminAPI.h"
#include "CUser.h"
#include "CAudit.h"

namespace se
{
    namespace admin
    {
        extern "C"
        {
            /* Open and obtain a reference to the api gateway */
            ErrorId ADMEXP AdminOpen(ISession* ps, IAdminAPI** ppGateway) 
            {
                CAdminAPI::__loadMessages__(
                {
                    CAdminAPI::CodeMessageItem(E_PaypalError, "Paypal Error."),
                    CAdminAPI::CodeMessageItem(E_UserLoginExists, "Login information already in the system."),
                    CAdminAPI::CodeMessageItem(E_UnknownUser, "Unknown user identifier or login id."),
                    CAdminAPI::CodeMessageItem(S_NewUserAddress, "Call is successful, a new address record is created."),
                    CAdminAPI::CodeMessageItem(E_RegistrationInfoInComplete, "Information provided for registration is incomplete. Please ensure that all required information are provided."),
                    CAdminAPI::CodeMessageItem(E_PartiallyRegistered, "Registration is partially completed. Check the registration stage and complete registration."),
                    CAdminAPI::CodeMessageItem(E_Validation, "Data validation failed."),
                    CAdminAPI::CodeMessageItem(E_ValidateEmail, "String provided as email address is not valid."),
                    CAdminAPI::CodeMessageItem(E_ValidateCountry, "Country provided is not recognized."),
                    CAdminAPI::CodeMessageItem(E_ValidateState, "State/Province provided is not recognized."),
                    CAdminAPI::CodeMessageItem(E_ValidatePassword, "Password is not given, or doesn't meet requirements or password doesn't match password confirmation field."),
                    CAdminAPI::CodeMessageItem(E_ValidateName, "First or last name or both not provided."),
                    CAdminAPI::CodeMessageItem(E_HelpDocHasChildren, "The action cannot take place because this item has children."),
                    CAdminAPI::CodeMessageItem(E_RegistrationEmailFailed, "Registration succeeded, but failed to notify the registered user through an email."),
                    CAdminAPI::CodeMessageItem(E_UserAlreadyLIcensed, "The user is already a licensed subscriber. Upgrade is not required."),
                    CAdminAPI::CodeMessageItem(E_CannotUpgradeUser, "Only registered standard users can be upgraded. Upgrade failed."),
                    CAdminAPI::CodeMessageItem(E_CannotDowngradeUser, "Only registered licensed users can be downgraded to a standard user. This user is not a licensed user."),
                    CAdminAPI::CodeMessageItem(E_UserRegistrationNotFound, "No registration information found in our system. It could be because the previously provided registration information has expired. Please register again."),
                    CAdminAPI::CodeMessageItem(S_RegistrationCompleted, "Registration is successful."),
                    CAdminAPI::CodeMessageItem(S_AnonymousSession, "An anonymous user session is created."),
                    CAdminAPI::CodeMessageItem(S_NewUserAddress, "A new user address is added to the system.")
                });
             
                if (!ps)
                {
                    BERR << "Session not provided";
                    return E_InvalidArg;
                }
                return CObject<CAdminAPI>::Create(ps, ppGateway);

            }
        }        
   }
}