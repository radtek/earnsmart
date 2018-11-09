/* 
 * File:   CAppMain.cpp
 * Author: santony
 * 
 * Created on January 24, 2015, 1:00 PM
 */

#include "CAppMain.h"
#include "../sewebmodels/CHome.h"
#include "../sewebmodels/CLogOn.h"
#include "../sewebmodels/CRegistration.h"
#include "../sewebmodels/CResetPwd.h"
#include "../sewebmodels/CSessionManageMethods.h"
#include "../sewebmodels/CAccount.h"
#include "../sewebmodels/CPwd.h"
#include <uuid/uuid.h>
#include "internal.h"

#define POST "POST"
#define GET "GET"

#define ISPOST request().request_method()==POST
#define ISGET request().request_method()==GET
#define POSTED(name) request().post(name)
#define GOT(name) request().get(name)

extern string _currentSkin;
extern string _emailtempls;


CAppMain::CAppMain(cppcms::service& srv) : CAppBase(srv)
{
    
    
    // Sign in page.
    dispatcher().assign("/signon$", &CAppMain::Signon, this);
    mapper().assign("signon", "/signon");

    // Sign out redirect page.
    dispatcher().assign("/logout$", [this]()
    {
        if (bs.get_UserId(session()))
        {
            bs.Clear(context());
            // reload session.
            cache().rise(session().get_session_cookie());
            session().clear();
            session().reset_session();
            response().set_redirect_header("/");
        }
    });
    mapper().assign("logout", "/logout");
    
    // registration page.
    //04e7c1e5-25b0-4854-b033-14b0a59a9ba0
    dispatcher().assign("/register(/[a-f0-9]{8}(?:-[a-f0-9]{4}){3}-[a-f0-9]{12})?$", &CAppMain::RegisterStep1_BasicOrLicensed, this, 1);
    mapper().assign("register", "/register{1}");

    dispatcher().assign("/register/1/([a-f0-9]{8}(?:-[a-f0-9]{4}){3}-[a-f0-9]{12})$", &CAppMain::RegisterStep3_PayCompleted, this, 1);
    mapper().assign("register1", "/register/1/{1}");

    dispatcher().assign("/register/2/([a-f0-9]{8}(?:-[a-f0-9]{4}){3}-[a-f0-9]{12})$", &CAppMain::RegisterStep3_PayCompleted, this, 1);
    mapper().assign("register2", "/register/2/{1}");

    // reset pwd page.
    dispatcher().assign("/resetpwd$", [this]()
    {
        CResetPwd reset;
        if (ISPOST)
        {
            reset.app(*this);
            reset.EmailAddress = request().post("signEmail");
            reset.Check();
        }
        
        RENDER:
        render(_currentSkin, "ResetPwdView", reset);
    });
    mapper().assign("resetpwd", "/resetpwd");
    
    //change password page.
    dispatcher().assign("/pwd[/]?([a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?)$", &CAppMain::Pwd, this, 1);
    mapper().assign("pwd", "/pwd/{1}");
    // accounts page
    dispatcher().assign("/account$", &CAppMain::Account, this);
    mapper().assign("account", "/account");
    
    // default page.
    dispatcher().assign("", [this]()
    {
        CHome homePage;
        homePage.Header.LoginOrRegister.UserSession = bs.get_WebSession(session());
        bs.set_LastUrl(session(), "/");
        render(_currentSkin, "HomeView", homePage);
    });
    mapper().assign("");
    mapper().root("/");

    
    // Initialize the master object with
    // header and footer
   
}

