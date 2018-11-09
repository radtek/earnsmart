/* 
 * File:   CAppBase.cpp
 * Author: santony
 * 
 * Created on January 24, 2015, 1:03 PM
 */

#include "CBrowserSession.h"
#include "CAppBase.h"
#include "../seglobal/exceptions.h"
#include <fstream>
#include <cppcms/http_response.h>
#include <booster/thread.h>
#include <c++/4.8/iosfwd>
#include <c++/4.8/bits/stl_pair.h>
#include "../semain/semain.h"
#include "../seglobal/exceptions.h"
#include "../seadmin/seadminapi.h"
#include "../senet/CGeoIPProvider.h"
#include <jsoncpp/json/reader.h>
#include "CJsonRPCApp.h"
#include "CInvestmentProducts.h"


using namespace se;
using namespace se::net;
using namespace std;
using namespace se::admin;

std::recursive_mutex CAppBase::_lock;
CBrowserSession CAppBase::bs;

// Child class must define the currently active skin.
// The skin could not statically defined or dynamically 
// chosen based on circumstances like a mobile phone browser.
string _currentSkin = "skdefault";
string _emailtempls = "skdefemailtemplates";

CAppBase::CAppBase(cppcms::service& srv) : application(srv)
{
    using namespace se::config;
    SP<IApp> pApp;
    SP<IConfigurator> pCfg;
    SEOpenApp(&pApp);
    THROWIFFAILED(pApp->get_Configurator(&pCfg), "Failed to open the configurator instance.");
    _rootPath = pCfg->get_ExecutableDir();
 
    attach(new CJsonRPCApp(srv), "rpc", "/rpc{1}", "/rpc(/(.*))*", 1);
    attach(new CInvestmentProducts(srv), "investmentproducts", "/investmentproducts{1}", "/investmentproducts(/(.*))*", 1);
   
    dispatcher().assign("/images[/\\w\\W]?/([\\_\\w\\W-\\.]+)\\.(bmp|tif|jpg|png|gif)$", &CAppBase::GetImage, this, 1, 2);
    mapper().assign("images", "/images/{1}.{2}");
    
    dispatcher().assign("/prog[/\\w\\W]?/([\\w\\W-\\.]+)\\.(js)$", &CAppBase::GetJs, this, 1, 2);
    mapper().assign("prog", "/prog/{1}.{2}");

    dispatcher().assign("/css[/\\w\\W]?/([\\w\\W-\\.]+)\\.(css)$", &CAppBase::GetCss, this, 1, 2);
    mapper().assign("css", "/css/{1}.{2}");

    dispatcher().assign("/html[/\\w\\W]?/([\\w\\W-\\.]+)\\.(html)$", &CAppBase::GetHtml, this, 1, 2);
    mapper().assign("html", "/html/{1}.{2}");
    
    dispatcher().assign("/txt[/\\w\\W]?/([\\w\\W-\\.]+)\\.(txt|json|doc|docx|xls|xlsx)$", &CAppBase::GetText, this, 1, 2);
    mapper().assign("txt", "/txt/{1}.{2}");
    
    dispatcher().assign("/ext[/\\w\\W]?/([\\w\\W-\\.]+)\\.(.+)$", &CAppBase::GetExt, this, 1, 2);
    mapper().assign("prog", "/prog/{1}.{2}");

    dispatcher().assign("/appsearch$", &CAppBase::GetSearchResults, this);
    mapper().assign("appsearch", "/appsearch");

}

CAppBase::~CAppBase() 
{
}

void CAppBase::OnSessionClear(SessionStorage& store) 
{

}


void CAppBase::GetFile(int location, const string& filename, const string& ext) 
{
    string f ;
    switch (location)
    {
        case 0: // images
            f = "images/" + filename + "." + ext;
            break;
        case 1: // css
            f = "css/" + filename + "." + ext;
            break;
        case 2: // js
            f = "prog/" + filename + "." + ext;
            break;
        case 3: // text
            f = "txt/" + filename + "." + ext;
            break;
        case 4:
            f = "ext/" + filename + "." + ext;
            break;
        case 5:
            f = "html/" + filename + "." + ext;
            break;
        default:
            throw seexception(E_NotFound, "File not found.");
    }
    
    FileCache::iterator itfind = _fileCache.find(f);
    if (_fileCache.end() != itfind )
    {
        response().out() <<  itfind->second;
    }
    else
    {
        std::lock_guard<std::recursive_mutex> g(_lock);
        auto fs = _rootPath + f;
        fstream strm;
        strm.open(fs);
        if (strm.is_open())
        {
#ifndef __DEBUG__
            string buff( (std::istreambuf_iterator<char>(strm)), std::istreambuf_iterator<char>());
            auto pr = _fileCache.insert(make_pair(f, buff));
            response().out() << pr.first->second;
#else
            response().out() << strm.rdbuf();
#endif
            strm.close();
        }
    }
}

