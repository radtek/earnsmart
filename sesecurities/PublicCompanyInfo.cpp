#include "internal.h"
#include "PublicCompanyInfo.h"
#include <string.h>
#include "../seglobal/Object.h"
#include "../seglobal/CString.h"
#include <mysql++.h>
#include <connection.h>
#include<jsoncpp/json/json.h>
#include<jsoncpp/json/writer.h>
#include<jsoncpp/json/value.h>
#include "../semysql/CSEConnections.h"
#include "CParseCompanyProfile.h"
#include "GICSInfo.h"
#include <boost/algorithm/string.hpp>
#include "../sexbrl/sexbrlapi.h"

#define SEDBCONN        CSEConnections::GetMain()

using namespace se::data;

CPublicCompanyInfo::CPublicCompanyInfo(ISession* ps) :
    CObjectRoot(ps),
    _bInitData(false)
{
    //ctor
}

CPublicCompanyInfo::~CPublicCompanyInfo()
{
    //dtor
}

ErrorId CPublicCompanyInfo::Init()
{
    return CreateSecurityDescription(get_Session(), &_pSecDesc);
}

ErrorId CPublicCompanyInfo::Init(ISecurityDescription* securityDesc)
{
    _pSecDesc = securityDesc;
    EnsureInitData();
    return S_Success;
}

ErrorId CPublicCompanyInfo::EnsureInitData()
{
    if (!_bInitData)
    {
        CParseCompanyProfile parser(get_Session(), _pSecDesc);
        RETURNIFFAILED(parser.Process(this));
        _bInitData = true;
    }
    return S_Success;
}

UID CPublicCompanyInfo::get_Id()
{
    return _pSecDesc->get_Id();
}

ErrorId CPublicCompanyInfo::get_GICInfo(IGICInfo** ppOut)
{
    _pGics.CopyTo(ppOut);
    return _pGics._ptr ? S_OK : S_False;
}

CSTR CPublicCompanyInfo::get_SICInfo()
{
    return _sSICInfo.c_str();
}

void CPublicCompanyInfo::set_SICInfo(const char* val)
{
    _sSICInfo = val;
}

//ErrorId CPublicCompanyInfo::get_PerformanceIndicators(IIndicatorList** ppOut)
//{
//    return E_NotImpl;
//}

CIK CPublicCompanyInfo::get_CIK()
{
    return _cik.c_str();
}

const char* CPublicCompanyInfo::get_UrlNews() 
{
    return _urlnews.c_str();
}

void CPublicCompanyInfo::set_UrlNews(const char* v) 
{
    _urlnews = v;
}

void CPublicCompanyInfo::set_CIK(CIK val)
{
    _cik = val;
}

CDate& CPublicCompanyInfo::get_FinancialYearEnd() 
{
    if (_finyearend.IsEmpty() && !_cik.empty())
    {
        SP<se::xbrl::IXbrl> pxbrl;
        if (SUCCEEDED(se::xbrl::XbrlOpen(get_Session(), &pxbrl)))
        {
            pxbrl->GetFinancialYearEnd(_cik.c_str(), &_finyearend);
        }
    }
    
    return _finyearend;
}



ErrorId CPublicCompanyInfo::get_Performance(IString** ppOut)
{
    /*
     * 1. Populate se_xbrl_perf_ratios table using the data in the xls.
     * 2. Using mysql to populate the data from the table into a CDataTable class
     * 3. Convert the CDataTable into json (refer previous work).
     * 
     * collection of the following structure
     * struct
     * {
     *  Name : string
     *  Description : string
     *  Importance: string
     *  Value : random floating value
     *  Priority: 0
     * }
     * 
     * { { "Name" : "ssssss", "Description" : "ssdsfsdfds", .... }, {....}, {.... } }
     * 
     * 
     */
    Json::Value root,child;
    Json::FastWriter writer;
    Json::Value vec(Json::arrayValue);
    se::data::CSmartConnection conn(SEDBCONN);
    mysqlpp::Query query = conn->query("select * from se_xbrl_perf_ratios order by perf_priority");
    mysqlpp::StoreQueryResult res = query.store();
    cout.setf(ios::left);

     for (int i = 0; i < res.num_rows(); ++i) 
     {   
         string name(res[i][1]);
         string desc(res[i][2]);
         string imp(res[i][3]);         
         string id(res[i][0]);

        child["Name"] = name;
        child["Description"] = desc;
        child["Importance"]=imp;
        child["Value"] = 0.f;
        vec.append(Json::Value(child));
        root[id]=vec;
        vec.clear();
     }
    string out= writer.write(root);
    CSTR jsonout =out.c_str();
    return IString::Create(jsonout, ppOut);
}

  
void CPublicCompanyInfo::Serialize(ISerializedData<IString>* pIn)
{
    if (!pIn)
        return;
    pIn->Add("Name", _sName.c_str());
    pIn->Add("Summary", _sSummary.c_str());
    pIn->Add("GICInfo", _pGics);
    pIn->Add("Address", _pAddress);
    pIn->Add("CIK", _cik.c_str());
    pIn->Add("SIC", _sSICInfo.c_str());
}

SE_CODE SECEXP se::securities::IPublicCompanyInfo::Create(ISession* ps, ISecurityDescription* pIn, IPublicCompanyInfo** ppOut)
{
    return CObject<CPublicCompanyInfo>::Create(ps, pIn, ppOut);
}