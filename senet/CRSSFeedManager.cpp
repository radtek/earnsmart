/* 
 * File:   CRSSFeedManager.cpp
 * Author: santony
 * 
 * Created on April 10, 2014, 6:45 PM
 */

#include "../seglobal/Object.h"
#include "senetinternal.h"
//#include <log4cxx/logger.h>
//#include <log4cxx/propertyconfigurator.h>
#include "../seglobal/exceptions.h"
#include "CRSSFeedManager.h"
#include <string.h>

namespace se
{
    namespace net
    {
        //LoggerPtr _logger = Logger::getLogger("se.net.rss");
        
        /////////////////////////////////CRSSChannel/////////////////////

        CRSSChannel::CRSSChannel(ISession* ps) : CObjectRoot(ps), _pCurrItem(0)
        {

        }

        
        CRSSChannel::~CRSSChannel()
        {
            // not required to free the items
            // only required to free the channel instance. 
            /*
            mrss_item_t* item = _channel->item;
            while (item->next)
            {
                mrss_item_t* itemnext = item->next;
                mrss_free(item);
                item = itemnext;
            }
             * */
            mrss_free(_channel);
        }

        long unsigned int CRSSChannel::Init(mrss_t* channel) 
        {
            _channel = channel;
            _pCurrItem = _channel->item;
            return S_OK;
        }

        const char* CRSSChannel::get_Title() 
        {
            return _channel->title;
        }
        
        CSTR CRSSChannel::get_Description()
        {
            if (!_channel->description)
                return get_OtherTag("description");
            return _channel->description;
        }
        
        CSTR CRSSChannel::get_Language()
        {
            return _channel->language;
        }
        
        CSTR CRSSChannel::get_Link()
        {
            return _channel->link;
        }
        
        const DATETIME& CRSSChannel::get_PublishedDate()
        {
            if (_pubdate.IsEmpty())
            {
                try
                {
                    _pubdate = DATETIME::Parse(_channel->pubDate?_channel->pubDate:_channel->lastBuildDate, "%a, %d %b %Y %H:%M:%S %z");
                }
                catch(std::exception& ex)
                {
                   // LOGE(_logger, "Failed to parse publish date with error " << ex.what());
                }
            }
            return _pubdate;
        }

        const DATETIME& CRSSChannel::get_PublishedDate(const char* format) 
        {
            if (_pubdate.IsEmpty())
            {
                try
                {
                    _pubdate = DATETIME::Parse(_channel->pubDate?_channel->pubDate:_channel->lastBuildDate, format);
                }
                catch(std::exception& ex)
                {
                   // LOGE(_logger, "Failed to parse publish date with error " << ex.what());
                }
            }
            return _pubdate;
        }

        const DATETIME& CRSSChannel::get_PublishedDate(const char* dateTag, const char* format) 
        {
            if (_pubdate.IsEmpty())
            {
                try
                {
                    _pubdate = DATETIME::Parse(get_OtherTag(dateTag) , format);
                }
                catch(std::exception& ex)
                {
                   // LOGE(_logger, "Failed to parse publish date with error " << ex.what());
                }
            }
            return _pubdate;

        }
        
        
        CSTR CRSSChannel::get_Contributor()
        {
            return _channel->contributor;
        }
        
        CSTR CRSSChannel::get_ContributorUrl()
        {
            return _channel->contributor_uri;
        }
        
        RSSVersion CRSSChannel::get_Version()
        {
            return _channel->version;
        }

        const DATETIME& CRSSChannel::get_Skip() 
        {
            if (_channel->skipDays)
                _skip.Day = strtol(_channel->skipDays->day, 0, 10);
            if (_channel->skipHours)
                _skip.Hour = strtol(_channel->skipHours->hour, 0, 10);;
            return _skip;
        }
        
        
        bool CRSSChannel::Enumerate(CRSSItem** ppItem)
        {
            if (!_pCurrItem)
                return false;
            
            if (FAILED(CObject<CRSSItem>::Create(get_Session(), _pCurrItem, ppItem)))
                return false;
            
            _pCurrItem = _pCurrItem->next;
            
            return true;
            
        }
        
        CSTR CRSSChannel::get_OtherTag(const char* tagName)
        {
            mrss_tag_t *tag = _channel->other_tags;
            while (tag)
            {
                if (strcmp(tag->name, tagName))
                    return tag->value;
                tag = tag->next;
            }
            return "";
        }
        
