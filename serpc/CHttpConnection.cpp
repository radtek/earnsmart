/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpConnection.cpp
 * Author: santony
 * 
 * Created on February 7, 2016, 8:21 AM
 */

#include "internal.h"
#include "CHttpConnection.h"

CHttpConnection::CHttpConnection(RawHttpConnection* rawconn) : _rawconn(rawconn)
{
    
}

CHttpConnection::CHttpConnection(const CHttpConnection& orig) : 
    _rawconn(orig._rawconn)
{
}

CHttpConnection::~CHttpConnection()
{
}

unsigned long CHttpConnection::UniqueId() const 
{
    return _rawconn->uid;
}

nxe_data CHttpConnection::HandlerParameters() const {
    return _rawconn->handler_param;
}

bool CHttpConnection::IsSecure() const {
    return _rawconn->secure;
}

bool CHttpConnection::IsResponseReady() const {
    return _rawconn->response_ready;
}

bool CHttpConnection::HasSubrequestFailed() const {
    return _rawconn->subrequest_failed;
}

bool CHttpConnection::IsInWorker() const {
    return _rawconn->in_worker;
}

bool CHttpConnection::IsConnectionClosing() const {
    return _rawconn->connection_closing;
}

nxe_time_t CHttpConnection::ConnectedTime() const {
    return _rawconn->connected_time;
}

const char* CHttpConnection::RemoteAddress() const {
    return _rawconn->remote_addr;
}






