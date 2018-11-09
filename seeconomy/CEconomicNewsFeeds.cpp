/* 
 * File:   CEconomicNewsFeeds.cpp
 * Author: santony
 * 
 * Created on April 10, 2014, 6:00 PM
 */

#include "seecconinternal.h"
#include "CEconomicNewsFeeds.h"

namespace se
{
    
    namespace economy
    {
        
        CEconomicNewsFeed::CEconomicNewsFeed(ISession* ps) : CObjectRoot(ps) 
        {
            
        }

        CEconomicNewsFeed::~CEconomicNewsFeed() 
        {
            
        }
        
        const char* CEconomicNewsFeed::get_Description()
        {
            return _desc.c_str();            
        }
        

        const char* CEconomicNewsFeed::get_Title()
        {
            return _title.c_str();
        }
        

        const char* CEconomicNewsFeed::get_Link()
        {
            return _link.c_str();
        }
        

        long unsigned int CEconomicNewsFeed::get_Items(IEconomicNewsFeeds** ppOut)
        {
            _pitems.CopyTo(ppOut);
            return _pitems._ptr ? S_OK : S_False;
        }
        

        const char* CEconomicNewsFeed::get_Language()
        {
            return _lang.c_str();
        }
        

        const DATETIME& CEconomicNewsFeed::get_PublishedOn()
        {
            return _publishedOn;
        }
        

        void CEconomicNewsFeed::set_Description(CSTR v)
        {
            _desc = v;
        }
        
        void CEconomicNewsFeed::set_Title(CSTR v)
        {
            _title = v;
        }
        
        void CEconomicNewsFeed::set_Link(CSTR v)
        {
            _link = v;
        }
        
        void CEconomicNewsFeed::set_Items(CEconomicNewsFeeds* pIn)
        {
            _pitems = pIn;
        }
        
        void CEconomicNewsFeed::set_Language(CSTR v)
        {
            _lang = v;
        }
        
        void CEconomicNewsFeed::set_PublishedOn(const DATETIME& ts)
        {
            _publishedOn = ts;
        }

        void CEconomicNewsFeed::Serialize(ISerializedData<IString>* pIn)
        {
            pIn->Add("Id", get_Id());
            pIn->Add("Description", get_Description());
            pIn->Add("Link", get_Link());
            pIn->Add("Language", get_Language());
            pIn->Add("Title", get_Title());
            SP<IString> pdt;
            _publishedOn.ToString(&pdt);
            pIn->Add("PublishedOn", pdt?pdt->get_Buffer():"");
            pIn->Add("Items", _pitems);
        }

        ////////////////////////////////CEconomicNewsFeeds/////////////////////////////////
        
        CEconomicNewsFeeds::CEconomicNewsFeeds(ISession* ps): CROSPList<IEconomicNewsFeed, IEconomicNewsFeeds>(ps) 
        {
        }


        CEconomicNewsFeeds::~CEconomicNewsFeeds() 
        {
        }


   }
}