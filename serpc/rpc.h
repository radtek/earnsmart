/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   jsonrpc1.h
 * Author: santony
 *
 * Created on December 27, 2015, 10:56 PM
 */

#ifndef JSONRPC1_H
#define JSONRPC1_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <nxweb.h>
    
    nxweb_result rpchandler_onload();
    nxweb_result rpchandler_onunload();
    nxweb_result rpchandler_generate_cache_key(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);
    nxweb_result rpchandler_on_select(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);
    nxweb_result rpchandler_on_headers(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);
    nxweb_result rpchandler_on_post_data(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);
    nxweb_result rpchandler_on_post_data_complete(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);
    nxweb_result rpchandler_on_complete(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);
    nxweb_result rpchandler_on_error(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);
    nxweb_result rpchandler_on_request(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);
    nxweb_result rpchandler_on_request(nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp);

#ifdef __cplusplus
}
#endif

#endif /* JSONRPC1_H */

