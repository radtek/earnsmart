/* 
 * File:   CPaypalExpressCheckout.cpp
 * Author: santony
 * 
 * Created on December 22, 2013, 12:27 AM
 */

#include "internal.h"
#include "CPaypalExpressCheckout.h"
#include "../seglobal/exceptions.h"
#include <vector>
#include <boost/tokenizer.hpp>
#include "../seglobal/CString.h"
#include "../semain/semain.h"

using namespace se::config;

namespace se
{
    namespace admin
    {
        CPaypalExpressCheckout::CPaypalExpressCheckout(ISession* ps) : CObjectRoot(ps) 
        {
        }



        CPaypalExpressCheckout::~CPaypalExpressCheckout() 
        {
        }
        
        ErrorId CPaypalExpressCheckout::Init()
        {
            try
            {
                RETURNIFFAILED(CPaypal::Init());

                SP<IApp> papi;
                THROWIFFAILED(SEOpenApp(&papi), "Cannot open IApp");
                // get config info for paypal
                SP<IConfigurator> pConfig;
                THROWIFFAILED(papi->get_Configurator(&pConfig), "Cannot open IConfigurator");

                SP<IConfigItemList> pCfgList;
                THROWIFFAILED(pConfig->get_Settings(GLOBALREGIONID, "Paypal", &pCfgList), "Cannot open IConfigItemList");
                _version = pCfgList->get_Item("Version")->get_Value();
                return S_Success;
            }
            CATCHCLAUSE("CPaypalExpressCheckout::Init");
            
        }
        
        size_t CPaypalExpressCheckout::CurlWriteDataFuncion1( char *ptr, size_t size, size_t nmemb, void *userdata)
        {
            string responseData;
            size = size*nmemb;
            for (int i=0; i<size; ++i)
                responseData.push_back(ptr[i]);
            responseData.push_back('\0');
            
            
            CPaypalExpressCheckout* pthis = static_cast<CPaypalExpressCheckout*>(userdata);
            pthis->SetResponse(1, responseData);
            return size;
        }
        
        void CPaypalExpressCheckout::SetResponse(int methodId, string& response)
        {
            int outLen;
            response = curl_easy_unescape(_curl, response.c_str(), response.length(), &outLen);
           // LOGIP(response);
            //Success message: TOKEN=EC-3RK90253T5290774J&TIMESTAMP=2013-12-23T23:55:38Z&CORRELATIONID=1edb1c15b8934&ACK=Success&VERSION=86&BUILD=8951431
            //Error Message : TIMESTAMP=2013-12-24T02:51:12Z&CORRELATIONID=dc704acf3c296&ACK=Failure&VERSION=86&BUILD=8951431&L_ERRORCODE0=10002&L_SHORTMESSAGE0=Security error&L_LONGMESSAGE0=Security header is not valid&L_SEVERITYCODE0=Error
            FillResponse(response);
            return;
        }
        
        const bool CPaypalExpressCheckout::IsResponseError() 
        {
            ResponseData::const_iterator it;
            if ( (it = _responseData.find("ACK") ) != _responseData.end())
            {
                if (it->second == "Success")
                    return false;
            }
            return true;
        }
        
        const string& CPaypalExpressCheckout::GetResponseErrorMessage() 
        {
            if (IsResponseError())
            {
                return _responseData["L_LONGMESSAGE0"];
            }
            return _blank;
        }
        
        void CPaypalExpressCheckout::FillResponse(const string& response)
        {
            using namespace boost;
            _responseData.clear();
            typedef tokenizer< char_separator<char> > Tokenizer;
            typedef vector<string> TokenVector;
            TokenVector vec;
            char_separator<char> sep("&", "", keep_empty_tokens);
            Tokenizer tok(response, sep);
            vec.assign(tok.begin(), tok.end());
            for(string item : vec)
            {
                size_t t = item.find_first_of("=");
                if (t != string::npos)
                {
                    string key = item.substr(0, t);
                    string value = item.substr(++t);
                    int outLen;
                    value = curl_easy_unescape(_curl, value.c_str(),value.length(), &outLen);
                    _responseData.insert(make_pair(key, value));
                }
            }
        }
        
