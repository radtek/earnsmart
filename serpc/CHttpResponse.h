/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpResponse.h
 * Author: santony
 *
 * Created on February 7, 2016, 8:17 AM
 */

#ifndef CHTTPRESPONSE_H
#define CHTTPRESPONSE_H

#include "CResponse.h"
#include "ContentTypes.h"
#include <map>

using namespace std;

typedef nxweb_http_response RawHttpResponse;
typedef map<string, string> HttpCookies;


class CHttpResponse //: public CResponse
{
private:
    RawHttpResponse* _rawResp;
    bool _hasEnded = false;
    //HttpCookies _cookies;
    bool _secure = false;
    
    
public:
    CHttpResponse(RawHttpResponse* rawResp);
    CHttpResponse(const CHttpResponse& orig);
    ~CHttpResponse();
    
    bool Secure();
    void Secure(bool);
    
    void ContentType(CSTR contentType);
    void ContentType(ContentTypes contentType);
    void MimeType(CSTR value);
    void Status(int code, CSTR value);
    void ContentCharset(CSTR value);
    void Header(CSTR key, CSTR value, bool safe = false);
    void Content(CSTR value);
    void Print(CSTR fmt, ...);
    void Append(CSTR value);
    void Append(CPOINTER data, int size);
    void Append(char c);
    void Append(U64 number);
    //void Cookie(CSTR key, CSTR value);
    void SessionCookie(CSTR value);
    void Redirect(int code, CSTR location, int secure);
    void Redirect(int code, CSTR location, CSTR locationPathInfo, int secure);
    void HttpError(int code, CSTR message);
    void Data(CPOINTER data, SIZE size, CSTR contentType);
    int File(char* fpath, const struct stat* finfo, int gzip_encoded,
        off_t offset, size_t size, const nxweb_mime_type* mtype, const char* charset); // finfo and mtype could be null => autodetect

    // overloads
    CHttpResponse& operator << (CSTR str);
    CHttpResponse& operator << (U64 invalue);
    CHttpResponse& operator << (char c);

    
    //void End();
    
   
private:

};

#endif /* CHTTPRESPONSE_H */

