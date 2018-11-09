/* 
 * File:   CResetPwd.h
 * Author: santony
 *
 * Created on July 30, 2015, 11:39 AM
 */

#ifndef CRESETPWD_H
#define	CRESETPWD_H

#include "CMaster.h"
#include "CModelBase.h"


class CResetPwd  : public CMaster 
{
public:
    std::string EmailAddress;
public:
    CResetPwd();
    CResetPwd(const CResetPwd& orig);
    virtual ~CResetPwd();
    
    const string& EmailValidationText();
    
    bool Check();
    
private:
    bool _checked = false;
};

#endif	/* CRESETPWD_H */

