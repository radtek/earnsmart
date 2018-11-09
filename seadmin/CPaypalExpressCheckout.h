/* 
 * File:   CPaypalExpressCheckout.h
 * Author: santony
 *
 * Created on December 22, 2013, 12:27 AM
 */

#ifndef CPAYPALEXPRESSCHECKOUT_H
#define	CPAYPALEXPRESSCHECKOUT_H

#include "CPaypal.h"
#include "seadminapi.h"
#include <map>

using namespace std;

namespace se
{
    namespace admin
    {

        class CPaypalExpressCheckout : public CPaypal, public CObjectRoot, public IPaypalExpressCheckout
        {
        public:
            CPaypalExpressCheckout(ISession*);
            CPaypalExpressCheckout(const CPaypalExpressCheckout& orig)=delete;
            virtual ~CPaypalExpressCheckout();
            virtual ErrorId Init() ;
            
            virtual ErrorId Step01_SetExpressCheckout(CSTR returnUrl, CSTR cancelUrl, CSTR logoUrl, IString** ppToken);
            virtual ErrorId Step02_GetExpressCheckoutDetails(CSTR token, BOOL* HasAccepted, IUser *&pInOut, IString** PayerId);
            virtual ErrorId Step03_CreateRecurringPaymentsWithTrialProfile(IProduct* product, CSTR payerId, DATETIME& startDate, IString** ppProfileId, IString** ppProvileStatus);
            virtual ErrorId Step0x1_CancelRecurringPaymentsProfileStatus(CSTR profileid, CSTR note, IString** ppProfileId );
            virtual ErrorId Step0x2_SuspendRecurringPaymentsProfileStatus(CSTR profileid, CSTR note, IString** ppProfileId );
            virtual ErrorId Step0x3_ReactivateRecurringPaymentsProfileStatus(CSTR profileid, CSTR note, IString** ppProfileId );
            //TODO virtual ErrorId UpdateRecurringPaymentsProfile(CSTR profileId, CDate& profileStartDate, CSTR subscriberName, CSTR note, R8 amount = 0.0, R8 taxAmount = 0.0);
            
        private:
            // methodId (step01 = 1, step02 = 2, and so on...)
            void SetResponse(int methodId, string& response);
            static size_t CurlWriteDataFuncion1( char *ptr, size_t size, size_t nmemb, void *userdata);
            void FillResponse(const string& response);
            const bool IsResponseError() ;
            const string& GetResponseErrorMessage();
            
            ErrorId ManageRecurringPaymentsProfileStatus(CSTR profileid, CSTR action, CSTR note, IString** ppProfileId );
            
        private:
            string _blank;
            typedef map<string, string> ResponseData;
            ResponseData _responseData;
            string _token;
            string _version;
            string _customerEmailAddress;
            
        };

    }
}

#endif	/* CPAYPALEXPRESSCHECKOUT_H */

