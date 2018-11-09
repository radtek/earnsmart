/* 
 * File:   CPaypal.cpp
 * Author: santony
 * 
 * Created on December 22, 2013, 12:27 AM
 */

#include <ext/slist>

#include "internal.h"
#include "CPaypal.h"
#include "../seglobal/exceptions.h"
#include "../semain/semain.h"


using namespace se;
using namespace se::config;

namespace se
{
    namespace admin
    {

        CPaypal::CPaypal() : //_logger(Logger::getLogger("se.admin.paypal")), 
            _curl(NULL), _headers(NULL)
        {
        }

        CPaypal::CPaypal(const CPaypal& orig) 
        {
            
        }
        
        ErrorId CPaypal::Init()
        {
            try
            {
                _curl = curl_easy_init();
                if (NULL == _curl)
                    throw seexception(E_MemoryUnavailable, "curl handle cannot be created. No memory.");
                
                SP<IApp> papi;
                THROWIFFAILED(SEOpenApp(&papi), "Cannot open IApp");
                // get config info for paypal
                SP<IConfigurator> pConfig;
                THROWIFFAILED(papi->get_Configurator(&pConfig), "Cannot open IConfigurator");
                
                SP<IConfigItemList> pCfgList;
                THROWIFFAILED(pConfig->get_Settings(GLOBALREGIONID, "Paypal", &pCfgList), "Cannot open IConfigItemList");
                _endpoint = pCfgList->get_Item("Endpoint")->get_Value();
                _userName = pCfgList->get_Item("UserName")->get_Value();
                _password = pCfgList->get_Item("Password")->get_Value();
                _signature = pCfgList->get_Item("Signature")->get_Value();
                
                // set url
                CURLcode res = curl_easy_setopt(_curl, CURLOPT_URL, _endpoint.c_str());
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set endpoint.");
                
                // peer verification
                res = curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed with peer verification.");                

                // host verification
                res = curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed with host verification.");                
                
                
                return S_Success;
            }
            CATCHCLAUSE("CPaypal::Init");
        }
        
        void CPaypal::ClearHeaders()
        {
            if (!_curl)
                throw seexception(E_InvalidPointer, "Curl is not initialized");
            if (_headers)
                curl_slist_free_all(_headers);
            _headers = NULL;
        }
        
        void CPaypal::AddHeader(const string& key, const string& value)
        {
            if (!_curl)
                throw seexception(E_InvalidPointer, "Curl is not initialized");
            _headers = curl_slist_append(_headers, (key + ": " + value).c_str());
            CURLcode res = curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _headers);
            if (res != CURLE_OK)
                throw seexception(E_UnknownError, "Curl failed to set headers.");
        }

        void CPaypal::ClearData()
        {
            if (!_curl)
                throw seexception(E_InvalidPointer, "Curl is not initialized");
            _data.clear();
        }
        
        void CPaypal::AddData(const string& key, const string& value)
        {
            if (!_curl)
                throw seexception(E_InvalidPointer, "Curl is not initialized");
            if (!_data.empty())
                _data += (string)"&" + curl_easy_escape(_curl, key.c_str(), key.length()) + "=" + curl_easy_escape(_curl, value.c_str(), value.length());
            else
                _data += (string)curl_easy_escape(_curl, key.c_str(), key.length()) + "=" + curl_easy_escape(_curl, value.c_str(), value.length());
            
//            if (!_data.empty())
//                _data += (string)"&" + key + "=" + value;
//            else
//                _data += key + "=" + value;
            
        }
        
        
        CPaypal::~CPaypal() 
        {
            if (_curl)
            {
                ClearHeaders();
                curl_easy_cleanup(_curl);
                _curl = NULL;
            }
        }
    }
}