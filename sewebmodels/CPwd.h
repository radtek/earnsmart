/* 
 * File:   CPwd.h
 * Author: santony
 *
 * Created on August 12, 2015, 2:30 PM
 */

#ifndef CPWD_H
#define	CPWD_H

#include "CMaster.h"

class CPwd : public CMaster
{
public:
    string EmailAddress;
    string Pwd;
    string NewPwd;
    string NewPwdConfirm;
    
public:
    CPwd();
    CPwd(const CPwd& orig);
    virtual ~CPwd();
    
    const string& PwdValidationText();
    const string& NewPwdValidationText();
    const string& NewPwdConfirmValidationText();
    

    void Check();
    void Login(IUserSession*& us);
    
private:

};

#endif	/* CPWD_H */

