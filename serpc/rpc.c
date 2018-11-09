/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "rpc.h"

NXWEB_DEFINE_HANDLER(rpc, 
    .on_load = rpchandler_onload,
    .on_unload = rpchandler_onunload,
    .on_complete = rpchandler_on_complete,
    .on_select = rpchandler_on_select,
    .on_error = rpchandler_on_error,
    .on_headers = rpchandler_on_headers,
    .on_post_data = rpchandler_on_post_data,
    .on_post_data_complete = rpchandler_on_post_data_complete,
    .on_complete = rpchandler_on_complete,
    .on_request = rpchandler_on_request,
    .on_generate_cache_key = rpchandler_generate_cache_key,
    .flags = NXWEB_HANDLE_ANY | NXWEB_PARSE_COOKIES | NXWEB_PARSE_PARAMETERS | NXWEB_INWORKER | NXWEB_ACCEPT_CONTENT
);