        ErrorId CPaypalExpressCheckout::Step01_SetExpressCheckout(CSTR returnUrl, CSTR cancelUrl, CSTR logoUrl, IString** ppToken)
        {
            // USER=insert_merchant_user_name_here
            // PWD=insert_merchant_password_here
            // SIGNATURE=insert_merchant_signature_value_here
            // METHOD : SetExpressCheckout
            // RETURNURL : http://www.smart-earn.com////
            // CANCELURL : ///
            // NOSHIPPING : 1
            // PAGESTYLE : TODO
            // HDRIMG : our https url for image shown on paypal page.
            // PAYFLOWCOLOR: background color 
            // LOGOIMG : https url for a logo image.
            // SOLUTIONTYPE: Mark
            // LANDINGPAGE: Login
            // CHANNELTYPE: Merchant
            // TOTALTYPE : Total
            // PAYMENTREQUEST_n_PAYMENTREASON = None // note Refund is the other value.
            // PAYMENTREQUEST_1_AMT: Amount figure (10$)
            // PAYMENTREQUEST_1_TAXAMT : tax amount
            // PAYMENTREQUEST_n_DESC : item description
            // PAYMENTREQUEST_n_INVNUM  : internal tracking number
            // PAYMENTREQUEST_n_TRANSACTIONID : paypal transaction id.
            // PAYMENTREQUEST_n_ALLOWEDPAYMENTMETHOD : InstantPaymentOnly
            // PAYMENTREQUEST_n_PAYMENTACTION : Sale
            // L_BILLINGTYPEn : RecurringPayments
            // BILLINGTYPE
            // L_BILLINGAGREEMENTDESCRIPTIONn
            // TOKEN
            
            try
            {
            
                if (!_curl)
                    throw seexception(E_InvalidPointer, "Curl is not initialized");
                
                ClearData();
                
                // add headers
                AddData("USER", _userName);  
                AddData("PWD", _password);
                AddData("SIGNATURE", _signature);
                AddData("METHOD", "SetExpressCheckout");
                AddData("RETURNURL", returnUrl);
                AddData("CANCELURL", cancelUrl);
                //AddData("NOSHIPPING", "1");
                if (logoUrl)
                    AddData("LOGOIMG", logoUrl);
                //AddData("SOLUTIONTYPE", "Mark");
                //AddData("LANDINGPAGE", "Login");
                //AddData("CHANNELTYPE", "Merchant");
                //AddData("TOTALTYPE", "Total");
                //AddData("PAYMENTREQUEST_0_PAYMENTREASON", "None");
                //AddData("PAYMENTREQUEST_0_AMT", "10.00");  // MUST BE FROM DB for the chosen product
                //AddData("PAYMENTREQUEST_0_DESC", "TODO FROM DB");
                //AddData("PAYMENTREQUEST_0_TAXAMT", "0.00");CreateRecurringPaymentsProfile
                AddData("VERSION", _version);
                AddData("L_BILLINGTYPE0","RecurringPayments");
                AddData("L_BILLINGAGREEMENTDESCRIPTION0", "Paid Subscription Membership");
                CURLcode res = curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, _data.c_str());
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set headers.");
                
                res = curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, CurlWriteDataFuncion1);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set write function.");
                
                res = curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set writedata function.");
                
                res = curl_easy_perform(_curl);
                if (res != CURLE_OK)
                {
                    throw seexception(E_UnknownError, curl_easy_strerror(res));
                }
                
                if (IsResponseError())
                    throw seexception(E_PaypalError, GetResponseErrorMessage().c_str());
                
                // return teh token
                THROWIFFAILED(CString::Create( (_token = _responseData["TOKEN"]).c_str(), ppToken), "Cannot create a string instance. Memory problem.");

