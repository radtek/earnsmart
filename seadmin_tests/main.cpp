/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on August 29, 2013, 7:43 PM
 */

#include <cstdlib>
#include "../semain/semain.h"
#include "../seadmin/seadminapi.h"
#include <stdio.h>
#include <iostream>
#include "../seglobal/CJsonSerializedData.h"

using namespace std;
using namespace se;
using namespace se::config;
using namespace se::admin;

/*
 * 
 */


void TestAudit()
{
    SP<IAdminAPI> api;
    AdminOpen(&api);
    SP<IAudit> pAudit = api->GetAuditor();
    pAudit->Critical("santony", "Test", "This is test audit message.");
    pAudit->Moderate("santony", "Test", "This is test audit message.");
    pAudit->Normal("santony", "Test", "This is test audit message.");
}

void TestCreateType()
{
    SP<IUserType> pUserType;
    ErrorId ret = IUserType::Create(&pUserType);
    SP<IUser> pUser;
    ret = IUser::Create(&pUser);
    SP<IPurchase> pPurchase;
    ret = IPurchase::Create(&pPurchase);
}

void get_UserType()
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    SP<IUserType> pUserType;
    ErrorId ret = adminApi->get_UserType(1,&pUserType);
    
    ret = adminApi->set_UserType(pUserType);
    
}

void get_UserTypes()
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    SP<IROSPList<IUserType> > pList;
    ErrorId ret =adminApi->get_UserTypes(&pList); 
}

void get_User()
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    SP<IUser> pUser;
//    ErrorId ret =adminApi->get_User("admin",&pUser);
//    ErrorId ret =adminApi->get_User(1,&pUser);
    
//    ret =adminApi->set_User(pUser);
}

void get_Users() 
{
    
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    SP<IROSPList<IUser> > ppUsers;  
    ErrorId ret =adminApi->get_Users("FirstName=arun;LastName=arun",&ppUsers);
}

void set_UserStatus() 
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    ErrorId ret =adminApi->set_UserStatus("admin",1); 
    
}

void get_UserSessions() 
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    SP<IUserSession> pUserSession;
    DATETIME from,to;
    from.Year = 2013;
    from.Month =1;
    from.Day = 1;
    from.Hour = 0;
    from.Minute =0;
    from.Second = 0;
    from.Millisecond =0;
    
    to.Year = 2013;
    to.Month =12;
    to.Day = 30;
    to.Hour = 0;
    to.Minute =0;
    to.Second = 0;
    to.Millisecond =0;
    SP<IROSPList<IUserSession> > pList;
    //ErrorId ret =adminApi->get_UserSessions(1,from, to,&pList);
    
}

void AuthenticateUser()
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    SP<IUserSession> ps;
    IUserSession::Create(&ps);
    ErrorId ret =adminApi->AuthenticateUser("arun", "arun", ps._ptr);
    
}

void get_Product()
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    SP<IProduct> pProduct;
    
    ErrorId ret =adminApi->get_Product(1, &pProduct);
    cout<<pProduct->get_PriceMonthlyRate()<<endl;
    
}

void set_Product()
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    SP<IProduct> pProduct;
    ErrorId retCode = pProduct->Create(&pProduct);
    pProduct->set_Name("basic");
    pProduct->set_PriceMonthlyRate(15.32);
    pProduct->set_PriceQuarterlyRate(20.34);
    pProduct->set_PriceYearlyRate(44.55);
    ErrorId ret =adminApi->set_Product( pProduct);
}
void get_Products()
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    
    SP<IROSPList<IProduct> > pProduct;  
    ErrorId ret =adminApi->get_Products(&pProduct) ;
    
}
void get_customer()
{
    SP<IAdminAPI>adminApi;
    AdminOpen(&adminApi);
    SP<IUser>ppur;
    
    ErrorId ret =adminApi->get_User(1,&ppur);
    
    ret =adminApi->set_User(ppur) ;
    
}

#include "../senet/CSmtpClient.h"

using namespace se::net;

void TestSmtp()
{
    CSmtpClient smtp;
    smtp.SendMail("sajiantony@hotmail.com", "admin@smart-earn.com", "Test : Smartearn", "This is a development test message. Please ignore.");
    return;
}

void TestThreadpool()
{
    SP<CMailSenderTask> sender;
    CMailSenderTask::Create(&sender);
    sender->set_From("admin@smart-earn.com");
    sender->set_ReceiptTo("error@smart-earn.com");
    sender->set_Subject("Smart-earn site Error");
    sender->set_Message("This is a test message.");

    SP<IApp> papp;
    se::SEOpenApp(&papp);
    SP<se::threading::IThreadPool> ppool;
    papp->get_ThreadPool(se::threading::TPT_SynchronousPool, &ppool);
    ppool->RunTask(sender);
    
}

