/* 
 * File:   CEconomicNewsFeeds.h
 * Author: santony
 *
 * Created on April 10, 2014, 6:00 PM
 */

#ifndef CECONOMICNEWSFEEDS_H
#define	CECONOMICNEWSFEEDS_H
#include "../seglobal/CROList.h"
#include "seeconomapi.h"
#include "../semysql/CSEConnections.h"

using namespace se::data;

namespace se
{
    
    namespace economy
    {
        
        class CEconomicNewsFeeds;
        
        class CEconomicNewsFeed : public CObjectRoot, public IEconomicNewsFeed
        {
        public:
            CEconomicNewsFeed(ISession*);
            virtual ~CEconomicNewsFeed();
            
            virtual const char* get_Description();

            virtual const char* get_Title();

            virtual const char* get_Link();

            virtual long unsigned int get_Items(IEconomicNewsFeeds** ppOut);

            virtual const char* get_Language();

            virtual const DATETIME& get_PublishedOn();
            
            void set_Description(CSTR v);
            
            void set_Title(CSTR v);
            
            void set_Link(CSTR v);
            
            void set_Items(CEconomicNewsFeeds* pIn);
            
            void set_Language(CSTR v);
            
            void set_PublishedOn(const DATETIME& ts);

            virtual void Serialize(ISerializedData<IString>* pIn);
            
        private:
            std::string 
                _desc,
                _title,
                _link,
                _lang;
            SP<CEconomicNewsFeeds> _pitems;
            DATETIME _publishedOn;
                    

        };
        

        class CEconomicNewsFeeds : public CROSPList<IEconomicNewsFeed, IEconomicNewsFeeds>
        {
        public:
            CEconomicNewsFeeds(ISession*);
            virtual ~CEconomicNewsFeeds();

        };
    }
}
#endif	/* CECONOMICNEWSFEEDS_H */

