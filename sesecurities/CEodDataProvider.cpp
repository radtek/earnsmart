/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEodDataProvider.cpp
 * Author: santony
 * 
 * Created on September 3, 2016, 5:54 PM
 */

#include "CEodDataProvider.h"
#include "../seglobal/exceptions.h"
#include "../seglobal/CStringUtility.h"
#include "../seglobal/cpplinq.hpp"

#define SEEODDATA        CSEConnections::GetEodData()


CEodDataProvider::CEodDataProvider(ISession* pSession, se::securities::ISecurityAPI *api) : CMarketDataProvider(pSession, api)
{
    
}
//
//CEodDataProvider::CEodDataProvider(const CEodDataProvider& orig) {
//}

CEodDataProvider::~CEodDataProvider() {
}

SE_CODE CEodDataProvider::get_Security(const Json::Value securityDesc, Json::Value& output) 
{
    // we need at least exch and symbol
    if (!securityDesc.isMember("Exch") || !securityDesc.isMember("Sym"))
    {
        BERR << "Neither 'Exch' nor 'Sym' are found in the securityDesc parameter. They are required fields.";
        return E_InvalidArg;
    }
    
    bool needfund = false, needtech = false;
    if (securityDesc.isMember("Include"))
    {
        if (securityDesc["Include"].isMember("Fundamentals"))
        {
            needfund = securityDesc["Include"]["Fundamentals"].asBool();
        }
        if (securityDesc["Include"].isMember("Technicals"))
        {
            needtech = securityDesc["Include"]["Technicals"].asBool();
        }
    }
    
    try
    {
        CSTR exch = securityDesc["Exch"].asCString();
        CSTR sym = securityDesc["Sym"].asCString();
        // ensure that exchange is valid.
        CSmartConnection conn(SEEODDATA);
        Query q = conn->query();
        q << "select eid, ecountry from eod_exchanges where ";
        q << "ecode = " << quote << exch;
        if (securityDesc.isMember("Country"))
            q << " and ecountry " << quote << securityDesc["Country"].asCString();
        auto estore = q.store();
        if (!estore.size())
        {
            BERR << "Exchange code not found.";
            return E_ExchNotFound;
        }
        CSTR country = estore[0]["ecountry"];
        string tableName = getsecurityTable(exch);
        q = conn->query();
        q << "select * from " << tableName << " where ssym = " << quote << sym;
        auto store = q.store();
        if (!store.size())
        {
            BERR << "Security not found for Exchange: " << exch << " and Symbol: " << sym;
            throw  E_NotFound ;
        }
        output = securityDesc; // copy
        output["Country"] = country;
        BuildSecurity(store[0], output);
        if (needfund)
        {
            output["Fundamentals"] = getfundamentals(conn, exch, (UID)store[0]["sid"]);
        }
        if (needtech)
        {
            output["Technicals"] = gettechnicals(conn, exch, (UID)store[0]["sid"]);
        }
        return S_OK;
    }
    CATCHCLAUSE
}

long unsigned int CEodDataProvider::get_ExchangeSecurities(const Json::Value input, Json::Value& output) 
{
    
    BDBG << input;
    
    if (!input.isMember("Exch"))
    {
        BERR << "Exchange parameter not provided.";
        return E_ExchNotFound;
    }
    bool needfund = false, needtech = false;
    if (input.isMember("Include"))
    {
        if (input["Include"].isMember("Fundamentals"))
        {
            needfund = input["Include"]["Fundamentals"].asBool();
        }
        if (input["Include"].isMember("Technicals"))
        {
            needtech = input["Include"]["Technicals"].asBool();
        }
    }
    CSTR exch = input["Exch"].asCString();
    string tablename = getsecurityTable(exch);
    CSmartConnection conn(SEEODDATA);
    Query q = conn->query();
    q << "SELECT * FROM " << tablename ;
    q << " ORDER BY  ssym";
    output["Exch"] = getexchangeDetails(conn, exch);
    Json::Value secs = output["Securities"];
    for(const auto& row : q.store())
    {
        Json::Value sec;
        sec["Sym"] = (CSTR)row["ssym"];
        sec["Name"] = (CSTR)row["sname"];
        sec["LName"] = (CSTR)row["slname"];
        sec["AssetType"] = getsecurityType(conn, row["stype"]);
        if (needfund)
        {
            sec["Fundamentals"] = getfundamentals(conn, exch, (UID)row["sid"]);
        }
        if (needtech)
        {
            sec["Technicals"] = gettechnicals(conn, exch, (UID)row["sid"]);
        }
        secs.append(sec);
    }
    output["Securities"] = secs;
    return S_OK;
}



