/* 
 * File:   CConfigurator.cpp
 * Author: santony
 * 
 * Created on November 6, 2012, 6:20 AM
 */

#include "semaininternal.h"
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <boost/filesystem.hpp>
//#include <log4cxx/log4cxx.h>
//#include <log4cxx/logger.h>
//#include <log4cxx/basicconfigurator.h>
//#include <log4cxx/propertyconfigurator.h>
//#include <log4cxx/helpers/exception.h>
//#include <log4cxx/xml/domconfigurator.h>
#include "../semysql/CSEConnections.h"
#include "../seglobal/exceptions.h"
#include "CConfigItem.h"
#include <boost/algorithm/string.hpp>
#include "../seglobal/CFunctionCache.h"
#include <boost/foreach.hpp>
#include "../seglobal/CDirectory.h"
#include "CApplication.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "CConfigurator.h"
#include "../seglobal/globallogger.h"
#include <syslog.h>

using namespace boost;
using namespace boost::property_tree;
using namespace Json;
using namespace se::data;

#define DEFAULTCONFIGLOCATION "../../../../config/"
#define SEDBCONN        CSEConnections::GetMain()

//ptree config_tree;

CConfigurator::CConfigurator(ISession* ps)  : CObjectRoot(ps), _bIsGlobalCached(false), _bIsMaintenanceMode(false)
{
    
}

CConfigurator::CConfigurator(const CConfigurator& orig) : CObjectRoot(orig)
{
}

CConfigurator::~CConfigurator() 
{
    // when configuration goes out of scope;
    CSEConnections::Shutdown();
}

ErrorId CConfigurator::Init()
{
    SP<IString> pstr;
    CDirectory::GetExePath(&pstr);
    _rootDir = pstr->get_Buffer();
    std::string dbconfig = "./se.json";
    bool fileCheck = boost::filesystem::exists("./se.json");
    if (!fileCheck)
    {
        auto lastof = _rootDir.find_last_of("/", _rootDir.length()-1);
        _rootBin = _rootDir.substr( lastof + 1);
        _rootDir = _rootDir.substr(0, lastof + 1);
        dbconfig = _rootDir + "se.json";
        fileCheck = boost::filesystem::exists(dbconfig);
    }
    else
    {
        dbconfig = CDirectory::GetCurDir() + "/se.json";
    }
    if (!fileCheck)
    {
        BERR << "Configuration File (se.json) not found.";
        return E_ConfigurationError;
    }
    else
    {
        BINF << "Configuration file found at " << dbconfig;
        _cfgfilepath = dbconfig;
        // open file
        std::ifstream fs;
        fs.open(dbconfig, std::ifstream::in);
        Reader rdr;
        if (rdr.parse(fs, _cfgroot, false))
        {
            if (_cfgroot.isMember("functionCaching"))
            {
                Value& v0 = _cfgroot["functionCaching"];
                if (v0.isMember("GlobalCaching"))
                {
                    Value& v1 = v0["GlobalCaching"];
                    _bIsGlobalCached = v1.asBool();
                }
            }
            if (_cfgroot.isMember("EnableMaintenanceMode"))
            {
                _bIsMaintenanceMode = _cfgroot["EnableMaintenanceMode"].asBool();
            }
            if (_cfgroot.isMember("env"))
            {
                const string& senv = _cfgroot["env"].asString();
                if (senv == "STAGE")
                    _env = ENV_Staging;
                else if (senv == "PROD")
                    _env = ENV_Prod;
                else if (senv == "DEV")
                    _env = ENV_Dev;
                else
                    _env = ENV_Dev;
            }
            fs.close();
            
            if (_cfgroot.isMember("logging"))
            {
                RETURNIFFAILED(InitLog(_cfgroot["logging"]));
            }
            
            if (_cfgroot.isMember("defmdprovider"))
            {
                string provider = _cfgroot["defmdprovider"].asString();
                if (provider == "eoddata")
                {
                    _defMarketDataProvider = MARKETDATAPROVIDERS::EodData;
                }
            }
            return InitDb(dbconfig);
        }
        else
        {
#ifdef __DEBUG__
            string s;
            s = rdr.getFormattedErrorMessages();
            cout << s << endl;
#endif
            return E_ConfigurationError;
        }
    }
}

ErrorId CConfigurator::InitLog(const Value& /* lgsettings */)
{
    return S_OK;
}

ErrorId CConfigurator::InitDb(const string& cfgfile)
{
    //LoggerPtr logger = Logger::getLogger("se.main.configuration.initdb");
    bool ret = CSEConnections::InitConfig(cfgfile.c_str());
    if (ret) 
    {
        return S_Success ;
    }
    else
    {
        BERR << "DB Connection Load error";
        return E_DBConnFile;
    }
}

MARKETDATAPROVIDERS CConfigurator::get_DefaultMarketDataProvider() const 
{
    return _defMarketDataProvider;
}


