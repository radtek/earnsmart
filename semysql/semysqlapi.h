/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   semysqlapi.h
 * Author: santony
 *
 * Created on December 30, 2015, 8:32 PM
 */

#ifndef SEMYSQLAPI_H
#define SEMYSQLAPI_H

#include "../seglobal/IObject.h"

#define MSQLEXP __attribute__ ((visibility ("default")))

namespace se
{
    namespace data
    {
        
       
        struct IStoreQueryResult : public se::IObject
        {
            virtual mysqlpp::StoreQueryResult& get_Result() = 0;
            static SE_CODE MSQLEXP GetDataTable(ISession*, CSTR query, IStoreQueryResult** ppOut);
            static SE_CODE MSQLEXP Create(ISession* ps, mysqlpp::StoreQueryResult&& result, IStoreQueryResult**) ;
        };
    }
}

#endif /* SEMYSQLAPI_H */

