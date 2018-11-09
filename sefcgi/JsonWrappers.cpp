/* 
 * File:   JsonWrappers.cpp
 * Author: santony
 * 
 * Created on September 3, 2012, 3:04 PM
 */
#include "../seglobal/stringconversions.h"
#include "../sexbrl/sexbrlapi.h"
#include <fcgiapp.h>
#include "FCgiIO.h"


#include <cgicc/HTMLClasses.h>
using namespace cgicc;

#include "JsonWrappers.h"


#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

#include "../sesecurities/sesecuritiesapi.h"
#include "../seglobal/stringconversions.h"

#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <sstream>


using namespace se;
using namespace se::gw;
using namespace se::xbrl;
using namespace se::securities;

CXbrlJsonWrapper::CXbrlJsonWrapper() 
        : _logger(log4cxx::Logger::getLogger("se.cgi.xbrlwrapper"))
{
    
}

CXbrlJsonWrapper::CXbrlJsonWrapper(const CXbrlJsonWrapper& orig) 
{
    
}

CXbrlJsonWrapper::~CXbrlJsonWrapper() 
{
    
}

ErrorId CXbrlJsonWrapper::ExecuteFunction(const std::string& url, std::ostream& io, I32 functionid, const FUNCPARAMS& parameters)
{
    ErrorId retVal;
    string sJson;
    switch (functionid)
    {
        case 1: // this function provides a public company information.
            if (parameters.size() == 0 || parameters[0].empty())
            {
                // LOG
                return E_InvalidArg;
            }
            retVal = GetCompanyInfo(parameters[0].c_str(), sJson);
            io << sJson << endl;
            break;
        case 2: // company performance. parameter is the ticker symbol
            if (parameters.size() == 0 || parameters[0].empty())
            {
                // LOG
                return E_InvalidArg;
            }
            retVal = GetCompanyPerformance(parameters[0].c_str(), sJson);
            io << sJson << endl;
            break;
    }
    return retVal;
}

ErrorId CXbrlJsonWrapper::GetCompanyInfo(CSTR ticker, string& jsonoutput)
{
    LOG4CXX_INFO(_logger, "Entering company info for ticker " << ticker);
            
     SP<ISecurity> sec; 
     SP<ISecurityAPI> api;
     ErrorId retVal = SecOpen(&api);
     Json::Value root;
     Json::FastWriter writer ;
     if (S_Success == retVal)
     {
         SP<ISecurityDescription> pSecDesc ;
         retVal = api->CreateSecurityDescription(&pSecDesc);
         if (S_Success == retVal)
         {
             pSecDesc->set_Symbol(ticker);
             
             SP<IPublicCompanyInfo> pCo;
             
             retVal = api->get_Company(pSecDesc, &pCo);
             if (S_Success == retVal)
             {
                auto name = pCo->get_Name();
                auto summary=pCo->get_Summary();
                SP<IAddress> Address;
                auto retAdd=pCo->get_Address(&Address);
                auto street=Address->get_Street();
                auto city=Address->get_City();
                auto state=Address->get_State();
                auto country=Address->get_Country();
                auto postalcode=Address->get_PostalCode();
                SP<IGICInfo> pGic;
                auto retGic=pCo->get_GICInfo(&pGic);
                auto sector=pGic->get_Sector();
                auto industry=pGic->get_Industry();
                if (S_Success == retVal)
                {


                  /*  {

                        name:  name,
                        address: 
                           [
                               street:street,
                               city:city,
                               state:state,
                               "postalcode":postalcode, 
                               "country":country,
                           ],
                        "summary":summary,

                         "gic":[
                        "sector":sector,
                         "industry":industry,
                                ]
                    };
                      */


                    /* using pCo create a json stream as below using jsconcpp */
                    /*
                     * {
                     *  name : xxxx,
                     *  address : 
                     *      {
                     *          street,
                     *          city,
                     *          state,
                     *          postalcode, 
                     *          country
                     *      },
                     *  summary : sssss,
                     *  gic : { sector, industry }
                     * }
                     */
                    /* and set it to sJason variable (see above)*/


                    root["ns"] = 1;    // xbrl namespace
                    root["fn"] = 1;      // get company function id.
                    root["p0"] = ticker;  // parameter.
                    root["name"] = name;
                    root["address"]["street"]=street;
                    root["address"]["city"]=city;
                    root["address"]["postalcode"]=postalcode;
                    root["address"]["country"]=country;
                    root["summary"]=summary;
                    root["gic"]["sector"]=sector; 
                    root["gic"]["industry"]=industry;               

                }
             }
         }
         SecClose(api);
     }
     if (S_Success != retVal)
     {
         LOG4CXX_ERROR(_logger, "Error in GetCompany function while generating json: Error code: " << retVal);
        root["ns"] = 1;    // xbrl namespace
        root["fn"] = 1;      // get company function id.
        root["p0"] = ticker;  // parameter.
        root["error"] = "Error reading company info. Please contact site mailto:admin: smartearn@smart-earn.com.";
     }
    jsonoutput = writer.write(root);
    
    LOG4CXX_INFO(_logger, "Exiting company info for ticker " << ticker << " with retCode " << retVal);

    return retVal;
 }

ErrorId CXbrlJsonWrapper::GetCompanyPerformance(CSTR ticker, string& jsonoutput)
{
    LOG4CXX_INFO(_logger, "Entering company performance for ticker " << ticker);

    SP<ISecurity> sec; 
     SP<ISecurityAPI> api;
     ErrorId retVal = SecOpen(&api);
      if (S_Success == retVal)
     {
         SP<ISecurityDescription> pSecDesc ;
         retVal = api->CreateSecurityDescription(&pSecDesc);
         if (S_Success == retVal)
         {
             pSecDesc->set_Symbol(ticker);
             
             SP<IPublicCompanyInfo> pCo;
             
             retVal = api->get_Company(pSecDesc, &pCo);
             if (S_Success == retVal)
             {
                 SP<IString> str;
                 retVal = pCo->get_Performance(&str);
                 if (S_Success == retVal)
                    jsonoutput = str->get_Buffer();
             }
         }
      }
    LOG4CXX_INFO(_logger, "Exiting company performance for ticker " << ticker << " with retCode " << retVal);

    return retVal;
}




/******************* ADMIN WRAPPER **********************/
CAdminJsonWrapper::CAdminJsonWrapper()
{
    
}

CAdminJsonWrapper::~CAdminJsonWrapper()
{
    
}


ErrorId CAdminJsonWrapper::ExecuteFunction(const std::string& url, std::ostream& io, signed int functionid, const std::vector<std::string>& parameters)
{
    FCgiIO& fcgiIo = dynamic_cast<FCgiIO&>(io);
    switch (functionid)
    {
    case 0: // Register a user.
        RegisterUser(fcgiIo);
        break; 
    }
    return S_Success;
}

using namespace cgicc;

ErrorId CAdminJsonWrapper::RegisterUser(FCgiIO& io)
{
    Cgicc cio(&io);
    form_iterator test1 = cio.getElement("test1");
    if (test1 != cio.getElements().end())
    {
        io << "test1 = " << **test1 << endl;
        return S_Success;
    }
    io << "not found" << endl;
    return E_NotFound;
}