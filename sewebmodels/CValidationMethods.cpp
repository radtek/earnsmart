/* 
 * File:   CValidationMethods.cpp
 * Author: santony
 * 
 * Created on August 7, 2015, 7:19 PM
 */

#include "CValidationMethods.h"
#include <regex>

const string emailPattern = "[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?";
const string pwdPattern = "[\\w\\W\\d]{8,25}";

string CValidationMethods::PwdValidationText = "Password must be construed from 8 to 25 characters.";

bool CValidationMethods::Validate(const string& text, const string& pattern) 
{
    std::regex rg(pattern);
#ifdef __DEBUG__
    bool b = std::regex_match(text, rg);
    return b;
#else
    return std::regex_match(text, rg);
#endif

}


bool CValidationMethods::ValidateEmail(const string& email) 
{
    return Validate(email, emailPattern);
}

bool CValidationMethods::ValidatePassword(const string& pwd) 
{
    return Validate(pwd, pwdPattern);
}
