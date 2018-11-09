/* 
 * File:   CRSSFeedManager.h
 * Author: santony
 *
 * Created on April 10, 2014, 6:45 PM
 */

#ifndef CRSSFEEDMANAGER_H
#define	CRSSFEEDMANAGER_H

#include "senetapi.h"
#include <mrss.h>

namespace se
{
    namespace net
    {
        typedef mrss_version_t RSSVersion;
        
        //wrapper for mrss_item_t
        class CRSSItem : public CObjectRoot, public IObject
        {
        private:
            mrss_item_t* _item;
            DATETIME _pubdate;
            DATETIME _skip;
        public:
            CRSSItem(ISession*);
            virtual ~CRSSItem();
            virtual long unsigned int Init(mrss_item_t* item);
            
            CSTR get_Title();
            CSTR get_Description();
            CSTR get_Link();
            const DATETIME& get_PublishedDate();
            const DATETIME& get_PublishedDate(CSTR format);
            const DATETIME& get_PublishedDate(CSTR dateTag, CSTR format);
            CSTR get_Contributor();
            CSTR get_ContributorUrl();
            CSTR get_OtherTag(CSTR tagName);

        };
        
        
        // wrapper for mrss_t
        class CRSSChannel : public CObjectRoot, public IObject
        {
        private:
            mrss_t* _channel;
            DATETIME _pubdate;
            mrss_item_t* _pCurrItem;
            DATETIME _skip;
            
        public:
            CRSSChannel(ISession*);
            virtual ~CRSSChannel();
            
            virtual long unsigned int Init(mrss_t* channel);
            
            CSTR get_Title();
            CSTR get_Description();
            CSTR get_Link();
            CSTR get_Language();
            CSTR get_Copyright();
            const DATETIME& get_PublishedDate();
            const DATETIME& get_PublishedDate(CSTR format);
            const DATETIME& get_PublishedDate(CSTR dateTag, CSTR format);
            CSTR get_Contributor();
            CSTR get_ContributorUrl();
            RSSVersion get_Version();
            const DATETIME& get_Skip();
            CSTR get_OtherTag(CSTR tagName);
            
            // enumerate through items until this 
            // method returns false.
            bool Enumerate(CRSSItem** ppItem);
            
        };
        
        class CRSSFeedManager 
        {
        public:
            static SE_CODE ParseUrl(ISession*, CSTR url, CRSSChannel** ppOut);
            static SE_CODE ParseString(ISession*, CSTR rssdata, CRSSChannel** ppOut);
        };
    }
}

#endif	/* CRSSFEEDMANAGER_H */

