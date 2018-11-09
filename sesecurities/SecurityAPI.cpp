#include "internal.h"
#include "SecurityAPI.h"
#include "../semysql/CSEConnections.h"
#include "../seglobal/SecurityDescription.h"
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#include<iostream>
#include "CParseCompanyProfile.h"
#include "../seglobal/stringconversions.h"
#include "../seglobal/CROList.h"
#include <cstdlib>

#include "Security.h"
#include<boost/regex.hpp>
#include "../seglobal/CJsonSerializedData.h"
#include "CSecurityPrice.h"

#include<boost/tokenizer.hpp>
#include<boost/regex.hpp>
#include<boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include<ctime>
#include <time.h>
#include "../senet/CHttpClient.h"
#include <fstream>      
#include "../seglobal/CDirectory.h"
#include <pwd.h>
#include "../semysql/CMyqlUtilities.h"
#include "../seglobal/exceptions.h"
#include "../seglobal/CFunctionCache.h"
#include <boost/date_time.hpp>
#include <boost/date_time/date_clock_device.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <time.h>
#include <bits/unordered_set.h>
#include <mutex>
#include "../semath/CCommon.h"
#include "CEconomincallyAdjustedPrices.h"
#include "CGICList.h"
#include "GICSInfo.h"
#include "CPublicCompanyInfoList.h"
#include "../seglobal/CIdName.h"
#include "CEarningCalendar.h"
#include "CSecuritySelectionStatistics.h"
#include "../senet/CRSSFeedManager.h"
#include "CSecurityNews.h"
#include "CSecurityPriceRSSFeed.h"
#include "CEodDataProvider.h"


using namespace boost;
using namespace std;
using namespace se;
using namespace se::securities;
using namespace se::data;
using namespace boost;
using namespace se::net;

using namespace se::math;

#define SEDBCONN        CSEConnections::GetMain()
#define SEEODDATA        CSEConnections::GetEodData()


//NamedObjectNamingType ISecurityAPIName = 0xF;

//static LoggerPtr _logger = Logger::getLogger("se_security");

#define QUERYFIELDLOGIC(q)          \
    if (lgx == LGX_And)             \
    {                               \
        if (!bWhere)                \
        {                           \
            q << " where ";         \
            bWhere = true;          \
        }                           \
        else                        \
            q << " and ";           \
    }                               \
    else                            \
    {                               \
        if (!bWhere)                \
        {                           \
            q << " where ";         \
            bWhere = true;          \
        }                           \
        else                        \
            q << " or ";            \
    }


sql_create_1
(
    DataRowHolidays,
    1,
    0,
    sql_date, holiday
);

struct DataRowHolidaysComparison
{
    DataRowHolidays _r;
    
    DataRowHolidaysComparison(){}
    
    DataRowHolidaysComparison(const DataRowHolidays& d)
    {
        _r = d;
    }
    
    bool operator()(const DataRowHolidays& rowa, const DataRowHolidays& rowb) const
    {
        return rowa.holiday < rowb.holiday;
    }

    bool operator()(const DataRowHolidays& other) const
    {
        return _r.holiday < other.holiday;
    }
} ;

typedef std::unordered_set<DataRowHolidays, DataRowHolidaysComparison> Holidays;
Holidays holidays;

Poco::Mutex CSecurityAPI::securityLock;

CSecurityAPI::CSecurityAPI(ISession* ps) : CObjectRoot(ps), _dataProvider(nullptr)
{
    
    //ctor
}

CSecurityAPI::~CSecurityAPI()
{
    delete _dataProvider;
}

MARKETDATAPROVIDERS CSecurityAPI::get_DefaultMarketDataProvider() 
{
    return _defProvider;
}


NamedObjectNamingType  CSecurityAPI::get_TypeId()
{
    return ISecurityAPIName;
}

long unsigned int CSecurityAPI::Init(MARKETDATAPROVIDERS defmdprovider) 
{
    this->_defProvider = defmdprovider;
    switch (_defProvider)
    {
        case MARKETDATAPROVIDERS::EodData:
            _dataProvider = new CEodDataProvider(get_Session(), this);
            break;
    }
    return S_OK;
}


/* Create a clean security description. it could be used for
setting information of a security */
ErrorId CSecurityAPI::CreateSecurityDescription(ISecurityDescription** ppOut) 
{
    return ISecurityDescription::Create(get_Session(), ppOut);
}



