/* 
 * File:   CRegistration.cpp
 * Author: santony
 * 
 * Created on July 19, 2015, 7:17 PM
 */

#include "common.h"
#include "CRegistration.h"
#include "CValidationMethods.h"
#include <sstream>


using namespace std;
using namespace se;
using namespace se::admin;

#define THROWLOGEMAILERROR( retCode, msg, emsg )      \
    if ( FAILED( (retCode) ) )                               \
    {   \
        ostringstream ostr;                                  \
        ostr << emsg << endl;                               \
        NotifyError(ostr.str());                            \
        BERR << msg;    \
        throw se::seexception(retCode, msg);    \
    }   


namespace cppcms
{
    // another specialization for RegStage enum type.
    template<>  
    struct archive_traits<CRegistration::RegStages, void>
    {  
        static void save(CRegistration::RegStages const &obj, archive &tgt)  
        {  
            std::ostringstream ostr;
            ostr << (I32)obj;
            const string& str = ostr.str();
            BOOSTER_DEBUG(__FUNCTION__) << "RegState length: " << str.length();
            // may require padding.
            tgt.write_chunk(str.c_str(), sizeof(I32));
        }  
        
        static void load(CRegistration::RegStages &obj, archive &src)  
        {  
            char data[sizeof(I32)];
            src.read_chunk(&data, sizeof(I32));
            obj = (CRegistration::RegStages)strtol(data, nullptr, 10);
            
        }  
    };

}


CRegistration::CRegistration() 
{
    IAddress::Create(&Address);
    
    
}

void CRegistration::CRegData::serialize(archive& a) 
{
    a & Basic & FirstName & LastName & EmailAddress & Pwd 
            & PwdConfirm & Street1 & Street2 & Town 
            & State & Country & PostalCode
            & Remember & Stage & Guid; 

}

void CRegistration::CRegData::serialize(Json::Value& value)
{
    value["Stage"] = (U32)Stage;
    value["Basic"] = Basic;
    value["FirstName"] = FirstName;
    value["LastName"] = LastName;
    value["EmailAddress"] = EmailAddress;
    value["Pwd"] = Pwd;
    value["PwdConfirm"] = PwdConfirm;
    value["Remember"] = Remember;
    value["Guid"] = Guid;
    value["Street1"] = Street1;
    value["Street2"] = Street2;
    value["Town"] = Town;
    value["State"] = State;
    value["Country"] = Country;
    value["PostalCode"] = PostalCode;
}

void CRegistration::CRegData::deserialize(const Json::Value& value)
{
    Stage = (RegStages)value["Stage"].asInt();
    Basic = value["Basic"].asBool();
    Remember = value["Remember"].asBool();
    FirstName = value["FirstName"].asString();
    LastName = value["LastName"].asString();
    EmailAddress = value["EmailAddress"].asString();
    Pwd = value["Pwd"].asString();
    PwdConfirm = value["PwdConfirm"].asString();
    Guid = value["Guid"].asString();
    Street1 = value["Street1"].asString();
    Street2 = value["Street2"].asString();
    Town = value["Town"].asString();
    State = value["State"].asString();
    Country = value["Country"].asString();
    PostalCode = value["PostalCode"].asString();
}

CRegistration::~CRegistration() 
{
    
}

void CRegistration::SaveAtSendEmail() 
{
    // serialize to json
    Json::Value root;
    Data.serialize(root);
    Json::FastWriter writer;
    CSTR strData = writer.write(root).c_str();
    
    // write registration info into db with guid as key.
    SP<IAdminAPI> papi;
    THROWLOGEMAILERROR( AdminOpen(&papi), "Failed to register. Please retry. Sorry for any inconvenience.", "Failed to open adminapi" );
    BERR << strData;
    THROWLOGEMAILERROR( papi->SaveRegistrationData(Data.Guid.c_str(), strData ) , "Failed to register. Please retry. Sorry for any inconvenience.", "Failed to save registration data" );

    
}

void CRegistration::LoadForEmailConfirmation() 
{
    SP<IAdminAPI> papi;
    THROWLOGEMAILERROR( AdminOpen(&papi), "Failed to register. Please retry. Sorry for any inconvenience.", "Failed to open adminapi" );
    
    // using GUID read registration information from database with guid as key.
    SP<IString> pRegData;
    THROWLOGEMAILERROR( papi->LoadRegistrationData(Data.Guid.c_str(), &pRegData), "Failed to register. Please retry. Sorry for any inconvenience.", "Failed to load registration data" );

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(pRegData->get_Buffer(), root, false))
    {
        Data.deserialize(root);
    }
}

void CRegistration::SaveAtPayment(const string& Guid) 
{

}

void CRegistration::LoadOnPaymentConfirmation(const string& Guid) 
{

}


void CRegistration::serialize(archive& a) 
{
    CMaster::serialize(a);
}

bool CRegistration::Validate() 
{
    // Stage based validation is done here. It is a progressive 
    // validation.
    auto stage = this->Data.Stage;
    // validation is done in stages. So 
    U32 uStage = (U32)stage;
    if (uStage > (U32) RegStages::UserTypeAndAddress)
    {
        if (Data.EmailAddress.empty())
            set_Error("EmailAddress", "Email address is not provided.");
        if (!CValidationMethods::ValidateEmail(Data.EmailAddress))
            set_Error("EmailAddress", "Email address format is not correct.");
        if (Data.FirstName.empty())
            set_Error("FirstName", "First Name is not provided.");
        if (Data.LastName.empty())
            set_Error("LastName", "Last Name is not provided.");
        if (Data.Pwd.empty())
            set_Error("Pwd", "Password is not provided.");
        if (!CValidationMethods::ValidatePassword(Data.Pwd))    
            set_Error("Pwd", CValidationMethods::PwdValidationText);        
        if (Data.PwdConfirm.empty())
            set_Error("PwdConfirm", "Confirmation password is not provided.");
        if (Data.Pwd != Data.PwdConfirm)
            set_Error("PwdConfirm", "Password and its confirmation are not the same.");
        if (not Data.Basic)
        {
            if (Data.Street1.empty())
                set_Error("Street1", "Street address not given.");
            if (Data.Town.empty())
                set_Error("Town", "Town not given.");
            if (Data.Country.empty())
                set_Error("Country", "Country not selected.");
            if (Data.State.empty())
                set_Error("State", "State not selected.");
            if (Data.PostalCode.empty())
                set_Error("PostalCode", "PostalCode not given.");
        }
    }
    return get_IsValid();
}


const string& CRegistration::FirstNameValidationText() 
{
    return get_Error("FirstName");
}

const string& CRegistration::SecondNameValidationText() 
{
    return get_Error("LastName");
}


const string& CRegistration::EmailValidationText() 
{
    return get_Error("EmailAddress");
}

const string& CRegistration::PwdValidationText() 
{
    return get_Error("Pwd");

}

const string& CRegistration::PwdConfirmValidationText() {
    return get_Error("PwdConfirm");

}


const string& CRegistration::Street1ValidationText() {

    return get_Error("Street1");
}

const string& CRegistration::TownValidationText() {

    return get_Error("Town");
}

const string& CRegistration::StateValidationText() {

    return get_Error("State");
}

const string& CRegistration::CountryValidationText() {

    return get_Error("Country");
}

const string& CRegistration::PostalCodeValidationText() {

    return get_Error("PostalCode");
}

