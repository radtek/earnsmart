/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CAdminAPITest.cpp
 * Author: santony
 * 
 * Created on January 1, 2016, 5:24 PM
 */

#include "CAdminAPITest.h"
#include "../../seglobal/CDirectory.h"
#include "../../seglobal/CStringUtility.h"
#include "CSqlAstTest.h"
#include "../../seglobal/CAst.h"
#include "../../sedata/QueryFields.h"

const string TestUser = "admin";
const string TestPass = "963051";


CAdminAPITest::CAdminAPITest() { }

CAdminAPITest::CAdminAPITest(const CAdminAPITest& orig) { }

CAdminAPITest::~CAdminAPITest() { }

void CAdminAPITest::testInternalUserRegistration() 
{
    const string userId = "jantony";
    const string pwd = "dT963051";
    // only administrator can add users.
}


void CAdminAPITest::testAnonymousRegistration() 
{
    this->NonInteractiveAnonLogin();
    CPPUNIT_ASSERT(pSession != nullptr);
    CPPUNIT_ASSERT(pSession->get_Id() != 0);
}

void CAdminAPITest::testStandardUserRegistration()
{
    this->NonInteractiveAnonLogin();
    RP<IAdminAPI> pAdmin = pSession->get_AdminApi();
    // test one without address given
    const string data1 = R"({
        "ConfirmPassword" : "aaaa",
        "Country" : 1,
        "EmailAddress" : "earnsmart@smartgazellc.com",
        "FirstName" : "Saji",
        "LastName" : "Antony",
        "Password" : "aaaa",
        "RegStages" : 0,
        "RegType" : "standard",
        "RememberMe" : false
    })";
    
    const string data2 = R"({
        "ConfirmPassword" : "aaaa",
        "Country" : 1,
        "EmailAddress" : "saji.antony@smartgazellc.com",
        "FirstName" : "Saji",
        "LastName" : "Antony",
        "Password" : "aaaa",
        "PostalCode" : "07410",
        "RegStages" : 0,
        "RegType" : "licensed",
        "RememberMe" : false,
        "State" : 30,
        "StateProvince" : "New Jersey",
        "Street1" : "1-34 17th Street",
        "Street2" : "",
        "Town" : "Fair Lawn"
    })";

    Json::Value regInfo0;
    Json::Reader rdr;
    
    bool bRet;
    bool standusertest = false;
    SE_CODE retCode;
    if (standusertest)
    {    
        bRet = rdr.parse(data1, regInfo0, false);
        CPPUNIT_ASSERT(bRet);
        retCode = pAdmin->RegisterUser(regInfo0);
        if (FAILED(retCode) && retCode != E_PartiallyRegistered)
        {
            CPPUNIT_ASSERT_MESSAGE(pAdmin->get_CodeMessage(retCode), ISOK(retCode));
        }
    }
    else
    {
        bRet = rdr.parse(data2, regInfo0, false);
        CPPUNIT_ASSERT(bRet);

        retCode = pAdmin->RegisterUser(regInfo0);
        if (FAILED(retCode) && retCode != E_PartiallyRegistered)
        {
            CPPUNIT_ASSERT_MESSAGE(pAdmin->get_CodeMessage(retCode), ISOK(retCode));
        }
    }
    
    // capture the session id returned and continue with email confirmation.
    CSTR sessionId = pSession->get_UniqueId();
    Json::Value confirmEmail;
    confirmEmail["RegStages"] = REGSTAGES::EmailConfirmed;
    confirmEmail["esid"] = sessionId;
    retCode = pAdmin->RegisterUser(confirmEmail);
    CPPUNIT_ASSERT_MESSAGE(pAdmin->get_CodeMessage(retCode), SUCCEEDED(retCode));
}


void CAdminAPITest::testLogin()
{
    this->NonInteractiveUserLogin("admin", "963051");
    RP<IUser> pUser = pSession->get_User();
    CPPUNIT_ASSERT(pUser);
    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE("User Ids matched.", !strcmp(pUser->get_UserId(), TestUser.c_str()));
    BINF << "Anonymous User: " << (I32) UserRole::Anonymous;
    BINF << "Standard User: " << (I32) UserRole::Standard;
    BINF << "Licensed User: " << (I32) UserRole::Licensed;
    BINF << "Internal User: " << (I32) UserRole::Internal;
    BINF << "Admin User: " << (I32) UserRole::Admin;
    
    if (pUser->get_IsInRole(se::UserRole::Admin))
    {
        BINF << "User is administrator.";
    }
    if (pUser->get_IsInRole(se::UserRole::Internal))
    {
        BINF << "User is internal user as well.";
    }
    
}

