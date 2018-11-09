/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CSmtpVmimeClient.cpp
 * Author: santony
 * 
 * Created on December 1, 2016, 9:34 PM
 */

#include "senetinternal.h"
#include "CSmtpVmimeClient.h"
#include <vmime/vmime.hpp>
#include <vmime/platforms/posix/posixHandler.hpp>

using namespace vmime;

SE_CODE SendMailUsingVmime(CSTR surl, CSTR emailUser, CSTR emailPwd, const char* to, const char* from, const char* subject, const char* message, bool IsHtmlEmail)
{
    class Office365Authenticator : public security::sasl::defaultSASLAuthenticator
    {
        vmime::string user, pwd;
    public:
        Office365Authenticator(vmime::string emailuser, vmime::string emailpwd) : user(emailuser), pwd(emailpwd)
        {
        }
	const vmime::string getUsername() const 
        {
            return user;
        }
	const vmime::string getPassword() const
        {
            return pwd;
        }
    };
    
    try
    {
        messageBuilder mb;
        mb.setSubject(text(subject));
        mb.setExpeditor(mailbox(from));
        vmime::ref<mailbox> mrecp = create<mailbox>(to);
        mb.getRecipients().appendAddress(mrecp);
        if (IsHtmlEmail)
        {
            mb.constructTextPart(mediaType(mediaTypes::TEXT, mediaTypes::TEXT_HTML));
        }
        else
        {
            mb.constructTextPart(mediaType(mediaTypes::TEXT, mediaTypes::TEXT_PLAIN));
        }
        mb.getTextPart()->setText(create<stringContentHandler>(message));
        //vmime::string msgToSend = mb.construct()->generate();
        
        utility::url url(surl);
        auto session = create<net::session>();
        auto sasl = create<Office365Authenticator>(emailUser, emailPwd);
        auto tp = session->getTransport(url, sasl);
        tp->connect();
        tp->send(mb.construct());
        tp->disconnect();
    }
    catch(vmime::exception& ex)
    {
    }
    catch(std::exception& ex)
    {
        
    }
}

SE_CODE SendTemplateMailUsingVMime(CSTR url, CSTR emailUser, CSTR emailPwd, const char* to, const char* from, CSTR subject, const char* templateFile)
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
    return SendMailUsingVmime(url, emailUser, emailPwd, to, from, subject, str.c_str(), true);
}