////////////////////// Private Methods ///////////////////////

void CEodDataProvider::BuildSecurity(const Row& row, Json::Value& output) 
{
    output["Name"] = (CSTR)row["sname"];
    output["LName"] = (CSTR)row["slname"];
}

string CEodDataProvider::getsecurityTable(const char* exch) 
{
    STR exchName = const_cast<STR>(exch);
    se::utils::CStringUtility::ChangeCase(exchName, strlen(exchName), true);
    return (string)"eod_" + exchName + "_securities";
}

string CEodDataProvider::getfundamentalsTable(const char* exch) 
{
    STR exchName = const_cast<STR>(exch);
    se::utils::CStringUtility::ChangeCase(exchName, strlen(exchName), true);
    return (string)"eod_" + exchName + "_fundamentals";
}

string CEodDataProvider::gettechnicalsTable(const char* exch) 
{
    STR exchName = const_cast<STR>(exch);
    se::utils::CStringUtility::ChangeCase(exchName, strlen(exchName), true);
    return (string)"eod_" + exchName + "_technicals";
}

string CEodDataProvider::getpriceHistoryTable(const char* exch) 
{
    STR exchName = const_cast<STR>(exch);
    se::utils::CStringUtility::ChangeCase(exchName, strlen(exchName), true);
    return (string)"eod_" + exchName + "_historydaily";
}

Json::Value CEodDataProvider::getexchangeDetails(CSmartConnection& conn, const char* exch) 
{
    Query q = conn->query();
    q << "select * from eod_exchanges where ecode = " << quote << exch;
    const auto& store = q.store();
    if (store.size())
    {
        Json::Value v;
        const Row& r = store[0];
        v["Id"] = (int)r["eid"];
        v["Code"] = (CSTR)r["ecode"];
        v["Name"] = (CSTR)r["ename"];
        v["Country"] = (CSTR)r["ecountry"];
        v["TimeZone"] = (CSTR)r["etimezone"];
        return v;
    }
    else
    {
        return Json::nullValue;
    }
}

string CEodDataProvider::getsecurityType(CSmartConnection& conn, unsigned long id) 
{
    Query q = conn->query();
    q << "SELECT * FROM eod_securitytypes WHERE stid = " << id << " ORDER BY stname";
    const auto store = q.store();
    if (store.size())
    {
        return (string)store[0]["stname"];
    }
    return "";
}


Json::Value CEodDataProvider::getsecurityTypes(CSmartConnection& conn) 
{
    Query q = conn->query();
    q << "SELECT * FROM eod_securitytypes ORDER BY stname";
    Json::Value output;
    for(const auto& r : q.store())
    {
        Json::Value v ;
        v["Id"] = (I32)r["stid"];
        v["Name"] = (CSTR)r["stname"];
        v["Supported"] = (I32)r["stsupported"]?true:false;
        output.append(v);
    }
    return output;
}

Json::Value CEodDataProvider::getfundamentals(CSmartConnection& conn, CSTR exchName, unsigned long secId) 
{
    string table = getfundamentalsTable(exchName);
    Query q = conn->query();
    q << "select * from " << table;
    q << " where sid = " << secId;
    q << " order by fdate desc limit 1";
    const auto store = q.store();
    if (store.size())
    {
        const Row& row = store[0];
        Json::Value ret ;
        ret["Date"] = (CSTR)row["fdate"];
        ret["Id"] = (Json::Int64)row["sid"];
        ret["Industry"] = (CSTR)row["findustry"];
        ret["Sector"] = (CSTR)row["fsector"];
        ret["NumberOfShares"] = (Json::Int64)row["fnumshares"];
        ret["MarketCap"] = (R8)row["fmarketcap"];
        ret["PricePerEarnings"] = (R8)row["fppe"];
        ret["EarningsPerShare"] = (R8)row["feps"];
        ret["NetTangibleAssets"] = (R8)row["fnta"];
        ret["DividendYield"] = (R8)row["fdivyld"];
        ret["Dividends"] = (R8)row["fdiv"];
        ret["DividendDate"] = (CSTR)row["fdivdate"];
        ret["DividendPerShare"] = (R8)row["fdivpershare"];
        ret["ImputCredits"] = (R8)row["fimputcredits"];
        ret["EBITDA"] = (R8)row["febitda"];
        ret["PricePerEarningsToGrowth"] = (R8)row["fpeg"];
        ret["pts"] = (R8)row["fpts"];
        ret["ptb"] = (R8)row["fptb"];
        ret["Yield"] = (R8)row["fyld"];
        return ret;
    }
    else
    {
        return Json::nullValue;
    }
}