void CAppMain::RegisterStep1_BasicOrLicensed(string guid) 
{
    CRegistration regist;
    regist.app(*this);
    try
    {
        if (!guid.empty())
        {
            if (session().is_set("RegistrationData"))
                session().fetch_data("RegistrationData", regist.Data);
            regist.Data.Guid = guid.substr(1);
            regist.LoadForEmailConfirmation();
            regist.Data.Stage = CRegistration::RegStages::UserTypeAndAddress;
        }

        if (ISPOST)
        {
            if (session().is_set("RegistrationData"))
                session().fetch_data("RegistrationData", regist.Data);
            /*
             * Post backs to handle are :
             * a. Basic or Licensed                             done
             * b. Changing country dropdown, populate states    
             * c.     
             */
            regist.Data.Stage = (CRegistration::RegStages)(strtol(POSTED("regStage").c_str(), nullptr, 10));
            regist.Data.Basic = POSTED("regUsertype")=="Basic";
            regist.Data.EmailAddress = POSTED("regEmail");
            regist.Data.Pwd = POSTED("regPwd");
            regist.Data.PwdConfirm = POSTED("regPwdConfirm");
            regist.Data.Remember = POSTED("regRem") != "";
            regist.Data.FirstName = POSTED("regFirstName");
            regist.Data.LastName = POSTED("regLastName");
            regist.Data.Remember = !(POSTED("regRem").empty());
            if (!regist.Data.Basic)
            {
                regist.Data.State = POSTED("regStates");
                regist.Data.Country = POSTED("regCountries");
                regist.Data.PostalCode = POSTED("regPostalCode");
                regist.Data.Street1 = POSTED("regStreet1");
                regist.Data.Street2 = POSTED("regStreet2");
                regist.Data.Town = POSTED("regTown");

                Json::Value geoinfo = Json::nullValue;
                if (PopulateGeoIpInfo(geoinfo))
                {
                    if (!geoinfo.isMember("bogon"))
                    {
                        regist.Data.State = geoinfo["region"].asString();
                        regist.Data.Country = geoinfo["country"].asString();
                        //regist.Data.PostalCode = geoinfo["postal"].asString();
                        //regist.Data.Town = geoinfo["city"].asString();
                    }
                }
                if (regist.Data.Country.empty())
                    regist.Data.Country = "US";

                regist.Countries = this->get_CachedCountries();
                get_States(regist.Data.Country, &regist.States);

            }

            if (regist.Data.Stage == CRegistration::RegStages::UserTypeAndAddress)
            {
                // nothing to do
            }
            else if (regist.Data.Stage == CRegistration::RegStages::SendEmail)
            {
                if (regist.Validate())
                {
                    // generate uuid
                    uuid_t guid;
                    uuid_generate(guid);
                    char sguid[36];
                    uuid_unparse_lower(guid, sguid);
                    // save this 
                    regist.Data.Guid = sguid;

                    // now upgrade the stage
                    regist.Data.Stage = CRegistration::RegStages::SendEmail;

                    regist.SaveAtSendEmail();

                    stringstream strm;
                    render(_emailtempls, "RegistrationEmailConfirmationView", strm, regist );
                    BDBG << strm.str();
                    regist.Notify(regist.Data.EmailAddress, "Verification of Email Address", strm.str());

                }
            }
        }
        
        

        RENDER:
            session().store_data("RegistrationData", regist.Data);
            render(_currentSkin, "RegistrationView", regist);
                    
    }
    catch (const std::exception)
    {
        CError e("We are sorry. An exception thrown while processing the page. A notification is sent to the administrator. Please retry.");
        render("ErrorView", e);
    }                
}

void CAppMain::RegisterStep2_EmailConfirm(string guid) 
{
    CRegistration regist;
    if (session().is_set("RegistrationData"))
        session().fetch_data("RegistrationData", regist.Data);

    if (!(ISPOST))
    {
        // load data from database
        regist.Data.Guid = guid;
        regist.LoadForEmailConfirmation();
    }
    else 
    {
        if (regist.Data.Stage == CRegistration::RegStages::EmailConfirmation)
        {
            regist.Data.Basic = POSTED("regUsertype")=="Basic";
            if (regist.Validate())
            {
                if (!regist.Data.Basic)
                {
                    regist.Data.Stage = CRegistration::RegStages::Payment;
                }
            }
        }
        else if (!regist.Data.Basic and regist.Data.Stage == CRegistration::RegStages::Payment)
        {
            if (regist.Validate())
            {
                // redirect to paypay
            }
        }
    }
    
    
    RENDER:
        session().store_data("RegistrationData", regist.Data);
        render(_currentSkin, "RegistrationView", regist);
                
}

