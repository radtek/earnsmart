/* 
 * File:   CPriceHistoryProcessor.cpp
 * Author: santony
 * 
 * Created on April 18, 2013, 9:48 PM
 */

#include "../secommon.h"
#include "CPriceHistoryProcessor.h"

////#include <log4cxx/logger.h>
#include "../semysql/CSEConnections.h"
#include "../senet/CHttpClient.h"
#include "../semysql/CSEConnections.h"
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"

#include<boost/tokenizer.hpp>
#include<boost/regex.hpp>
////#include <log4cxx/logger.h>
#include<boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
//#include<boost/locale.hpp>
#include<ctime>
#include <time.h>

#include <iostream>     
#include <fstream>      
#include <vector>
#include <string>
#include <algorithm>    
#include <iterator> 
#include <libxml/xmlstring.h>
#include <libxml/xmlreader.h>
#include <libxml/HTMLparser.h>
#include<jsoncpp/json/json.h>
#include<curl/curl.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>


//using namespace log4cxx;
using namespace se;

using namespace se::xbrl;
using namespace se::net;
using namespace std;
using namespace boost;





//static LoggerPtr _plogger = Logger::getLogger("se.bg.pricehistory");

#define SEDBCONN        CSEConnections::GetMain()

using namespace se::data;

CPriceHistoryProcessor::CPriceHistoryProcessor(ISession* ps):
        CTimedFunction()
        //,_pLogger(log4cxx::Logger::getLogger("Company History Parser")) 
{
    _pSession = ps;
    //LOG4CXX_DEBUG(_pLogger, "ctor");
    _psec.Assign((ISecurityAPI*) _pSession->get_Object(ISecurityAPIName)._ptr);
}

CPriceHistoryProcessor::~CPriceHistoryProcessor() 
{
     //LOG4CXX_DEBUG(_pLogger, "dtor");
}

void CPriceHistoryProcessor::run()
{
    BoolCompletion busy(_bIsBusy);
    
    //LOG4CXX_INFO(_pLogger, "Starting History processing....");
//    PickNextProductId();
    I32 size;
    CSmartConnection conn(SEDBCONN);
    {
        Query q=conn->query();
        q<<"SELECT COUNT( * ) FROM  `se_sec_products`" ;
        auto res =q.use();
        if (res) 
        { 
            auto row = res.fetch_row();
            size = row[0];
        } 
        else 
        { 
            //LOG4CXX_INFO(_pLogger, "Table not contain symbols");
            return ; 
        }
    }
    for(auto product_id = 101;product_id<size;product_id++)
    {
        //LOG4CXX_DEBUG(_pLogger, "Starting Historical Data processing...."); 
        Query q=conn->query();
        q<< "select * from se_sec_products where product_id =";
        q<< product_id;
        auto res = q.use();        
        if (!res)
        {
            //LOG4CXX_INFO(_pLogger, "Table not contain Data");
            return;
        }
        auto row = res.fetch_row();
        SP<ISecurityDescription>pSecDec;
        _psec->CreateSecurityDescription(&pSecDec);
        pSecDec->set_Country((CSTR)row["product_country_id"]);
        //pSecDec->set_Exchange((CSTR)row["product_exchange_id"]);
        pSecDec->set_Currency((CSTR)row["product_currency_id"]);
        //pSecDec->set_Name((CSTR)row["product_name"]);
        pSecDec->set_Symbol((CSTR)row["product_symbol"]);
        pSecDec->set_Id(product_id);
        
        _psec->ProcessSecurityHistoricalPrices(pSecDec);
       
        //LOG4CXX_DEBUG(_pLogger, "End Historical Data processing...."); 
        
    }
    //LOG4CXX_INFO(_pLogger, "Completed history processing....");
}


int CPriceHistoryProcessor::GetCSV(const char* symbol)
{
    //LOG4CXX_DEBUG(_pLogger, "Start Parsing Url for CSV");
    string url1="http://finance.yahoo.com/q/hp?s=";
    string Surl=url1+symbol;
    char* url;
    xmlChar *key;
    xmlDocPtr doc;
    xmlNodePtr cur; 
    url=&Surl[0];
    xmlBufferPtr nodeBuffer = xmlBufferCreate();
    doc = htmlParseFile (url, NULL);
    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    cur = cur->next;
    xmlNodeDump(nodeBuffer, doc, cur, 0, 1);
    key=nodeBuffer->content;
    _csvHtml = (const char *)key; 
    ReadCsv();
    //LOG4CXX_DEBUG(_pLogger, "End Parsing URL");
    
}

