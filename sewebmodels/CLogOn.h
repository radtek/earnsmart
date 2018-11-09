/* 
 * File:   CLogOn.h
 * Author: santony
 *
 * Created on July 19, 2015, 7:17 PM
 */

#ifndef CLOGON_H
#define	CLOGON_H

#include "CMaster.h"
#include "CModelBase.h"
#include "../seadmin/seadminapi.h"

using namespace se::admin;

class CLogOn : public CMaster 
{
public:    
    string EmailAddress;
    string Password;
    bool Remember = false;
    
public:
    CLogOn();
    CLogOn(const CLogOn& orig);
    virtual ~CLogOn();
    void Validate(IUserSession*&);
    
    const string& EmailValidationText();
    const string& PwdValidationText();
    
private:

};

#endif	/* CLOGON_H */

