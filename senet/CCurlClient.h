/* 
 * File:   CCurlClient.h
 * Author: santony
 *
 * Created on August 16, 2015, 3:28 PM
 */

#ifndef CCURLCLIENT_H
#define	CCURLCLIENT_H

namespace se
{
    namespace net
    {
        class CCurlClient 
        {
        private:
            char* _errorBuffer;
            void* _conn = nullptr;
            
        public:
            enum class CurlReqType
            {
                HttpRequest,
            };

        public:
            CCurlClient();
            CCurlClient(const CCurlClient& orig) = delete;
            virtual ~CCurlClient();

            SE_CODE Read(CurlReqType reqType, CSTR requestUrl, IString** ppOutResponse);

        private:
            SE_CODE ReturnError();

        };

    }
}


#endif	/* CCURLCLIENT_H */

