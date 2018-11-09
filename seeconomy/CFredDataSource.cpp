/* 
 * File:   CFredDataSource.cpp
 * Author: santony
 * 
 * Created on September 10, 2015, 1:39 PM
 */

#include "seecconinternal.h"
#include "CFredDataSource.h"
#include "../semain/semain.h"
#include "CIndicatorCategory.h"



CFredDataSource::CFredDataSource(ISession* ps) : CObjectRoot(ps)
{
    
}

CFredDataSource::~CFredDataSource() 
{
    
}

SE_CODE ECONOMYEXP IFredDataSource::Create(ISession* ps, IFredDataSource** ppOut)
{
    return CObject<CFredDataSource>::Create(ps, ppOut);
}

long unsigned int CFredDataSource::Init() 
{
    // read fed key
    SP<IApp> pApp;
    SP<se::config::IConfigurator> pCfg;
    RETURNIFFAILED(SEOpenApp(&pApp));
    RETURNIFFAILED(pApp->get_Configurator(&pCfg));
    const Json::Value& root = pCfg->get_ConfigRoot();
    const Json::Value& cfg = root["stlouisfed"];
    if (cfg == Json::nullValue)
    {
        BERR << "stlouisfed key not found in configuration." << endl;
        return E_ConfigurationError;
    }
    _fredKey = cfg.get("fredkey", "").asString();
    _fredRoot = cfg.get("apiroot", "").asString();
    if (_fredKey.empty())
    {
        BERR << "fredkey key not found in configuration." << endl;
        return E_SecurityAuthorization;
    }
    if (_fredRoot.empty())
    {
        BERR << "_fredRoot key not found in configuration." << endl;
        return E_ConfigurationError;
    }
    
    _http.EnableHostNameVerification();
    _http.EnablePeerVerification();
    _http.EnableFollowLocation();
    
    return S_OK;
}

long unsigned int CFredDataSource::get_Category(unsigned long categoryId, IIndicatorCategory** ppOut) 
{
    SP<IString> presponse;
    try
    {
        ostringstream outbuff;
        ostringstream ocatId;
        ocatId << categoryId;
        string url = _fredRoot + "/category?category_id=" + ocatId.str() + "&api_key=" + _fredKey + "&file_type=json";
        THROWIFFAILED(_http.GetBuffer(url.c_str(), &outbuff), "curl error while requesting fred category info.");
        Json::Reader jsr;
        Json::Value root;
        BDBG << outbuff.str();
        if (!jsr.parse(outbuff.str(), root, false))
        {
            throw se::seexception("Failed to parse fred category json response data");
        }
        if (root.isMember("categories"))
        {
            // now we the json.
            THROWIFFAILED(CObject<CIndicatorCategory>::Create(get_Session(), root["categories"][0], ppOut), "IIndicatorCategory::Create");
            return S_OK;
        }
        else
        {
            throw se::seexception(E_NoDataToReturn, "No category returned.");
        }
    }
    CATCHCLAUSE("CFredDataSource::get_Category")
}

template<typename T>
SE_CODE CreateFromJson(ISession* ps, const Json::Value& js, T** ppOut)
{
    return CObject<T>::Create(ps, js, ppOut);
}

long unsigned int CFredDataSource::get_CategoryChildren(unsigned long categoryId, IIndicatorCategoryList** ppOut) 
{
    //https://api.stlouisfed.org/fred/category/children?category_id=13&api_key=abcdefghijklmnopqrstuvwxyz123456
    try
    {
        ostringstream outbuff;
        ostringstream ocatId;
        ocatId << categoryId;
        string url = _fredRoot + "/category/children?category_id=" + ocatId.str() + "&api_key=" + _fredKey + "&file_type=json";
        THROWIFFAILED(_http.GetBuffer(url.c_str(), &outbuff), "curl error while requesting fred category children info.");
        Json::Reader jsr;
        Json::Value root;
        BDBG << outbuff.str();
        if (!jsr.parse(outbuff.str(), root, false))
        {
            throw se::seexception("Failed to parse fred category json response data");
        }
        if (root.isMember("categories"))
        {
            // now we the json.
            SP<IIndicatorCategoryList> pCatList;
            THROWIFFAILED( CIndicatorCategoryList::CreateSPList(get_Session(), &pCatList), "IIndicatorCategoryList::Create");
            const Json::Value& cats = root["categories"];
            for (const Json::Value& cat : cats)
            {
                SP<CIndicatorCategory> pcat;
                THROWIFFAILED( CreateFromJson<CIndicatorCategory>(get_Session(), cat, &pcat), "Failed to create category from json" );
                pcat->set_ParentId(categoryId);
                pCatList->Add(pcat);
            }
            pCatList.CopyTo(ppOut);
            return S_OK;
        }
        else
        {
            throw se::seexception(E_NoDataToReturn, "No category children returned.");
        }
    }
    CATCHCLAUSE("CFredDataSource::get_CategoryChildren")
    

}

