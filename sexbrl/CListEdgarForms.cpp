/* 
 * File:   CListEdgarForms.cpp
 * Author: santony
 * 
 * Created on July 11, 2012, 7:58 PM
 */

#include "sexbrlinternal.h"
#include "CListEdgarForms.h"

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <cstring>
#include<string>
#include<boost/tokenizer.hpp>
#include<boost/regex.hpp>

#include <CSEConnections.h>

#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#define SEDBCONN        CSEConnections::GetMain()

using namespace std;
using namespace boost;	
using namespace se::data;
using namespace mysqlpp;
////using namespace log4cxx;
const int MAX_CHARS_PER_LINE = 512;
const int MAX_TOKENS_PER_LINE = 5;
const char* const DELIMITER = "|";



sql_create_3(se_edgar_formrecs, 1, 2,
       
        sql_int,cik,
        sql_date,filedate,
      //  sql_int,filedate,
        sql_varchar,location
        );



CListEdgarForms::CListEdgarForms(int year, int quarter, CSTR formType) : 
        _year(year), _quarter(quarter) //, _logger(Logger::getLogger("se.xbrl.listedgarforms"))
{
    _formType = formType;
}

CListEdgarForms::CListEdgarForms(const CListEdgarForms& orig) //: _logger(Logger::getLogger("se.xbrl.listedgarforms"))
{
    _year = orig._year;
    _quarter = orig._quarter;
    _formType = orig._formType;
}

CListEdgarForms::~CListEdgarForms() 
{
    
}

int CListEdgarForms::ParseRecord(std::string** records)
{
    return 0;
}

void CListEdgarForms::UpdateDatabase(std::string* records, int size)
{
    
}

ErrorId CListEdgarForms::Process(IXbrlCompletionEvent<int>* listener)
{
    //LOG4CXX_INFOINFO(_logger, "se_edgar_formrecs Started to process xbrl concepts document.");
    try
    {
        ifstream fin;
        fin.open("/home/arun/xbrl.idx");       
        int n = 0,cik1; 
        int year,month,day;
        string strDate,str;
        CSmartConnection conn(SEDBCONN);
        Query query = conn->query();
        //query.exec("truncate table se_edgar_formrecs");
        Query::RowCountInsertPolicy<> policy(25);
        typedef vector<se_edgar_formrecs> LIST;
        LIST list;
        const string pattern="(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d)";
        
        boost::regex regexPattern(pattern, boost::regex::extended); 
        boost::smatch what; 
      
        while (!fin.eof())
        {
         
            se_edgar_formrecs formrecs;
            formrecs.instance_table("se_edgar_formrecs");
            char buf[MAX_CHARS_PER_LINE];
            fin.getline(buf, MAX_CHARS_PER_LINE);
  
            if(buf[0]=='D'||buf[0]=='-'||buf[0]==' '||buf[0]=='L'||buf[0]=='C'||buf[0]=='A')
                continue;
               
            const char* token[MAX_TOKENS_PER_LINE] = {0};
            token[0] = strtok(buf, DELIMITER);  
            if (token[0]) 
            {                   
                for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
                {
                    token[n] = strtok(0, DELIMITER); 
                    if (!token[n]) continue;
                }
            }
   
            strDate = token[3]==NULL?" ":token[3];
                
            std::string strType = token[2]==NULL?" ":token[2];
             
              
            bool isMatchFound = boost::regex_match(strDate, what, regexPattern);
            if (isMatchFound) 
            {
                for (unsigned int i=0; i < what.size(); i++) 
                { 
                    if(i==2)
                    {
  
                            stringstream(what[1]) >>year;
                            stringstream(what[2]) >>month;
                            stringstream(what[3]) >>day;

                      }
                  }
              } 
              std::string strCik = token[0]==NULL?" ":token[0];
              stringstream(token[0]) >>cik1;
              strDate = token[3]==NULL?" ":token[3];
                
              std::string strLocation = token[4]==NULL?" ":token[4];
              mysqlpp::Query query = conn->query("SELECT cik FROM se_edgar_formrecs where location='"+strLocation+"' and cik='"+strCik+"'");  
                    /*if (mysqlpp::StoreQueryResult res = query.store()) 
                    {
                    cout << "duplicate" <<endl;
                    continue;
                    //  cout<<"found"<<endl;   
                    }*/
             
               if(_formType==strType){
                    formrecs.location=strLocation; 
                    formrecs.cik=cik1;
             
                    sql_date dt1(year,month,day);
                             
                    formrecs.filedate=dt1;
                    list.push_back(formrecs);
                    query.insert(formrecs);

                    query.execute();
                    
                    cout<<_formType<<"\t"<<strType<<"\tcik\t"<<cik1<<"\tdate\t"<<dt1<<"\tlocation\t"<<strLocation<<endl;
                }
    }
    //LOG4CXX_INFOINFO(_logger, "se_edgar_formrecs: Reading file completed. Sending the vector to the database.");

      
    query.insertfrom(list.begin(), list.end(), policy);

    //LOG4CXX_INFOINFO(_logger, "se_edgar_formrecs: Successfully updated the database.");
        
        
    return S_Success; 
 }
 catch (const mysqlpp::BadQuery& er) 
    {
        //LOG4CXX_INFOERROR(_logger, "Query Error: " << er.what());
        return E_DBQueryException;
    }
    catch (const mysqlpp::BadConversion& er) 
    {
        //LOG4CXX_INFOERROR(_logger, "Conversion Error: " << er.what());
        return E_DBQueryException;
    }
    catch (const mysqlpp::Exception& er) 
    {
        //LOG4CXX_INFOERROR(_logger, "Error: " << er.what());
        return E_DBQueryException;
    }
    catch (const std::exception& er)
    {
        //LOG4CXX_INFOERROR(_logger, "Error: " << er.what());
        return E_DBQueryException;
    }
    
    
}    
     
     
     
     /* 
     a. Start a new thread from threadpool. // refer : /usr/include/pthread_workqueue.h  (install libpthread-workqueue-dev using synaptic)
     b. open the document
     c. go to first line
     d. ParseRecord, obtain the record array
     e. Call UpdateDatabase, passing the record
     f. Call listener with completed event.
     g. Report any errors through the listener if any exception occurs.*/
    



