/* 
 * File:   CEconomicIndicatorFeeder.cpp
 * Author: santony
 * 
 * Created on June 19, 2014, 12:24 AM
 */

#include "seecconinternal.h"
#include "CEconomicIndicatorFeeder.h"
#include "../seglobal/exceptions.h"
#include "CFredFeeder.h"

using namespace std;
using namespace mysqlpp;

namespace se
{
    
    namespace economy
    {

        #define SEDB se::data::CSEConnections::GetMain()

        CEconomicIndicatorFeeder::CEconomicIndicatorFeeder()  
            //_logger(Logger::getLogger("se.economy.feeder"))
        {
        }

        CEconomicIndicatorFeeder::CEconomicIndicatorFeeder(const CEconomicIndicatorFeeder& orig) 
        {
            
        }

        CEconomicIndicatorFeeder::~CEconomicIndicatorFeeder() 
        {
            
        }

        long unsigned int CEconomicIndicatorFeeder::Run() 
        {
            CSmartConnection conn(SEDB);
            Query q = conn->query();
            q << "SELECT ei_id FROM sa.se_eco_indicators where ifnull(downloadlink, '') <> ''";
            q << " and autoupdateenabled = '1'";
            q << " order by ei_id";
            for (const Row& row : q.store())
            {
                try
                {
                    Run(row["ei_id"]);
                }
                catch(...)
                {
                   // LOGEP("Error reported.");
                    //return E_UnknownError;
                }
            }
            return S_OK;
        }

        long unsigned int CEconomicIndicatorFeeder::Run(unsigned long eid) 
        {
            try
            {
                //LOGI(_logger, "Running for eid : " << eid );
                
#ifdef __DEBUG__
                if (eid == 100)
                {
                    string s = "break;";
                }
#endif
                // check if time is right.
                // first get the release date
                // then compare it with lastupdated. if lastupdate is less than today
                // and if today is same or greater than release day, then do the update.
                
                // get the download link.
                // create a map of date-value and load data
                // upload the last 25 years.
                CSmartConnection conn(SEDB);
                Query q = conn->query();
                q << "SELECT ei_id, PublishPeriod, TableName, downloadlink, ReleaseDates, lastupdated, feeder FROM sa.se_eco_indicators ";
                q << "where ei_id = " << eid;
                StoreQueryResult sqr = q.store();
                if (!sqr.size())
                {
                    //LOGE(_logger, "Economic Indicator " << eid << " is not found.");
                    return E_NotFound;
                }
                const Row& reid = sqr[0];
                sql_varchar_null downloadlink = reid["downloadlink"];
                if (downloadlink.is_null)
                {
                   // LOGE(_logger, "Download link is not set for economic indicator " << eid);
                    return S_False;
                }
                
                // default feeder is fredfeeder
                auto feeder = CAbstractFeeder::GetFeeder((CSTR)reid["feeder"]);
                
                sql_datetime_null lastupdated = reid["lastupdated"];
                if (lastupdated.is_null)
                {
                    return feeder->Update(eid, reid);
                }
                
                // today
                CDate dtNow = CDate::Now();
                int releaseday;
                try
                {
                    releaseday = (U32)reid["ReleaseDates"];
                }
                catch(...)
                {
                    // set last day of the month
                    releaseday = dtNow.get_LastDayOfMonth();
                }
                
                CDate dtLastUpdated;
                try
                {
                    dtLastUpdated.set_Year(lastupdated.data.year());
                    dtLastUpdated.set_Month(lastupdated.data.month());
                    dtLastUpdated.set_Day(lastupdated.data.day());
                }
                catch(...)
                {
                    dtLastUpdated.set_Year(2000);
                    dtLastUpdated.set_Month(1);
                    dtLastUpdated.set_Day(1);
                }
                if (dtLastUpdated.IsInvalid())
                {
                    dtLastUpdated.set_Year(2000);
                    dtLastUpdated.set_Month(1);
                    dtLastUpdated.set_Day(1);
                }
                //CDate dtReleaseDate(dtNow.get_Year(), dtNow.get_Month(), releaseday);
                string publishperiod = (CSTR)reid["PublishPeriod"];
                auto periodcounts = (dtNow - dtLastUpdated);
                auto numMonths = periodcounts.Month;
                auto weeks = periodcounts.Week;
                auto numDays = periodcounts.Day;
                if ( (publishperiod == "Quarterly" || publishperiod == "Annually") && numMonths > 1)
                    return feeder->Update(eid, reid);
                else if (publishperiod == "Monthly" || publishperiod == "Weekly" && weeks > 1)
                    return feeder->Update(eid, reid);
                else if (publishperiod == "Daily" && numDays >= 1)
                    return feeder->Update(eid, reid);
                else
                {
                   // LOGI(_logger, "Downloading is not required for economic indicator " << eid);
                }
                return S_OK;
                
            }
            //CATCHCLAUSE("Run(eid)")
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "Exception thrown: " << ex.what());
                return E_UnknownError;
            }
            catch(...)
            {
               // LOGEP("Unknown exception is thrown. Continuing to next item.");
                return E_UnknownError;
            }
        }
    }
}