int CPriceHistoryProcessor::ReadCsv()
{
    //LOG4CXX_DEBUG(_pLogger, "serching for csv link....");
    string &answer = _csvHtml;
    
    const string pattern1="(.*)(></tr></table><p><a href=\")(.*)(\"><img src=)(.*)(><strong>Download to Spreadsheet</strong>)(.*)";
    boost::regex regexPattern1(pattern1, boost::regex::extended);
    boost::smatch what;
    bool isMatchFound1 = boost::regex_match(answer, what, regexPattern1);
    if (isMatchFound1)
    {
        _csvLink = what[3];
        //LOG4CXX_DEBUG(_pLogger, "CSV Link  ="<<what[3]);
    }
}

int CPriceHistoryProcessor::DailyPrice()
{
    //LOG4CXX_INFO(_pLogger, "ADD Daily Price");
    string Date,Open,High,Low,Close,Volume,Adj_Close;
    string url1= "http://query.yahooapis.com/v1/public/yql?q=select%20symbol,%20LastTradeDate,%20Open,%20DaysLow,%20DaysHigh,%20close,%20LastTradePriceOnly,%20AverageDailyVolume,%20DividendShare%20from%20yahoo.finance.quotes%20where%20symbol%20in(%22%20";
    string url2 ="%20%22)&env=http://datatables.org/alltables.env&format=json" ;
    string url = url1+_symbol+url2;
    const char *surl = url.c_str();
    CHttpClient htp;
    htp.GetFile(surl,"daily.json");
    Json::Value root;
    std::ifstream file("/home/aashok/Documents/smartearn/Linux/libraries/sebgpd/daily.json");
    file >> root;
    Json::StyledWriter writer;
    Json::Value date = root[ "query" ][ "results"]["quote"]["LastTradeDate"];
    Date = writer.write( date );
    Date.erase(remove( Date.begin(), Date.end(), '\"' ),Date.end());
    trim(Date);
    std::vector<std::string> strs;
    boost::split(strs,Date, boost::is_any_of("/"));
    Date = strs[2]+"-"+strs[0]+"-"+strs[1];
    Json::Value open = root[ "query" ][ "results"]["quote"]["Open"];
    Open = writer.write( open ); 
    Open.erase(remove( Open.begin(), Open.end(), '\"' ),Open.end());
    Json::Value high = root[ "query" ][ "results"]["quote"]["DaysHigh"];
    High = writer.write(high); 
    High.erase(remove( High.begin(), High.end(), '\"' ),High.end());
    Json::Value low = root[ "query" ][ "results"]["quote"]["DaysLow"];
    Low = writer.write( low ); 
    Low.erase(remove( Low.begin(), Low.end(), '\"' ),Low.end());
    Json::Value close = root[ "query" ][ "results"]["quote"]["LastTradePriceOnly"];
    Close = writer.write( close ); 
    Close.erase(remove( Close.begin(), Close.end(), '\"' ),Close.end());
    Json::Value volume = root[ "query" ][ "results"]["quote"]["AverageDailyVolume"];
    Volume = writer.write( volume ); 
    Volume.erase(remove( Volume.begin(), Volume.end(), '\"' ),Volume.end()); 
    Adj_Close = Close;
    SaveHistoricalPrices(_pID,Date,Open,High,Low,Close,Volume,Adj_Close);
    return 0;
    
}
int CPriceHistoryProcessor::PickNextProductId()
{
    //select product_id, product_symbol from se_sec_products where product_id not in (select distinct product_id from se_sec_eq_pricehistory where history_date = (CURDATE()-1))

    
    I32 product_id,size,min,hour;
    CSmartConnection conn2(SEDBCONN);
    auto sql2 = "SELECT COUNT( * ) FROM  `se_sec_products`" ;
    auto query2 = conn2->query(sql2);
    auto res = query2.use();
    if (res) 
    { 
         mysqlpp::Row row2;
         row2 = res.fetch_row();
         size = row2[0];
    } 
    else 
    { 
        //LOG4CXX_INFO(_pLogger, "Table not contain symbols");
        return 1; 
    } 
    
    for(product_id = 1;product_id<size;product_id++)
    {
        time_t     now = time(0);
        struct tm  tstruct;
        tstruct = *localtime(&now);
        min = tstruct.tm_min;
        hour = tstruct.tm_hour;
        //LOG4CXX_DEBUG(_pLogger, "Starting Historical Data processing...."); 
        CSmartConnection conn(SEDBCONN);
        stringstream str;
        str << "select product_symbol from se_sec_products where product_id =";
        str << product_id;
        auto sql = str.str();
        mysqlpp::Row row;
        auto query = conn->query(sql);
        auto qres = query.use();        
        if (!qres)
        {
            //LOG4CXX_INFO(_pLogger, "Table not contain Data");
            return 1;
        }
        row = qres.fetch_row();
        _symbol = (string)row["product_symbol"];
        CSmartConnection conn1(SEDBCONN);
        mysqlpp::Row row1;
        stringstream str1;
        str1 << "select * from se_sec_eq_pricehistory where product_id =";
        str1<< product_id;
        auto sql1 = str1.str();
        auto query1 = conn1->query(sql1);
        auto qres1 = query1.use(); 
        if (!qres1)
        {
            //LOG4CXX_ERROR(_pLogger, "ERROR");
            return 1;
        }
        row1 = qres1.fetch_row();
        _pID = product_id;
        if(row1.empty())
        {
            //LOG4CXX_INFO(_pLogger, "Table not contain Historical Prices");
            sleep(1);
            DownloadHistoricalPrices(row["product_symbol"]);
            //check is time after 4pm
            if(hour>16&&min>1)
            {
                DailyPrice();
            }
        }
        else
        {
           //check is time after 4pm
            //LOG4CXX_INFO(_pLogger, "Slected ID already in Database");
            if(hour>16&&min>1)
            {
                DailyPrice();
            }
            
        }
           
    }
    return 0;
}


