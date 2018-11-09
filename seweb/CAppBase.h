/* 
 * File:   CAppBase.h
 * Author: santony
 *
 * Created on January 24, 2015, 1:03 PM
 */

#ifndef CAPPBASE_H
#define	CAPPBASE_H

#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include <cppcms/application.h>
#include <booster/log.h>
#include <map>
#include <unordered_map>
#include <mutex>
#include "../sewebmodels/CMaster.h"
#include "../sewebmodels/CError.h"
#include "../seglobal/IObject.h"
#include "CBrowserSession.h"

using namespace cppcms;
using namespace booster;
using namespace std;
using namespace se;

typedef unordered_map<string, OmniType> SessionStorage;
typedef map<string, string> CountryList;

class CAppBase : public application 
{
    typedef map<string, string> FileCache;

public:
    CAppBase(cppcms::service& srv);

    virtual ~CAppBase();

    void OnSessionClear(SessionStorage& store);

    
protected:
    string _rootPath;
    FileCache _fileCache;
    static std::recursive_mutex _lock;
    static CBrowserSession bs; // browser session
    // GEOIP Info is populated in the following json format:

protected: // cached protected functions.
    ICountryList* get_CachedCountries();
    void get_States(const string& countryCode, IStateList** ppOut);
    
protected:
    void RedirectToLastPage();
    void RedirectToHome();
    bool PopulateGeoIpInfo(Json::Value&);

private:
    void GetFile(int location, const string& filename, const string& ext );
    void GetImage(string filename, string ext );
    void GetJs(string filename, string ext );
    void GetCss(string filename, string ext );
    void GetHtml(string filename, string ext );
    void GetText(string filename, string ext );
    void GetExt(string filename, string ext );

    void GetSearchResults();
    
protected:
    
};

#endif	/* CAPPBASE_H */

