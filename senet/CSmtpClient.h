/* 
 * File:   CSmtpClient.h
 * Author: santony
 *
 * Created on December 25, 2013, 1:59 PM
 */

#ifndef CSMTPCLIENT_H
#define	CSMTPCLIENT_H

#include "../seglobal/Object.h"

#define USE_POCO
//#define USE_VMIME
//#define USE_CURL

namespace se
{
    namespace net
    {
        
        enum class EMAILNOTIFYTYPE
        {
            Error,
            Info,
            Sales
        };


        class CSmtpClient 
        {
        public:
            CSmtpClient();
            CSmtpClient(const CSmtpClient& orig);
            virtual ~CSmtpClient();
            
            void Init();
            
            SE_CODE SendMail(CSTR to, CSTR from, CSTR subject, CSTR message, bool IsHtmlEmail = true);

            SE_CODE SendMailSecured(CSTR to, CSTR from, CSTR subject, CSTR message, bool IsHtmlEmail = true);

            SE_CODE SendTemplateMail(CSTR to, CSTR from, CSTR subject, CSTR templateFile);
            
            SE_CODE SendMail(CSTR to, EMAILNOTIFYTYPE notifyType, CSTR subject, CSTR message, bool IsHtmlEmail = true);
            
            SE_CODE SendTemplateMail(CSTR to, EMAILNOTIFYTYPE notifyType, CSTR subject, CSTR templateFile);
            
            static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
            
        private:
            string FromEmail(EMAILNOTIFYTYPE notifyType);
            
        private:
            // cache
            static string _userId, 
                    _server,
                    _password;
            static I16 _port;
            
            string _msgpayload;

        };
        
#ifdef USE_POCO
        
        class CMailSenderTask : public CObjectRoot, public se::threading::ITask
        {
        public:
            CMailSenderTask(ISession*);
            virtual ~CMailSenderTask();
            
            void set_ReceiptTo(CSTR to);
            void set_From(CSTR from);
            void set_Subject(CSTR subject);
            void set_Message(CSTR message);
            virtual void Execute();
            virtual SE_CODE Init();
            
            static SE_CODE Create(ISession*, CMailSenderTask** ppOut);
            
        private:
            // cache
            static string _userId, 
                    _server,
                    _password;
            static I16 _port;
            
            string _from, _to, _subject, _message;
            
        };
        
#endif

    }
}
#endif	/* CSMTPCLIENT_H */

