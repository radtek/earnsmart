/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CAuthenticationFilter.cpp
 * Author: santony
 * 
 * Created on December 27, 2015, 8:50 PM
 */

#include "internal.h"
#include "sessionfilter.h"
#include "CSessionFilter.h"
#include "sessionfilter.h"
#include <functional>

using namespace std;


CSessionFilter filter;


CSessionFilter::CSessionFilter()
{
    
}

CSessionFilter::~CSessionFilter()
{
}


nxweb_filter_data* SessionInit(
        nxweb_filter* filter, 
        nxweb_http_server_connection* conn, 
        nxweb_http_request* req, 
        nxweb_http_response* resp)
{
//    nxweb_filter_data* fdata= (nxweb_filter_data*) nxb_calloc_obj(req->nxb, sizeof(nxweb_filter_data));
//    fdata->bypass = false;
//    fdata->cache_key = "s";
//    return fdata;
    return 0;
}

void SessionFinalize(nxweb_filter* filter, nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp, nxweb_filter_data* fdata)
{
 //   BINF << fdata->cache_key;
}

nxweb_result SessionTranslateCacheKey(nxweb_filter* filter, nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp, nxweb_filter_data* fdata, const char* key)
{
    
    return NXWEB_OK;
}

nxweb_result SessionDoFilter(nxweb_filter* filter, nxweb_http_server_connection* conn, nxweb_http_request* req, nxweb_http_response* resp, nxweb_filter_data* fdata)
{
    BINF << fdata->cache_key;
    return NXWEB_ERROR;
}