void TestGetCountry()
{
    SP<IAdminAPI> papi;
    AdminOpen(&papi);
    /*
    SP<ICountryList> pcountries;
    if (SUCCEEDED(papi->get_Countries(&pcountries, 
            [](ICountry::QUERYFLDS flds, LGX& lgx, BOOL& include)
            {
                if (flds == ICountry::QUERYFLDS::IsPaypalavailable)
                {
                    include = true;
                    return "1";
                }
                return "";
            })))
    {
        SP<ISerializedData<>> ser;
        CJsonSerializedData::Create(&ser, true);
        pcountries->Serialize(ser);
        SP<IString> output;
        ser->get_Output(&output);
        cout << output->get_Buffer() << endl;
    }
     * 
     */

    SP<ICountry> pc;
    if (SUCCEEDED(papi->get_Country
            (&pc, 
            [](ICountry::QUERYFLDS flds, LGX& lgx, BOOL& include)
                {
                    if (flds == ICountry::QUERYFLDS::TwoChar)
                    {
                        include = true;
                        return "IN";
                    }
                    return "";
                }
            )))
    {
        SP<ISerializedData<>> ser;
        CJsonSerializedData::Create(&ser, true);
        pc->Serialize(ser);
        SP<IString> output;
        ser->get_Output(&output);
        cout << output->get_Buffer() << endl;
    }            
}

void TestStates()
{
    SP<IAdminAPI> papi;
    AdminOpen(&papi);
    SP<IStateList> pstates;
    if (SUCCEEDED(papi->get_States(&pstates, [](IState::QUERYFLDS fld, LGX& lgx, BOOL& b)
    {
       if (fld == IState::QUERYFLDS::CountryId)
       {
           b = true;
           return "1";
       }
       return "";
    })))
    {
        SP<ISerializedData<>> ser;
        CJsonSerializedData::Create(&ser, true);
        pstates->Serialize(ser);
        SP<IString> output;
        ser->get_Output(&output);
        cout << output->get_Buffer() << endl;

    }
}

void TestUserRegistration()
{
    // create a user
    SP<IUser> pUser;
    SP<IAdminAPI> pApi;
    if (SUCCEEDED(AdminOpen(&pApi)))
    {
        //pApi->UnregisterUser(7);
        if (SUCCEEDED(IUser::Create(&pUser)))
        {
            SP<IAddress> pAddr;
            if (SUCCEEDED(CreateAddress(&pAddr)))
            {
                pAddr->set_City("Fair Lawn");
                pAddr->set_Street(("1-34 17th Street"));
                pAddr->set_State(("NJ"));
                pAddr->set_Country(("US"));
                pAddr->set_PostalCode("07410");
                pUser->set_Address(pAddr);
            }
            else
                return;
            SP<IUserType> pUType;
            if (SUCCEEDED(pApi->get_UserType(2, &pUType)))
            {
                pUser->add_UserType(pUType);
            }
            else
                return;
            pUser->set_UserId("sajiantony@live.com");
            pUser->set_FirstName("Saji");
            pUser->set_LastName(("Antony"));
            
            SP<IUserSession> pSess;
            IUserSession::Create(&pSess);
            pSess->set_SessionId("xx11xx");
            ErrorId retCode = pApi->RegisterUser(pUser, "dT963051", "yy11yy", pSess._ptr);
            if (FAILED(retCode))
            {
                cout << "Failed.";
            }
            
        }
    }
}

void TestUserUnregistration()
{
    SP<IAdminAPI> pApi;
    if (SUCCEEDED(AdminOpen(&pApi)))
    {
        pApi->UnregisterUser(1);
    }    
}

void TestAuthentication()
{
    SP<IAdminAPI> pApi;
    if (SUCCEEDED(AdminOpen(&pApi)))
    {
        // verify user
        SP<IUserSession> ps;
        IUserSession::Create(&ps);
        SE_CODE c = pApi->AuthenticateUser("sajiantony@live.com", "dT963051", ps._ptr);
        cout << c << endl;
    }   
}


int main(int argc, char** argv) 
{
    
    SP<se::IApp> pApp;
    string progname = argv[0];
    SEGetApp(progname.c_str(), &pApp);
    
    SP<se::config::IConfigurator> pConfig;
    pApp->get_Configurator(&pConfig);
    
    
    
    //TestUserUnregistration();
    //TestUserRegistration();
    TestAuthentication();

    //TestStates();
    
    //TestGetCountry();
    
    //TestThreadpool();
    //TestSmtp();
//    get_customer();
//    get_Users(); 
//    AuthenticateUser();
//    get_UserSessions() ;
//    set_UserStatus(); 
//    get_User();
//    get_UserTypes();
//    get_UserType();  
//    TestCreateType();
//    
//    TestAudit();
   

    return 0;
}