CSTR CConfigurator::get_ExecutableDir() 
{
    return _rootDir.c_str();
}

ENV CConfigurator::get_Environment()
{
    return _env;
}

CSTR CConfigurator::get_ExecutableFile() 
{
    return _rootBin.c_str();
}

ErrorId CConfigurator::get_Settings(U16 regionId, const char* parent, IConfigItemList** ppOut)
{
    //LoggerPtr _logger = Logger::getLogger("se.main.CConfigurator");
    try
    {
        string funcName = "se.config.get_Settings";
        string senv;
        string par = parent;
        boost::to_upper(par);
        
        CConfigItemList* p;
        if (!FC.TrySPGetFValue(funcName, par, (I32)_env, p))
        {
            std::lock_guard<std::recursive_mutex> g(CApplication::appLock);
            if (!FC.TrySPGetFValue(funcName, par, (I32)_env, p))
            {
                CSmartConnection conn(SEDBCONN);
                switch (_env)
                {
                    case ENV_Prod:
                        senv = "PROD";
                        break;
                    case ENV_Staging:
                        senv = "STAGE";
                        break;
                    case ENV_Dev:
                    default:
                        senv = "DEV";
                        break;
                }
                Query q = conn->query();
                q << "SELECT * FROM sa.se_gbl_config where";
                q << " config_env in ('ALL', " << quote << senv << ") ";
                q << " and config_reg = " << regionId ;
                q << " and config_parent_id = (select config_id from se_gbl_config where upper(config_name) = ";
                q << quote << par << ")";

                //LOGIP(q.str())

                StoreQueryResult qr = q.store();
                if (!qr.size())
                    throw seexception(E_DBQueryException, "No config records found for given config parent and environment.");

                ErrorId ret = CObject<CConfigItemList>::Create(get_Session(), &p);
                if (FAILED(ret))
                    throw seexception(ret, "Failed to create an instance of CConfigItemList.");
                for (Row& rr : qr)
                {
                    CConfigItem* pItem;
                    ret = CObject<CConfigItem>::Create(get_Session(), &pItem);
                    if (FAILED(ret))
                        throw seexception(ret, "Failed to create an instance of CConfigItem.");
                    pItem->_name = rr["config_name"].c_str();
                    pItem->_type = (DATATYPES)(I32)rr["config_type"];
                    pItem->_value = rr["config_value"].c_str();
                    pItem->set_Id(rr["config_id"]);
                    p->Add(pItem);
                }
                FC.SPSetFValue(funcName, par, (I32)_env, p);
                *ppOut = p;
                return S_Success;
            }
        }
        p->AddRef();
        *ppOut = p;
        return S_Success;
    }
    CATCHCLAUSE("get_Settings");
}

long unsigned int CConfigurator::set_Settings(unsigned short regionId, const char* parent, IConfigItem* pItem) 
{
    CSmartConnection conn(SEDBCONN);
    try
    {
        conn.BeginTransaction();
        U64 parentId;
        auto q = conn->query();
        // get parent id
        auto condition = (string)"config_name='" + parent + "'";
        auto str = conn.get_ScalarValue("se_gbl_config", "config_id", condition.c_str());
        if (str.empty())
        {
            // save parent record
            q = conn->query();
            q << "insert into se_gbl_config (config_name, config_parent_id, config_env, config_reg) ";
            q << "values ('" << parent << "', 0, 'ALL', " << regionId << ");";
            q.exec();
            parentId = q.insert_id();
        }
        else
        {
            istringstream istr(str);
            istr >> parentId;
        }
        q = conn->query();
        q << "insert into se_gbl_config (config_name, config_value, config_type, config_parent_id, config_env, config_reg) ";
        q << " values (";
        q << "'" << pItem->get_Name() << "', '" << pItem->get_Value() << "', '" << (I32)pItem->get_Type() << "', " << parentId << ", 'ALL', " << regionId << " ";
        q << ") ON DUPLICATE KEY UPDATE config_value ='" << pItem->get_Value() << "';";
        
        BINF << q.str();
        q.exec();
        
        conn.CommitTransaction();
        
        return S_OK;
    }
    ROLLBACKCATCHCLAUSE("set_Settings");
}


bool CConfigurator::get_FunctionCacheSettings(const char* funcName) 
{
    //if (_cfgroot.isMember("functionCaching"))
    //{
    //}
    return false;   // not used any more.
}

bool CConfigurator::get_GlobalCacheEnabled() 
{
    return _bIsGlobalCached;
}

bool CConfigurator::get_IsInMaintenanceMode() 
{
    return _bIsMaintenanceMode;
}

CSTR CConfigurator::get_ConfigFilePath() 
{
    return _cfgfilepath.c_str();
}

const Json::Value& CConfigurator::get_ConfigRoot() const 
{
    return _cfgroot;
}

