/* 
 * File:   CHttpClient.cpp
 * Author: santony
 * 
 * Created on April 18, 2013, 9:44 PM
 */

#include "senetinternal.h"
#include "CHttpClient.h"
#include <curl/curl.h>
#include <stdio.h>
#include <sstream>
#include "../seglobal/CString.h"

namespace se
{
    namespace net
    {

        CHttpClient::CHttpClient()
        {
            curl_global_init(CURL_GLOBAL_DEFAULT);
            pcurl = curl_easy_init();
        }

        CHttpClient::~CHttpClient()
        {
            curl_easy_cleanup(pcurl);
            curl_global_cleanup();
        }

        void CHttpClient::EnableFollowLocation()
        {
            if (pcurl)
            {
                curl_easy_setopt(pcurl, CURLOPT_FOLLOWLOCATION, true);
            }
        }

        void CHttpClient::EnableHostNameVerification()
        {
            if (pcurl)
            {
                curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYHOST, 0L);
            }
        }

        void CHttpClient::EnablePeerVerification()
        {
            if (pcurl)
            {
                curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYPEER, 0L);
            }
        }

        void CHttpClient::Timeout(short timeout)
        {
            if (pcurl)
            {
                curl_easy_setopt(pcurl, CURLOPT_TIMEOUT, (I32) timeout);
            }
        }

        size_t CHttpClient::WriteHtmlBuffer(char *in, size_t size, size_t nmemb, ostringstream *out)
        {
            size_t r = size * nmemb;
            *out << in;
            return r;
        }

        long unsigned int CHttpClient::GetBuffer(const char* url, IString** ppOut)
        {
            ostringstream buffer;

            auto rCode = GetBuffer(url, &buffer);
            if (FAILED(rCode))
                return rCode;

            rCode = CString::Create(buffer.str().c_str(), ppOut);
            if (FAILED(rCode))
                return rCode;

            return S_OK;
        }

        ErrorId CHttpClient::GetBuffer(CSTR url, ostringstream* outStream)
        {
            char curl_errbuf[CURL_ERROR_SIZE];
            if (!pcurl)
            {
                BERR << "curl context is not created. Could be a memory issue.";
                return E_MemoryUnavailable;
            }
            curl_easy_reset(pcurl);
            
            curl_easy_setopt(pcurl, CURLOPT_URL, url);
            curl_easy_setopt(pcurl, CURLOPT_ERRORBUFFER, curl_errbuf);
            curl_easy_setopt(pcurl, CURLOPT_NOPROGRESS, 0L);
#ifdef __DEBUG__
            curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1L);
#endif
            curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, WriteHtmlBuffer);
            curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, outStream);
            auto res = curl_easy_perform(pcurl);
            if (res != CURLE_OK)
            {
                BERR << "Curl failed to perform with error " << curl_easy_strerror(res);
                return E_HttpError;
            }
            return S_OK;
        }

//        size_t CHttpClient::WriteHtmlToFile(char* in, size_t size, size_t nmemb, FILE* file) 
//        {
//
//        }


        ErrorId CHttpClient::GetFile(const char* url, const char* destination)
        {
            FILE* file = 0;

            try
            {

                curl_easy_reset(pcurl);
                curl_easy_setopt(pcurl, CURLOPT_URL, url);

                FILE* file = fopen(destination, "w");

                
#ifdef __DEBUG__
                curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1L);
#endif
                curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, file);

                curl_easy_perform(pcurl);

                fclose(file);
                file = 0;
                return S_Success;
            }            
            catch (...)
            {

                if (file)
                    fclose(file);

                return E_UnknownError;
            }
        }

        long unsigned int CHttpClient::PostJson(const char* url, const Json::Value& data, IString** retMsg)
        {
            std::vector<string> headers;
            return PostJson(url, data, headers);
        }

        long unsigned int CHttpClient::PostJson(const char* url, const Json::Value& data, const std::vector<string>& vheaders, IString** retMsg)
        {
            
            curl_easy_reset(pcurl);
            
            if (data == Json::nullValue)
            {
                BERR << "Argument exception. data argument cannot be empty.";
                return E_InvalidArg;
            }
            std::string strdata;
            Json::FastWriter writer;
            strdata = writer.write(data);
            if (!pcurl)
            {
                BERR << "Failed to initialize curl context.";
                return E_HttpError;
            }
            
            

            struct curl_slist* headers = NULL;
            //headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            for (const string& h : vheaders)
            {
                headers = curl_slist_append(headers, h.c_str());
            }
            curl_easy_setopt(pcurl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(pcurl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(pcurl, CURLOPT_POSTFIELDS, strdata.c_str());

            SE_CODE retcode;
            if (retMsg != nullptr)
            {
                retcode = GetBuffer(url, retMsg);
            } 
            else
            {
                auto res = curl_easy_perform(pcurl);
                if (res != CURLE_OK)
                {
                    BERR << "Curl failed to perform with error " << curl_easy_strerror(res);
                    retcode = E_HttpError;
                } else
                {
                    retcode = S_OK;
                }
            }
            curl_slist_free_all(headers);
            return retcode;
        }
    }
}