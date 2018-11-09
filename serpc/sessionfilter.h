/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   authenticationfilter.h
 * Author: santony
 *
 * Created on December 27, 2015, 10:18 PM
 */

#ifndef AUTHENTICATIONFILTER_H
#define AUTHENTICATIONFILTER_H

#ifdef __cplusplus
extern "C"
{
#endif

    
#include <nxweb.h>
    
nxweb_filter_data* SessionInit(nxweb_filter* filter, nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);
void SessionFinalize(nxweb_filter* filter, nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp, nxweb_filter_data* fdata);
nxweb_result SessionTranslateCacheKey(nxweb_filter* filter, nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp, nxweb_filter_data* fdata, const char* key);
nxweb_result SessionDoFilter(nxweb_filter* filter, nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp, nxweb_filter_data* fdata);

#ifdef __cplusplus
}
#endif

#endif /* AUTHENTICATIONFILTER_H */

