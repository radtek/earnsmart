/* 
 * File:   CPaypal.h
 * Author: santony
 *
 * Created on December 22, 2013, 12:27 AM
 */

#ifndef CPAYPAL_H
#define	CPAYPAL_H

#include <curl/curl.h>


namespace se
{
    namespace admin
    {


        class CPaypal 
        {
        public:
            CPaypal();
            CPaypal(const CPaypal& orig);
            virtual ~CPaypal();
            
            virtual ErrorId Init();
            void AddHeader(const string& key, const string& value);
            void ClearHeaders();
            void AddData(const string& key, const string& value);
            void ClearData();
            
        protected:
            //LoggerPtr _logger;
            string _endpoint;
            string _userName;
            string _password;
            string _signature;
            CURL* _curl;
            struct curl_slist* _headers;
            string _data;
        };

    }
}

#endif	/* CPAYPAL_H */

