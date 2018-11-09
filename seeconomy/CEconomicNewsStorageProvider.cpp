/* 
 * File:   CEconomicNewsStorageProvider.cpp
 * Author: santony
 * 
 * Created on April 10, 2014, 6:29 PM
 */

//#include <log4cxx/propertyconfigurator.h>

#include "seecconinternal.h"
#include "CEconomicNewsFeeds.h"
#include "CEconomicNewsStorageProvider.h"
#include "../senet/CRSSFeedManager.h"
#include "../semysql/CSEConnections.h"
#include "../semysql/CMyqlUtilities.h"
#include <boost/algorithm/string.hpp>
#include "../seglobal/exceptions.h"

using namespace se::net;
using namespace se::data;
using namespace boost;

namespace se
{
    namespace economy
    {
        CEconomicNewsStorageProvider::CEconomicNewsStorageProvider(ISession* ps) : _ps(ps)   
            //_logger(Logger::getLogger("se.economy.CEconomicNewsStorageProvider"))
        {
            
        }

        CEconomicNewsStorageProvider::~CEconomicNewsStorageProvider() 
        {
        }
        
        
        void CEconomicNewsStorageProvider::ProcessRssFeed(UID feedId, const string& feedurl, const string& dateformat, const string& tags)
        {
           // LOGI(_logger, "Processing feedinfo with id " << feedId);
            try
            {
                SP<CRSSChannel> channel;
                if (ISOK(CRSSFeedManager::ParseUrl(_ps, feedurl.c_str(), &channel)))
                {
                    CSmartConnection conn(SEDBCONN);
                    while (true)
                    {
#ifdef __DEBUG__
                        if (feedId == 12)
                        {
                            string s = "break";
                        }
#endif
                        SP<CRSSItem> item;
                        if (!channel->Enumerate(&item))
                            break;
                        string datefield;
                        char* cztags = (STR)tags.c_str();
                        if (!tags.empty())
                        {
                            // tokenize tags by ;
                            string tok = (CSTR)strtok(cztags, ";");
                            // get string up to =
                            string name = tok.substr(0, tok.find("=", 0));
                            string val = tok.substr(tok.find("=", 0) + 1);
                            if (name == "Date")
                                datefield = val;
                        }

                        DATETIME dt ;
                        if (!dateformat.empty() && !datefield.empty())
                            dt = item->get_PublishedDate(datefield.c_str(), dateformat.c_str());
                        else if (!dateformat.empty())
                            dt = item->get_PublishedDate(dateformat.c_str());
                        else
                            dt = item->get_PublishedDate();

                        // time to store if pub date is ok.
                        sql_datetime sqldt;
                        CMyqlUtilities::PopulateDateTime(dt, sqldt);
                        Query q = conn->query();
                        q << "select rss_date from se_eco_rssfeeds_data where rss_id = " << feedId;
                        q << " and rss_link = " << quote << item->get_Link();
                        StoreQueryResult sqr = q.store();
                        sql_datetime rssdate;
                        bool bOk = 0 == sqr.size();
                        if (sqr.size())
                        {
                            rssdate = sqr[0][0];
                            bOk = bOk | (sqldt.hour() > rssdate.hour() && sqldt.day() >= rssdate.day() && 
                                    sqldt.month() >= rssdate.month() &&
                                    sqldt.year() >= rssdate.year());
                        }
                        
                        if (bOk)
                        {
                            if (item->get_Title() && item->get_Link())
                            {
                                string desc = item->get_Description() ? item->get_Description() : "";
                                trim(desc);
                                string title = item->get_Title();
                                trim(title);
                                string link = item->get_Link();
                                trim(link);

                                q = conn->query();
                                q << "insert into se_eco_rssfeeds_data (rss_id, rss_date, rss_title, rss_description, rss_link) values (" ;
                                q << feedId << ", ";
                                q << quote << sqldt << ", ";
                                q << quote << title << ", ";
                                q << quote << desc << ", ";
                                q << quote << link << ") ";
                                //LOGI(_logger, q.str());
                                try
                                {
                                    q.exec();
                                }
                                catch(const std::exception& ex)
                                {
                                   // LOGE(_logger, ex.what());
                                    throw ex;
                                }
                            }
                            else
                            {
                               // LOGW(_logger, "Feed " << feedId << " (" << feedurl << ") doesn't have description or title or link defined.");
                            }
                        }
                    }
                    // at last.
                    Query q = conn->query();
                    q << "update se_eco_rssfeeds set lastupdate = current_timestamp where rss_id = " << feedId;
                    //LOGIP(q.str());
                    q.exec();
                    //LOGI(_logger, "Successfully processed feedinfo with id " << feedId);
                }
                //else
                   // LOGE(_logger, "Failed to parse feed info url.");
            }
            catch(...)
            {
                //LOGE(_logger, "Exception throws while processing feed info.");
                throw ;
            }
        }
        
        long unsigned int CEconomicNewsStorageProvider::Run() 
        {
            try
            {
                // store latest in se_eco_rssfeeds and se_eco_rssfeeds_data tables
                //se::net::CRSSFeedManager feeder;

               // LOGIP("Running news feed storage manager.");
                // get rss feeds info.
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "SELECT rss_id, name, feedurl, dateformat, tags FROM sa.se_eco_rssfeeds";
                for (const Row& r : q.store())
                {
                    sql_varchar_null tag = r["tags"];
                    sql_varchar_null df = r["dateformat"];
                    ProcessRssFeed(r["rss_id"], (string)r["feedurl"], df.is_null? "" : (string)df.data, tag.is_null ? "" : (string)tag.data);
                }
               // LOGIP("News feed storage manager successfully completed its run.");
                return S_OK;
            }
            CATCHCLAUSE("CEconomicNewsStorageProvider::Run")
        }
    }
}