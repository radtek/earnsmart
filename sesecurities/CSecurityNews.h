/* 
 * File:   CSecurityNews.h
 * Author: santony
 *
 * Created on September 4, 2014, 8:27 PM
 */

#ifndef CSECURITYNEWS_H
#define	CSECURITYNEWS_H

#include "sesecuritiesapi.h"
#include "../seglobal/CList.h"
#include "../senet/CRSSFeedManager.h"

#include <tidy/tidy.h>
#include <tidy/buffio.h>

using namespace se::net;

namespace se
{
    namespace securities
    {
        class CSecurityNews : public CObjectRoot, public ISecurityNews
        {
        public:
            CSecurityNews(ISession*);
            CSecurityNews(const CSecurityNews& orig)=delete;
            virtual ~CSecurityNews();

            virtual const DATETIME& get_Date();

            virtual const char* get_NewsContent(bool* pbIshtml = 0);

            virtual const char* get_NewsPrecis(bool* pbIshtml = 0);

            virtual const char* get_Title(bool* pbIshtml = 0);

            virtual CSTR get_Link();
            
            virtual SNS get_Source();
            
            virtual void set_Source(SNS source);
            
            virtual void Serialize(ISerializedData<IString>* pIn);

            SE_CODE Init(CSTR symbol);

            SE_CODE Init(CSTR symbol, CRSSItem* rssItem);
            
            void set_RSSItem(CRSSItem* p);
            
        private:
            // Downloads the news html content
            // as a tidy document.
            SE_CODE DownloadNewsContent();
            // recursively parses from root node until
            // news content node is found. Then calls
            // ExtractContent to get the content.
            SE_CODE RecurseParseHtmlDoc(TidyDoc, TidyNode);
            // extracts news content and saves in the content variable.
            SE_CODE ExtractContent(TidyDoc, TidyNode);
        
            static size_t WriteHtmlBuffer(char *in, size_t size, size_t nmemb, TidyBuffer *out);

        private:
            SNS _source;
            string _title, _newscontent, _desc, _link;
            DATETIME _dt;
        };
        
        
        ////////////////////CSecurityNewsList/////////////////
        class CSecurityNewsList : public CSPList<ISecurityNews, ISecurityNewsList>
        {
            typedef CSPList<ISecurityNews, ISecurityNewsList> Base;

        public:
            CSecurityNewsList(ISession*);
            virtual ~CSecurityNewsList();
            virtual void Add(ISecurityNews* item);

        };

    }
}

#endif	/* CSECURITYNEWS_H */

