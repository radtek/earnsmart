/* 
 * File:   COptionFeeder.cpp
 * Author: santony
 * 
 * Created on August 11, 2014, 11:48 PM
 */

#include "internal.h"
#include "COptionFeeder.h"
#include "COptionYahooFeeder.h"

namespace se
{
    namespace securities
    {

        COptionFeeder::COptionFeeder(ISession* ps):CObjectRoot(ps) 
        {
        }

        COptionFeeder::~COptionFeeder() 
        {
        }
        
        ////////////////////////////////// creation /////////////////////////////////////////
        SE_CODE SECEXP IOptionFeeder::GetFeeder(ISession* ps, IOptionFeeder** ppOut)
        {
            return GetFeeder(ps, IOptionFeeder::Yahoo, ppOut);
        }
        
        SE_CODE SECEXP IOptionFeeder::GetFeeder(ISession* ps, Feeders feeder, IOptionFeeder** ppOut)
        {
            switch (feeder)
            {
                case IOptionFeeder::Yahoo:
                    return CObject<COptionYahooFeeder>::Create(ps, ppOut);
                default:
                    return E_NotSupported;
            }
        }
    }
}