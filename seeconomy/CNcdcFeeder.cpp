/* 
 * File:   CNcdcFeeder.cpp
 * Author: santony
 * 
 * Created on August 1, 2014, 4:38 PM
 */
#include "seecconinternal.h"
#include "CNcdcFeeder.h"
#include "../external/mariadbpp/ssqls.h"
#include <jsoncpp/json/json.h>
#include <fstream>

using namespace std;
using namespace mysqlpp;
using namespace Json;

namespace se
{
    
    namespace economy
    {
        
        sql_create_6(ncdcrecord, 5, 6, sql_smallint, country_id, sql_smallint, state_id, sql_smallint, year, sql_smallint, month, sql_smallint, day, sql_decimal, value);
        
        
        /* {MM} - month
         * {YYY-} - prev year
         * {YYYY} - curr year
         * 
         * Weather - Temp
         * http://www.ncdc.noaa.gov/cag/time-series/us/110/00/tavg/1/{MM}/{YYY-}-{YYYY}.json?base_prd=true&firstbaseyear=1901&lastbaseyear=2000
         * 
         * {
            "description":[
              "title": "Contiguous U.S., Average Temperature, June",
              "units": "Degrees Fahrenheit",
              "base_period": "1901-2000"
            ]
            "dataCollection": [
              "data": {
                "date": "201306",
                "value": "70.39",
                "anomaly": "1.90"
              },
              "data": {
                "date": "201406",
                "value": "69.57",
                "anomaly": "1.08"
              }
            ]
            }
         * 
         * Precipitation
         * http://www.ncdc.noaa.gov/cag/time-series/us/110/00/pcp/1/{MM}/{YYY-}-{YYYY}.json?base_prd=true&firstbaseyear=1901&lastbaseyear=2000
         * {
            "description":[
              "title": "Contiguous U.S., Precipitation, June",
              "units": "Inches",
              "base_period": "1901-2000"
            ]
            "dataCollection": [
              "data": {
                "date": "201306",
                "value": "3.34",
                "anomaly": "0.41"
              },
              "data": {
                "date": "201406",
                "value": "3.62",
                "anomaly": "0.69"
              }
            ]
          }
         * 
         * // drought index
         * http://www.ncdc.noaa.gov/cag/time-series/us/110/00/pdsi/1/{MM}/{YYY-}-{YYYY}.json?base_prd=true&firstbaseyear=1901&lastbaseyear=2000
         * 
         * {
            "description":[
              "title": "Contiguous U.S., Palmer Drought Severity Index (PDSI), June",
              "base_period": "1901-2000"
            ]
            "dataCollection": [
              "data": {
                "date": "201306",
                "value": "1.08",
                "anomaly": "0.79"
              },
              "data": {
                "date": "201406",
                "value": "1.28",
                "anomaly": "0.99"
              }
            ]
          }
         */

        CNcdcFeeder::CNcdcFeeder() 
        {
            //_logger = Logger::getLogger("se.economy.ncdcfeeder");
        }

        CNcdcFeeder::CNcdcFeeder(const CNcdcFeeder& orig) 
        {
        }

        CNcdcFeeder::~CNcdcFeeder() 
        {
            
            
        }

        long unsigned int CNcdcFeeder::Update(unsigned long eid, const Row& row) 
        {
            typedef std::vector<ncdcrecord> Records;
            Records recs;
            CSmartConnection conn(SEDB);
            string downloadlink = (CSTR)row["downloadlink"];
            string publishperiod = (CSTR)row["PublishPeriod"];
            string tablename = (CSTR)row["TableName"];
            
            SIZE crows;
            CheckTableCreateTable(conn, tablename, &crows);
            
            CDate dtNow = CDate::Now();
            CDate dtTmp = dtNow.AddSubMonths(-1);
            ncdcrecord::table(tablename.c_str());
            
            Query qcheck = conn->query();
            qcheck << "select year, month, day from " << tablename << " where country_id = 1 and state_id = 0 order by year desc, month desc, day desc limit 1";
            auto sqr = qcheck.store();
            
            if (sqr.size())
            {
                const Row& r = sqr[0];
                I16 year = r["year"];
                I16 month = r["month"];
                I16 day = r["day"];
                if (year == dtTmp.get_Year() && month == dtTmp.get_Month())
                {
                   // LOGI(_logger, "EID " << eid << " has current data. No need of an update.");
                    return S_OK;
                }
            }

            int i =0;
            do
            {
                string dlink = downloadlink;
                ostringstream ostr;
                string mm, yyyy;

                ostr << dtTmp.get_Month();
                if (ostr.str().length() == 1)
                    mm = "0" + ostr.str();
                else
                    mm = ostr.str();
                ostr.str("");
                ostr << dtTmp.get_Year();
                yyyy = ostr.str();

                replace_all(dlink, "{MM}", mm);
                replace_all(dlink, "{YYY-}", yyyy);
                replace_all(dlink, "{YYYY}", yyyy);
                
                string file = Download(eid, dlink);
                ifstream ifs(file);
                if (!ifs.is_open())
                {
                   // LOGE(_logger, "Temporary file created from downloaded link " << downloadlink << " for eid " << eid << " not found.");
                    return E_FileNotFound;
                }
                Value json;
                Reader reader;
                if (!reader.parse(ifs, json))
                {
                   // LOGE(_logger, "Temporary file created from downloaded link " << downloadlink << " for eid " << eid << " is not json formatted. Check this erro: " << reader.getFormattedErrorMessages());
                    return E_NotJsonDeserializable;
                }
                
                /*
                 {
                    "description": {
                        "title": "Contiguous U.S., Average Temperature, July",
                        "units": "Degrees Fahrenheit",
                        "base_period": "1901-2000"
                    },
                    "data": {
                        "201307": {
                            "value": "74.21",
                            "anomaly": "0.60"
                        },
                        "201407": {
                            "value": "73.31",
                            "anomaly": "-0.30"
                        }
                    }
                }
                 */
                
                const Value& data = json["data"];
                if (data.isNull())
                    throw seexception("Json parsing error. Cannot continue.");
                
                // the key is in the form YYYYMM which we have above
                string key = yyyy+mm;
                Value def = Json::nullValue;
                if (data.isObject() && data.isMember(key))
                {
                    Value val = data.get(key, def);
                    R8 value = strtof(val["value"].asCString(), nullptr);

                    // update table record
                    ncdcrecord rec;
                    rec.country_id = 1;
                    rec.state_id = 0;
                    rec.day = 1;
                    rec.month = dtTmp.get_Month();
                    rec.year = dtTmp.get_Year();
                    rec.value = value;
                    recs.push_back(rec);
                }

                dtTmp = dtTmp.AddSubMonths(-1);
            
            } while(i++ <= 2);
            
            Query qreplace = conn->query();
            if (recs.size())
            {
                qreplace.replace(recs.begin(), recs.end()).execute();
                // update lastupdated field
                Query qupdate = conn->query();
                qupdate << "update se_eco_indicators set lastupdated = current_timestamp where ei_id = " << eid;
                qupdate.exec();
            }
            
            return S_OK;
                
        }
    }
}