long unsigned int CSecurityAPI::get_SecurityDescriptions(const char* symbol, ISecurityDescriptionList** ppOut) 
{
    try
    {
        CSmartConnection conn(SEEODDATA);
        Query q = conn->query();
        q << "call find_MatchingSymbols(" << quote << symbol << ")";
        SP<ISecurityDescriptionList> pl;
        RETURNIFFAILED(ISecurityDescriptionList::Create(get_Session(), &pl));
        SE_CODE code = E_NotFound;
        q.for_each([this, &pl, &code](const Row& r)
        {
            UID prodId = r["esid"];
            SP<ISecurityDescription> pSecDesc ;
            code = CreateSecurityDescription(&pSecDesc);
            if (ISOK(code))
            {
                pSecDesc->set_ESid(prodId);
                pSecDesc->set_ExchCode(r["ecode"]);
                pSecDesc->set_Symbol(r["ssym"]);
                pl->Add(pSecDesc);
            }
        });
        pl.CopyTo(ppOut);
        return code;
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::get_SecurityDescriptionsByName(const char* namepattern, ISecurityDescriptionList** ppOut) 
{
    try
    {
        CSmartConnection conn(SEEODDATA);
        Query q = conn->query();
        q << "call find_MatchingNames(" << quote << namepattern << ")";
        SP<ISecurityDescriptionList> pl;
        RETURNIFFAILED(ISecurityDescriptionList::Create(get_Session(), &pl));
        SE_CODE code = E_NotFound;
        q.for_each([this, &code, &pl](const Row& r)
        {
            UID prodId = r["esid"];
            SP<ISecurityDescription> pSecDesc ;
            code = CreateSecurityDescription(&pSecDesc);
            if (ISOK(code))
            {
                pSecDesc->set_ESid(prodId);
                pSecDesc->set_ExchCode(r["ecode"]);
                pSecDesc->set_Symbol(r["ssym"]);
                pl->Add(pSecDesc);
            }
        });
        pl.CopyTo(ppOut);
        return S_OK;
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::get_Exchanges(IExchangeList** ppOut) 
{
    try
    {
        CSmartConnection conn(SEEODDATA);
        Query q = conn->query();
        q << "select * from eod_exchanges";
        SP< CROSPList<IExchange, IROSPList<IExchange>> > pList;
        RETURNIFFAILED(CROSPList< IExchange>::CreateROSPList(get_Session(), &pList));
        SE_CODE code = E_NotFound;
        q.for_each([this, &code, &pList](const Row& r)
        {
            SP<IExchange> pexch;
            code = IExchange::Create(get_Session(), &pexch);
            if (ISOK(code))
            {
                pexch->set_Id(r["eid"]);
                pexch->set_Name(r["ename"]);
                pexch->set_Code(r["ecode"]);
                pexch->set_Country(r["ecountry"]);
                pexch->set_TimeZone(r["etimezone"]);
                pList->Add(pexch);
            }
        });
    }
    CATCHCLAUSE

}


SE_CODE CSecurityAPI::get_Exchanges(Json::Value& jsOut)
{
    try
    {
        CSmartConnection conn(SEEODDATA);
        Query q = conn->query();
        q << "select * from eod_exchanges order by ename";
        for(auto exch : q.store())
        {
            Json::Value val;
            val["key"] = (I32)exch["eid"];
            val["name"] = (string)exch["ename"];
            val["code"] = (string)exch["ecode"];
            val["country"] = (string)exch["ecountry"];
            val["tz"] = (string)exch["etimezone"];
            jsOut.append(val);
        }
        return S_OK;
    }
    CATCHCLAUSE;
}


/* get information about a security */
ErrorId CSecurityAPI::get_Security(ISecurityDescription* pSecDesc, ISecurity** ppOut) 
{
    try
    {
        ISecurity* pOut = nullptr;
        auto find = _securities.find(pSecDesc->get_Id());
        if (find != _securities.end())
            pOut = find->second;
        else
        {
            std::lock_guard<std::recursive_mutex> g(_lock);
            find = _securities.find(pSecDesc->get_Id());
            if (find != _securities.end())
                pOut = find->second;
            else 
            {
                CSmartConnection c(SEEODDATA);
                RETURNIFFAILED(get_SecurityDetails(c, pSecDesc, &pOut));
            }
        }
        if (pOut)
        {
            pOut->AddRef();
            *ppOut = pOut;
            return S_OK;
        }
        else
        {
            BERR << "Security Eid : " << pSecDesc->get_ESid() << ", Symbol: " << pSecDesc->get_Symbol() << " and Exch: " << pSecDesc->get_ExchCode() << " not found.";
            return E_NotFound;
        }
    }
    CATCHCLAUSE
}


SE_CODE CSecurityAPI::get_SecurityDetails(CSmartConnection& c, ISecurityDescription* pSecDesc, ISecurity** p)
{
    SP<CSecurity> psec;
    Query q = c->query();
    string exch = pSecDesc->get_ExchCode();
    std::transform(exch.begin(), exch.end(), exch.begin(), ::tolower);
    q << "select * from eod_" << exch << "_securities where sid=" << pSecDesc->get_ESid();
    SE_CODE code = E_NotFound;
    q.for_each([this, &pSecDesc, &psec, &code](const Row& rec)
    {
        code = CObject<CSecurity>::Create(get_Session(), pSecDesc, &psec);
        if (ISOK(code))
        {
            psec->set_Name( rec["sname"] );
            psec->set_LongName( rec["slname"] );
        }
    });
    return code;
}

long unsigned int CSecurityAPI::get_SecurityHistoricalPrices(ISecurityDescription* pDesc, Json::Value& root) 
{
    try
    {
        CSmartConnection conn(SEEODDATA);
        auto q = conn->query();
        q << "call list_HistoricalPrices(" << pDesc->get_ESid() << ", " << quote << pDesc->get_ExchCode() << ")";
        Json::Value records;
        q.for_each([&root, &records](const Row& rec)
        {
            Json::Value v;
            v["date"] = (CSTR)rec["hdate"];
            v["close"] = (R8)rec["hclose"];
            records.append(v);
        });
        root["Daily Prices"] = records;
        return S_OK;
    }
    CATCHCLAUSE

}


/* get a public company's information */
ErrorId CSecurityAPI::get_Company(ISecurityDescription* pSecDesc, IPublicCompanyInfo** ppOut) 
{

    /*
     *      a. Create a IGICSInfo class. Populate values from the table for the security
        populate values from the table for the security (call
     *      b. Create an IAddress instance (example above) and set to IPublicCompanyInfo set property for address.
     *      */
    try
    {
        const string funcName = "se.securities.get_Company";
        IPublicCompanyInfo* p;
        if (!FC.TrySPGetFValue(funcName, pSecDesc-get_Id(), p))
        {
            Poco::ScopedLock<Poco::Mutex> g(securityLock);
            if (!FC.TrySPGetFValue(funcName, pSecDesc-get_Id(), p))
            {
                RETURNIFFAILED(CObject<CPublicCompanyInfo>::Create(get_Session(), pSecDesc, &p));
                FC.SPSetFValue(funcName, pSecDesc->get_Id(), p);
                *ppOut = p;
                return S_Success;
            }
        }
        p->AddRef();
        *ppOut = p;
        return S_Success;
    }
    CATCHCLAUSE
}


ErrorId CSecurityAPI::SynchronizeSecurityDescription(ISecurityDescription* pIn)
{
    
    if (!pIn || !pIn->get_Symbol())
        return E_InvalidArg;

    ErrorId err = E_UnknownError; 
    stringstream sql(stringstream::in | stringstream::out);
    sql << "select security_id from se_securities where symbol = '";
    sql << + pIn->get_Symbol() ;
    sql << "'"; //TODO: and countryId = "
    CSmartConnection conn(SEDBCONN);
    auto res = conn->query(sql.str()).use();
    if (res)
    {
        auto row = res.fetch_row();
        if (row)
            return S_Success;
    }
    
    SP<ISecurity> pSecurity ;
    if (S_Success == (err = get_Security(pIn, &pSecurity)))
    {
        SP<IPublicCompanyInfo> pCo;
        auto retVal = pSecurity->get_Company(&pCo);
        auto name = pCo->get_Name();
        stringstream sql1(stringstream::in | stringstream::out);
        sql1 << "insert into se_securities (symbol, orgname) values ('" << pIn->get_Symbol() << "', '" << name << "')";
        string ss = sql1.str();
        if (conn->query(ss).exec())
        {
            return S_Success;
        }
        else
            return E_DBQueryException;
    }
    return err;
}

bool  CSecurityAPI::IsHoliday(se::EXCHANGEID exchangeId, se::CDate& date)
{
    I32 year = date.get_Year();
    I32 month = date.get_Month();
    I32 day = date.get_Day();
    stringstream Date;
    Date<<year;
    Date<<"-";
    Date<<month;
    Date<<"-";
    Date<<day;
    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q << "SELECT count(*) FROM  `se_holidays` WHERE  `exchange_id` = " 
            << exchangeId <<" AND  `holiday` = " << Date.str();
    auto qres = q.store();
    return qres.size();
}

CDate CSecurityAPI::LastTradingDay()
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        return LastTradingDay(conn);
    }
    catch(std::exception& ex)
    {
       // LOGE(_logger, ex.what());
        throw;
    }
}


CDate CSecurityAPI::LastTradingDay(CSmartConnection& conn, bool OnlyCompletedDays)
{
    CDate dtLt;
//    // if holiday loop again
//    const string funcName = "se.securities.holidays";
//    if (!FC.TryGetFValue(funcName, holidays))
//    {
//        Query qHolidays = conn->query();
//        qHolidays << "select distinct(holiday) from se_holidays where exchange_id in (1, 2, 3);";    // we need to fix this in a future release.
//        qHolidays.storein_set(holidays);
//        FC.SetFValue(funcName, holidays);
//    }
//    
//    //loop from today.
//    CDate dtcheck = CDate::Now();
//
//    if (OnlyCompletedDays)
//    {
//        if (dtcheck.Hour < 17)
//            dtcheck = dtcheck - 1;
//    }
//    
//    for(int i=0; i!=7; ++i)
//    {
//        // if time is not after 5pm, then 'today' should not be considered.
//        dtcheck.
//        if (dtcheck.get(period::day_of_week()) == 1)
//        {
//            dtcheck = dtcheck - (2 * period::day());
//        }
//        else if (dtcheck.get(period::day_of_week()) == 7)
//        {
//            dtcheck = dtcheck - period::day();
//        }
////        else
////        {
////            dtcheck = dtcheck - period::day();    // for historical checks we will always go until valid previous day.
////        }
//        sql_date dt;
//        dt.year(dtcheck.get(period::year()));
//        dt.month(dtcheck.get(period::month()) + 1);
//        dt.day(dtcheck.get(period::day()));
//        
//        DataRowHolidays row;
//        row.holiday = dt;
//        
//        if (holidays.find(row) == holidays.end())
//            break;
//        
//        dtcheck = dtcheck - period::day();
//        
//    }
//    
//    
//    
//    dtLt.set_Day(dtcheck.get(period::day()));
//    dtLt.set_Month(dtcheck.get(period::month()) + 1);
//    dtLt.set_Year(dtcheck.get(period::year()));
    return dtLt;
}

long unsigned int CSecurityAPI::SearchModule(const char* searchText, SearchMethod searchMethod, Json::Value& output)
{
    try
    {
        BDBG << "Searching securities for searchtext "  << searchText;
        //call find_MatchingSymbols('T', 1);
        Json::Value tickers;
        auto ret = SearchSymbols(searchText, searchMethod, tickers);
        if (FAILED(ret))
        {
            BERR << get_CodeMessage(ret);
        }
        else
            output.append(tickers);
            
        //call find_MatchingNames('AT&', 1);
        Json::Value products;
        ret = SearchProducts(searchText, searchMethod, products);
        if (FAILED(ret))
        {
            BERR << get_CodeMessage(ret);
        }
        else
            output.append(products);
        
        if (ISOK(ret))
            BDBG << "Searching securities for searchtext "  << searchText << " succeeded";
        
        
        return ret;
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::SearchSymbols(const char* searchText, SearchMethod searchMethod, Json::Value& output)
{
    try
    {
        CSmartConnection conn(SEEODDATA);
        auto q = conn->query();
        q << "call find_MatchingSymbols(" << quote << searchText << " , " << (searchMethod == SearchMethod::Exact ? 1 : 0) << " )";
        Json::Value symbols;
        auto sqr = q.store();
        for (auto r : sqr)
        {
            Json::Value p;
            p["Id"] = (Json::Int64)r["esid"];
            p["Symbol"] = (CSTR)r["ssym"];
            p["Name"] = (CSTR)r["sname"];
            p["Type"] = (CSTR)r["stype"];
            p["Exchange"] = (CSTR)r["ecode"];
            symbols.append(p);
        }
        output["Category"] = "Tickers";
        output["Items"] = symbols;
        while (q.more_results())
            q.store_next();
        return S_OK;
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::SearchProducts(const char* searchText, SearchMethod searchMethod, Json::Value& output)
{
    try
    {
        CSmartConnection conn(SEEODDATA);
        auto q = conn->query();
        q << "call find_MatchingNames(" << quote << searchText << " , " << (searchMethod == SearchMethod::Exact ? 1 : 0) << " )";
        Json::Value products;
        auto sqr = q.store();
        for (auto r : sqr)
        {
            Json::Value p;
            p["Id"] = (Json::Int64)r["esid"];
            p["Symbol"] = (CSTR)r["ssym"];
            p["Name"] = (CSTR)r["sname"];
            p["Type"] = (CSTR)r["stype"];
            p["Exchange"] = (CSTR)r["ecode"];
            products.append(p);
        }
        output["Category"] = "Products";
        output["Items"] = products;
        while (q.more_results())
            q.store_next();
        return S_OK;
    }
    CATCHCLAUSE
}



ErrorId CSecurityAPI::ProcessSecurityHistoricalPrices(ISecurityDescription* pIn)
{
    CSmartConnection conn(SEDBCONN);
    return ProcessSecurityHistoricalPrices(conn, pIn);
}

ErrorId CSecurityAPI::ProcessSecurityHistoricalPrices(CSmartConnection& conn, ISecurityDescription* pIn)
{
    ErrorId retCode = E_UnknownError;
    try
    { 
        CDate dt = LastTradingDay(conn);
        CDate dtMax;
        Query qmaxdate = conn->query();
        //qmaxdate << "select ifnull(max(history_date), '0-0-0') as history_date from se_sec_eq_pricehistory where product_id = " << pIn->get_Id();
        qmaxdate << "select max(history_date) as history_date from se_sec_eq_pricehistory where product_id = " << pIn->get_Id();
        StoreQueryResult rmaxdate = qmaxdate.store();
        if (rmaxdate.size())
        {
            sql_date_null f = (sql_date)rmaxdate[0][0];
            if (!f.is_null)
            {
                dtMax.set_Day((I16)f.data.day());
                dtMax.set_Month((I16)f.data.month());
                dtMax.set_Year((I16)f.data.year());
            }
        }
        
        if (dtMax.IsEmpty())    // grab all
        {
            //LOG4CXX_INFO(_logger, "Table not contain Historical Prices");
            sleep(1);
            retCode = DownloadHistoricalPrices(conn, pIn);
            if (FAILED(retCode))
            {
                //LOG4CXX_ERROR(_logger, "download error");
                return retCode;
            }
            return S_Success;
        }
        else
        {
            DATAPERIOD dp = dt - dtMax;
            if (dp.Day == 0)
            {
                //LOG4CXX_INFO(_logger, "No update required for security " << pIn->get_Symbol() << "(" << pIn->get_Id() << ")");
                return S_Success;
            }
            return DownloadHistoricalPrices(conn, pIn, dtMax, dt);
        }
    }
    catch(std::exception ex)
    {
       //LOG4CXX_ERROR(_logger, "Error:"<< ex.what());
        
        return E_UnknownError;
    }
}

ErrorId CSecurityAPI::ProcessSecurityDailyPrice(ISecurityDescription* pIn)
{
    try
    { 
        struct passwd* pPwd = getpwuid(getuid()); 
        string strDest = pPwd->pw_dir;
        strDest += "/PriceHistory/";
        if (!CDirectory::Create(strDest.c_str()))
        {
            //LOG4CXX_INFO(_logger, "Already created edgar schema folder.");
        }
        
        string url = "http://download.finance.yahoo.com/d/quotes.csv?s=";
        url = url + (string)pIn->get_Symbol() + "&f=d1d2ovhgp.csv";
        CHttpClient htp;
        
        
        strDest  = strDest+"/daily.csv";        
        htp.GetFile(url.c_str(),strDest.c_str());        
        string data(strDest.c_str());
        ifstream in(data.c_str());
        if (!in.is_open()) return 1;
        typedef tokenizer< escaped_list_separator<char> > Tokenizer;
        vector< string > vec;
        string line;
        string prevDate,curDate,Open,Volume,High,Low,prevClose;
        while (getline(in,line))
        {
            Tokenizer tok(line);
            vec.assign(tok.begin(),tok.end());
            prevDate=vec[0]; 
            curDate = vec[1];
            Open = vec[2];
            Volume = vec[3];
            High = vec[4];
            Low = vec[5];
            prevClose = vec[6];
        }
        string cDate =  dateConversion(curDate.c_str());
        string pDate =  dateConversion(prevDate.c_str());
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q<<"UPDATE se_sec_eq_pricehistory SET history_close = "<< prevClose <<" where product_id = "<<pIn->get_Id()<<" and history_date = " << pDate;
        q.exec();
        SaveHistoricalPrices(conn, pIn->get_Id(),cDate,Open,High,Low,0,Volume,0);
    }
    catch(std::exception ex)
    {
       //LOG4CXX_INFO(_logger, "Error:"<< ex.what());
        
        return E_UnknownError;
    }
}

ErrorId CSecurityAPI::get_SecurityHistoricalPrices(ISecurityDescription* pIn, CDate start, CDate end, EAM economicAdjustments, HPAM averagingMethod, IROList<PeriodPriceAdjustedPair> **ppOut)
{
   // LOGIP("Entering get_SecurityHistoricalPrices");
    try
    {
        if (pIn->get_Id() == 0)
        {
           // LOGEP("pIn parameter doesn provide a security id.");
            return E_InvalidArg;
        }
        const string funcName = "se.securities.get_SecurityHistoricalPrices";
        IROList<PeriodPriceAdjustedPair>* p;
        if (!FC.TrySPGetFValue(funcName, pIn, start.ToNumber(), end.ToNumber(), (I32) averagingMethod, p))
        {
            Poco::ScopedLock<Poco::Mutex> g(securityLock);
            if (!FC.TrySPGetFValue(funcName, pIn, start.ToNumber(), end.ToNumber(), (I32) averagingMethod, p))
            {

                //select * from se_sec_eq_pricehistory where product_id = 102 AND history_date BETWEEN '2013-11-05' AND '2013-11-11' order by history_date desc;
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "select history_date, history_close from se_sec_eq_pricehistory where product_id = " << pIn->get_Id();

                if (!start.IsEmpty() && !end.IsEmpty())
                {
                    SP<IString> pStrStart;
                    start.ToString(&pStrStart);

                    SP<IString> pStrEnd;
                    end.ToString(&pStrEnd);

                    q << " AND history_date BETWEEN '" << pStrStart->get_Buffer() << "' AND '" << pStrEnd->get_Buffer() << "'";
                }
                else
                {
                    if (!start.IsEmpty())
                    {
                        SP<IString> pStrStart;
                        start.ToString(&pStrStart);
                        q << " and history_date >= '" << pStrStart->get_Buffer() << "'";
                    }
                    if (!end.IsEmpty())
                    {
                        SP<IString> pStrEnd;
                        end.ToString(&pStrEnd);
                        q << " and history_date <= '" << pStrEnd->get_Buffer() << "'";
                    }
                }

                q << " order by history_date;" ;

               // LOGIP(q.str());

                // execute and store
                StoreQueryResult result = q.store();
                if (!result.size())
                {
                   // LOGWP("No data for the given criteria. Existing S_False");
                    return S_False;
                }
                CCommon fmath;
                typedef list<R8> PriceList;
                PriceList prices;

                RETURNIFFAILED(CROListNonSerialized<PeriodPriceAdjustedPair>::Create(get_Session(), &p));
                R8 startInflation = 1.0;
                if (economicAdjustments & EAM_Inflation)
                {
                    sql_date date = result[0][0];
                    Query qInflation = conn->query();
                    qInflation << "select value from se_eco_inflation_cpi where country_id = 1 and state_id = 0"
                        << " and year = " << date.year() 
                        << " and month = " << (date.month() == ' ' ? 1 : date.month()) << ";";
                   // LOGIP(qInflation.str());
                    StoreQueryResult r = qInflation.store();
                    if (r.size())
                        startInflation = r[0][0];
                }


                if (averagingMethod == HPAM_Daily)
                {
                    for (Row& r : result)
                    {
                        sql_date date = r[0];
                        DATAPERIOD dp;
                        dp.Day = date.day();
                        dp.Month = date.month();
                        dp.Year = date.year();
                        R8 price = (R8)r[1];
                        CEconomincallyAdjustedPrices *ppdata;
                        CObject<CEconomincallyAdjustedPrices>::Create(get_Session(), &ppdata);
                        ppdata->set_Value(EAM_None, price);
                        if (economicAdjustments & EAM_Inflation)
                        {
                            //price = // adjust for infliation from a inflation hashmap.
                        }
                        auto tpl = make_tuple(dp, ppdata);
                        ((CROListNonSerialized<PeriodPriceAdjustedPair>*)p)->Add(tpl);
                    }            
                }
                else if (averagingMethod == HPAM_Monthly)
                {
                    int month = 0, year = 0;
                    for (Row& r : result)
                    {
                        sql_date date = r[0];
                        R8 price = r[1];
                        if (month != date.month())
                        {
                            if (month && year)
                            {
                                R8 hm = fmath.HarmonicMean(prices.begin(), prices.end());
                                if (economicAdjustments & EAM_Inflation)
                                {
                                    //hm = // adjust for infliation from a inflation hashmap.
                                }
                                MONTHYEAR f ;
                                f.Month = month;
                                f.Year = year;
                                CEconomincallyAdjustedPrices *ppdata;
                                CObject<CEconomincallyAdjustedPrices>::Create(get_Session(), &ppdata);
                                ppdata->set_Value(EAM_None, hm);
                                if (economicAdjustments & EAM_Inflation)
                                {
                                    //price = // adjust for infliation from a inflation hashmap.
                                    Query qInflation = conn->query();
                                    qInflation << "select value from se_eco_inflation_cpi where country_id = 1 and state_id = 0"
                                            << " and year = " << year 
                                            << " and month = " << month << ";";
                                    StoreQueryResult rInflation = qInflation.store();
                                    if (rInflation.size())
                                    {
                                        R8 adj = hm * startInflation/rInflation[0][0];
                                        ppdata->set_Value(EAM_Inflation, adj);
                                    }
                                    else
                                        ppdata->set_Value(EAM_Inflation, hm);
                                }
                                auto tpl = make_tuple(f, ppdata);
                                ((CROListNonSerialized<PeriodPriceAdjustedPair>*)p)->Add(tpl);
                                prices.clear();
                            }
                            month = date.month();
                            year = date.year();
                        }
                        prices.push_back(price);
                    }
                    if (prices.size())
                    {
                        R8 hm = fmath.HarmonicMean(prices.begin(), prices.end());
                        MONTHYEAR f ;
                        f.Month = month;
                        f.Year = year;
                        CEconomincallyAdjustedPrices *ppdata;
                        CObject<CEconomincallyAdjustedPrices>::Create(get_Session(), &ppdata);
                        ppdata->set_Value(EAM_None, hm);
                        if (economicAdjustments & EAM_Inflation)
                        {
                            ppdata->set_Value(EAM_Inflation, hm);
                        }
                        auto tpl = make_tuple(f, ppdata);
                        ((CROListNonSerialized<PeriodPriceAdjustedPair>*)p)->Add(tpl);
                    }
                }
                else if (averagingMethod == HPAM_Weekly)
                {
    //                for (Row& r : result)
    //                {
    //
    //                }            
                }
                FC.SPSetFValue(funcName, pIn, start.ToNumber(), end.ToNumber(), (I32) averagingMethod, p);
                *ppOut = p;
                return S_Success;
            }
        }
        p->AddRef();
        *ppOut = p;
       // LOGIP("Exiting get_SecurityHistoricalPrices");
        return S_Success;
    }
    catch (std::exception& ex)
    {
       // LOGE(_logger, "Exiting get_SecurityHistoricalPrices Method reported an error. Error: " << ex.what());
        return E_UnknownError;
    }
}

ErrorId CSecurityAPI::get_SecurityHistoricalPrices(ISecurityDescription* pIn, CDate start, CDate end, IROSPList<ISecurityPriceEx>** ppOut)
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        SP<CROSPList<ISecurityPriceEx>> pList;
        ErrorId retCode = CObject<CROSPList<ISecurityPriceEx>>::Create(get_Session(), &pList);
        if (FAILED(retCode))
        {
            //LOG4CXX_ERROR(_logger, "Error Creating List");
            return retCode;
        }
        I64 product_id;
        {
            Query q=conn->query();
            q<<"SELECT product_id FROM list_ListedCompaniesExtended where product_symbol ="<<quote<<pIn->get_Symbol();
            auto res = q.use();
            auto row = res.fetch_row();
            product_id = (I64)row["product_id"];
        }
        Query q = conn->query();
        sql_date sqlstart,sqlend;
        CMyqlUtilities::PopulateDateTime(start, sqlstart);
        CMyqlUtilities::PopulateDateTime(end, sqlend);
        if(start.IsEmpty() && !end.IsEmpty())
            q<<"select * from se_sec_eq_pricehistory where product_id = "<<product_id<<" and history_date <"<<quote<<sqlend ;
        else if(!start.IsEmpty() && end.IsEmpty())
            q<<"select * from se_sec_eq_pricehistory where product_id = "<<product_id<<" and history_date >"<<quote<<sqlstart ;
        else if(!start.IsEmpty() && end.IsEmpty())
            q<<"select * from se_sec_eq_pricehistory where product_id = "<<product_id;
        else
            q<<"select * from se_sec_eq_pricehistory where product_id ="<< product_id << " and history_date \
            BETWEEN "<<quote<<sqlstart<<" and "<<quote<<sqlend;
        auto qRes = q.use();        
        if (!qRes)
        {
            //LOG4CXX_ERROR(_logger, "can't found history");
            return E_DBQueryException;
        }        
        Row row ;        
        while (row = qRes.fetch_row())
        {        
            SP<ISecurityPriceEx> pSecurityPriceEx ;
            retCode = CObject<CSecurityPriceEx>::Create(get_Session(), &pSecurityPriceEx);
            if (FAILED(retCode))
            {
                //LOG4CXX_ERROR(_logger, "can't create SecurityPriceEx");
                return retCode;
            }
            retCode = SetSecurityPriceEx(row, &pSecurityPriceEx);        
            if (FAILED(retCode))
            {
                //LOG4CXX_ERROR(_logger,"Failed with error code " << retCode);
                return retCode;
            }
            pList->Add(pSecurityPriceEx);          
        }
        pList.CopyTo(ppOut);
        return S_Success; 
    }
    catch(std::exception ex)
    {
        
        //LOG4CXX_INFO(_logger, "Error:"<< ex.what());
        return E_UnknownError;
    }
}

ErrorId CSecurityAPI::get_SecurityPrice(ISecurityDescription* pIn, ISecurityPriceEx** ppOut, CDate date)
{
    try
    {
        
        
        CSmartConnection conn(SEDBCONN);
        I64 product_id;
        {
            Query q=conn->query();
            q<<"SELECT product_id FROM list_ListedCompaniesExtended where product_symbol ="<<quote<<pIn->get_Symbol();
            auto res = q.use();
            auto row = res.fetch_row();
            product_id = (I64)row["product_id"];
        }
        Query q = conn->query();
        sql_date sqldate;
        CMyqlUtilities::PopulateDateTime(date, sqldate);
        q<<"select * from se_sec_eq_pricehistory where product_id = "<<product_id<<" and history_date = "<<quote<<sqldate ;
        
        auto qRes = q.use();        
        if (!qRes)
        {
            //LOG4CXX_ERROR(_logger, "can't found history");
            return E_DBQueryException;
        }        
        Row row ; 
        row = qRes.fetch_row();
        if(row)
        {
            ErrorId retCode = SetSecurityPriceEx(row, ppOut);        
            if (FAILED(retCode))
            {
                //LOG4CXX_ERROR(_logger,"Failed with error code " << retCode);
                return retCode;
            }
        }
        else
        {
            //LOG4CXX_ERROR(_logger, "Not Found");
            return E_NotFound;
        }
    }
    catch(std::exception ex)
    {
        
        //LOG4CXX_INFO(_logger, "Error:"<< ex.what());
        return E_UnknownError;
    }
}


ErrorId CSecurityAPI::ProcessSecDailyPrice(I32 WaitPerSymbol)
{
    try
    { 
       // LOGEP("Starting ProcessSecDailyPrice");
        
        struct passwd* pPwd = getpwuid(getuid()); 
        string strDest = pPwd->pw_dir;
        strDest += "/PriceHistory/";
        if (!CDirectory::Create(strDest.c_str()))
        {
            //LOG4CXX_INFO(_logger, "Already created edgar schema folder.");
        }
        CSmartConnection conn(SEDBCONN);

        string cList = "";
        
        CDate dt = LastTradingDay(conn);
        SP<IString> pStrDate;
        RETURNIFFAILED(dt.ToString(&pStrDate));
       // LOGI(_logger, "Checking daily price availability for date " << pStrDate->get_Buffer());
       // get today's date.
        Query qSel = conn->query();
        
        qSel << "select a.product_id, a.product_symbol, ifnull(a.product_yahoosymbol, a.product_symbol) as product_yahoosymbol, a.product_exchange_id from se_sec_products a " ;
        qSel << "join list_ListedCompaniesExtended b using (product_id) " ;
        qSel << "where product_id not in (select product_id from se_sec_eq_pricehistory ";
        qSel << "where history_date = " << quote << pStrDate->get_Buffer() << ") order by product_id;";

       // LOGIP(qSel.str());
        typedef map<string, I64> PRODUCTIDMAP;
        auto rSel = qSel.store();
        if (!rSel.size())
        {
            //LOG4CXX_INFO(_logger, "All security records are processed for daily prices.");
            return S_Success;
        }
        
       // LOGI(_logger, "Found " << rSel.size() << " securities to be processed for daily prices.");
        
        int index = 0;
        for (Row rp : rSel)
        {
            I64 product_id = rp[0];
            string symbol = (string)rp[2];
            trim(symbol);
            try
            {
               // LOGI(_logger, "Checking historical price for product :" << product_id << "(yahoo-symbol : " << symbol << ")");
                SP<ISecurityDescription> pSd;
                RETURNIFFAILED(ISecurityDescription::Create(get_Session(), &pSd));
                pSd->set_Id(product_id);
                pSd->set_Symbol(symbol.c_str());
                if (FAILED(ProcessSecurityHistoricalPrices(conn, pSd)))
                {
                   // LOGE(_logger, "Failed to process historical prices for product :" << product_id << "(symbol : " << symbol << ")");
                    continue;   // no history data for this product. ignore. don't add to the map.
                }
               // LOGI(_logger, "Done with checking historical price for product :" << product_id << "(symbol : " << symbol << ")");
            }
            catch(std::exception& ex)
            {
               // LOGE(_logger, "Rolling back transaction to insert historical prices for productid/symbol " << product_id << "/" << symbol << ". Reported error : " << ex.what());
            }
            //sleep(WaitPerSymbol);
        }            
       // LOGIP("Completed history price checks");

        /*WE DON'T BELOW OPERATIONS BASED ABOVE CHANGES. USE IT ONLY IF REALLY NEEDED.*/
//       // LOGI(_logger, "Processing daily prices for select list of symbols in " << cList);
//        PRODUCTIDMAP pidmap;
//        string url = "http://download.finance.yahoo.com/d/quotes.csv?s=";
//        string flags = "&f=sxd1ohgl1vd";
//        string syms;
//        index = 0;
//        // get today's price all our symbols
//        Query qAll = conn->query();
//        qAll << "select product_id, product_symbol, product_exchange_id from se_sec_products order by product_id";
//        auto rAll = qAll.store();
//        for (auto rrAll : rAll)
//        {
//            string s = (string)rrAll[1];
//            trim(s);
//            if (syms.empty())
//                syms = s;
//            else
//                syms += "+" + s;
//            pidmap.insert(make_pair(s, (I64)rrAll[0]));
//            if ( (index > 0 && 0 == (index % 10)) || (index == rAll.size()-1) )
//            {
//                // note the order of yahoo format : 
//                // exchange, date, open, high, low, close, volume, dividends
//                // adjusted close from dividends is calculated as close - ((1 - div)/close)
//                // for split stocks, we have to split in the ratio, so 2:1, means every stock is split into equal half 
//                // means close / 2
//                // if 1:2, every stock is doubled in value, close * 2.
//
//                string uri = url + syms + flags;
//                CHttpClient htp;
//                stringstream filePath;
//                filePath << strDest << "ph-" << cList << ".csv";                
//                htp.GetFile(uri.c_str(),filePath.str().c_str());        
//                string data(filePath.str().c_str());
//                ifstream in(data.c_str());            
//
//
//                if (!in.is_open()) return 1;
//                typedef tokenizer< escaped_list_separator<char> > Tokenizer;
//                vector< string > vec;
//                string line;
//                string sym, exch, date,Open,Volume,High,Low,Close,div;
//                while (getline(in,line))
//                {
//                    Tokenizer tok(line);
//                    vec.assign(tok.begin(),tok.end());
//                    // ensure vector has required size
//                    if (vec.size() < 9) 
//                    {
//                        in.close();
//                        continue;
//                    }
//                    sym = vec[0];
//                    exch = vec[1];
//                    date=vec[2]; 
//                    Open = vec[3];
//                    High = vec[4];
//                    Low = vec[5];
//                    Close = vec[6];
//                    Volume = vec[7];
//                    div = vec[8];
//                    if(date != "N/A")
//                        date =  dateConversion(date.c_str());
//                    stringstream adjclose;
//                    if (div != "")
//                    {
//                        double dClose = atof(Close.c_str());
//                        if (dClose > 0.0)
//                        {
//                            double dDiv = atof(div.c_str());
//                            if (dDiv > 0.0)
//                            {
//                                double dAdj = dClose * ((1 - dDiv)/dClose);
//                                adjclose.setf(std::ios::fixed, std::ios::floatfield);
//                                adjclose.precision(4);
//                                adjclose << dAdj;
//                            }
//                            else
//                                adjclose << Close;
//                        }
//                        else
//                            adjclose << Close;
//                    }
//                    else
//                    {
//                        adjclose << Close;
//                    }
//
//                    unlink(filePath.str().c_str());
//                    
//                    trim(sym);
//                    I64 pid = pidmap[sym];
//                    SaveHistoricalPrices(conn, pid, date,Open,High,Low,Close,Volume, adjclose.str());
//                   // LOGI(_logger, "Saved daily price for product id " << pid << "(Symbol:" << sym << ")");
//                }
//                syms.clear();
//                pidmap.clear();
//                sleep(2);
//            }
//            ++index;
//        }
        
        //LOG4CXX_INFO(_logger, "Completed function ProcessSecDailyPrice call successfully.");
        return S_Success;
    }
    catch(std::exception ex)
    {
       //LOG4CXX_INFO(_logger, "Error:"<< ex.what());
        
        return E_UnknownError;
    }
    catch(...)
    {
       // LOGEP("Unknown error reported while processing daily security price. Failed");
        return E_UnknownError;
    }
}


/***************************Private Methods***************************************/
ErrorId CSecurityAPI::SetSecurityPriceEx(Row const& row, ISecurityPriceEx** ppOut)
{
    if (!row) return E_NotFound;
    ErrorId retCode = CObject<CSecurityPriceEx>::Create(get_Session(), ppOut);
    if (FAILED(retCode))
        return retCode;
    sql_date sqldate;
    CDate date;
    sqldate = (sql_date)row["history_date"]; 
    CMyqlUtilities::PopulateDateTime(sqldate, date);
    ((CSecurityPriceEx*)(*ppOut))->_dt = date;
    ((CSecurityPriceEx*)(*ppOut))->_price = (R8)row["history_open"];
    ((CSecurityPriceEx*)(*ppOut))->_volume = (R8)row["history_vol"];
    ((CSecurityPriceEx*)(*ppOut))->_high = (R8)row["history_high"];
    ((CSecurityPriceEx*)(*ppOut))->_low = (R8)row["history_low"];
    ((CSecurityPriceEx*)(*ppOut))->_close = (R8)row["history_close"];
    ((CSecurityPriceEx*)(*ppOut))->_adjClose = (R8)row["history_adjustedclose"];
    return S_Success;
}

CSTR CSecurityAPI::dateConversion(CSTR date)
{ 
    string ldt = date;
    string first,middle,last;
    if(ldt.size() == 9)
    {
        first = date;
        middle = date;
        last = date;
        first.erase(1);
        middle.erase(0,2);
        last = middle;
        middle.erase(2);
        last.erase(0,3);
    }
   
    else
    {
        first = date;
        middle = date;
        last = date;
        first.erase(2);
        middle.erase(0,3);
        last = middle;
        middle.erase(2);
        last.erase(0,3);
    }
    string dt = last + "-" + first + "-" + middle;
    return dt.c_str();
}

ErrorId CSecurityAPI::DailyPrice(CSTR symbol)
{
    struct passwd* pPwd = getpwuid(getuid()); 
    string strDest = pPwd->pw_dir;
    strDest += "/PriceHistory/";
    if (!CDirectory::Create(strDest.c_str()))
    {
        //LOG4CXX_INFO(_logger, "Already created edgar schema folder.");
    }
    
    string url = "http://download.finance.yahoo.com/d/quotes.csv?s=";
    url = url + symbol + "&f=d1d2ovhgp.csv";
    CHttpClient htp;
    strDest  = strDest+"/dailyhistory.csv";        
    htp.GetFile(url.c_str(),strDest.c_str());  
    string data(strDest.c_str());       
  
    ifstream in(data.c_str());
    if (!in.is_open()) return 1;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    string line;
    string prevDate,curDate,Open,Volume,High,Low,prevClose;
    while (getline(in,line))
    {
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        prevDate=vec[0]; 
        curDate = vec[1];
        Open = vec[2];
        Volume = vec[3];
        High = vec[4];
        Low = vec[5];
        prevClose = vec[6];
    }
    string cDate =  dateConversion(curDate.c_str());
    string pDate =  dateConversion(prevDate.c_str());
    CSmartConnection conn(SEDBCONN);
    Query q = conn->query();
    q<<"UPDATE se_sec_eq_pricehistory SET history_close = "<< prevClose <<" where product_id = "<<_pID<<" and history_date = " << pDate;
    q.exec();
    SaveHistoricalPrices(conn, _pID,cDate,Open,High,Low,0,Volume,0);
}

ErrorId CSecurityAPI::ReadCsv()
{
    //LOG4CXX_DEBUG(_logger, "serching for csv link....");
    string &answer = _csvHtml;
    
    const string pattern1="(.*)(></tr></table><p><a href=\")(.*)(\"><img src=)(.*)(><strong>Download to Spreadsheet</strong>)(.*)";
    boost::regex regexPattern1(pattern1, boost::regex::extended);
    boost::smatch what;
    bool isMatchFound1 = boost::regex_match(answer, what, regexPattern1);
    if (isMatchFound1)
    {
        _csvLink = what[3];
        //LOG4CXX_DEBUG(_logger, "CSV Link  ="<<what[3]);
    }
    return S_Success;
}

ErrorId CSecurityAPI::GetCSV(CSTR symbol)
{
    //LOG4CXX_DEBUG(_logger, "Start Parsing Url for CSV");
    xmlDocPtr doc = 0;
    xmlBufferPtr nodeBuffer = 0;
    try
    {
        string url1="http://finance.yahoo.com/q/hp?s=";
        string Surl=url1+symbol;
        char* url;
        xmlChar *key;
        xmlNodePtr cur; 
        url=&Surl[0];
        nodeBuffer = xmlBufferCreate();
        doc = htmlParseFile (url, NULL);
        cur = xmlDocGetRootElement(doc);
        cur = cur->xmlChildrenNode;
        cur = cur->next;
        xmlNodeDump(nodeBuffer, doc, cur, 0, 1);
        key=nodeBuffer->content;
        _csvHtml = (const char *)key; 
        ErrorId retCode = ReadCsv();
        // delete buffer
        xmlBufferFree(nodeBuffer);
        nodeBuffer = 0;
        xmlFreeDoc(doc);
        doc = 0;
        if (FAILED(retCode))
           throw se::seexception("Reading historical price csv file failed.");     
        //LOG4CXX_DEBUG(_logger, "End Parsing URL");
        return retCode;
    }
    catch(std::exception& ex)
    {
        if (nodeBuffer)
            xmlBufferFree(nodeBuffer);
        
        if (doc)
            xmlFreeDoc(doc);
        
       // LOGE(_logger, "Failed to downoad csv for historical prices with error " << ex.what());
        
        return E_UnknownError;
    }
}

ErrorId CSecurityAPI::GetCSV(CSTR symbol, CDate& from, CDate& to)
{
    //LOG4CXX_DEBUG(_logger, "Start Parsing Url for CSV");
    xmlDocPtr doc = 0;
    xmlBufferPtr nodeBuffer = 0;
    try
    {
        stringstream url ;
        url << "http://finance.yahoo.com/q/hp?s=" 
            << symbol 
            << "&a="<< from.get_Month()-1 << "&b=" << from.get_Day() << "&c=" << from.get_Year() 
            << "&d=" << to.get_Month()-1 << "&e=" << to.get_Day() << "&f=" << to.get_Year() 
            << "&g=d";
       // LOGIP(url.str());
        xmlChar *key;
        xmlNodePtr cur; 
        nodeBuffer = xmlBufferCreate();
        doc = htmlParseFile (url.str().c_str(), NULL);
        cur = xmlDocGetRootElement(doc);
        cur = cur->xmlChildrenNode;
        cur = cur->next;
        xmlNodeDump(nodeBuffer, doc, cur, 0, 1);
        key=nodeBuffer->content;
        _csvHtml = (const char *)key; 
        ErrorId retCode = ReadCsv();
        // delete buffer
        xmlBufferFree(nodeBuffer);
        nodeBuffer = 0;
        xmlFreeDoc(doc);
        doc = 0;
        if (FAILED(retCode))
           throw se::seexception("Reading historical price csv file failed.");     
        //LOG4CXX_DEBUG(_logger, "End Parsing URL");
        return retCode;
    }
    catch(std::exception& ex)
    {
        if (nodeBuffer)
            xmlBufferFree(nodeBuffer);
        
        if (doc)
            xmlFreeDoc(doc);
        
       // LOGE(_logger, "Failed to downoad csv for historical prices with error " << ex.what());
        
        return E_UnknownError;
    }
}

ErrorId CSecurityAPI::DownloadHistoricalPrices(CSmartConnection& conn, ISecurityDescription* pSec)
{
    try
    {
        GetCSV(pSec->get_Symbol());
        struct passwd* pPwd = getpwuid(getuid()); 
        string strDest = pPwd->pw_dir;
        strDest += "/PriceHistory/";
        if (!CDirectory::Create(strDest.c_str()))
        {
            //LOG4CXX_INFO(_logger, "Already created edgar schema folder.");
        }

        //http://ichart.finance.yahoo.com/table.csv?s=CSCO&a=02&d=10&e=6&f=2013&g=d&b=26&c=1990&ignore=.csv
        
        CHttpClient htp;
        const char *url = _csvLink.c_str();

        strDest  = strDest+"/history.csv";        
        htp.GetFile(url,strDest.c_str());  
        string data(strDest.c_str()); 

        ifstream in(data.c_str());
        if (!in.is_open()) return E_ReadError;
        typedef tokenizer< escaped_list_separator<char> > Tokenizer;
        vector< string > vec;
        string line;
        string Date,Open,High,Low,Close,Volume,Adj_Close;
        I32 flag=0;
        try
        {
            conn.BeginTransaction();
            while (getline(in,line))
            {
                if(flag==0)
                {
                    flag=1;
                    continue;
                }
                Tokenizer tok(line);
                vec.assign(tok.begin(),tok.end());
                if (vec.size() < 7)
                {
                    in.close();
                   // LOGW(_logger, "No history price data found for " << pSec->get_Symbol());
                    conn.RollbackTransaction();
                    return S_False;
                }
                Date=vec[0]; 
                Open = vec[1];
                High = vec[2];
                Low = vec[3];
                Close = vec[4];
                Volume = vec[5];
                Adj_Close = vec[6];
                ErrorId retCode = SaveHistoricalPrices(conn, pSec->get_Id(),Date,Open,High,Low,Close,Volume,Adj_Close);
                if (FAILED(retCode))
                {
                    conn.RollbackTransaction();
                    in.close();
                    //LOG4CXX_ERROR(_logger, "save price error");
                    return retCode;
                }
            }
            in.close();
            conn.CommitTransaction();
        }
        catch(std::exception& ex)
        {
            conn.RollbackTransaction();
            in.close();
           // LOGE(_logger, "Download history prices method failed while saving history prices with error " << ex.what());
            throw;
        }
        return S_Success;
    }
    catch(std::exception ex)
    {
       // LOGE(_logger, "Download history price method reported error " << ex.what());
        return E_UnknownError;
    }
}

ErrorId CSecurityAPI::DownloadHistoricalPrices(CSmartConnection& conn, ISecurityDescription* pSec, CDate& dtFrom, CDate& dtTo)
{
    try
    {
        /* this one we don't need a dual fetch */
        /*
         * GetCSV(pSec->get_Symbol(), dtFrom, dtTo);
         * */
        
        struct passwd* pPwd = getpwuid(getuid()); 
        string strDest = pPwd->pw_dir;
        strDest += "/PriceHistory/";
        if (!CDirectory::Create(strDest.c_str()))
        {
            //LOG4CXX_INFO(_logger, "Already created edgar schema folder.");
        }
        
        // sample csv http://ichart.finance.yahoo.com/table.csv?s=RDN&amp;a=9&amp;b=31&amp;c=2013&amp;d=10&amp;e=4&amp;f=2013&amp;g=d&amp;ignore=.csv

        stringstream urlstrm ;
        urlstrm << "http://ichart.finance.yahoo.com/table.csv?s=" 
                << pSec->get_Symbol() << "&amp;a=" << dtFrom.get_Month()-1 
                << "&amp;b=" << dtFrom.get_Day() << "&amp;c=" << dtFrom.get_Year() << "&amp;d=" 
                << dtTo.get_Month()-1 << "&amp;e=" << dtTo.get_Day() << "&amp;f=" << dtTo.get_Year() 
                << "&amp;g=d&amp;ignore=.csv";
        
        CHttpClient htp;
        const char *url = urlstrm.str().c_str();
        
       // LOGIP(urlstrm.str());

        strDest  = strDest+"history.csv";        
        htp.GetFile(url,strDest.c_str());  
        string data(strDest.c_str()); 

        ifstream in(data.c_str());
        if (!in.is_open()) return E_ReadError;
        typedef tokenizer< escaped_list_separator<char> > Tokenizer;
        vector< string > vec;
        string line;
        string Date,Open,High,Low,Close,Volume,Adj_Close;
        I32 flag=0;
        try
        {
            conn.BeginTransaction();
            while (getline(in,line))
            {
                if(flag==0)
                {
                    flag=1;
                    continue;
                }
                Tokenizer tok(line);
                vec.assign(tok.begin(),tok.end());
                if (vec.size() < 7)
                {
                    in.close();
                   // LOGW(_logger, "No history price data found for " << pSec->get_Symbol());
                    conn.RollbackTransaction();
                    return S_False;
                }
                Date=vec[0]; 
                Open = vec[1];
                High = vec[2];
                Low = vec[3];
                Close = vec[4];
                Volume = vec[5];
                Adj_Close = vec[6];
                ErrorId retCode = SaveHistoricalPrices(conn, pSec->get_Id(),Date,Open,High,Low,Close,Volume,Adj_Close);
                if (FAILED(retCode))
                {
                    conn.RollbackTransaction();
                    in.close();
                    //LOG4CXX_ERROR(_logger, "save price error");
                    return retCode;
                }
            }
            in.close();
            conn.CommitTransaction();
        }
        catch(std::exception& ex)
        {
            conn.RollbackTransaction();
            in.close();
           // LOGE(_logger, "Download history prices method failed while saving history prices with error " << ex.what());
            throw;
        }
        return S_Success;
    }
    catch(std::exception ex)
    {
       // LOGE(_logger, "Download history price method reported error " << ex.what());
        return E_UnknownError;
    }
}

ErrorId CSecurityAPI::SaveHistoricalPrices(CSmartConnection& conn, int productid, std::string date, std::string open, std::string high, std::string low, std::string close, std::string volume, std::string adj_close)
{
    try
    {
        Query qSel = conn->query();
        qSel<<"select count(1) from se_sec_eq_pricehistory where product_id = "<<productid<<" and history_date = "<<quote<<date;
        auto qRes = qSel.store();           
        if (!qRes.size() || 0 == (int)qRes[0][0])
        {
            Query qInsert = conn->query();
            qInsert << "INSERT INTO se_sec_eq_pricehistory(`product_id`, `history_date`, `history_open`, `history_high`,`history_low`, `history_close`, `history_vol`, `history_adjustedclose`)"<<
                    "VALUES ('"<<productid<<"','"
                    <<date<<"','"
                    <<open<<"','"
                    <<high<<"','"
                    <<low<<"','"
                    <<close<<"','"
                    <<volume<<"','"
                    <<adj_close<<"'"
                    <<")"; 
            qInsert.execute();
        }
        //LOG4CXX_INFO(_logger, "History updated sucessfully"); 
        return S_Success;
    }
    catch (const mysqlpp::BadQuery& er) 
    {
        //LOG4CXX_ERROR(_logger, "Query Error: " << er.what());
        return E_DBQueryException;
    }
    catch (const mysqlpp::BadConversion& er) 
    {
        //LOG4CXX_ERROR(_logger, "Conversion Error: " << er.what());
        return E_DBQueryException;
    }
    catch (const mysqlpp::Exception& er) 
    {
        //LOG4CXX_ERROR(_logger, "Error: " << er.what());
        return E_DBQueryException;
    }
    catch (const std::exception& er)
    {
        //LOG4CXX_ERROR(_logger, "Error: " << er.what());    
        return E_DBQueryException;
    }
}

long unsigned int CSecurityAPI::get_GICS(Json::Value& ojs)
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        Json::Value root;
        root["Id"] = 0;
        root["Name"] = "Global Industry Classification Standard";
        root["Children"] = Json::Value::null;
        LoadGICs(conn, root);
    }
    CATCHCLAUSE
}

void CSecurityAPI::LoadGICs(CSmartConnection& conn, Json::Value& parent)
{
    Query q = conn->query();
    q << "select * from list_Gics where gics_parent_id = " << parent.asInt();
    for (const Row& r : q.store())
    {
        Json::Value v;
        v["Id"] = (U32)r["gics_id"];
        v["Name"] = (string)r["gics_name"];
        U32 uid = v["Id"].asInt();
        LoadGICs(conn, parent);
    }
}

long unsigned int CSecurityAPI::get_GICS(IGICList** ppOut, QUERYFUNC(IGICInfo) criteria) 
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        SP<CGICList>  plist;
        Query q = conn->query();
        if (!criteria)
        {
            q << "select * from list_Gics";
            RETURNIFFAILED(CObject<CGICList>::Create(get_Session(), &plist));
            for (const Row& r : q.store())
            {
                SP<IGICInfo> pgic;
                if (FAILED(LoadGic(r, &pgic)))
                {
                   // LOGE(_logger, "LoadGic (list_Gics) failed for gic Id " << r["gic_id"] << "-" << r["industry"]);
                    continue;
                }
                plist->Add(pgic);
            }
        }
        else
        {
            q << "select * from list_Gics ";

            bool bWhere = false;
            BOOL bresult = false;
            LGX lgx = LGX_None;
            CSTR sret = criteria(IGICInfo::Id, lgx, bresult);
            if (bresult && sret)
            {
                q << " where gic_id = " << sret;
                bWhere=true;
            }

            bresult = false;
            lgx = LGX_None;
            sret = criteria(IGICInfo::Sector, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " sector like " << "'%" << sret << "%'";
            }

            bresult = false;
            lgx = LGX_None;
            sret = criteria(IGICInfo::Industry, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << "industry like " << "'%" << sret << "%'";
            }
           // LOGIP(q.str());
        }
        RETURNIFFAILED(CObject<CGICList>::Create(get_Session(), &plist));
        for (const Row& r : q.store())
        {
            SP<IGICInfo> pgic;
            if (FAILED(LoadGic(r, &pgic)))
            {
               // LOGE(_logger, "LoadGic (list_Gics) failed for gic Id " << r["gic_id"] << "-" << r["industry"]);
                continue;
            }
            plist->Add(pgic);
        }
        plist.CopyTo(ppOut);
        return S_OK;
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::get_PublicCompanies(
        IPublicCompanyInfoList** ppOut, 
        QUERYFUNC(IPublicCompanyInfo) criteria) 
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        SP<CPublicCompanyInfoList>  plist;
        Query q = conn->query();
        if (!criteria)
        {
            // pretty expensive call.
            q << "SELECT * FROM sa.list_ListedCompanies;";
        }
        else
        {
            q << "SELECT * FROM sa.list_ListedCompaniesExtended ";
            
            bool bWhere = false;
            BOOL bresult = false;
            LGX lgx = LGX_None;
            CSTR sret = criteria(IPublicCompanyInfo::Id, lgx, bresult);
            if (bresult && sret)
            {
                q << " where product_id = " << sret;
                bWhere=true;
            }

            bresult = false;
            lgx = LGX_None;
            sret = criteria(IPublicCompanyInfo::GicId, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " product_gic_id = " << sret;
            }

            bresult = false;
            lgx = LGX_None;
            sret = criteria(IPublicCompanyInfo::Cik, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " cik = " << quote << sret;
            }

            bresult = false;
            lgx = LGX_None;
            sret = criteria(IPublicCompanyInfo::ExchangeId, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " product_exchange_id = " << sret;
            }
            
            bresult = false;
            lgx = LGX_None;
            sret = criteria(IPublicCompanyInfo::ExchangeName, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " exchange_name like " << "'%" << sret << "%'";
            }

            bresult = false;
            lgx = LGX_None;
            sret = criteria(IPublicCompanyInfo::Sector, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " sector like " << "'%" << sret << "%'";
            }
            
            bresult = false;
            lgx = LGX_None;
            sret = criteria(IPublicCompanyInfo::Industry, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " industry like " << "'%" << sret << "%'";
            }

            bresult = false;
            lgx = LGX_None;
            sret = criteria(IPublicCompanyInfo::Ticker, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " product_symbol = " << quote << sret ;
            }
            
            bresult = false;
            lgx = LGX_None;
            sret = criteria(IPublicCompanyInfo::State, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " product_state = " << quote << sret ;
            }

            bresult = false;
            lgx = LGX_None;
            sret = criteria(IPublicCompanyInfo::Country, lgx, bresult);
            if (bresult && strlen(sret))
            {
                QUERYFIELDLOGIC(q);
                q << " product_country = " << quote << sret ;
            }
            
           // LOGIP(q.str());
        }
        RETURNIFFAILED(CObject<CPublicCompanyInfoList>::Create(get_Session(), &plist));
        for (const Row& r : q.store())
        {
            SP<ISecurityDescription> psecdesc;
            if (ISOK(CreateSecurityDescription(&psecdesc)))
            {
//                if (ISOK(LoadSecDescriptions(conn, r[0], psecdesc._ptr)))
//                {
//                    SP<IPublicCompanyInfo> pco;
//                    // pretty expensive call. need a fix in future.
//                    if (ISOK(CObject<CPublicCompanyInfo>::Create(get_Session(), psecdesc, &pco)))
//                    {
//                        plist->Add(pco);
//                    }
//                }
            }
        }
        plist.CopyTo(ppOut);
        return S_OK;
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::get_GICHierarchy(unsigned long gicId, IIdNameList** ppOut) 
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q << "select gics_id, gics_name from se_sec_gics where gics_parent_id = " << gicId;
        SP<CIdNameList> pl;
        RETURNIFFAILED(CObject<CIdNameList>::Create(get_Session(), &pl));
        for (const Row& r : q.store())
        {
            SP<CIdName<> > p;
            if (ISOK(CObject<CIdName<> >::Create(get_Session(), r[1], &p)))
            {
                p->set_Id(r[0]);
                pl->Add(p);
            }
        }
        pl.CopyTo(ppOut);
        return S_OK;
    }
    CATCHCLAUSE
    
}

