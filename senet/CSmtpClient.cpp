/* 
 * File:   CSmtpClient.cpp
 * Author: santony
 * 
 * Created on December 25, 2013, 1:59 PM
 */


#include "senetinternal.h"
#include "CSmtpClient.h"
#ifdef USE_POCO
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/MailMessage.h>
#else
#include<curl/curl.h>
#endif
#include <stdio.h>
#include "../seglobal/CDirectory.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
//#include <log4cxx/propertyconfigurator.h>
#include "../seglobal/exceptions.h"
#include <boost/tokenizer.hpp>
#include "../semain/semain.h"

#ifdef USE_POCO
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/SSLException.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/AutoPtr.h>
#endif

#include "CSmtpVmimeClient.h"

using namespace boost;
using namespace boost::property_tree;

namespace se
{
    namespace net
    {

        string CSmtpClient::_userId = "";
        string CSmtpClient::_password = "";
        string CSmtpClient::_server = "";
        I16 CSmtpClient::_port = 25;
 

        CSmtpClient::CSmtpClient() 
        {
            
        }

        CSmtpClient::CSmtpClient(const CSmtpClient& orig) {
        }

        CSmtpClient::~CSmtpClient() 
        {
        }
        
        void CSmtpClient::Init() 
        {
            if (_server.empty())
            {
                using namespace se::config;
                SP<IConfigurator> pcfg;
                SP<IApp> papp;
                se::SEOpenApp(&papp);
                papp->get_Configurator(&pcfg);
                const Json::Value& val = pcfg->get_ConfigRoot();
                const Json::Value& smtp = val["mail"]["smtp"];
                _userId = smtp["userid"].asString();
                _password = smtp["password"].asString();
                _server = smtp["server"].asString();
                _port = smtp["port"].asInt();
            }
        }
        
#ifdef USE_POCO
                
        long unsigned int CSmtpClient::SendMailSecured(const char* to, const char* from, const char* subject, const char* message, bool IsHtmlEmail)
        {
            using namespace Poco;
            using namespace Poco::Net;
            Init();
            MailRecipient recp;
            recp.setAddress((string)to);
            MailMessage msg;
            msg.addRecipient(recp);
            msg.setSender((string)from);
            msg.setSubject((string)subject);
            msg.setContent((string)message);

            SharedPtr<InvalidCertificateHandler> ptrHandler = 
                    new AcceptCertificateHandler(false);
            Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
            SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);
            SocketAddress sa(_server, _port);
            SecureStreamSocket sock(sa);
            SMTPClientSession client(sock);
            try
            {
                client.login(SMTPClientSession::AUTH_LOGIN, _userId, _password);
                client.sendMessage(msg);
                client.close();
            }
            catch (SSLException& ex)
            {
                client.close();
                BERR << ex.what();
                return E_SmtpError;
            }
            catch(SMTPException& ex)
            {
                client.close();
                BERR << ex.what();
                return E_SmtpError;
            }
            catch(NetException& ex)
            {
                client.close();
                BERR << ex.what();
                return E_SmtpError;
            }
            catch(Poco::Exception& ex)
            {
                client.close();
                BERR << ex.what();
                return E_SmtpError;
            }

        }

        
        ErrorId CSmtpClient::SendMail(const char* to, const char* from, const char* subject, const char* message, bool IsHtmlEmail)
        {
            using namespace Poco::Net;
            //LoggerPtr logger = Logger::getLogger("se.net.smtpclient.sendmail0");
            try
            {
                Init();
                SMTPClientSession client(_server, _port);
                client.login(SMTPClientSession::AUTH_LOGIN, _userId, _password);
                MailRecipient recp;
                recp.setAddress((string)to);
                MailMessage msg;
                msg.addRecipient(recp);
                msg.setSender((string)from);
                msg.setSubject((string)subject);
                msg.setContent((string)message);
                if (IsHtmlEmail)
                    msg.setContentType("text/html; charset=\"ISO-8859-1\"");
                client.sendMessage(msg);
                client.close();
                return S_Success;
            }
            catch(Poco::Exception& ex)
            {
                BERR << ex.what();
                return E_SmtpError;
            }
            catch(const std::exception& ex)
            {
                BERR << ex.what();
                return E_SmtpError;
            }
            catch(...)
            {
                BERR << "Failed to send email.";
                return E_SmtpError;
            }    
        }
        
