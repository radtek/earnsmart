/* 
 * File:   CSecurityNews.cpp
 * Author: santony
 * 
 * Created on September 4, 2014, 8:27 PM
 */

#include <tidy/tidyenum.h>


#include <tidy/platform.h>


#include <tidy/tidy.h>


#include "internal.h"
#include "CSecurityNews.h"
#include "../senet/CRSSFeedManager.h"
#include "Security.h"
#include <curl/curl.h>
#include "../senet/senetapi.h"

namespace se
{
    namespace securities
    {
        
        // http://articlefeeds.nasdaq.com/nasdaq/symbols?symbol=UBNT

        CSecurityNews::CSecurityNews(ISession* ps) : CObjectRoot(ps) 
        {
        }

        CSecurityNews::~CSecurityNews() 
        {
        }

        long unsigned int CSecurityNews::Init(const char* symbol) 
        {
            return S_OK;
        }
        
        long unsigned int CSecurityNews::Init(const char* symbol, CRSSItem* rssItem) 
        {
            set_RSSItem(rssItem);
            return S_OK;
        }

        void CSecurityNews::set_RSSItem(CRSSItem* p) 
        {
            _dt = p->get_PublishedDate();
            _title = p->get_Title();
            _desc = p->get_Description();
            _link = p->get_Link();
        }

        const char* CSecurityNews::get_Link() 
        {
            return _link.c_str();
        }
        
        const DATETIME& CSecurityNews::get_Date() 
        {
            return _dt;
        }
        
        const char* CSecurityNews::get_NewsContent(bool* pbIshtml) 
        {
            if (pbIshtml)
                *pbIshtml = true;
            
            if (_newscontent.empty())
            {
                // we are going to use tidyhtml for extracting news html from source website and
                // store in _newscontent.cstr;
                if (FAILED(DownloadNewsContent()))
                {
                    return nullptr;
                }
            }
            return _newscontent.c_str();
        }
        
        const char* CSecurityNews::get_NewsPrecis(bool* pbIshtml) 
        {
            if (pbIshtml)
                *pbIshtml = true;
            return _desc.c_str();
        }
        
        const char* CSecurityNews::get_Title(bool* pbIshtml) 
        {
            if (pbIshtml)
                *pbIshtml = true;
            return _title.c_str();
        }

        SNS CSecurityNews::get_Source() 
        {
            return _source;
        }

        void CSecurityNews::set_Source(SNS source) 
        {
            _source = source;
        }
        
        void CSecurityNews::Serialize(ISerializedData<IString>* pIn)
        {
            
        }
        
        size_t CSecurityNews::WriteHtmlBuffer(char *in, size_t size, size_t nmemb, TidyBuffer *out)
        {
          size_t r = size * nmemb;
          tidyBufAppend(out, in, r );
          return r;
        }        
        
        
        SE_CODE CSecurityNews::DownloadNewsContent()
        {
            SE_CODE retCode;
            CURL* pcurl;
            char curl_errbuf[CURL_ERROR_SIZE];
            TidyDoc tdoc;
            TidyBuffer docbuf = {0};
            TidyBuffer tidy_errbuf = {0};
            int err;
            pcurl = curl_easy_init();
            if (!pcurl)
                return E_MemoryUnavailable;
            curl_easy_setopt(pcurl, CURLOPT_URL, _link.c_str());
            curl_easy_setopt(pcurl, CURLOPT_ERRORBUFFER, curl_errbuf);
            curl_easy_setopt(pcurl, CURLOPT_NOPROGRESS, 0L);
            //curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, WriteHtmlBuffer);
            curl_easy_setopt(pcurl, CURLOPT_FOLLOWLOCATION, true);
            tdoc = tidyCreate();
            tidyOptSetBool(tdoc, TidyForceOutput, yes);
            tidyOptSetInt(tdoc, TidyWrapLen, 10000);
            tidySetErrorBuffer(tdoc, &tidy_errbuf);
            tidyBufInit(&docbuf);
            
            curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, &docbuf);
            err = curl_easy_perform(pcurl);
            
            if (err)
                return E_HttpError;
            
            err = tidyParseBuffer(tdoc, &docbuf);
            if (err  < 0)
                return E_HtmlParseError;
            err = tidyCleanAndRepair(tdoc);
            if (err < 0)
                return E_HtmlParseError;

            retCode = RecurseParseHtmlDoc(tdoc, tidyGetRoot(tdoc));
            
            curl_easy_cleanup(pcurl);
            tidyBufFree(&docbuf);
            tidyBufFree(&tidy_errbuf);
            tidyRelease(tdoc);
            
            return retCode;
            
        }

        const string articleId = "articleText";
        const string tag = "div";

        SE_CODE CSecurityNews::RecurseParseHtmlDoc(TidyDoc d, TidyNode n)
        {
            TidyNode child;
            for (child = tidyGetChild(n); child; child = tidyGetNext(child))
            {
                TidyTagId tagId = tidyNodeGetId(child);
                if (tagId == TidyTag_DIV)
                {
                    // make sure name is articleId
                    TidyAttr attr = tidyAttrGetById(child, TidyAttr_ID);
                    if (attr)
                    {
                        string attrIdVal = tidyAttrValue(attr);
                        if (attrIdVal == articleId)
                            return ExtractContent(d, child);
                    }
                }
                SE_CODE ret = RecurseParseHtmlDoc(d, child);
                if (ISOK(ret))
                    ret;
            }
        }

        SE_CODE CSecurityNews::ExtractContent(TidyDoc d, TidyNode n)        
        {
            TidyBuffer buf;
            tidyBufInit(&buf);
            if (tidyNodeGetText(d, n, &buf))
            {
                _newscontent = (CSTR)buf.bp;
                tidyBufFree(&buf);
                return S_OK;
            }
            else
                return E_HtmlParseError;
        }
    
        //////////////////////////////////////////////////////////////////
        CSecurityNewsList::CSecurityNewsList(ISession* ps): CSPList<ISecurityNews, ISecurityNewsList>(ps)
        {

        }
        
        CSecurityNewsList::~CSecurityNewsList()
        {
            
        }

        void CSecurityNewsList::Add(ISecurityNews* item) 
        {
            auto size = _vector.size();
            item->set_Id( size );
            Base::Add(item);
        }


        SE_CODE SECEXP ISecurityNewsList::Create(ISession* ps, ISecurityNewsList** ppOut)
        {
            return CObject<CSecurityNewsList>::Create(ps, ppOut);
        }
        
    }
    
}