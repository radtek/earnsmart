/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CSmtpVmimeClient.h
 * Author: santony
 *
 * Created on December 1, 2016, 9:34 PM
 */

#ifndef CSMTPVMIMECLIENT_H
#define CSMTPVMIMECLIENT_H

// example url = smtp://smtp.abc.com:587 
SE_CODE SendMailUsingVmime(CSTR url, CSTR emailUser, CSTR emailPwd, CSTR to, CSTR from, CSTR subject, CSTR message, bool IsHtmlEmail = true);
SE_CODE SendTemplateMailUsingVMime(CSTR url, CSTR emailUser, CSTR emailPwd, const char* to, const char* from, CSTR subject, const char* templateFile);

#endif /* CSMTPVMIMECLIENT_H */

