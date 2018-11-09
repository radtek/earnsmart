/* 
 * File:   UserManageModels.cpp
 * Author: santony
 * 
 * Created on December 20, 2013, 6:12 PM
 */

#include "models.h"
#include "commontypes.h"
#include <fstream>
#include <sstream>
#include <boost/type_traits/is_pointer.hpp>
#include "../seglobal/CDirectory.h"

using namespace std;

namespace sg
{
    namespace web
    {
        namespace models
        {
            
            //Register
            
            Register::Register(cppcms::http::context& ctxt): BaseContent(ctxt, "RegisterView")
            {
                
            }
            
            void Register::Init()
            {
                
            }
            
            void Register::Init(IUser* pUser)
            {
                getContext().session().set("UserPointer", (POINTER) pUser);
            }
            
            IUser* Register::get_User()
            {
                if (getContext().session().is_set("UserPointer"))
                {
                    // try accessing id property
                    IUser* pUser = static_cast<IUser*>(getContext().session().get<POINTER>("UserPointer"));
                    return pUser;
                }
                return nullptr;
            }
            
            IAddress* Register::get_UserAddress()
            {
                pAddr.Clear();
                IUser* pUser = get_User();
                if (pUser)
                    pUser->get_Address(&pAddr);
                if (!pAddr)
                    CreateAddress(&pAddr);
                return pAddr._ptr;
            }
            
            IPaypalExpressCheckout* Register::get_PaypalInstance()
            {
                if (getContext().session().is_set("PaypalInstance"))
                    return static_cast<IPaypalExpressCheckout*>(getContext().session().get<POINTER>("PaypalInstance"));
                return NULL;
                
            }

            void Register::set_PaypalInstance(IPaypalExpressCheckout *p) 
            {
                getContext().session().set("PaypalInstance", (POINTER) p);
            }


            
            void Register::ClearRegSessionData()
            {
                session_interface& s = getContext().session();
                s.erase("UserPointer");
                s.erase("PaypalInstance");
                s.erase("PayPalToken");
                s.erase("PayPalPayerId");
                s.erase("PayPalPayerProfileId");
                s.erase("Login");
                s.erase("Password");
                s.erase("HasAgreed");
            }
            
            const string Register::get_PayPalToken()
            {
                session_interface& s = getContext().session();
                return s.get("PayPalToken");
            }
            
            const string Register::get_PayPalPayerId()
            {
                session_interface& s = getContext().session();
                return s.get("PayPalPayerId");
            }
            
            const string Register::get_Agreement()
            {
                // read file in blob directory
                SP<IString> str;
                CDirectory::GetExePath(&str);
                string filepath = (string)str->get_Buffer() + "blob/agreement.txt";
                stringstream sstr;
                ifstream strm;
                strm.open(filepath);
                if (strm.is_open())
                {
                    sstr << strm.rdbuf();
                    strm.close();
                }                
                return sstr.str();
            }

            const string Register::get_Login()
            {
                session_interface& s = getContext().session();
                return s.get("Login");
            }
            const string Register::get_Password()
            {
                session_interface& s = getContext().session();
                return s.get("Password");
            }
            
            const string Register::get_PayPalPayerProfileId()
            {
                session_interface& s = getContext().session();
                return s.get("PayPalPayerProfileId");
            }
            
            bool Register::get_HasAgreed() 
            {
                session_interface& s = getContext().session();
                return s.get<bool>("HasAgreed");
            }
            
            const string Register::get_PaypalProfileStatus() 
            {
                session_interface& s = getContext().session();
                return s.get("PaypalProfileStatus");
            }


            void Register::set_PayPalPayerId(const string& v)
            {
                session_interface& s = getContext().session();
                s.set("PayPalPayerId", v);
            }
            
            void Register::set_PayPalPayerProfileId(const string& v)
            {
                session_interface& s = getContext().session();
                s.set("PayPalPayerProfileId", v);
            }
            
            void Register::set_PayPalToken(const string& v)
            {
                session_interface& s = getContext().session();
                s.set("PayPalToken", v);
            }

            void Register::set_Login(const string& v)
            {
                session_interface& s = getContext().session();
                s.set("Login", v);
            }

            void Register::set_Password(const string& v)
            {
                session_interface& s = getContext().session();
                s.set("Password", v);
            }
            
            
            void Register::set_HasAgreed(bool b) 
            {
                session_interface& s = getContext().session();
                s.set("HasAgreed", b);
            }

            void Register::set_PaypalProfileStatus(const string& v) 
            {
                session_interface& s = getContext().session();
                s.set("PaypalProfileStatus", v);
            }
            
            
            //Signin
            SignIn::SignIn(cppcms::http::context& ctxt): BaseContent(ctxt, "SignInView"), DisplayError("none")
            {
                
            }
            
            void SignIn::Init()
            {
                
            }

            //SignOut
            SignOut::SignOut(cppcms::http::context& ctxt): BaseContent(ctxt, "SignOutView")
            {
                
            }
            
            void SignOut::Init()
            {
                
            }
            
            
        }
    }
}
