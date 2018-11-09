/* 
 * File:   CParseConcepts.cpp
 * Author: arun
 * 
 * Created on 4 August, 2012, 7:10 AM
 */

#include "sexbrlinternal.h"
#include "CParseConcepts.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <cstring>
	
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <boost/tokenizer.hpp>
#include <CSEConnections.h>

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

#include <boost/algorithm/string/trim.hpp>

#define HOST "njmain"
#define DB "sa"
	
using namespace std;
using namespace boost;	
using namespace se::data;
using namespace mysqlpp;
using namespace log4cxx;

#define SEDBCONN        CSEConnections::GetMain()



///* Create our table structure */
//sql_create_6(stock, 1, 6,
//    sql_char, item,
//    sql_bigint, num,
//    sql_double, weight,
//    sql_decimal, price,
//    sql_date, sdate,
//    Null<sql_mediumtext>, description);

//                    pstmt->setString(1, name);
//                    pstmt->setString(2, type);
//                    pstmt->setString(3, enu);
//                    pstmt->setString(4, subgroup);
//                    pstmt->setString(5, balance);
//                    pstmt->setString(6, period);
//                    pstmt->setString(7, abstract);
//                    pstmt->setString(8, label);
//                    pstmt->setString(9, doc);


sql_create_9(XbrlConcepts, 1, 2, 
        sql_varchar, xbrl_name,
        sql_varchar, xbrl_type,
        sql_varchar, xbrl_enumerations,
        sql_varchar, xbrl_substituiongroup,
        sql_varchar, xbrl_balance,
        sql_varchar, xbrl_PeriodType,
        sql_bool, xbrl_IsAbstract,
        sql_varchar, xbrl_label,
        sql_varchar, xbrl_desc
        );



CParseConcepts::CParseConcepts(CSTR fileName) : _fileName(fileName)
{
    
}

CParseConcepts::CParseConcepts(const CParseConcepts& orig) 
{
}

ErrorId CParseConcepts::Process(IXbrlCompletionEvent<int>* listener)
{
<<<<<<< .mine
 
    CSmartConnection conn(SEDBCONN);
    
    auto delQuery = conn->query();
    delQuery.exec("truncate table se_xbrl_concepts");
    
    // create a packet size policy
=======
    LOG4CXX_INFO(logger, "XbrlConcepts: Started to process xbrl concepts document.");
>>>>>>> .r367
    
    try
    {
    
        CSmartConnection conn(SEDBCONN);

        Query query = conn->query();
        query.exec("truncate table se_xbrl_concepts");

        // create a packet size policy

        // create a rowcount policy instance
        Query::RowCountInsertPolicy<> policy(25);

        // create a vector of rows to be inserted
        typedef vector<XbrlConcepts> LIST;
        LIST list;


        //string data("/home/arun/Documents/smartearn/Linux/SA/xbrlparser/UGT-2012-Concepts.csv");
        ifstream in(_fileName.c_str());

        if (!in.is_open()) 
        {
            // log and return.
            LOG4CXX_ERROR(logger, "Input file error.")
            return E_UnknownError;
        }

        LOG4CXX_INFO(logger, "XbrlConcepts: Reading file. Tokenizing and populating a vector.");

        typedef tokenizer< escaped_list_separator<char> > Tokenizer;
        typedef vector<string> TokenVector;
        TokenVector vec;
        string line;
        int flag=0;

        while(getline(in,line))
        {
            //cout<<line<<endl;
            if(flag==0)
            {
                flag=1;
                continue;
            }
            Tokenizer tok(line);
            vec.assign(tok.begin(), tok.end());
            //cout<<vec[1]<<endl;
            if (vec.size()>0)
            {
                XbrlConcepts concept;
                concept.instance_table("se_xbrl_concepts");

                auto sPfx = vec[1];
                trim(sPfx);
                sPfx.erase(remove(sPfx.begin(), sPfx.end(), '\"'), sPfx.end());
                auto sName = vec[2];
                trim(sName);
                sName.erase(remove(sName.begin(), sName.end(), '\"'), sName.end());
                cout<<sName<<endl;
                concept.xbrl_name = sPfx + ":" + sName;
                concept.xbrl_type = vec[3];
                string& str = vec[4];
                str.erase(remove(str.begin(), str.end(), '\"'), str.end());
                trim(str);
                concept.xbrl_enumerations = str;
                str = vec[5];
                str.erase(remove(str.begin(), str.end(), '\"'), str.end());
                trim(str);
                concept.xbrl_substituiongroup = str;            
                concept.xbrl_balance = vec[6];            
                str = vec[7];
                str.erase(remove(str.begin(), str.end(), '\"'), str.end());
                trim(str);
                concept.xbrl_PeriodType = str;            
                str = vec[8];
                str.erase(remove(str.begin(), str.end(), '\"'), str.end());
                trim(str);
                concept.xbrl_IsAbstract = str == "true" ? 1 : 0;
                str = vec[9];
                str.erase(remove(str.begin(), str.end(), '\"'), str.end());
                trim(str);
                concept.xbrl_label = str;            
                str = vec[10];
                str.erase(remove(str.begin(), str.end(), '\"'), str.end());
                trim(str);
                concept.xbrl_desc = str;            

                list.push_back(concept);
            }
        }

        LOG4CXX_INFO(logger, "XbrlConcepts: Reading file completed. Sending the vector to the database.");

        query.insertfrom(list.begin(), list.end(), policy);

        return S_Success;

    }
    catch (const mysqlpp::BadQuery& er) 
    {
        LOG4CXX_ERROR(logger, "Query Error: " << er.what());
        return E_DBQueryException;
    }
    catch (const mysqlpp::BadConversion& er) 
    {
        LOG4CXX_ERROR(logger, "Conversion Error: " << er.what());
        return E_DBQueryException;
    }
    catch (const mysqlpp::Exception& er) 
    {
        return E_DBQueryException;
    }
}
