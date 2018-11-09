/* 
 * File:   CJsonWrapper.cpp
 * Author: santony
 * 
 * Created on September 3, 2012, 1:24 PM
 */

#include <iostream>
#include "CJsonWrapper.h"
#include "JsonWrappers.h"

#include <cgicc/HTMLClasses.h>

using namespace se;
using namespace se::gw;
using namespace cgicc;

CJsonWrapper::CJsonWrapper() :_logger(Logger::getLogger("se::gw::jsonwrapper"))
{
}

CJsonWrapper::CJsonWrapper(const CJsonWrapper& orig) :_logger(Logger::getLogger("se::gw::jsonwrapper"))
{
}

CJsonWrapper::~CJsonWrapper() 
{
    // delete the map's content
    for (MODULEMAP::iterator it = moduleMap.begin(); 
            it != moduleMap.end(); 
            ++it)
        delete it->second;
    moduleMap.clear();
}

static int counter = 0;

ErrorId CJsonWrapper::ExecuteFunction(const std::string& url, std::ostream& io, I32 ModuleId, I32 FunctionId, const FUNCPARAMS& parameters)
{
    // if moduleid = 0 and functionid = 0, we simply return 
    // function details.
    // if wrapper class is available.
    LOG4CXX_INFO(_logger, "Entering execute function");
   
    CJsonWrapperBase* pWrapper = 0;
    MODULEMAP::const_iterator it; 
    it = moduleMap.find(ModuleId);

    if (it != moduleMap.end())
    {
        //io << "Found the item." << br();
        pWrapper = it->second;
    }
    else
    {
        /* IMPORTANT:
         * For each module, a CASE statement block need to be added
         * below.
         */
        switch (ModuleId)
        {
            case 0:
                io << "<URI>" << url << "</URI>";
                return S_Success;
            case 1: // sexbrl module
                //io << "creating xbrlwrapper." << br();
                pWrapper = new CXbrlJsonWrapper();
                break;
            case 2: // sesecurities module
                break;
            case 3: // seadmin module
                pWrapper = new CAdminJsonWrapper();
                break;
        }
        pair<I32, CJsonWrapperBase*> p(ModuleId, pWrapper);
        moduleMap.insert(p);
    }
    ErrorId retCode = pWrapper->ExecuteFunction(url, io, FunctionId, parameters);
    
    LOG4CXX_INFO(_logger, "Exiting execute function with retCode " << retCode);

    return retCode;
    
}