Json::Value CEodDataProvider::gettechnicals(CSmartConnection& conn, CSTR exchName, unsigned long secId) 
{
    UID secType = 1;
    string table1 = getpriceHistoryTable(exchName);
    string table2 = gettechnicalsTable(exchName);
    Query q = conn->query();
    q << "select * from " << table1;
    q << " where sid = " << secId;
    q << " order by hdate desc limit 1";
    const auto store1 = q.store();
    Json::Value ret = Json::nullValue;
    if (store1.size())
    {
        const Row& row = store1[0];
        ret["Date"] = (CSTR)row["hdate"];
        ret["Price"]["Open"] = (R8)row["hopen"];
        ret["Price"]["High"] = (R8)row["hhigh"];
        ret["Price"]["Low"] = (R8)row["hlow"];
        ret["Price"]["Close"] = (R8)row["hclose"];
        ret["Volume"]["Current"] = (R8)row["hvol"];
        if (secType == 4 || secType == 8)
        {        // only for futures and options. get security type flag.
            ret["Price"]["OpenInterest"] = (R8)row["hopeninterest"];
        }
        ret["Price"]["Previous"] = (R8)row["hprev"];
        ret["Price"]["Change"]["Current"] = (R8)row["hchg"];
        ret["Price"]["Bid"] = (R8)row["hbid"];
        ret["Price"]["Ask"] = (R8)row["hask"];
    }
    q = conn->query();
    q << "select * from " << table2;
    q << " where sid = " << secId;    
    q << " order by tdate desc limit 1";
    const auto store2 = q.store();
    if (store2.size())
    {
        const Row& row = store2[0];
        ret["Price"]["MovingAverage"]["1Week"] = (R8)row["tma1"];
        ret["Price"]["MovingAverage"]["2Weeks"] = (R8)row["tma2"];
        ret["Price"]["MovingAverage"]["5Weeks"] = (R8)row["tma5"];
        ret["Price"]["MovingAverage"]["20Weeks"] = (R8)row["tma20"];
        ret["Price"]["MovingAverage"]["100Weeks"] = (R8)row["tma100"];
        ret["Price"]["MovingAverage"]["200Weeks"] = (R8)row["tma200"];
        ret["Price"]["MovingAverage"]["Pct"] = (R8)row["tmapct"];
        ret["Price"]["MovingAverage"]["Ret"] = (R8)row["tmaret"];
        ret["Price"]["Change"]["3Months"] = (R8)row["t3mchg"];
        ret["Price"]["Change"]["6Months"] = (R8)row["t6mchg"];
        ret["Price"]["Week"]["High"] = (R8)row["tweekhigh"];
        ret["Price"]["Week"]["Low"] = (R8)row["tweeklow"];
        ret["Price"]["Week"]["AverageChange"] = (R8)row["tavgweekchg"];
        
        ret["Volume"]["Week"]["Average"] = (R8)row["tavgweekvol"];
        ret["Volume"]["Week"]["Volume"] = (R8)row["tweekvol"];
        
        ret["Price"]["Month"]["High"] = (R8)row["tmonhigh"];
        ret["Price"]["Month"]["Low"] = (R8)row["tmonlow"];
        ret["Price"]["Month"]["Change"] = (R8)row["tmonchg"];
        ret["Price"]["Month"]["AverageChange"] = (R8)row["tavgmonchg"];
        ret["Volume"]["Month"]["Average"] = (R8)row["tavgmonvol"];

        ret["Price"]["Year"]["High"] = (R8)row["tyearhigh"];
        ret["Price"]["Year"]["Low"] = (R8)row["tyearlow"];
        ret["Price"]["Year"]["Change"] = (R8)row["tyearchg"];
        ret["Price"]["Year"]["AverageChange"] = (R8)row["tavgyearchg"];
        ret["Volume"]["Year"]["Average"] = (R8)row["tavgyearvol"];
        
        ret["Price"]["YearToDayChange"] = (R8)row["tytdchg"];
        
        ret["Price"]["Volatility"] = (R8)row["tvolatility"];
        ret["Liquidity"] = (R8)row["tliquidity"];
        
        // things to add
        // trsi14, tst09, twpr14, tmtm14, troc14, tpic, tsar
    }
    return ret;
}



