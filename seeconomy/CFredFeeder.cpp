/* 
 * File:   CFredFeeder.cpp
 * Author: santony
 * 
 * Created on August 1, 2014, 4:10 PM
 */

#include "seecconinternal.h"
#include "CFredFeeder.h"
#include "../external/mariadbpp/ssqls.h"

using namespace se::net;
using namespace std;
using namespace mysqlpp;




namespace se
{
    
    namespace economy
    {

        sql_create_6(record, 5, 6, sql_smallint, country_id, sql_smallint, state_id, sql_smallint, year, sql_smallint, month, sql_smallint, day, sql_decimal, value);
        

        CFredFeeder::CFredFeeder() 
        {
            //_logger = Logger::getLogger("se.economy.fredfeeder");
        }

        CFredFeeder::CFredFeeder(const CFredFeeder& orig) 
        {
        }

        CFredFeeder::~CFredFeeder() 
        {
        }

        long unsigned int CFredFeeder::Update(unsigned long eid, const Row& row) 
        {
            string downloadlink = (CSTR)row["downloadlink"];
            string publishperiod = (CSTR)row["PublishPeriod"];
            string tablename = (CSTR)row["TableName"];
            //sql_date_null lastupdated = row["lastupdated"];
 
            ifstream ifs(Download(eid, downloadlink));
            if (!ifs.is_open())
            {
               // LOGE(_logger, "Temporary file created from downloaded link " << downloadlink << " for eid " << eid << " not found.");
                return E_FileNotFound;
            }
            string line;
            //string slastupdateddate;
            while (std::getline(ifs, line))
            {
                // loop until DATE VALUE Line to start with
                trim(line);
                /*
                if (line.substr(0, strlen("Last Updated:")) == "Last Updated:")
                {
                    slastupdateddate = line.substr(strlen("Last Updated:")+1);
                    trim(slastupdateddate);
                }
                */
                if (line.substr(0, 4) == "DATE")
                {
                    break;
                }
            }
            // TOKENIZE ON SPACE.
            char_separator<char> sep("  ","" , keep_empty_tokens);
            typedef tokenizer< char_separator<char> > Tokenizer;
            typedef vector<string> TokenVector;
            record::table(tablename.c_str());

            typedef std::vector<record> Records;
            Records recs;
            while (std::getline(ifs, line))
            {
                trim(line);
                Tokenizer tok(line, sep);
                TokenVector vec(tok.begin(), tok.end());
                string sDate = vec.front();
                string sVal = *vec.rbegin();
                if (sVal.empty() || !std::isdigit(sVal[0]))
                    continue;
                CDate dt = CDate::Parse(sDate.c_str());
                double value = sVal == "" ? ER8 : strtod(sVal.c_str(), nullptr);
                record rec(CountryId, StateId, dt.get_Year(), dt.get_Month(), dt.get_Day(), value);
                recs.push_back(rec);
            }            
            CSmartConnection conn(SEDB);
            try
            {
                
                SIZE crows;
                CheckTableCreateTable(conn, tablename, &crows);
                
//                if (crows < 0)
//                {
//                    // truncate the table since we always add all the records from the source.
//                    Query qtruncate = conn->query();
//                    qtruncate << "truncate table " << tablename;
//                    qtruncate.execute();
//                   // LOGI(_logger, "Table " << tablename << " is not new. Truncated it.");
//                }                
                
                Records::const_iterator itfirst;
                Records::const_iterator itend;
                int size = recs.size();
                if (publishperiod == "Monthly")
                {
                    if (size > (25 * 12))
                    {
                        int startpos = size - (25 * 12);
                        itfirst = recs.cbegin() + startpos;
                        itend = recs.cend();
                    }
                    else
                    {
                        itfirst = recs.cbegin();
                        itend = recs.cend();
                    }
                }
                else if (publishperiod == "Quarterly")
                {
                    // update database last 25 * 4 records
                    if (size > (25 * 4))
                    {
                        int startpos = size - (25 * 4);
                        itfirst = recs.cbegin() + startpos;
                        itend = recs.cend();
                    }
                    else
                    {
                        itfirst = recs.cbegin();
                        itend = recs.cend();
                    }
                }
                else if (publishperiod == "Weekly")
                {
                    // update database last 25 * 4 records
                    if (size > (25 * 52))
                    {
                        int startpos = size - (25 * 52);
                        itfirst = recs.cbegin() + startpos;
                        itend = recs.cend();
                    }
                    else
                    {
                        itfirst = recs.cbegin();
                        itend = recs.cend();
                    }
                }
                else if (publishperiod == "Daily")
                {
                    // update database last 25 * 4 records
                    if (size > (25 * 365))
                    {
                        int startpos = size - (25 * 365);
                        itfirst = recs.cbegin() + startpos;
                        itend = recs.cend();
                    }
                    else
                    {
                        itfirst = recs.cbegin();
                        itend = recs.cend();
                    }
                }
                else if (publishperiod == "Annually")
                {
                    // update database last 25 * 4 records
                    if (size > 25)
                    {
                        int startpos = size - 25;
                        itfirst = recs.cbegin() + startpos;
                        itend = recs.cend();
                    }
                    else
                    {
                        itfirst = recs.cbegin();
                        itend = recs.cend();
                    }
                }
                else
                    return S_OK;
                
                if (recs.size())
                {
                    const auto& lastrecord = recs.back();
                    Query qcheck = conn->query();
                    qcheck << "select year, month, day from " << tablename << " order by year desc, month desc, day desc limit 1";
                    auto sqr = qcheck.store();
                    if (sqr.size())
                    {
                        const Row& r = sqr[0];
                        I16 year = r["year"];
                        I16 month = r["month"];
                        I16 day = r["day"];
                        if (year == lastrecord.year && 
                                month == lastrecord.month &&
                                day == lastrecord.day)
                        {
                           // LOGI(_logger, "EID " << eid << " has current data. No need of an update.");
                            return S_OK;
                        }
                    }
                    Query qreplace = conn->query();
                    qreplace.replace(itfirst, itend).execute();
                    
                    // update lastupdated field
                    Query qupdate = conn->query();
                    qupdate << "update se_eco_indicators set lastupdated = current_timestamp where ei_id = " << eid;
                    qupdate.exec();
                }

            }
            catch (const std::exception& ex)
            {
               // LOGE(_logger, "DB error : " << ex.what() );
                throw ex;
            }

            return S_OK;
        }

    }
    
}