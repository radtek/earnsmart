/* 
 * File:   CConfigurator.h
 * Author: santony
 *
 * Created on November 6, 2012, 6:20 AM
 */

#ifndef CCONFIGURATOR_H
#define	CCONFIGURATOR_H

#include <jsoncpp/json/json.h>

using namespace se;
using namespace se::config;
using namespace Json;

class CConfigurator : public CObjectRoot, public IConfigurator
{
public:
    CConfigurator(ISession*);
    CConfigurator(const CConfigurator& orig);
    virtual ~CConfigurator();
    virtual ErrorId Init();     // override
    virtual ErrorId InitLog(const Value& lgsettings) ;
    virtual ErrorId InitDb(const string& cfgfile) ;
    virtual CSTR get_ExecutableDir() ;
    virtual CSTR get_ExecutableFile() ;
    virtual ENV get_Environment();
    virtual ErrorId get_Settings(U16 regionId, CSTR parent, IConfigItemList** ppOut);
    virtual SE_CODE set_Settings(U16 regionId, CSTR parent, IConfigItem* pItem);
    virtual bool get_FunctionCacheSettings(const char* funcName);
    virtual bool get_GlobalCacheEnabled();
    virtual bool get_IsInMaintenanceMode();
    virtual CSTR get_ConfigFilePath();

    virtual const Json::Value& get_ConfigRoot() const;

    MARKETDATAPROVIDERS get_DefaultMarketDataProvider() const override;


private:
    string _cfgfilepath;
    std::string _rootDir;
    std::string _rootBin;
    Json::Value _cfgroot;
    ENV _env;
    bool _bIsGlobalCached = false, _bIsMaintenanceMode = false ;
    MARKETDATAPROVIDERS _defMarketDataProvider = MARKETDATAPROVIDERS::NONE ;
};

#endif	/* CCONFIGURATOR_H */

