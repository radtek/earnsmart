/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ISessionInterface.h
 * Author: santony
 *
 * Created on December 28, 2015, 12:48 PM
 */

#ifndef ISESSIONINTERFACE_H
#define ISESSIONINTERFACE_H

#include "../seadmin/seadminapi.h"
#include "../seeconomy/seeconomapi.h"
#include "../seeoddata/seeoddata.h"
#include "../sesecurities/sesecuritiesapi.h"
#include "../sexbrl/sexbrlapi.h"

using namespace se;
using namespace se::securities;
using namespace se::admin;
using namespace se::economy;
using namespace se::xbrl;
using namespace se::eoddata;

namespace se
{

    struct ISessionInterface : public ISession
    {
        virtual RP<IUser> get_User() = 0;
        virtual SE_CODE Reset(IUser*) = 0;
        virtual RP<ISecurityAPI> get_SecurityApi() = 0;
        virtual RP<IXbrl> get_XbrlApi()=0;
        virtual RP<IEconomyApi> get_EconomyApi()=0;
        virtual RP<IEODData> get_EodDataApi()=0;
        virtual RP<IAdminAPI> get_AdminApi()=0;


    };

}

#endif /* ISESSIONINTERFACE_H */