        //////////////////////////////////CRSSItem//////////////////////////

        CRSSItem::CRSSItem(ISession* ps) : CObjectRoot(ps)
        {
            
        }
        
        CRSSItem::~CRSSItem()
        {
            
        }

        long unsigned int CRSSItem::Init(mrss_item_t* item) 
        {
            _item = item;
            return S_OK;
        }
        
        const char* CRSSItem::get_Title() 
        {
            return _item->title;
        }
        
        CSTR CRSSItem::get_Description()
        {
            if (!_item->description)
                return get_OtherTag("description");
            return _item->description;
        }
        
        CSTR CRSSItem::get_Link()
        {
            return _item->link;
        }
        
        const DATETIME& CRSSItem::get_PublishedDate()
        {
            if (_pubdate.IsEmpty())
            {
                try
                {
                    if (_pubdate.IsEmpty())
                        _pubdate = DATETIME::Parse(_item->pubDate, "%a, %d %b %Y %H:%M:%S %z");
                }
                catch(std::exception& ex)
                {
                   // LOGE(_logger, "Failed to parse publish date with error " << ex.what() << ". Pubdate: " << _item->pubDate << ". Format used: " << "%a, %d %b %Y %H:%M:%S %z");
                }
            }
            return _pubdate;
        }

        const DATETIME& CRSSItem::get_PublishedDate(const char* format) 
        {
            if (_pubdate.IsEmpty())
            {
                try
                {
                    if (_pubdate.IsEmpty())
                        _pubdate = DATETIME::Parse(_item->pubDate, format);
                }
                catch(std::exception& ex)
                {
                   // LOGE(_logger, "Failed to parse publish date with error " << ex.what() << ". Pubdate: " << _item->pubDate << ". Format used: " << format);
                }
            }
            return _pubdate;
        }

        const DATETIME& CRSSItem::get_PublishedDate(const char* dateTag, const char* format) 
        {
            if (_pubdate.IsEmpty())
            {
                try
                {
                    if (_pubdate.IsEmpty())
                        _pubdate = DATETIME::Parse(get_OtherTag(dateTag), format);
                }
                catch(std::exception& ex)
                {
                   // LOGE(_logger, "Failed to parse publish date with error " << ex.what() << ". Pubdate: " << _item->pubDate << ". Format used: " << format);
                }
            }
            return _pubdate;
        }
        
        
        CSTR CRSSItem::get_Contributor()
        {
            return _item->contributor;
        }
        
        CSTR CRSSItem::get_ContributorUrl()
        {
            return _item->contributor_uri;
        }

        const char* CRSSItem::get_OtherTag(const char* tagName) 
        {
            mrss_tag_t *tag = _item->other_tags;
            while (tag)
            {
                if (!strcasecmp(tag->name, tagName))
                    return tag->value;
                tag = tag->next;
            }
            return "";
        }

        

        /////////////////////////////////////CRSSFeedManager///////////////////

        long unsigned int CRSSFeedManager::ParseUrl(ISession* ps, const char* url, CRSSChannel** ppOut) 
        {
            //log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("CRSSFeedManager::ParseUrl");
            mrss_t* channel;
            mrss_error_t retCode = mrss_parse_url((char*)url, &channel);
            if (MRSS_OK == retCode)
                return CObject<CRSSChannel>::Create(ps, channel, ppOut);
            //LOG4CXX_ERROR(logger, mrss_strerror(retCode));
            return E_RSSError;
        }
        
        SE_CODE CRSSFeedManager::ParseString(ISession* ps, const char* rssdata, CRSSChannel** ppOut)
        {
            //log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("CRSSFeedManager::ParseString");
            size_t bufferLen;
            if (!rssdata || (bufferLen = strlen(rssdata)) == 0)
            {
                //LOG4CXX_ERROR(logger, "Invalid rssdata parameter. Must be valid string.");
                return E_InvalidArg;
            }
            
            mrss_t* channel;
            mrss_error_t retCode = mrss_parse_buffer((char*)rssdata, bufferLen, &channel);
            if (MRSS_OK == retCode)
                return CObject<CRSSChannel>::Create(ps, channel, ppOut);
            
            //LOG4CXX_ERROR(logger, mrss_strerror(retCode));
            return E_RSSError;
            
        }
        
    }
}
