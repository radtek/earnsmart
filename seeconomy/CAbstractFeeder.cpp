/* 
 * File:   CAbstractFeeder.cpp
 * Author: santony
 * 
 * Created on August 1, 2014, 4:09 PM
 */

#include "seecconinternal.h"
#include "CAbstractFeeder.h"
#include "CFredFeeder.h"
#include "CNcdcFeeder.h"

using namespace std;
using namespace mysqlpp;
using namespace se::net;

namespace se
{
    
    namespace economy
    {


        CAbstractFeeder::CAbstractFeeder() //: _logger(Logger::getLogger("se.economy.sourcefeeder"))
        {
        }

        CAbstractFeeder::CAbstractFeeder(const CAbstractFeeder& orig) 
        {
        }

        CAbstractFeeder::~CAbstractFeeder() 
        {
            if (!strdir.empty())
                CDirectory::Delete(strdir.c_str());
        }

        long unsigned int CAbstractFeeder::CheckTableCreateTable(CSmartConnection& conn, const string& tablename, long unsigned int* rows) 
        {

            Query hastable = conn->query();
            hastable << "SHOW TABLES LIKE " << quote << tablename ;
            if (!hastable.store().size())
            {
                /* check if table exists and if not create one */
                Query qcreate = conn->query();
                qcreate << "CREATE TABLE `sa`.`" << tablename << "` (";
                qcreate << "`country_id` SMALLINT NOT NULL, ";
                qcreate << "`state_id` SMALLINT NOT NULL, ";
                qcreate << "`year` SMALLINT NOT NULL, ";
                qcreate << "`month` SMALLINT NOT NULL, ";
                qcreate << "`day` SMALLINT NOT NULL, ";
                qcreate << "`value` DECIMAL(12,4) NULL, ";
                qcreate << "PRIMARY KEY (`country_id`, `state_id`, `year`, `month`, `day`)) ";
                qcreate << "ENGINE = MyISAM;" ;
                qcreate.execute();
                *rows = -1; // new table.
            }                
            else
            {
                Query qcount = conn->query();
                qcount << "select count(1) from " << tablename;
                *rows = (SIZE)qcount.store()[0][0];
            }
            return S_OK;

        }

        string CAbstractFeeder::Download(UID eid, const string& downloadlink) const 
        {
            CHttpClient http;
            http.EnableFollowLocation();
            ostringstream file;
            SP<IString> homepath;
            CDirectory::GetHome(&homepath);
            file << homepath->get_Buffer() << "/sebgpd/" << eid;
            string strdir = file.str();
            CDirectory::Create(file.str().c_str());
            file << "/indicator.txt";
            http.EnableFollowLocation();
            if (FAILED(http.GetFile(downloadlink.c_str(), file.str().c_str())))
                return "";
            return std::move(file.str());
        }

        

        std::shared_ptr<CAbstractFeeder> CAbstractFeeder::GetFeeder(const string& feederName) 
        {
            if (feederName == "NCDC")  //weather
            {
                return std::make_shared<CNcdcFeeder>();
            }
            else    // default feeder is FRED.
            {
                return std::make_shared<CFredFeeder>();
            }
        }
    }
}
