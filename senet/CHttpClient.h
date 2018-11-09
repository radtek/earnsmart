/* 
 * File:   CHttpClient.h
 * Author: santony
 *
 * Created on April 18, 2013, 9:44 PM
 */

#ifndef CHTTPCLIENT_H
#define	CHTTPCLIENT_H


/*
 * sample
 * http://curl.haxx.se/libcurl/c/simple.html
 */

#include <iostream>

using namespace std;

namespace se
{
    namespace net
    {

        class CHttpClient 
        {
        public:
            CHttpClient();
            CHttpClient(const CHttpClient& orig) = delete;
            virtual ~CHttpClient();

            //Call this if you're getting response like 'moved permanently'.
            void EnableFollowLocation() ;

            void EnablePeerVerification();

            void EnableHostNameVerification();

            // timeout for the complete scope of the connection 
            void Timeout(I16 timeout); 
            
            /* Obtain a file from the remote ftp directory. 
             * The url must be provided as complete upto a file name.
             * eg: to download a file readme.txt in doc folder in the host ftp.host.com, 
             * url will be ftp://ftp.host.com/doc/readme.txt             
             */
            ErrorId GetFile(CSTR url, CSTR destination);
            ErrorId GetBuffer(CSTR url, IString **ppOut);
            ErrorId GetBuffer(CSTR url, ostringstream* outStream);
            
            //Post data into a remote webapi. This method assumes no basic, digest or other authentication is not required.
            SE_CODE PostJson(CSTR url, const Json::Value& data, IString** retMsg = nullptr);
            SE_CODE PostJson(CSTR url, const Json::Value& data, const std::vector<string>& headers, IString** retMsg = nullptr);

        private:
            static size_t WriteHtmlBuffer(char *in, size_t size, size_t nmemb, ostringstream *out);
            //static size_t WriteHtmlToFile(char *in, size_t size, size_t nmemb, FILE* file);

        private:
            I16 _timeout = 0;
            void* pcurl;

        };

    }
}
#endif	/* CHTTPCLIENT_H */

