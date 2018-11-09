/* 
 * File:   CRegistration.h
 * Author: santony
 *
 * Created on July 19, 2015, 7:17 PM
 */

#ifndef CREGISTRATION_H
#define	CREGISTRATION_H

#include "CMaster.h"
#include "CModelBase.h"



class CRegistration  : public CMaster 
{
public:
    
    enum class RegStages
    {
        UserTypeAndAddress  = 0x0,
        SendEmail           = 0x1,
        EmailConfirmation   = 0x2,
        Payment             = 0x3,
        Completed           = 0x4
    };
    
    
   
struct CRegData : public serializable
{
    RegStages Stage = RegStages::UserTypeAndAddress;
    bool Basic = true;
    string FirstName;
    string LastName;
    string EmailAddress;
    string Pwd;
    string PwdConfirm;
    bool Remember = false;
    string Guid;

    string  Street1, 
            Street2,
            Town,
            State,
            Country,
            PostalCode;
    

    void serialize(Json::Value& value);
    
    void deserialize(const Json::Value& value);
    
    virtual void serialize(archive& a);

};
    
    SP<IAddress> Address;
    CRegData Data;
    
public:
    
    SP<ICountryList> Countries;
    SP<IStateList> States;
    
    
public:
    CRegistration();
    CRegistration(const CRegistration& orig) = delete;
    virtual ~CRegistration();
    

    void SaveAtSendEmail();
    void LoadForEmailConfirmation();
    void SaveAtPayment(const string& Guid);
    void LoadOnPaymentConfirmation(const string& Guid);
    
    virtual void serialize(archive& a);

    
    bool Validate();
    
    const string& FirstNameValidationText();
    const string& SecondNameValidationText();
    const string& EmailValidationText();
    const string& PwdValidationText();
    const string& PwdConfirmValidationText();
    const string& Street1ValidationText();
    const string& TownValidationText();
    const string& StateValidationText();
    const string& CountryValidationText();
    const string& PostalCodeValidationText();
    
    

};

#endif	/* CREGISTRATION_H */

