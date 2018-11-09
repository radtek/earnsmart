#include "internal.h"
#include "Security.h"
#include "../semysql/CMyqlUtilities.h"
#include <iostream>  
#include "../semysql/CSEConnections.h"
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#include "CSecurityPrice.h"

#define SEDBCONN        CSEConnections::GetMain()
using namespace std;

#define SEDBCONN        CSEConnections::GetMain()
using namespace se::data;

CSecurity::CSecurity(ISession* ps) : CObjectRoot(ps)
{
    _marketcap = _mktcapsicshare = 0.0;
}

CSecurity::~CSecurity()
{
    //dtor
}

ErrorId CSecurity::Init(ISecurityDescription* secDesc)
{
    if (!secDesc)
    {
        return E_InvalidPointer;
    }
    _pSecDesc = secDesc;
    
    if (_pSecDesc)
        set_Id(_pSecDesc->get_Id());
    
    return S_Success;
}

CSTR CSecurity::get_Symbol() 
{
    return _pSecDesc->get_Symbol();
}

ErrorId CSecurity::get_SecurityDescription(ISecurityDescription** ppOut) 
{
    _pSecDesc.CopyTo(ppOut);
    return S_Success;
}

const char* CSecurity::get_Name() 
{
    return _name.c_str();
}

const char* CSecurity::get_LongName() 
{
    return _lname.c_str();
}

void CSecurity::set_Name(const char* name) 
{
    _name = name;
}

void CSecurity::set_LongName(const char* longName) 
{
    _lname = longName;
}


ErrorId CSecurity::get_Company(IPublicCompanyInfo** ppOut) 
{
    if (!_pco)
    {
        SP<ISecurityAPI> pApi;
        SP<se::config::IConfigurator> pcfg;
        pcfg = get_Session()->get_App()->get_Configurator();
        RETURNIFFAILED( SecOpen(get_Session(), pcfg->get_DefaultMarketDataProvider(), &pApi));
        RETURNIFFAILED(pApi->get_Company(_pSecDesc, &_pco));
    }
    _pco.CopyTo(ppOut);
    return S_OK;
}

void CSecurity::set_Company(IPublicCompanyInfo* pin) 
{
    _pco = pin;
}


void CSecurity::Serialize(ISerializedData<IString>* pIn)
{
    if (!pIn) return;
    pIn->Add("Id", get_Id());
    pIn->Add("Symbol",get_Symbol());
    pIn->Add("SecurityDescription",_pSecDesc);
    SP<IPublicCompanyInfo> pCo;
    if (SUCCEEDED(get_Company(&pCo)))
        pIn->Add("Company",pCo);
}

ErrorId CSecurity::get_AvgHistoricalPriceAndVolume(CDate const& from, CDate const& to, ISecurityPrice** ppOut)
{
    CSmartConnection conn(SEDBCONN);
    CDate dateFrom = from;
    CDate dateTo = to;
    sql_date sqlFrom,sqlTo;
    CMyqlUtilities::PopulateDateTime(dateFrom, sqlFrom);
    CMyqlUtilities::PopulateDateTime(dateTo, sqlTo);
    I64 product_id;
    {
        Query q=conn->query();
        q<<"SELECT product_id FROM se_sec_products where product_symbol ="<<quote<<_pSecDesc->get_Symbol();
        auto res = q.use();
        auto row = res.fetch_row();
        product_id = (I64)row["product_id"];
    }
    R8 size =0,total = 0.0,average =0.0,volTotal =0.0,volAvg = 0.0;
    {
        Query q=conn->query();
        q<<"select * from se_sec_eq_pricehistory where product_id ="<< product_id << " and history_date \
        BETWEEN "<<quote<<sqlFrom<<" and "<<quote<<sqlTo;
        if (mysqlpp::StoreQueryResult res = q.store())
        {
            for (auto it = res.begin(); it != res.end(); ++it) 
            {
                auto row = *it;
                total = total + (R8)row["history_close"];
                volTotal = volTotal+(R8)row["history_vol"];
            }
            size = (R8)res.size();
        }
    }
    if(size == 0)
    {
        average = 0;
        volAvg=0;
    }
    else
    {
        average = (R8)total/size;
        volAvg=(R8)volTotal/size;
    }   
    ErrorId retCode = CObject<CSecurityPrice>::Create(get_Session(), ppOut);
    if (FAILED(retCode))
    {
        return retCode;
    }
    ((CSecurityPrice*)(*ppOut))->_dt = to;
    ((CSecurityPrice*)(*ppOut))->_price = average;
    ((CSecurityPrice*)(*ppOut))->_volume = volAvg;
    return S_Success;
}

double CSecurity::get_Marketcap() 
{
    return _marketcap;
}

double CSecurity::get_IndustryMarketcapShare() 
{
    return _mktcapsicshare;
}

void CSecurity::set_Marketcap(double v) 
{
    _marketcap = v;
}

void CSecurity::set_IndustryMarketcapShare(double v) 
{
    _mktcapsicshare = v;
}

long unsigned int CSecurity::get_News(ISecurityNewsList** ppOut) 
{
    SP<ISecurityAPI> papi;
    auto pcfg = get_Session()->get_App()->get_Configurator();
    RETURNIFFAILED(SecOpen(get_Session(), pcfg->get_DefaultMarketDataProvider(), &papi));
    return papi->get_SecurityNews(_pSecDesc._ptr, ppOut);
}


SE_CODE SECEXP ISecurity::Create(ISession* ps, ISecurityDescription* pSecDesc, ISecurity** ppOut)
{
    return CObject<CSecurity>::Create(ps, pSecDesc, ppOut);
}

