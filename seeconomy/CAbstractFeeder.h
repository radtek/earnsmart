/* 
 * File:   CAbstractFeeder.h
 * Author: santony
 *
 * Created on August 1, 2014, 4:09 PM
 */

#ifndef CABSTRACTFEEDER_H
#define	CABSTRACTFEEDER_H

#include "../semysql/CSEConnections.h"
#include "../seglobal/exceptions.h"
#include "../senet/CHttpClient.h"
#include "../seglobal/CDirectory.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

using namespace se::data;

namespace se
{
    
    namespace economy
    {
        
        #define SEDB se::data::CSEConnections::GetMain()
        #define CountryId   1
        #define StateId     0



        class CAbstractFeeder 
        {
        public:
            CAbstractFeeder();
            CAbstractFeeder(const CAbstractFeeder& orig);
            virtual ~CAbstractFeeder();
            static std::shared_ptr<CAbstractFeeder> GetFeeder(const string& feederName);
            
        public:
            virtual SE_CODE Update(unsigned long eid, const Row& row) = 0;
            SE_CODE CheckTableCreateTable(CSmartConnection& conn, const string& tablename, SIZE *rows);
            
        protected:
            string Download(UID eid, const string& downloadlink) const;
            
        protected:
            //LoggerPtr _logger;
        
        private:
            string strdir;
        };
    }
    
}

#endif	/* CABSTRACTFEEDER_H */