                return S_Success;
                
            }
            CATCHCLAUSE("CPaypalExpressCheckout::Step01_SetExpressCheckout");
                
        }
        
        ErrorId CPaypalExpressCheckout::Step02_GetExpressCheckoutDetails(CSTR token, BOOL* HasAccepted, IUser *&pInOut, IString** PayerId)
        {
            try
            {
            
                if (!_curl)
                    throw seexception(E_InvalidPointer, "Curl is not initialized");
                
                // verify tokens are same 
                if (strcmp(token, _token.c_str()))
                {
                    throw seexception(E_PaypalError, "Toten saved doesn't match with token parameter");
                }
                
            /*
             * sample response:
            TOKEN=EC-6SD20542UT9356831&
            BILLINGAGREEMENTACCEPTEDSTATUS=1&
            CHECKOUTSTATUS=PaymentActionNotInitiated&
            TIMESTAMP=2013-12-31T03:59:35Z&
            CORRELATIONID=792fb939f0bc3&
            ACK=Success&
            VERSION=94&
            BUILD=8951431&
            EMAIL=jsantony@gmail.com&
            PAYERID=QZH3B9QHGRSSQ&
            PAYERSTATUS=verified&
            FIRSTNAME=Saji&
            LASTNAME=Antony&
            COUNTRYCODE=US&
            SHIPTONAME=Saji Antony&
            SHIPTOSTREET=1 Main St&
            SHIPTOCITY=San Jose&
            SHIPTOSTATE=CA&
            SHIPTOZIP=95131&
            SHIPTOCOUNTRYCODE=US&
            SHIPTOCOUNTRYNAME=United States&
            ADDRESSSTATUS=Confirmed&
            CURRENCYCODE=USD&
            AMT=0.00&
            SHIPPINGAMT=0.00&
            HANDLINGAMT=0.00&
            TAXAMT=0.00&
            INSURANCEAMT=0.00&
            SHIPDISCAMT=0.00&PAYMENTREQUEST_0_CURRENCYCODE=USD&PAYMENTREQUEST_0_AMT=0.00
            &PAYMENTREQUEST_0_SHIPPINGAMT=0.00&PAYMENTREQUEST_0_HANDLINGAMT=0.00&
            PAYMENTREQUEST_0_TAXAMT=0.00&PAYMENTREQUEST_0_INSURANCEAMT=0.00&
            PAYMENTREQUEST_0_SHIPDISCAMT=0.00&PAYMENTREQUEST_0_INSURANCEOPTIONOFFERED=false&
            PAYMENTREQUEST_0_SHIPTONAME=Saji Antony&PAYMENTREQUhttps://www.smart-earn.comEST_0_SHIPTOSTREET=1 Main St&
            PAYMENTREQUEST_0_SHIPTOCITY=San Jose&PAYMENTREQUEST_0_SHIPTOSTATE=CA&
            PAYMENTREQUEST_0_SHIPTOZIP=95131&PAYMENTREQUEST_0_SHIPTOCOUNTRYCODE=US&
            PAYMENTREQUEST_0_SHIPTOCOUNTRYNAME=United States&
            PAYMENTREQUEST_0_ADDRESSSTATUS=Confirmed&
            PAYMENTREQUESTINFO_0_ERRORCODE=0
             * 
             */                

                
                ClearData();
                
                // add headers
                AddData("USER", _userName);  
                AddData("PWD", _password);
                AddData("SIGNATURE", _signature);
                AddData("METHOD", "GetExpressCheckoutDetails");
                AddData("VERSION", _version);
                AddData("TOKEN",_token);
                CURLcode res = curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, _data.c_str());
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set headers.");
                
                res = curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, CurlWriteDataFuncion1);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set write function.");
                
                res = curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set writedata function.");
                
                res = curl_easy_perform(_curl);
                if (res != CURLE_OK)
                {
                    throw seexception(E_UnknownError, curl_easy_strerror(res));
                }
                
                if (IsResponseError())
                    throw seexception(E_PaypalError, GetResponseErrorMessage().c_str());
                
                // return teh token
                *HasAccepted = _responseData["BILLINGAGREEMENTACCEPTEDSTATUS"]=="1" ? true : false;
                string payerid = _responseData["PAYERID"];
                THROWIFFAILED(CString::Create(payerid.c_str(), PayerId), "Cannot create a string instance. Memory problem.");
                
                pInOut->set_FirstName( _responseData["FIRSTNAME"].c_str());
                pInOut->set_LastName( _responseData["LASTNAME"].c_str());
                SP<IAddress> pAddr;
                CreateAddress(get_Session(), &pAddr);
                pAddr->set_City(_responseData["SHIPTOCITY"].c_str());
                pAddr->set_State(_responseData["SHIPTOSTATE"].c_str());
                pAddr->set_Country(_responseData["SHIPTOCOUNTRYCODE"].c_str());
                pAddr->set_Street(_responseData["SHIPTOSTREET"].c_str());
                pAddr->set_PostalCode(_responseData["SHIPTOZIP"].c_str());
                // compare
                SP<IAddress> pAddr1;
                pInOut->get_Address(&pAddr1);
                if (!pAddr1 || !pAddr1->Equals(pAddr))
                    pInOut->set_Address(pAddr);
                
                _customerEmailAddress = _responseData["EMAIL"];
                
                return S_Success;
                
            }
            CATCHCLAUSE("CPaypalExpressCheckout::Step02_GetExpressCheckoutDetails");        
        }
        
        ErrorId CPaypalExpressCheckout::Step03_CreateRecurringPaymentsWithTrialProfile(IProduct* product, const char* payerId, DATETIME& startDate, IString** ppProfileId, IString** ppProfileStatus)
        {
            try
            {
                // if not token, then there is a problem.
                if (_token.empty())
                    throw seexception(E_PaypalError, "No token generated. Step01 and Step02 is must invoked prior to this call.");
                
                ClearData();
                
                AddData("USER", _userName);  
                AddData("PWD", _password);
                AddData("SIGNATURE", _signature);
                AddData("METHOD", "CreateRecurringPaymentsProfile");
                AddData("VERSION", _version);
                AddData("Token", _token);
                AddData("PAYERID", payerId);
                SP<IString> strdtstart;
                startDate.ToString(&strdtstart);
                AddData("PROFILESTARTDATE", strdtstart->get_Buffer());
               // LOGI(_logger, "PROFILESTARTDATE: " << strdtstart->get_Buffer());
                stringstream strMonthlyRate;
                strMonthlyRate.precision(2);
                strMonthlyRate << product->get_PriceMonthlyRate();
               // LOGI(_logger, "Monthly Bill: " << strMonthlyRate.str());
                AddData("AMT", strMonthlyRate.str());
                AddData("EMAIL", _customerEmailAddress);
                AddData("DESC", "Paid Subscription Membership");
                AddData("BILLINGPERIOD", "Month");
                AddData("BILLINGFREQUENCY", "1");
                AddData("TRIALBILLINGPERIOD", "Month");
                AddData("TRIALBILLINGFREQUENCY", "1");
                AddData("TRIALTOTALBILLINGCYCLES", "1");
                AddData("TRIALAMT", "0.00");
                AddData("CURRENCYCODE", "USD");
                AddData("COUNTRYCODE", "US");
                AddData("MAXFAILEDPAYMENTS", "3");
                
                CURLcode res = curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, _data.c_str());
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set headers.");
                
                res = curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, CurlWriteDataFuncion1);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set write function.");
                
                res = curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set writedata function.");
                
                res = curl_easy_perform(_curl);
                if (res != CURLE_OK)
                {
                    throw seexception(E_UnknownError, curl_easy_strerror(res));
                }
                
                if (IsResponseError())
                    throw seexception(E_PaypalError, GetResponseErrorMessage().c_str());
                
                THROWIFFAILED(CString::Create(_responseData["PROFILEID"].c_str(), ppProfileId), "Cannot create a string instance. Memory problem.");
                THROWIFFAILED(CString::Create(_responseData["PROFILESTATUS"].c_str(), ppProfileStatus), "Cannot create a string instance. Memory problem.");
                return S_Success;
            }
            CATCHCLAUSE("CPaypalExpressCheckout::Step03_CreateRecurringPaymentsProfile");        
        }
        
        ErrorId CPaypalExpressCheckout::Step0x1_CancelRecurringPaymentsProfileStatus(CSTR profileid, CSTR note, IString** ppProfileId )
        {
            return ManageRecurringPaymentsProfileStatus(profileid, "Cancel", note, ppProfileId);
        }

        ErrorId CPaypalExpressCheckout::Step0x2_SuspendRecurringPaymentsProfileStatus(CSTR profileid, CSTR note, IString** ppProfileId )
        {
            return ManageRecurringPaymentsProfileStatus(profileid, "Cancel", note, ppProfileId);
        }
        
        ErrorId CPaypalExpressCheckout::Step0x3_ReactivateRecurringPaymentsProfileStatus(CSTR profileid, CSTR note, IString** ppProfileId )
        {
            return ManageRecurringPaymentsProfileStatus(profileid, "Cancel", note, ppProfileId);
        }
        
        
        ErrorId CPaypalExpressCheckout::ManageRecurringPaymentsProfileStatus(CSTR profileid, CSTR action, CSTR note, IString** ppProfileId )
        {
            try
            {
                ClearData();
                AddData("USER", _userName);  
                AddData("PWD", _password);
                AddData("SIGNATURE", _signature);
                AddData("Method", "ManageRecurringPaymentsProfileStatus");
                AddData("VERSION", _version);
                AddData("PROFILEID", profileid);
                AddData("ACTION", action);
                AddData("NOTE", note);

                CURLcode res = curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, _data.c_str());
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set headers.");
                
                res = curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, CurlWriteDataFuncion1);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set write function.");
                
                res = curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);
                if (res != CURLE_OK)
                    throw seexception(E_UnknownError, "Curl failed to set writedata function.");
                
                res = curl_easy_perform(_curl);
                if (res != CURLE_OK)
                {
                    throw seexception(E_UnknownError, curl_easy_strerror(res));
                }
                if (IsResponseError())
                    throw seexception(E_PaypalError, GetResponseErrorMessage().c_str());
                
                THROWIFFAILED(CString::Create(_responseData["PROFILEID"].c_str(), ppProfileId), "Cannot create a string instance. Memory problem.");
                
                return S_Success;
            }
            CATCHCLAUSE("CPaypalExpressCheckout::ManageRecurringPaymentsProfileStatus");        
        }
    }
}
