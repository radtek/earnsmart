/* 
 * File:   CPurchase.cpp
 * Author: aashok
 * 
 * Created on August 31, 2013, 3:04 PM
 */

#include "internal.h"
#include "CPurchase.h"
#include "CCustomer.h"

namespace se
{
    namespace admin
    {
        CPurchase::CPurchase(ISession* ps) : CObjectRoot(ps)
        {
            
        }


        CPurchase::~CPurchase() 
        {
            
        }
        
        ErrorId IPurchase::Create(ISession* ps, IPurchase** ppOut)
        {
            return CObject<CPurchase>::Create(ps, ppOut);
        }
        
//        ICustomer* CPurchase::get_Customer()
//        {
//            SP<ICustomer> pCustomer;
//            ErrorId retCode = CObject<CCustomer>::Create(&pCustomer);
//            SP<IAdminAPI> api;
//            ErrorId ret = AdminOpen(&api);
//            if (FAILED(ret))
//            {
//               // LOGEP("Failed to open admin api.");
//                return nullptr;
//            }
//            ret = api->get_Customer(_customerId,&pCustomer);
//            if (FAILED(ret))
//            {
//               // LOGEP("Failed to Find Customer.");
//                return nullptr;
//            }
//            return pCustomer;
//        }
        
        ErrorId CPurchase::get_Customer(ICustomer** ppOut)
        {
/*
             SP<IAdminAPI> api;
            ErrorId ret = AdminOpen(get_Session(), &api);
            if (FAILED(ret))
            {
               // LOGEP("Failed to open admin api.");
                return ret;
            }
            ret = api->get_Customer(_customerId,ppOut);
            if (FAILED(ret))
            {
               // LOGEP("Failed to Find Customer.");
                return ret;
            }
            return ret;
*/
		return E_NotImpl;
        }
        
        ErrorId CPurchase::get_Product(IProduct** ppOut)
        {
            SP<IAdminAPI> api;
            ErrorId ret = AdminOpen(get_Session(), &api);
            if (FAILED(ret))
            {
               // LOGEP("Failed to open admin api.");
                return ret;
            }
            ret = api->get_Product(_productId, ppOut);
            if (FAILED(ret))
            {
               // LOGEP("Failed to Find Product.");
                return ret;
            }
            return ret;
        }
        
        ErrorId CPurchase::get_SubscriptionInfo(ICustomerSubscriptionInfo** ppOut)
        {
            SP<IAdminAPI> api;
            ErrorId ret = AdminOpen(get_Session(), &api);
            if (FAILED(ret))
            {
               // LOGEP("Failed to open admin api.");
                return ret;
            }
//            ret = api->get_CustomerSubscriptionInfo(_subscriptionInfoId, ppOut);
  //          if (FAILED(ret))
    //        {
      //         // LOGEP("Failed to Find Subscription Info.");
        //        return ret;
          //  }
            return ret;
        }
        
       
        
        CDate CPurchase::get_PurchaseDate()
        {
            return _purchaseDate;
        }
        
        R4 CPurchase::get_Amount()
        {
            return _amount;
        }
        
        ErrorId CPurchase::get_PurchaseType(ICustomerSubscriptionType** ppOut)
        {
            SP<IAdminAPI> api;
            ErrorId ret = AdminOpen(get_Session(), &api);
            if (FAILED(ret))
            {
               // LOGEP("Failed to open admin api.");
                return ret;
            }
            //ret = api->get_CustomerSubscriptionType(_subscriptionType, ppOut);
            if (FAILED(ret))
            {
               // LOGEP("Failed to Find Subscription Type.");
                return ret;
            }
            return ret;
        }
        
        void CPurchase::set_CustomerId(UID id)
        {
            _customerId = id;
        }
        
        void CPurchase::set_ProductId(UID id)
        {
            _productId = id;
        }
        
        void CPurchase::set_PurchaseTypeId(UID id)
        {
            _subscriptionInfoId = id;
        }
        
        void CPurchase::set_PurchaseDate(CDate const& value)
        {
            _purchaseDate= value;
        }
        
        void CPurchase::set_Amount(R4 value)
        {
            _amount = value;
        }
        
        void CPurchase::set_CustomerSubscriptionInfoId(UID value)
        {
             _subscriptionInfoId = value;
        }
        
        void CPurchase::Serialize(ISerializedData<IString>* pIn)
        {
            //pIn->Add("Customer", )
        }
    }
}



