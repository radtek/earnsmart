/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpConnection.h
 * Author: santony
 *
 * Created on February 7, 2016, 8:21 AM
 */

#ifndef CHTTPCONNECTION_H
#define CHTTPCONNECTION_H

#include "internal.h"

typedef nxweb_http_server_connection RawHttpConnection;
typedef nxe_data RawData;

class CHttpConnection 
{
private:
    RawHttpConnection* _rawconn;
    
public:
    CHttpConnection(RawHttpConnection* rawconn);
    CHttpConnection(const CHttpConnection& orig);
    ~CHttpConnection();


    U64 UniqueId() const;
    nxe_data HandlerParameters() const;
    bool IsSecure() const;
    bool IsResponseReady() const;
    bool HasSubrequestFailed() const;
    bool IsInWorker() const;
    bool IsConnectionClosing() const;
    nxe_time_t ConnectedTime() const;
    CSTR RemoteAddress() const;
    
};

#endif /* CHTTPCONNECTION_H */

