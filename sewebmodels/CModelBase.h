/* 
 * File:   CModelBase.h
 * Author: santony
 *
 * Created on January 24, 2015, 2:35 PM
 */

#ifndef CMODELBASE_H
#define	CMODELBASE_H

#include <string>
#include <cppcms/view.h>
#include <cppcms/base_content.h>
#include "common.h"
#include <map>
#include "cppcmsfilters.h"
#include <regex>
#include "../senet/CSmtpClient.h"
#include <cppcms/serialization.h>
////// custom serialization traits ////////////////
#include "CSerializationTraits.h"


using namespace cppcms;
using namespace std;
using namespace se;
using namespace se::admin;




typedef map<string, string> ValidationErrors;

class CModelBase : public base_content, public cppcms::serializable
{
public:
    string RequiredStr = "*";
            
private:
    ValidationErrors Errors;
    string _title;
    string _viewName;
    static se::net::CSmtpClient* pEmailClient; 
    
protected:
    void set_Error(const string& control, const string& error);
    void set_CommonError(const string& error);
    
    const string& get_Error(const string& control);
    
    
    
public:
    CModelBase(const string& viewName = "", const string& title = "");
    CModelBase(const CModelBase& orig) = delete;
    virtual ~CModelBase();
    
    virtual const string& get_Title() ;
    
    virtual void set_Title(const string& title);

    
    const string& get_ViewName();
    
    void set_ViewName(const string&);
    
    cppcms::application& app();

    virtual void app(cppcms::application& app);
    
    string get_RootUrl() ;
    
    virtual bool get_IsValid();
    
    virtual void Notify(const string& to, const string& sub, const string& msg);

    virtual void NotifyError(const string& msg);
    
    virtual const string& CommonValidationText();


    virtual void serialize(archive& a);


};

#endif	/* CMODELBASE_H */