SE_CODE CSecurityAPI::CreatePredicationInstance(unsigned long productId, UID matchId, se::training::IPredictionSetupList* pList)
{
    using namespace se::training;
    try
    {
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        q << "SELECT * FROM sa.list_PredictionSetupProducts where product_id = " << productId;
        q << " and match_id = " << matchId;
        for (const Row& r : q.store())
        {
            SP<IPredictionSetup> p;
            RETURNIFFAILED(IPredictionSetup::Create(get_Session(), &p));
            SetPredictionSetupDetails(conn, r, p);
            p->set_Property("ProductId", productId);
            p->set_Property("MatchId", matchId);
            pList->Add(p);
        }

        q = conn->query();
        q << "SELECT product_gic_id, parent_gics_id FROM list_ListedCompaniesExtended where product_id = " << productId;
        StoreQueryResult sqr = q.store();
        if (sqr.size())
        {
            const Row& rw = sqr[0];
            UID gicId = rw["product_gic_id"];
            UID pgicid = rw["parent_gics_id"];
            Query q = conn->query();
            q << "SELECT * FROM list_PredictionSetupGics where gics_id in (" << gicId << ", " << pgicid << ")";
            q << " and match_id = " << matchId;
            for (const Row& r : q.store())
            {
                SP<IPredictionSetup> p;
                RETURNIFFAILED(IPredictionSetup::Create(get_Session(), &p));
                SetPredictionSetupDetails(conn, r, p);
                p->set_Property("ProductId", productId);
                p->set_Property("MatchId", matchId);
                pList->Add(p);
            }        
        }
        q = conn->query();
        q << "SELECT * FROM sa.list_PredictionSetupAll where match_id = " << matchId;
        sqr = q.store();
        if (sqr.size())
        {
            const Row& rw = sqr[0];
            SP<IPredictionSetup> p;
            RETURNIFFAILED(IPredictionSetup::Create(get_Session(), &p));
            SetPredictionSetupDetails(conn, rw, p);
            p->set_Property("ProductId", productId);
            p->set_Property("MatchId", matchId);
            pList->Add(p);
        } 
        return S_OK;
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::get_SecurityPredictionSetups(unsigned long productId, UID matchId, se::training::IPredictionSetupList** ppOut) 
{
    try
    {
        // load the prediction setup from following tables:
        // first check SELECT * FROM sa.list_PredictionSetupProducts, 
        // then check SELECT * FROM sa.list_PredictionSetupGics;
        // then utilize default setup.
        
        using namespace se::training;
        IPredictionSetupList* pList;
        if (!FC.TrySPGetFValue("get_SecurityPredictionSetups", productId, pList))
        {
            Poco::ScopedLock<Poco::Mutex> g(securityLock);
            if (!FC.TrySPGetFValue("get_SecurityPredictionSetups", productId, pList))
            {
                RETURNIFFAILED(IPredictionSetupList::Create(get_Session(), &pList));
                RETURNIFFAILED(CreatePredicationInstance(productId, matchId, pList));
                FC.SPSetFValue("get_SecurityPredictionSetups", productId, pList, 86400);
                *ppOut = pList;
                return S_OK;
            }
        }
        pList->AddRef();
        *ppOut = pList;
        return S_OK;
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::ProcessEarningCalendar(PERIOD processPeriod) 
{
    CEarningCalendar cal;
    return cal.Run(processPeriod);
}

long unsigned int CSecurityAPI::get_RandomListOfBestQuarterlySecurities(
    ISecuritySelectionStatisticsList** ppOut, 
    SIZE listSize,
    QUERYFUNC(ISecuritySelectionStatistics) criteria)
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        
        
        // must be joined with list_ListedCompanies to ensure that invalid products are not included
        q << "SELECT * FROM list_CurrentProductKeyStats ";
        // capture query filters
        if (criteria != nullptr)
        {
            //TODO: ensure that exchanges are given in comma separated form without an ending comma.
            LGX lgx;
            bool yes;
            CSTR exchanges = criteria(ISecuritySelectionStatistics::ExchangeIds, lgx, yes);
            if (yes and exchanges and strlen(exchanges) > 0)
            {
                q << " where product_exchange_id in (";
                q << exchanges;
                q << ") ";
            }
        }
        q << "order by expected_return_rate desc, stddev asc";
        return CObject<CSecuritySelectionStatisticsList>::Create(get_Session(), listSize, q.store(), ppOut);
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::get_SecuritySelectionStatistics(unsigned long productId, ISecuritySelectionStatistics** ppOut) 
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        Query q = conn->query();
        // must be joined with list_ListedCompanies to ensure that invalid products are not included
        q << "SELECT * FROM list_CurrentProductKeyStats where a.product_id = " << productId;
        auto use = q.use();
        if (!use)
            return E_NotFound;
        const Row& r  = use.fetch_row();
        if (!r)
            return E_NotFound;
        return CSecuritySelectionStatistics::PopulateInstance(get_Session(), r, ppOut);
    }
    CATCHCLAUSE
}

long unsigned int CSecurityAPI::get_SecurityNews(ISecurityDescription *pDesc, ISecurityNewsList** ppOut) 
{
    
    try
    {
        ISecurityNewsList* pNewsList;
        const string funcName = "se.sec.news";
        if (!FC.TrySPGetFValue(funcName, pDesc->get_Id(), pNewsList))
        {
            Poco::ScopedLock<Poco::Mutex> g(securityLock);
            if (!FC.TrySPGetFValue(funcName, pDesc->get_Id(), pNewsList))
            {

                using namespace se::net;
                string url = "http://articlefeeds.nasdaq.com/nasdaq/symbols?symbol=" + (string) pDesc->get_Symbol();
                // parse rss
                SP<CRSSChannel> pChannel;
                if (FAILED(CRSSFeedManager::ParseUrl(get_Session(), url.c_str(), &pChannel)))
                {
                   // LOGE(_logger, "Cannot read the news from the source website url " << url);
                    return E_HttpError;
                }
                RETURNIFFAILED(ISecurityNewsList::Create(get_Session(), &pNewsList));
                // enumerate the channels
                while (true)
                {
                    SP<CRSSItem> pRssItem;
                    if (!pChannel->Enumerate(&pRssItem))
                        break;
                    SP<CSecurityNews> pn;
                    if (ISOK(CObject<CSecurityNews>::Create(get_Session(), pDesc->get_Symbol(), pRssItem, &pn)))
                    {
                        //LOGI(_logger, pRssItem->get_Title() << ", " << pRssItem->get_Description());
                        pn->set_Source(SNS::Nasdaq);
                        pNewsList->Add(pn);
                    }
                }

                if (!pNewsList->get_Count())
                {
                   // LOGE(_logger, "No news from the source website url " << url);
                    return E_HttpError;
                }

               // LOGI(_logger, "Successfully populated news feed for " << pDesc->get_Symbol() << ". There are " << pNewsList->get_Count() << " news items.");
               // LOGIP("Caching news feed.");
                FC.SPSetFValue(funcName, pDesc->get_Id(), pNewsList, 900);
                
                *ppOut = pNewsList;

               // LOGIP("Success");

                return S_OK;
            }
        }
        pNewsList->AddRef();
        *ppOut = pNewsList;

       // LOGIP("Success");
        return S_OK;
    }
    CATCHCLAUSE;
    
}

long unsigned int CSecurityAPI::get_SecurityPriceRSSFeedCurrent(ISecurityDescription* pDesc, SPS source, ISecurityPriceRSSFeed** ppOut) 
{
    try
    {

        using namespace se::net;
        if (source == SPS::Nasdaq)
        {
            // nasdaq rss feed
            //http://www.nasdaq.com/aspxcontent/NasdaqRSS.aspx?data=quotes&symbol={SYM}

            std::ostringstream os;
            os << "http://www.nasdaq.com/aspxcontent/NasdaqRSS.aspx?data=quotes&symbol=" << pDesc->get_Symbol();
            
            SP<CRSSChannel> pChannel;
            if (FAILED(CRSSFeedManager::ParseUrl(get_Session(), os.str().c_str(), &pChannel)))
            {
               // LOGE(_logger, "Cannot read the news from the source website url " << os.str());
                return E_HttpError;
            }
            
            // enumerate the channel items . Note we will break after getting the first item in nasdaq's case.
            while (true)
            {
                SP<CRSSItem> pRssItem;
                if (!pChannel->Enumerate(&pRssItem))
                    break;
                
                SP<CSecurityPriceRSSFeed> pr;
                if (ISOK(CObject<CSecurityPriceRSSFeed>::Create(get_Session(), SPS::Nasdaq, pRssItem, &pr)))
                {
                    //LOGI(_logger, pRssItem->get_Title() << ", " << pRssItem->get_Description());
                    pr.CopyTo(ppOut);
                    return S_OK;
                }
            }
            
            return E_HttpError;
        }
        else
        {   // current not other sources are supported
           // LOGE(_logger, "Security price source is not currently implemented.");
            return E_NotImpl;
        }

    }    
    CATCHCLAUSE
}


////////*****************************************************************************************************

void CSecurityAPI::SetPredictionSetupDetails(CSmartConnection& conn, const Row& r, se::training::IPredictionSetup* p) 
{
    p->set_Id(r["set_id"]);
    p->set_Name(r["set_name"]);
    p->set_Algo(r["set_algo"]);
    p->set_Desc(r["set_desc"]);
    
    Query q = conn->query();
    q << "select result_id, source from se_trainingsets_result where set_id = " << p->get_Id();
    Row rr = q.use().fetch_row();
    if (rr)
    {
        se::training::TRAINING_RESULT tr;
        tr.Source = ((CSTR)rr["source"])[0];
        tr.Identifier = rr["result_id"];
        p->set_ResultInfo(tr);
    }
}


long unsigned int CSecurityAPI::LoadGic(const Row& row, IGICInfo** ppOut) 
{
    RETURNIFFAILED(CObject<CGICSInfo>::Create(get_Session(), row["sector"], row["industry"], ppOut));
    (*ppOut)->set_Id(row["gic_id"]);
    (*ppOut)->set_Property("pgic_id", (UID)row["pgic_id"]);
    return S_OK;
}

const char* CSecurityAPI::get_CodeMessage(long unsigned int errorCode) 
{
    auto find = msgs.find(errorCode);
    if (find == msgs.end())
        return IErrorDescription::get_CodeMessage(errorCode);
    else
        return find->second;
}

////////////////////// JSON METHODS BEGINS /////////////////////////////////

SE_CODE CSecurityAPI::get_Security(const Json::Value securityDesc, Json::Value& output) 
{
    return _dataProvider->get_Security(securityDesc, output);
}

SE_CODE CSecurityAPI::get_ExchangeSecurities(const Json::Value input, Json::Value& output) 
{
    return _dataProvider->get_ExchangeSecurities(input, output);
}




///////////////////////JSON METHODS ENDS ///////////////////////////