        ErrorId CSmtpClient::SendTemplateMail(const char* to, const char* from, CSTR subject, const char* templateFile)
        {
            // open file read to a string
            if (!to || !strlen(to))
            {
                BERR << "to parameter is empty.";
                return E_InvalidArg;
            }
            if (!from || !strlen(from))
            {
                BERR << "from parameter is empty.";
                return E_InvalidArg;
            }
            if (!templateFile || !strlen(templateFile))
            {
                BERR << "templateFile parameter is empty.";
                return E_InvalidArg;
            }
            
            std::fstream f(templateFile);
            if (!f.good())
            {
                BERR << templateFile << " cannot be opened.";
                return E_FileNotFound;
            }
            stringstream strm ;
            strm << f.rdbuf();
            string str = strm.str();
            if (str.empty())
            {
                BERR << templateFile << " is empty.";
                return E_InvalidArg;
            }
            
            return SendMail(to, from, subject, str.c_str(), true);
                    
        }

        SE_CODE CSmtpClient::SendMail(const char* to, EMAILNOTIFYTYPE notifyType, const char* subject, const char* message, bool IsHtmlEmail)
        {
            string from = FromEmail(notifyType);
            if (from.empty())
            {
                BERR << "from email address not available.";
                return E_InvalidArg;
            }
            return this->SendMail(to, from.c_str(), subject, message, IsHtmlEmail);
        }
        
        SE_CODE CSmtpClient::SendTemplateMail(const char* to, EMAILNOTIFYTYPE notifyType, const char* subject, const char* templateFile)
        {
            string from = FromEmail(notifyType);
            if (from.empty())
            {
                BERR << "from email address not available.";
                return E_InvalidArg;
            }
            return this->SendTemplateMail(to, from.c_str(), subject, templateFile);
        }

        string CSmtpClient::FromEmail(EMAILNOTIFYTYPE notifyType) 
        {
            SP<IApp> papp;
            auto retCode = se::SEOpenApp(&papp);
            if (FAILED(retCode))
            {
                throw seexception(retCode);
            }
            Json::Value value = papp->get_Configurator()->get_ConfigRoot();
            if (!value.isMember("mail"))
                return "";
            value = value["mail"];
            if (!value.isMember("notifications"))
                return "";
            value = value["notifications"];
            if (notifyType == EMAILNOTIFYTYPE::Error)
            {
                return value.get("error", "").asString();
            }
            else if (notifyType == EMAILNOTIFYTYPE::Info)
            {
                return value.get("info", "").asString();
            }
            else if (notifyType == EMAILNOTIFYTYPE::Sales)
            {
                return value.get("sales", "").asString();
            }
            return "";
        }

        
#elif USE_CURL

        size_t CSmtpClient::payload_source(void* ptr, size_t size, size_t nmemb, void* userp)
        {
            CSmtpClient* pthis = static_cast<CSmtpClient*>(userp);
            memcpy(ptr, pthis->_msgpayload.c_str(), pthis->_msgpayload.length());
            return 0;
        }
        
        ErrorId CSmtpClient::SendMail(const char* to, const char* from, const char* subject, const char* message)
        {
            Init();
            ErrorId ret;
            CURL* curl = 0;
            curl_slist* rcptList = 0;
            string sMsg;
            try
            {
                
                /*
                typedef tokenizer< char_separator<char> > Tokenizer;
                typedef vector<string> TokenVector;
                TokenVector vec;
                char_separator<char> sep(";", "", keep_empty_tokens);
                Tokenizer tok(to, sep);
                vec.assign(tok.begin(), tok.end());
                 * */
                
                // construct payload
                DATETIME dt = DATETIME::Now();
                SP<IString> pdt;
                dt.ToString(&pdt);
                stringstream pl;
                pl << "Date: " << pdt->get_Buffer() << endl;
                pl << "To: " << to << endl;
                pl << "From: " << from << endl;
                pl << "Subject: " << subject << endl;
                pl << endl ;
                pl << message << endl;
                _msgpayload = pl.str();
                
                curl = curl_easy_init();
                if (!curl)
                    throw seexception("Curl cannot initialized.");
                std::ostringstream url ;
                url << "smtp://" << _server << ":" << _port ;
                
                CURLcode res = curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str() );
                if (res != CURLE_OK)
                {
                    sMsg = curl_easy_strerror(res);
                    throw;
                }
                
                res = curl_easy_setopt(curl, CURLOPT_USERNAME, _userId.c_str());
                if (res != CURLE_OK)
                {
                    sMsg = curl_easy_strerror(res);
                    throw;
                }

                res = curl_easy_setopt(curl, CURLOPT_PASSWORD, _password.c_str());
                if (res != CURLE_OK)
                {
                    sMsg = curl_easy_strerror(res);
                    throw;
                }

                res = curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
                if (res != CURLE_OK)
                {
                    sMsg = curl_easy_strerror(res);
                    throw;
                }
                
                
                rcptList = curl_slist_append(rcptList, to);                

                res = curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, rcptList);
                if (res != CURLE_OK)
                {
                    sMsg = curl_easy_strerror(res);
                    throw;
                }
                