void CAppMain::RegisterStep3_PayCompleted(string guid) 
{
    CRegistration regist;
    if (session().is_set("RegistrationData"))
        session().fetch_data("RegistrationData", regist.Data);

    if (!(ISPOST))
    {
        // load data from database
        regist.Data.Guid = guid;
        regist.LoadForEmailConfirmation();
    }
    else if (regist.Data.Stage == CRegistration::RegStages::Completed)
    {

    }
    RENDER:
        session().store_data("RegistrationData", regist.Data);
        render(_currentSkin, "RegistrationView", regist);
}



void CAppMain::Pwd(string email) 
{
    CPwd pwd;
    // email address is received in session or get method
    if (email.empty())
        email = (CSTR)bs.get_Value(session(), "PWD_EMAIL");

    if (request().request_method() == POST)
    {
        pwd.EmailAddress = email;
        pwd.Pwd = request().post("signPwd");
        pwd.NewPwd = request().post("signNewPwd");
        pwd.NewPwdConfirm = request().post("signNewPwdConfirm");

        pwd.Check();
        if (!pwd.get_IsValid())
            goto RENDER;

        // redirect to home after authentication.
        SP<IUserSession> pUserSession;
        IUserSession::Create(&pUserSession);
        CSessionManageMethods::BuildUserSession(context(), pUserSession._ptr);
        pUserSession->set_SessionId(session().get_session_cookie().c_str());
        pwd.Login(pUserSession._ptr);
        if (pUserSession->get_UserId())
        {
            bs.Clear(context());
            bs.set_UserId(context(), pUserSession->get_UserId());
            RedirectToHome();
        }
    }

    // email address is received in session or get method
    if (email.empty())
        email = (CSTR)bs.get_Value(session(), "PWD_EMAIL");
    if (email.empty())
    {
        goto RENDER;
    }

    pwd.EmailAddress = email;

RENDER:
    bs.set_LastUrl(session(), request().path_translated());
    render(_currentSkin, "PwdView", pwd);

}

void CAppMain::Signon()
{
    if (bs.get_IsAuthorized(session()))
    {
        RedirectToHome();
        return;
    }

    CLogOn logon;
    logon.app(*this);
    if (request().request_method() == POST)
    {
        // validate fields
        logon.EmailAddress = request().post("signEmail");
        logon.Password = request().post("signPwd");
        logon.Remember = "" != request().post("signRem");
        SP<IUserSession> pUserSession;
        IUserSession::Create(&pUserSession);
        CSessionManageMethods::BuildUserSession(context(), pUserSession._ptr);
        pUserSession->set_SessionId(session().get_session_cookie().c_str());
        pUserSession->set_Remember(logon.Remember);
        logon.Validate(pUserSession._ptr);
        bool b = logon.get_IsValid();
        if (b)
        {
            if (pUserSession->get_UserId())
            {
                bs.Clear(context());
                bs.set_UserId(context(), pUserSession->get_UserId());
                //RedirectToHome();
                RedirectToLastPage();
            }
        }        
    }

RENDER:
    render(_currentSkin, "SignOnView", logon);
    return;
}

void CAppMain::Account()     
{
    CAccount account;
    bs.set_LastUrl(session(), "/account");
    this->render(_currentSkin, "AccountView", account);
}


CAppMain::~CAppMain() 
{
}

void CAppMain::init() 
{
    if (!bs.get_IsInit(session()))
        bs.Initialize(context());

}


void CAppMain::main(std::string url) 
{
    
    BOOSTER_INFO(__FUNCTION__) << "Session Id: " << session().get_session_cookie();
    
    if (!dispatcher().dispatch(url))
    {
        response().status(http::response::not_found);
        CError e("There is no corresponding page for the URL.");
        render(_currentSkin, "ErrorView", e);
    }

    if (bs.get_HasExpired(session()))
    {
        SP<IWebSession> pws = bs.get_WebSession(session());
        if (pws && pws->get_UserId())
        {
            bs.Clear(context());
            // TODO: Redirect to session expired page.
            response().set_redirect_header("/signon");
        }
    }
}
