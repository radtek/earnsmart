/* 
 * File:   CPriceHistoryProcessor.h
 * Author: santony
 *
 * Created on April 18, 2013, 9:48 PM
 */

#ifndef CPRICEHISTORYPROCESSOR_H
#define	CPRICEHISTORYPROCESSOR_H
#include<string>
#include "IFunctions.h"
////#include <log4cxx/logger.h>
#include "../sesecurities/sesecuritiesapi.h"
#include <zthread/Runnable.h>

using namespace ZThread;
using namespace se;
using namespace se::securities;

/****************DEPRECATED WITH CSECURITYPRICEPROCESSOR**********************************/

class CPriceHistoryProcessor  : public Runnable, public CTimedFunction
{
public:
    CPriceHistoryProcessor(ISession* ps);
    virtual ~CPriceHistoryProcessor();
    //1
    void run();
    
    
    
private:
    std::string _csvHtml;
    std::string _csvLink;
    std::string _symbol;
    int _pID;
    SP<ISession> _pSession;
    
private:
    
    //log4cxx::LoggerPtr _pLogger;
    // 2 (1) : find a product id which is not in se_sec_price_history, but in se_sec_product table.
    int PickNextProductId();
    
    //3 (1) : check  
    ErrorId DownloadHistoricalPrices(CSTR symbol);

    //4 (3) : save to database.
    ErrorId SaveHistoricalPrices(int productid,std::string date, std::string open, std::string high, std::string low, std::string close, std::string volume, std::string adj_close);
    int GetCSV(const char* symbol);
    int ReadCsv() ;
    int DailyPrice();
    //bool HasDailyPrice(/*todaydate*/ int productid);
    RP<ISecurityAPI> _psec ;
    
};

#endif	/* CPRICEHISTORYPROCESSOR_H */

