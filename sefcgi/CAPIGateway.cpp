/* 
 * File:   CAPIGateway.cpp
 * Author: santony
 * 
 * Created on August 31, 2012, 1:42 AM
 */

#include "CJsonWrapper.h"
#include "CAPIGateway.h"
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPXHTMLHeader.h>
#include <cgicc/XMLDeclaration.h>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include "../seglobal/exceptions.h"

using namespace cgicc;
using namespace boost;
using namespace se::gw;

CAPIGateway::CAPIGateway() : _logger(Logger::getLogger("se::gw::apigateway"))
{
}

CAPIGateway::CAPIGateway(const CAPIGateway& orig) :_logger(Logger::getLogger("se::gw::apigateway"))
{
}

CAPIGateway::~CAPIGateway() 
{
}


void CAPIGateway::InvokeMethod(const std::string& url, std::ostream& io)
{
    /* 
     * delegate to subhandlers using the threadpool.
     * after handling the request by the threadpool, close request by the threadpool handler thread.
     * 
     * NOTE: just before writing to io, ensure that
     * Cgicc is created passing the io pointer.
     *
     */
    
    try
    {
    
        io << HTTPHTMLHeader() << endl;


        //const string& sReqUri = io.getenv("REQUEST_URI");
        const string& sReqUri = url;
        // the request uri is of this form :
        // http://domain.com/cgi-bin/segw.jac/xbrl/1/goog
        // our job is to extract string after segw.jac, which is 
        // xbrl/1/goog. This part of the string is converted to a function
        // name like the following:

        /*
         *  xbrl = sexbrl library
         *  1 = function identifier. 
         *  goog = function parameter.
         */
        int start = sReqUri.find("segw.jac") + 9;
        string sfdets = sReqUri.substr(start);
        typedef tokenizer< char_separator<char> > Tokenizer;
        typedef vector<string> TokenVector;
        TokenVector vec;
        char_separator<char> sep("/", "", keep_empty_tokens);
        Tokenizer tok(sfdets, sep);
        vec.assign(tok.begin(), tok.end());
        string modName, funcName;
        vector<string> params;
        if (vec.size() >= 1)
            modName = vec[0];
        if (vec.size() >= 2)
            funcName = vec[1];
        if (vec.size() >=3 )
        {
            for(int i = 2; i != vec.size(); ++i)
                params.push_back(vec[i]);
        }
        
        ErrorId retVal ;
        if (S_Success != (retVal = _jsonWrapper.ExecuteFunction(
                url,
                io,
                atoi(modName.c_str()), 
                atoi(funcName.c_str()),
                params
                )))
        {
            throw se::seexception(retVal);
        }
    }
    catch (const exception& err)
    {
        throw err;
    }
    catch (...)
    {
        throw std::runtime_error("An exception occurred while returning data. Please contact administrator.");
    }
}