long unsigned int CFredDataSource::get_RelatedCategories(unsigned long categoryId, IIndicatorCategoryList** ppOut) 
{
    //https://api.stlouisfed.org/fred/category/related?category_id=13&api_key=abcdefghijklmnopqrstuvwxyz123456
    try
    {
        ostringstream outbuff;
        ostringstream ocatId;
        ocatId << categoryId;
        string url = _fredRoot + "/category/related?category_id=" + ocatId.str() + "&api_key=" + _fredKey + "&file_type=json";
        THROWIFFAILED(_http.GetBuffer(url.c_str(), &outbuff), "curl error while requesting fred related category info.");
        Json::Reader jsr;
        Json::Value root;
        BDBG << outbuff.str();
        if (!jsr.parse(outbuff.str(), root, false))
        {
            throw se::seexception("Failed to parse fred category json response data");
        }
        if (root.isMember("categories") )
        {
            // now we the json.
            SP<IIndicatorCategoryList> pCatList;
            THROWIFFAILED( CIndicatorCategoryList::CreateSPList(get_Session(), &pCatList), "IIndicatorCategoryList::Create");
            const Json::Value& cats = root["categories"];
            for (const Json::Value& cat : cats)
            {
                SP<CIndicatorCategory> pcat;
                THROWIFFAILED( CreateFromJson<CIndicatorCategory>(get_Session(), cat, &pcat), "Failed to create category from json" );
                pcat->set_ParentId(categoryId);
                pCatList->Add(pcat);
            }
            pCatList.CopyTo(ppOut);
            return S_OK;
        }
        else
        {
            throw se::seexception(E_NoDataToReturn, "No related categories returned.");
        }
    }
    CATCHCLAUSE("CFredDataSource::get_RelatedCategories")

}

long unsigned int CFredDataSource::get_IndicatorsByCategory(unsigned long categoryId, IEconomyIndicatorList** ppOut) 
{
    //https://api.stlouisfed.org/fred/category/series?category_id=125&api_key=abcdefghijklmnopqrstuvwxyz123456&file_type=json
    try
    {
        ostringstream outbuff;
        ostringstream scatid ;
        scatid << categoryId;
        string url = _fredRoot + "/category/series?category_id=" + scatid.str() + "&api_key=" + _fredKey + "&file_type=json";
        THROWIFFAILED(_http.GetBuffer(url.c_str(), &outbuff), "curl error while requesting fred category series info.");
        Json::Reader jsr;
        Json::Value root;
        BDBG << outbuff.str();
        if (!jsr.parse(outbuff.str(), root, false))
        {
            throw se::seexception("Failed to parse fred category series json response data");
        }
        if (root.isMember("seriess"))
        {
            SP<IEconomyIndicatorList> plist;
            THROWIFFAILED( CEconomyIndicatorList::CreateSPList(get_Session(), &plist), "IEconomyIndicatorList::Create");
            const Json::Value& inds = root["seriess"];
            for (const Json::Value& ind : inds)
            {
                SP<CEconomicIndicator> p;
                THROWIFFAILED( CreateFromJson<CEconomicIndicator>(get_Session(), ind, &p), "Failed to create category series from json" );
                plist->Add(p);
            }
            plist.CopyTo(ppOut);
            return S_OK;
        }
        else
        {
            throw se::seexception(E_NoDataToReturn, "No category series returned.");
        }
    }
    CATCHCLAUSE("CFredDataSource::get_IndicatorsByCategory")
}

long unsigned int CFredDataSource::get_Indicator(CSTR code, IEconomyIndicator** ppOut) 
{
    //https://api.stlouisfed.org/fred/series?series_id=GNPCA=13&api_key=abcdefghijklmnopqrstuvwxyz123456

    try
    {
        ostringstream outbuff;
        string url = _fredRoot + "/series?series_id=" + code + "&api_key=" + _fredKey + "&file_type=json";
        THROWIFFAILED(_http.GetBuffer(url.c_str(), &outbuff), "curl error while requesting fred series info.");
        Json::Reader jsr;
        Json::Value root;
        BDBG << outbuff.str();
        if (!jsr.parse(outbuff.str(), root, false))
        {
            throw se::seexception("Failed to parse fred category json response data");
        }
        if (root.isMember("seriess") && root["seriess"].size())
        {
            // now we the json.
            THROWIFFAILED(CObject<CEconomicIndicator>::Create(get_Session(), root["seriess"][0], ppOut), "IEconomyIndicator::Create");
            return S_OK;
        }
        else
        {
            throw se::seexception(E_NoDataToReturn, "No related categories returned.");
        }
    }
    CATCHCLAUSE("CFredDataSource::get_Indicator")
}