void CAdminAPITest::testLicensedUserRegistrationCycle() 
{

    this->NonInteractiveAnonLogin();
    RP<IAdminAPI> pAdmin = pSession->get_AdminApi();
    
    // generate a temporary email address
    string emailAddress = se::utils::CStringUtility::GenTempString() + "@earn-smart.com";
    
    const string data = R"({
        "ConfirmPassword" : "aaaa",
        "Country" : 1,
        "EmailAddress" : "jsantony@gmail.com",
        "FirsName" : "",
        "FirstName" : "Saji",
        "LastName" : "Antony",
        "Password" : "aaaa",
        "PostalCode" : "07410",
        "RegStages" : 0,
        "RegType" : "licensed",
        "RememberMe" : false,
        "State" : 30,
        "StateProvince" : "New Jersey",
        "Street1" : "1-34 17th Street",
        "Street2" : "",
        "Town" : "Fair Lawn"
    })";

    //UserTypeAndAddress = 0,
    //SendEmailForConfirmation = 1,
    Json::Value regInfo0;
    Json::Reader rdr;
    bool bRet = rdr.parse(data, regInfo0, false);
    CPPUNIT_ASSERT(bRet);

    regInfo0["EmailAddress"] = emailAddress;
    
    //1. first phase which is saving registration info. (User info and Email)
    SE_CODE retCode = pAdmin->RegisterUser(regInfo0);
    CPPUNIT_ASSERT_MESSAGE(pAdmin->get_CodeMessage(retCode), ISOK(retCode));
    
    //EmailConfirmed = 2,
    // email confirmation
    // NOTE: in the web application the session ids are compared.
    // Here we are not doing it because session is already opened at the first line.
    Json::Value regInfo1 ;
    regInfo1["RegStages"] = (int)REGSTAGES::EmailConfirmed;
    regInfo1["esid"] = pSession->get_UniqueId();
    retCode = pAdmin->RegisterUser(regInfo1);
    CPPUNIT_ASSERT_MESSAGE(pAdmin->get_CodeMessage(retCode), ISOK(retCode));
    
    
    //PaymentProvided = 3,
    Json::Value regInfo2 ;
    regInfo2["RegStages"] = (int)REGSTAGES::PaymentProvided;
    regInfo2["esid"] = pSession->get_UniqueId();
    regInfo2["ppprofileid"] = "9000901222323243";
    retCode = pAdmin->RegisterUser(regInfo2);
    CPPUNIT_ASSERT_MESSAGE(pAdmin->get_CodeMessage(retCode), ISOK(retCode));
    
    // mainly we will store paypayprofile id.
    //RegistrationCompleted = 4
    // finalized. save status as completed.
    Json::Value regInfo3 ;
    regInfo3["RegStages"] = (int)REGSTAGES::RegistrationCompleted;
    regInfo3["esid"] = pSession->get_UniqueId();
    retCode = pAdmin->RegisterUser(regInfo3);
    CPPUNIT_ASSERT_MESSAGE(pAdmin->get_CodeMessage(retCode), ISOK(retCode));
}

void CAdminAPITest::testQueryStates() 
{
    this->NonInteractiveAnonLogin();
    RP<IAdminAPI> pAdmin = pSession->get_AdminApi();
    Json::Value states;
    CSqlAst ast = AST_CONDITION(QUERYFIELDS::Country::Id, AST_OPERATOR::Equal, 1);
    SE_CODE ret = pAdmin->get_States(states, &ast);
    cout << states;
}

#include "../../seadmin/seadminapi.h"

void CAdminAPITest::testPositions() 
{
    this->NonInteractiveUserLogin("admin", "963051");
    RP<IAdminAPI> pAdmin = pSession->get_AdminApi();
    SP<IPositions> pPositions ;
    SE_CODE ret = pAdmin->get_Positions(pSession->get_UserId(), &pPositions);
    CPPUNIT_ASSERT(ret == S_OK);
    cout << pPositions->get_Root();
    while(true)
    {
        sleep(1);
    }
    
}