void CAppBase::GetExt(string filename, string ext) 
{
    GetFile(4, filename, ext);
}


void CAppBase::GetImage(string filename, string ext)
{
    GetFile(0, filename, ext);
}

void CAppBase::GetCss(string filename, string ext) {

    return GetFile(1, filename, ext);
}

void CAppBase::GetHtml(string filename, string ext) {

    return GetFile(5, filename, ext);
}

void CAppBase::GetJs(string filename, string ext) {

    return GetFile(2, filename, ext);
}

void CAppBase::GetText(string filename, string ext) {
    return GetFile(3, filename, ext);
}

void CAppBase::GetSearchResults() 
{
    
    auto str = request().post("term");
    
    response().set_content_header("application/json");
    Json::Value searchRoot;
    SP<ISecurityAPI> psecapi;
    // fill in all stocks
    // fill in all futures
    // fill in all options
    // fill in other securities if available.
    auto ret = SecOpen(&psecapi);
    if (ISOK(ret))
    {
        ret = psecapi->SearchModule(str.c_str(), SearchMethod::Exact, searchRoot);
    }
    stringstream ostr;
    Json::Value searchResults ;
    for (auto& jv : searchRoot)
    {
        for (auto& v : jv["Items"])
        {
            Json::Value item;
            item["label"] = v["Symbol"].asString() + ":" + v["Exchange"].asString() + " | " + v["Name"].asString() + " | " + v["Type"].asString();
            item["category"] = jv["Category"];
            ostr.str("");
            ostr << v["Id"].asInt64();
            item["value"] = "ip=" + v["Exchange"].asString() + "=" + ostr.str();
            searchResults.append(item);
        }
    }
    
    Json::FastWriter writer;
    BDBG << writer.write(searchResults);
    response().out() << writer.write(searchResults);
    
//    // fill more on news
//    response().out() << "[ \
//      { \"label\": \"anders\", \"category\": \"\" }, \
//      { \"label\": \"andreas\", \"category\": \"\" },\
//      { \"label\": \"antal\", \"category\": \"\" },\
//      { \"label\": \"annhhx10\", \"category\": \"Products\" },\
//      { \"label\": \"annk K12\", \"category\": \"Products\" },\
//      { \"label\": \"annttop C13\", \"category\": \"Products\" },\
//      { \"label\": \"anders andersson\", \"category\": \"People\" },\
//      { \"label\": \"andreas andersson\", \"category\": \"People\" },\
//      { \"label\": \"andreas johnson\", \"category\": \"People\" } \
//    ]";
}

bool CAppBase::PopulateGeoIpInfo(Json::Value& geoinfo) 
{
    CGeoIPProvider provider;
    SP<IString> output;
    string ip = request().remote_addr();
    //string ip = "74.205.59.143";
    //string ip = "100.37.110.203";
    if (ISOK(provider.get_GeoIpCountry(ip.c_str(), &output)))
    {
        Json::Reader reader;
        reader.parse(output->get_Buffer(), geoinfo, false);
        BOOSTER_INFO(__FUNCTION__) << output->get_Buffer();
        return true;
    }
    return false;
}


void CAppBase::RedirectToHome() 
{
    response().set_redirect_header("/");
}

void CAppBase::RedirectToLastPage() 
{
    const string& lastpage = bs.get_Url(session());
    if (lastpage.empty())
        RedirectToHome();
    else
        response().set_redirect_header(lastpage);
}

ICountryList* CAppBase::get_CachedCountries() 
{
    ICountryList* pCList = nullptr;
    const string key = "COUNTRIES";
    OmniType val;
    if ((val = bs.get_Value(key)).IsEmpty())
    {
        std::lock_guard<CBrowserSession> g(bs);
        if ((val = bs.get_Value(key)).IsEmpty())
        {
            SP<IAdminAPI> api;
            AdminOpen(&api);
            api->get_Countries(&pCList);
            if (pCList)
            {
                bs.set_Value(key, (IObject*)pCList); // OmniType adds ref count by one.
                pCList->DecRef();
                return pCList;
            }
        }
        else
            pCList =(ICountryList*)(IObject*)val;
    }
    else
        pCList =(ICountryList*)(IObject*)val;
    return pCList;
}

void CAppBase::get_States(const string& countryCode, IStateList** ppOut) 
{
    SP<IAdminAPI> api;
    AdminOpen(&api);
    api->get_States(ppOut, [this, &countryCode](IState::QUERYFLDS flds, LGX& lgx, bool& b)->CSTR
    {
       if (flds == IState::QUERYFLDS::CountryCode)
       {
           b = true;
           return countryCode.c_str();
       }
       return "";
    });
}



