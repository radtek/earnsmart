/* 
 * File:   CJsonRPCApp.h
 * Author: santony
 *
 * Created on September 1, 2015, 3:33 PM
 */

#ifndef CJSONRPCAPP_H
#define	CJSONRPCAPP_H

#include <cppcms/application.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <string>
#include "../seglobal/IObject.h"


using namespace std;

class CJsonRPCApp : 
    public cppcms::application
{
   
   
public:
    CJsonRPCApp(cppcms::service &srv);
    CJsonRPCApp(const CJsonRPCApp& orig)=delete;
    virtual ~CJsonRPCApp();
   
    
protected:
   
    void ReturnError(int code, const string& message, int id = 0, const string& data = "");
    void ReturnSuccess(int id, const Json::Value& value);
    
    void Default();
    void Countries(int id, const string& token, const string& code = "");
    void div(int id, const string& token, double x, double y);
    
    void CheckLoginId(int id, const string& token, const string& emailAddress);
    
    // internal
    //RPC Error code starts from 33000

};

#endif	/* CJSONRPCAPP_H */