long unsigned int CFredDataSource::get_IndicatorCategories(const char* code, IIndicatorCategoryList** ppOut) 
{
    //series/categories?series_id=EXJPUS
    //https://api.stlouisfed.org/fred/series/categories?series_id=EXJPUS&api_key=abcdefghijklmnopqrstuvwxyz123456
    try
    {
        ostringstream outbuff;
        string url = _fredRoot + "/series/categories?series_id=" + code + "&api_key=" + _fredKey + "&file_type=json";
        THROWIFFAILED(_http.GetBuffer(url.c_str(), &outbuff), "curl error while requesting fred series categories info.");
        Json::Reader jsr;
        Json::Value root;
        BDBG << outbuff.str();
        if (!jsr.parse(outbuff.str(), root, false))
        {
            throw se::seexception("Failed to parse fred category json response data");
        }
        if (root.isMember("categories"))
        {
            // now we the json.
            SP<IIndicatorCategoryList> pCatList;
            THROWIFFAILED( CIndicatorCategoryList::CreateSPList(get_Session(), &pCatList), "IIndicatorCategoryList::Create");
            const Json::Value& cats = root["categories"];
            for (const Json::Value& cat : cats)
            {
                SP<CIndicatorCategory> pcat;
                THROWIFFAILED( CreateFromJson<CIndicatorCategory>(get_Session(), cat, &pcat), "Failed to create category from json" );
                pCatList->Add(pcat);
            }
            pCatList.CopyTo(ppOut);
            return S_OK;
        }
        else
        {
            throw se::seexception(E_NoDataToReturn, "No series categories returned.");
        }
    }
    CATCHCLAUSE("CFredDataSource::get_IndicatorCategories")
}

long unsigned int CFredDataSource::get_IndicatorObservations(const char* code, se::training::ITrainingTable** ppOut, const char* frequency, const char* aggr, const CDate& start, const CDate& end)
{
    try
    {
        ostringstream sd, ed;
        if (start != CDate::Empty())
            sd << start;
        if (end != CDate::Empty())
            ed << end;
        ostringstream outbuff;
        string url ;
        //url = _fredRoot + "/series/observations?series_id=" + code + "&frequency=" + frequency + "&aggregation_method=" + aggr + "&api_key=" + _fredKey + "&file_type=json";
        url = _fredRoot + "/series/observations?series_id=" + code + "&aggregation_method=" + aggr + "&api_key=" + _fredKey + "&file_type=json";

        if (start != CDate::Empty()) 
            url += "&observation_start=" + sd.str();
        if (end != CDate::Empty())
            url += "&observation_end=" + ed.str();
        THROWIFFAILED(_http.GetBuffer(url.c_str(), &outbuff), "curl error while requesting fred series observations.");
        Json::Reader jsr;
        Json::Value root;
        BDBG << outbuff.str();
        if (!jsr.parse(outbuff.str(), root, false))
        {
            throw se::seexception("Failed to parse fred series observation json response data");
        }
        if (root.isMember("observations"))
        {
            SP<se::training::ITrainingTable> ptbl;
            THROWIFFAILED(se::training::ITrainingTable::Create(get_Session(), &ptbl), "se::training::ITrainingTable::Create");
            R8 prevVal = ER8;
            for (const Json::Value obs : root["observations"])
            {
                DATAPERIOD dp = obs["date"].asCString();
                //std::istringstream istr(obs["date"].asString());
                //istr >> dp;
                const string strVal = obs["value"].asString();
                if (strVal.empty() && !ISER8(prevVal))
                {
                    ptbl->AddRecord(dp, prevVal);
                }
                else if (!strVal.empty())
                {
                    try
                    {
                        R8 val = strtod(strVal.c_str(), nullptr);
                        ptbl->AddRecord(dp, val);
                        prevVal = val;
                    }
                    catch (const std::exception)
                    {
                        if (!ISER8(prevVal))
                        {
                            ptbl->AddRecord(dp, prevVal);
                        }
                    }
                }
            }
            ptbl.CopyTo(ppOut);
            return S_OK;
        }
        else
        {
            throw se::seexception(E_NoDataToReturn, "No series observations returned.");
        }
    }
    CATCHCLAUSE("CFredDataSource::get_IndicatorObservations")
}


long unsigned int CFredDataSource::save_CategoryTags() {

}

long unsigned int CFredDataSource::save_RelatedCategoryTags() {

}

long unsigned int CFredDataSource::save_RelatedReleaseTags() {

}

long unsigned int CFredDataSource::save_ReleaseDates() {

}

long unsigned int CFredDataSource::save_ReleaseSources() {

}

long unsigned int CFredDataSource::save_ReleaseTags() {

}

long unsigned int CFredDataSource::save_Releases() {

}

long unsigned int CFredDataSource::save_SeriesUpdates() 
{
    //TODO
    //https://api.stlouisfed.org/fred/series/updates?api_key=abcdefghijklmnopqrstuvwxyz123456&file_type=json
}

long unsigned int CFredDataSource::find_Indicators(const char* searchText, IIndicatorCategoryList**) {

}











