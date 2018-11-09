/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpUtils.cpp
 * Author: santony
 * 
 * Created on March 6, 2016, 7:45 PM
 */

#include <sys/param.h>

#include "CHttpUtils.h"

const MIMETYPE* CHttpUtils::GetDefaultMIMEType()
{
    return nxweb_get_default_mime_type();
}

void CHttpUtils::SetDefaultMIMEType(const MIMETYPE* mtype)
{
    nxweb_set_default_mime_type(mtype);
}

char* CHttpUtils::UrlDecode(STR source, string& destination)
{
    char dest[MAXPATHLEN];
    char* r = nxweb_url_decode(source, dest);
    destination = dest;
    return r;
}

int CHttpUtils::UrlPrefixMatch(const char* url, int UrlLen, const char* prefix, int prefixLen)
{
    return nxweb_url_prefix_match(url, UrlLen, prefix, prefixLen);
}

const MIMETYPE* CHttpUtils::ConvertMIMEType(const char* mimeType)
{
    return nxweb_get_mime_type(mimeType);
}

const MIMETYPE* CHttpUtils::MIMETypeByExt(const char* filepathorextension)
{
    return nxweb_get_mime_type_by_ext(filepathorextension);
}

int CHttpUtils::FormatHttpTime(char* buffer, tm* t)
{
    return nxweb_format_http_time(buffer, t);
}

int CHttpUtils::FormatIso8601Time(char* buffer, tm* t)
{
    return nxweb_format_iso8601_time(buffer, t);
}

time_t CHttpUtils::ParseHttpTime(const char* timestr)
{
    return nxweb_parse_http_time(timestr);
}

int CHttpUtils::RemoveDotsFromUriPath(char* path)
{
    return nxweb_remove_dots_from_uri_path(path);
}








