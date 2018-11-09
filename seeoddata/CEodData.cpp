/* 
 * File:   CEodData.cpp
 * Author: santony
 * 
 * Created on February 1, 2015, 8:17 PM
 */

#include "seeoddataint.h"
#include "CEodData.h"
#include "CEODDataWrappers.h"
#include <jsoncpp/json/json.h>
#include "../semain/semain.h"
#include "nsDataSoapProxy.h"
#include "CWSClientSession.h"
#include <fstream>



namespace se
{
namespace eoddata
{

#define LOGGER "CEodData"

//NamedObjectNamingType IEODData::Name = 0xB;

CEodData::CEodData(ISession* ps) : CObjectRoot(ps)
{
}

CEodData::CEodData(const CEodData& orig) : CObjectRoot(orig)
{
}

CEodData::~CEodData()
{
}

NamedObjectNamingType CEodData::get_TypeId()
{
    return IEODDataName;
}

long unsigned int CEodData::Init()
{

    return S_OK;
}

long unsigned int CEodData::LoginToWSClient(const char* userName, const char* password, IWSClientSession** ppOut)
{
    return CObject<CWSClientSession>::Create(get_Session(), userName, password, ppOut);
}

long unsigned int CEodData::OpenDefault(IWSClientSession** ppOut)
{
    // read json for user id and password.
    // executable directory
    SP<IApp> papp;
    RETURNIFFAILED(SEOpenApp(&papp));
    SP<se::config::IConfigurator> pcfg;
    RETURNIFFAILED(papp->get_Configurator(&pcfg));
    CSTR cfgpath = pcfg->get_ConfigFilePath();
    Json::Reader reader;
    Json::Value root;
    std::ifstream ifs(cfgpath);
    if (!reader.parse(ifs, root, false))
    {
        BERR << "Error reading the configuration file " << cfgpath;
        return E_ConfigurationError;
    }

    if (!root.isMember("eoddata"))
    {
        BERR << "Configuration file " << cfgpath << " does not contain reference to eoddata.";
        return E_ConfigurationError;
    }

    Json::Value eoddata = root["eoddata"];

    SP<IWSClientSession> psession;
    RETURNIFFAILED(CObject<CWSClientSession>::Create(get_Session(), eoddata["userid"].asCString(), eoddata["password"].asCString(), &psession));
    psession.CopyTo(ppOut);

    return S_OK;
}

long unsigned int CEodData::ReadEODFile(const string& fileName, const EODFileTraits& traits)
{
    return E_NotImpl;
}

}
}