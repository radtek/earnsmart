/* 
 * File:   CSecurityPriceRSSFeed.h
 * Author: santony
 *
 * Created on September 15, 2014, 7:21 PM
 */

#ifndef CSECURITYPRICERSSFEED_H
#define	CSECURITYPRICERSSFEED_H

#include "sesecuritiesapi.h"
#include "../senet/CRSSFeedManager.h"

using namespace se::net;

namespace se
{
    namespace securities
    {

        class CSecurityPriceRSSFeed : public CObjectRoot, public ISecurityPriceRSSFeed
        {
        public:
            CSecurityPriceRSSFeed(ISession*);
            CSecurityPriceRSSFeed(const CSecurityPriceRSSFeed& orig)=delete;
            virtual ~CSecurityPriceRSSFeed();

            long unsigned int Init(SPS source, CRSSItem* rssItem);
            
            virtual SPS get_Source();
            virtual const char* get_Feed();

        private:
            string _feed;
            SPS _src;

        };

    }
    
}
#endif	/* CSECURITYPRICERSSFEED_H */

