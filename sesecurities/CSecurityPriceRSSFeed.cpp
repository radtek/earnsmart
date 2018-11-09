/* 
 * File:   CSecurityPriceRSSFeed.cpp
 * Author: santony
 * 
 * Created on September 15, 2014, 7:21 PM
 */

#include "internal.h"
#include "CSecurityPriceRSSFeed.h"

namespace se
{
    namespace securities
    {

        CSecurityPriceRSSFeed::CSecurityPriceRSSFeed(ISession* ps) : CObjectRoot(ps) 
        {
        }

        CSecurityPriceRSSFeed::~CSecurityPriceRSSFeed() {
        }

        long unsigned int CSecurityPriceRSSFeed::Init(SPS source, CRSSItem* rssItem) 
        {
            _src = source;
            _feed = rssItem->get_Description();
            return S_OK;
        }


        SPS CSecurityPriceRSSFeed::get_Source() 
        {
            return _src;
        }
        
        const char* CSecurityPriceRSSFeed::get_Feed() 
        {
            return _feed.c_str();
        }

        
    }
    
}