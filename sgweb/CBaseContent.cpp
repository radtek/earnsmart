/* 
 * File:   CBaseContent.cpp
 * Author: santony
 * 
 * Created on September 18, 2013, 9:19 PM
 */

#include "CBaseContent.h"
#include "../senet/senetapi.h"
#include "../senet/CSmtpClient.h"
#include <sstream>
#include "../semain/semain.h"
#include "models.h"
using namespace std;
namespace sg
{
    namespace web
    {
        namespace models
        {
            string BaseContent::_adminNotify = "";
            string BaseContent::_errorNotify = "";
            
            BaseContent::BaseContent() : httpctxt(nullptr)
            {
                hasError = false;
            }
            
            BaseContent::BaseContent(const BaseContent& cp)
            {
                _error = cp._error;
                _pUserSession = cp._pUserSession;
                this->hasError = cp.hasError;
                this->httpctxt = cp.httpctxt;
                viewName = cp.viewName;
            }
            
            BaseContent::BaseContent(cppcms::http::context& httpctxt)
            {
                this->httpctxt = &httpctxt;
                hasError = false;
            }
            
            BaseContent::BaseContent(cppcms::http::context& httpctxt, string const& viewName) 
                : viewName(viewName), httpctxt(&httpctxt)
            {
                hasError = false;
            }

            void BaseContent::Init() 
            {
            }

            void BaseContent::Init(cppcms::http::context& httpctxt) 
            {
                this->httpctxt = &httpctxt;
            }

            void BaseContent::Init(cppcms::http::context& httpctxt, const string& viewName) 
            {
                this->httpctxt = &httpctxt;
                this->viewName = viewName;
            }
            
            cppcms::http::context& BaseContent::getContext()
            {
                return *httpctxt;
            }

            void BaseContent::ModifyView(const string& newViewName) 
            {
                viewName = newViewName;
            }
            
            
            void BaseContent::set_Error(const string& error, bool email)
            {
                _error = error;
                
                if (email)
                {
                    if (_adminNotify.empty())
                    {
                        try
                        {
                            SP<IApp> papp;
                            SEOpenApp(&papp);
                            const Json::Value& root = papp->get_Configurator()->get_ConfigRoot();
                            const Json::Value& notifies = root["mail"]["notifications"];
                            _adminNotify = notifies.get("admin", "").asString();
                            _errorNotify = notifies.get("error", "").asString();
                        }
                        catch(const std::exception& ex)
                        {
                            BERR << "Configuration error: Notify email addresses not found. " << ex.what();
                        }
                    }
                    
                    // sent email
                    using namespace se::net;
                    ///CSmtpClient sender;
                    stringstream msg;
                    msg << "Session id : " << getContext().session().get_session_cookie() << endl;
                    msg << "Error Message: " << error << endl;

                    se::net::CSmtpClient smtp;
                    smtp.Init();
                    smtp.SendMail(_errorNotify.c_str() , _adminNotify.c_str() , "Smart-earn site Error", msg.str().c_str());

                }
                
                hasError = true;
            }
            
            const string& BaseContent::get_Error()
            {
                return _error;
            }

            IWebSession* BaseContent::get_Session() 
            {
                return _pUserSession;
            }

            void BaseContent::set_Session(IWebSession* p) 
            {
                _pUserSession = p;
            }

            void BaseContent::WriteFormattedDouble(std::ostream& os, double value, int precision) const 
            {
                double fyval = value;
                int lg = (int)std::log10(std::abs(fyval));
                int prevprec = os.precision();
                os.precision(precision);
                if (lg >= 12)
                {
                    fyval = fyval / 1e12;
                    os << fyval << "tln";
                }
                if (lg >= 9)
                {
                    fyval = fyval / 1e9;
                    os << fyval << "bln";
                }
                else if (lg >= 6)
                {
                    fyval = fyval / 1e6;
                    os << fyval << "mln";
                }
                else if (lg >= 3)
                {
                    fyval = fyval / 1e3;
                    os << fyval << "tho";
                }
                else
                {
                    os << value;
                }
                os.precision(prevprec);
            }


            void BaseContent::WriteFormattedDouble(std::ostream& os, const OmniType& value, int precision) const 
            {
                if (!value.IsEmpty() && value.dt == DT_R8)
                {
                    WriteFormattedDouble(os, (R8)value, precision);
                }
                else
                    os << value;
            }
        }
    }
}