                //curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
                //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
                
                
                res = curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
                if (res != CURLE_OK)
                {
                    sMsg = curl_easy_strerror(res);
                    throw;
                }

                res = curl_easy_setopt(curl, CURLOPT_READDATA, this);
                if (res != CURLE_OK)
                {
                    sMsg = curl_easy_strerror(res);
                    throw;
                }

                res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                if (res != CURLE_OK)
                {
                    sMsg = curl_easy_strerror(res);
                    throw;
                }

                res = curl_easy_perform(curl);
                if (res != CURLE_OK)
                {
                    sMsg = curl_easy_strerror(res);
                    throw 1;
                }
                
                return S_Success;
            }
            catch(const std::exception& ex)
            {
                sMsg = ex.what();
                ret = E_UnknownError;
                goto RET;
            }
            catch(...)
            {
                ret = E_UnknownError;
                goto RET;
            }
            
        RET:
            if (!sMsg.empty())
            {
                BOOSTER_INFO(__FUNCTION__) << sMsg;
            }
            if (curl)
                curl_easy_cleanup(curl);
        
            if (rcptList)
                curl_slist_free_all(rcptList);
        
            return ret;
        }
        
        ErrorId CSmtpClient::SendMail(const char* to, const char* from, const char* templateFile)
        {
            Init();
            return E_NotImpl;
        }
        
#endif

        
#ifdef USE_POCO
        
        string CMailSenderTask::_userId = "";
        string CMailSenderTask::_password = "";
        string CMailSenderTask::_server = "";
        I16 CMailSenderTask::_port = 25;
        
        
    ///////////////////////////////CMailSenderTask/////////////////////////////////
        CMailSenderTask::CMailSenderTask(ISession* ps) : CObjectRoot(ps)
        {

        }

        CMailSenderTask::~CMailSenderTask()
        {
        }
        
        ErrorId CMailSenderTask::Init()
        {
            if (_server.empty())
            {
                try
                {
                    SP<IApp> papp;
                    RETURNIFFAILED( (SEOpenApp(&papp)) );
                    const Json::Value& root =  papp->get_Configurator()->get_ConfigRoot();
                    try
                    {
                        const Json::Value& smtp = root["mail"]["smtp"];
                        _userId = smtp["userid"].asString();
                        _password = smtp["password"].asString();
                        _server = smtp["server"].asString();
                        _port = (short) smtp["port"].asInt();
                        return S_Success;
                    }
                    catch (const std::exception& ex)
                    {
                        BERR << "'threading.pool.numthreads' element not found in configuration file.";
                        return E_ConfigurationError;
                    }
                }
                CATCHCLAUSE("CMailSenderTask::Init")
            }
            return S_OK;            
        }
        
        void CMailSenderTask::set_ReceiptTo(const char* to)
        {
            _to = to;
                
        }
        
        void CMailSenderTask::set_From(const char* from)
        {
            _from = from;
        }
        
        void CMailSenderTask::set_Message(const char* message)
        {
            _message = message;
        }
        
        void CMailSenderTask::set_Subject(const char* subject)
        {
            _subject = subject;
        }
        
        void CMailSenderTask::Execute()
        {
            using namespace Poco::Net;
            //LoggerPtr _logger = Logger::getLogger("se.net.CMailSenderTask");
            try
            {
                SMTPClientSession client(_server, _port);
                client.login(SMTPClientSession::AUTH_LOGIN, _userId, _password);
                MailRecipient recp;
                recp.setAddress(_to);
                MailMessage msg;
                msg.addRecipient(recp);
                msg.setSender(_from);
                msg.setSubject(_subject);
                msg.setContent(_message);
                client.sendMessage(msg);
            }
            catch(seexception& ex)      
            {                           
               // LOGE(_logger, "(" << "Execute" << ") : Reported std error as " << ex.what());      
                throw ;
            }                           
            catch(std::exception& ex)   
            {                           
               // LOGE(_logger, "(" << "Execute" << ") : Reported std error as " << ex.what());      
                throw ;
            }                           
            catch(...)                  
            {                           
               // LOGE(_logger, "(" << "Execute" << ") : Unknown error reported.");                          
                throw ;
            }                           
        }
        
        ErrorId CMailSenderTask::Create(ISession* ps, CMailSenderTask** ppOut)
        {
            return CObject<CMailSenderTask>::Create(ps, ppOut);
        }
#endif

    }
    
    
}