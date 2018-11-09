/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpUtils.h
 * Author: santony
 *
 * Created on March 6, 2016, 7:45 PM
 */

#ifndef CHTTPUTILS_H
#define CHTTPUTILS_H

#include "internal.h"
#include <string>

using namespace std;

typedef nxweb_mime_type MIMETYPE;


class CHttpUtils 
{
public:
    
    
    static STR UrlDecode(STR source, string& destination);
    static int UrlPrefixMatch(CSTR url, int UrlLen, CSTR prefix, int prefixLen);
    static const MIMETYPE* ConvertMIMEType(CSTR mimeType);
    static const MIMETYPE* GetDefaultMIMEType();
    static void SetDefaultMIMEType(const MIMETYPE* mtype);
    static const MIMETYPE* MIMETypeByExt(CSTR filepathorextension);
  
    static int FormatHttpTime(STR buffer, tm* t);
    static int FormatIso8601Time(STR buffer, tm* t);
    static time_t ParseHttpTime(CSTR timestr);
    static int RemoveDotsFromUriPath(STR path);
    
    
    
};

#endif /* CHTTPUTILS_H */

