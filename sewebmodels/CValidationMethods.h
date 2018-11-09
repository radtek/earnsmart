/* 
 * File:   CValidationMethods.h
 * Author: santony
 *
 * Created on August 7, 2015, 7:19 PM
 */

#ifndef CVALIDATIONMETHODS_H
#define	CVALIDATIONMETHODS_H

#include <string>

using namespace std;

class CValidationMethods 
{
    static bool Validate(const string& text, const string& pattern);
public:
    static string PwdValidationText;
    
    static bool ValidateEmail(const string& email);
    static bool ValidatePassword(const string& email);
};

#endif	/* CVALIDATIONMETHODS_H */