ErrorId CPriceHistoryProcessor::DownloadHistoricalPrices(const char* symbol)
{
    GetCSV(symbol);
    CHttpClient htp;
    const char *url = _csvLink.c_str();
    htp.GetFile(url,"history.csv");
    string data("/home/aashok/Documents/smartearn/Linux/libraries/sebgpd/history.csv");
    ifstream in(data.c_str());
    if (!in.is_open()) return 1;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    string line;
    string Date,Open,High,Low,Close,Volume,Adj_Close;
    I32 flag=0;
    while (getline(in,line))
    {
        if(flag==0)
        {
            flag=1;
            continue;
        }
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        Date=vec[0]; 
        Open = vec[1];
        High = vec[2];
        Low = vec[3];
        Close = vec[4];
        Volume = vec[5];
        Adj_Close = vec[6];
        SaveHistoricalPrices(_pID,Date,Open,High,Low,Close,Volume,Adj_Close);
    }
    
}

ErrorId CPriceHistoryProcessor::SaveHistoricalPrices(int productid, string date, string open, string high, string low, string close, string volume, string adj_close)
{
    try
    {
        CSmartConnection conn(SEDBCONN);  
        Query q = conn->query();
        q << "INSERT INTO se_sec_eq_pricehistory(`product_id`, `history_date`, `history_open`, `history_high`,`history_low`, `history_close`, `history_vol`, `history_adjustedclose`)"<<
                "VALUES ('"<<productid<<"','"
                <<date<<"','"
                <<open<<"','"
                <<high<<"','"
                <<low<<"','"
                <<close<<"','"
                <<volume<<"','"
                <<adj_close<<"'"
                <<")"; 
        q.execute();
        //LOG4CXX_INFO(_pLogger, "History updated sucessfully"); 
    }
    catch (const mysqlpp::BadQuery& er) 
    {
        //LOG4CXX_ERROR(_pLogger, "Query Error: " << er.what());
        //return E_DBQueryException;
    }
    catch (const mysqlpp::BadConversion& er) 
    {
        //LOG4CXX_ERROR(_pLogger, "Conversion Error: " << er.what());
        //return E_DBQueryException;
    }
    catch (const mysqlpp::Exception& er) 
    {
        //LOG4CXX_ERROR(_pLogger, "Error: " << er.what());
        //return E_DBQueryException;
    }
    catch (const std::exception& er)
    {
        //LOG4CXX_ERROR(_pLogger, "Error: " << er.what());
        //return E_DBQueryException;
    }
}

