/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpRequest.cpp
 * Author: santony
 * 
 * Created on February 7, 2016, 8:17 AM
 */

#include "internal.h"
#include "CHttpRequest.h"

CHttpRequest::CHttpRequest(RawHttpRequest* rawReq) : 
    _rawReq(rawReq), 
    _headers(_rawReq->headers),
    _cookies(_rawReq->cookies),
    _parameters(_rawReq->parameters)
{

}

CHttpRequest::CHttpRequest(const CHttpRequest& orig) : 
    _rawReq(orig._rawReq),
    _headers(orig._rawReq->headers),
    _cookies(orig._rawReq->cookies),
    _parameters(orig._rawReq->parameters)
{
    
}

CHttpRequest::~CHttpRequest() {
}

const char* CHttpRequest::Method() const {
    return _rawReq->method;
}

const char* CHttpRequest::Uri() const {
    return _rawReq->uri;
}

const char* CHttpRequest::HttpVersion() const {
    return _rawReq->http_version;
}

const char* CHttpRequest::Host() const {
    return _rawReq->host;
}

const char* CHttpRequest::Cookie() const {
    return _rawReq->cookie;
}

const char* CHttpRequest::UserAgent() const {
    return _rawReq->user_agent;
}

const char* CHttpRequest::ContentType() const {
    return _rawReq->content_type;
}

const char* CHttpRequest::Content() const {
    return _rawReq->content;
}


long unsigned int CHttpRequest::ContentLength() const {
    return _rawReq->content_length;
}

long unsigned int CHttpRequest::ContentReceived() const {
    return _rawReq->content_received;
}

const char* CHttpRequest::TransferEncoding() const {
    return _rawReq->transfer_encoding;
}

const char* CHttpRequest::AcceptEncoding() const {
    return _rawReq->accept_encoding;
}

const char* CHttpRequest::Range() const {
    return _rawReq->range;
}

const char* CHttpRequest::PathInfo() const {
    return _rawReq->path_info;
}

const CHttpHeaders& CHttpRequest::Headers() {
    return this->_headers;
}

const CHttpCookies& CHttpRequest::Cookies() {
    return this->_cookies;
}

const char* CHttpRequest::SessionCookie() 
{
    return _cookies.findvalue("esid");
}


const CHttpParameters& CHttpRequest::Parameters() {
    return this->_parameters;
}

RawHttpRequest* CHttpRequest::RawData() const 
{
    return this->_rawReq;
}


