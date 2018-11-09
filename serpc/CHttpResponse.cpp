/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpResponse.cpp
 * Author: santony
 * 
 * Created on February 7, 2016, 8:17 AM
 */

#include "internal.h"
#include "CHttpResponse.h"
#include "CHttpRequest.h"

CHttpResponse::CHttpResponse(RawHttpResponse* rawResp) : _rawResp(rawResp)
{
    
}

CHttpResponse::CHttpResponse(const CHttpResponse& orig) : _rawResp(orig._rawResp)
{
    
}

CHttpResponse::~CHttpResponse()
{
//    this->End();
}

void CHttpResponse::ContentType(const char* contentType) 
{
    nxweb_set_response_content_type(_rawResp, contentType);
}

void CHttpResponse::ContentType(ContentTypes contentType) 
{
    std::string str = "";
    switch (contentType)
    {
        case ContentTypes::Json:
            str = "application/json";
            break;
        case ContentTypes::Jsonrpc:
            str = "application/jsonrpc";
            break;
        case ContentTypes::Xml:
            str = "application/xml";
            break;
        case ContentTypes::Html:
        default:
            str = "text/html";
            
    }
    nxweb_set_response_content_type(_rawResp, str.c_str());
}

void CHttpResponse::MimeType(const char* value) 
{
    
}

void CHttpResponse::Status(int code, const char* value) {
    nxweb_set_response_status(_rawResp, code, value);
}

void CHttpResponse::ContentCharset(const char* value) 
{
    nxweb_set_response_charset(_rawResp, value);
}

void CHttpResponse::Header(const char* key, const char* value, bool safe) 
{
    if (!safe)
        nxweb_add_response_header(_rawResp,key, value);
    else
        nxweb_add_response_header_safe(_rawResp, key, value);
}

void CHttpResponse::Content(const char* value) 
{
    nxweb_response_append_str(_rawResp, value);
}

void CHttpResponse::Print(const char* fmt, ...) 
{
    va_list ap;
    va_start(ap, fmt);
    nxb_printf_va(_rawResp->nxb, fmt, ap);
    va_end(ap);
}

void CHttpResponse::Append(const char* value) 
{
    nxweb_response_append_str(_rawResp, value);
}

void CHttpResponse::Append(CPOINTER data, int size) 
{
    nxweb_response_append_data(_rawResp, data, size);
}

void CHttpResponse::Append(unsigned long number)
{
    nxweb_response_append_uint(_rawResp, number);
}
    
void CHttpResponse::Append(char c)
{
    nxweb_response_append_char(_rawResp, c);
}

CHttpResponse& CHttpResponse::operator<<(const char* str) 
{
    nxweb_response_append_str(_rawResp, str);
    return *this;
}

CHttpResponse& CHttpResponse::operator<<(U64 ivalue) 
{
    nxweb_response_append_uint(_rawResp, ivalue);
    return *this;
}

CHttpResponse& CHttpResponse::operator <<(char c)
{
    nxweb_response_append_char(_rawResp, c);
    return *this;
}

//void CHttpResponse::Cookie(const char* key, const char* value) 
//{
//    //TODO: To allow appending cookie key/value pairs, the cookie content
//    // need to stored as a dict at class level and then appended as below
//    // just before response is delivered (see End method in this class)
//
//    _cookies.insert(make_pair(key, value));
//}

bool CHttpResponse::Secure() 
{
    return _secure;
}

void CHttpResponse::Secure(bool value) 
{
    this->_secure = value;
}


void CHttpResponse::SessionCookie(const char* value) 
{
    char cookie[1024];
    if (_secure)
        sprintf(cookie, "esid=%s;path=/;secure", value);
    else
        sprintf(cookie, "esid=%s;path=/", value);
    
    this->Header("Set-Cookie", cookie, true);
}


void CHttpResponse::Redirect(int code, const char* location, int secure) {
    nxweb_send_redirect(_rawResp, code, location, secure);
}

void CHttpResponse::Redirect(int code, const char* location, const char* locationPathInfo, int secure) {
    nxweb_send_redirect2(_rawResp, code, location, locationPathInfo, secure);
}

void CHttpResponse::HttpError(int code, const char* message) 
{
    nxweb_send_http_error(_rawResp, code, message);
}

void CHttpResponse::Data(const void* data, long unsigned int size, const char* contentType) 
{
    nxweb_send_data(_rawResp, data, size, contentType);
}

int CHttpResponse::File(char* fpath, const struct stat* finfo, int gzip_encoded,
        off_t offset, size_t size, const nxweb_mime_type* mtype, const char* charset) 
{
    return nxweb_send_file(_rawResp, fpath, finfo, gzip_encoded, offset, size, mtype, charset);
}

//void CHttpResponse::End() 
//{
//    if (!_hasEnded)
//    {
//        ostringstream ostr;
//        for (auto pr : _cookies)
//        {
//            ostr << pr.first << "=" << pr.second << ";";
//        }
//        nxweb_add_response_header(_rawResp, "Set-Cookie", ostr.str().c_str());
//        this->_hasEnded = true;
//    }
//}
