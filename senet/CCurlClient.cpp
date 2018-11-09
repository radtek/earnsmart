/* 
 * File:   CCurlClient.cpp
 * Author: santony
 * 
 * Created on August 16, 2015, 3:28 PM
 */

#include "senetinternal.h"
#include "CCurlClient.h"
#include <curl/curl.h>
#include <system_error>
#include "../seglobal/CString.h"

using namespace se;
using namespace se::net;

CCurlClient::CCurlClient() 
{
    _errorBuffer = new char[CURL_ERROR_SIZE];
}

CCurlClient::~CCurlClient() 
{
    delete [] _errorBuffer;
}

long unsigned int CCurlClient::ReturnError() 
{
    BERR << "Curl Error: " << _errorBuffer;
    if (_conn)
        curl_easy_cleanup(_conn);
    return E_CurlError;
}

int writeFunction(char* data, size_t size, size_t nmemb, std::string* writerData)
{
    if (writerData == nullptr)
        return 0;
    auto sz = size*nmemb;
    writerData->append(data, sz);
    return sz;
}

long unsigned int CCurlClient::Read(CurlReqType reqType, const char* requestUrl, IString** ppOutResponse) 
{
    CURLcode code;
    std::string buffer;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    _conn = curl_easy_init();
    if (!_conn)
    {
        BERR << "Failed to create curl connection";
        return E_CurlError;
    }
    // setup error buffer
    code = curl_easy_setopt(_conn, CURLOPT_ERRORBUFFER, _errorBuffer);
    if (code != CURLE_OK)
    {
        BERR << "Failed to set error buffer";
        return E_CurlError;
    }
    code = curl_easy_setopt(_conn, CURLOPT_URL, requestUrl);
    if (code != CURLE_OK)
    {
        ReturnError();
    }
    code = curl_easy_setopt(_conn, CURLOPT_WRITEFUNCTION, writeFunction);
    if (code != CURLE_OK)
    {
        ReturnError();
    }
    code = curl_easy_setopt(_conn, CURLOPT_WRITEDATA, &buffer);
    if (code != CURLE_OK)
    {
        ReturnError();
    }
    
    // Retrieve content for the URL
    code = curl_easy_perform(_conn);    
    if (code != CURLE_OK)
    {
        ReturnError();
    }
    
    curl_easy_cleanup(_conn);
    
    RETURNIFFAILED( IString::Create(std::move(buffer).c_str(), ppOutResponse));
    
    return S_OK;
}
