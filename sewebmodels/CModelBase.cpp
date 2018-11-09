/* 
 * File:   CModelBase.cpp
 * Author: santony
 * 
 * Created on January 24, 2015, 2:35 PM
 */

#include "../../../../commonlib/cpplinq.hpp"
#include "common.h"
#include "CModelBase.h"
#include <regex>

#define TEXTCOMMON      "TEXTCOMMON"

using namespace std;

se::net::CSmtpClient* CModelBase::pEmailClient = new se::net::CSmtpClient();

CModelBase::CModelBase(const string& viewName, const string& title) : 
    _viewName(viewName), _title(title)
{
    if (_title.empty() && !_viewName.empty())
        _title = _viewName;
    Errors.insert(make_pair("NoField", ""));
}

CModelBase::~CModelBase() 
{
    Errors.clear();
}

void CModelBase::serialize(archive& a) 
{
    a & _title & _viewName & Errors;
}


const string& CModelBase::get_Error(const string& control) 
{
    return Errors[control]; //returns default value if not found which could be an empty string.
}


void CModelBase::set_Error(const string& control, const string& error) 
{
    auto find = Errors.find(control);
    if (find != Errors.end())
        Errors[control] = error;
    else
        Errors.emplace(make_pair(control, error));
}

void CModelBase::set_CommonError(const string& error) 
{
    auto find = Errors.find(TEXTCOMMON);
    if (find != Errors.end())
        Errors[TEXTCOMMON] = error;
    else
        Errors.emplace(make_pair(TEXTCOMMON, error));
}


const string& CModelBase::get_Title() 
{
    return _title;
}

void CModelBase::set_Title(const string& title) 
{
    _title = title;
}

const string& CModelBase::get_ViewName() {
    return _viewName;
}

void CModelBase::set_ViewName(const string& name) 
{
    _viewName = name;
}

cppcms::application& CModelBase::app()
{
    return base_content::app();
}

void CModelBase::app(cppcms::application& app)
{
    base_content::app(app);
}

string CModelBase::get_RootUrl() 
{
    return app().request().server_name();
}


bool CModelBase::get_IsValid() 
{
    return Errors.size()==1;
}

void CModelBase::Notify(const string& to, const string& sub, const string& msg) 
{
    if (pEmailClient)
    {
        const string& from = "sajiantony@hotmail.com";
        pEmailClient->SendMail(to.c_str(), from.c_str(), sub.c_str(), msg.c_str());
    }
}

void CModelBase::NotifyError(const string& msg) 
{
    if (pEmailClient)
    {
        const string& from = "sajiantony@live.com";
        const string& to = "sajiantony@hotmail.com";
        const string& sub = "Earnsmart Reports An Error";
        pEmailClient->SendMail(to.c_str(), from.c_str(), sub.c_str(), msg.c_str());
    }

}

const string& CModelBase::CommonValidationText() 
{
    return get_Error(TEXTCOMMON);
}



//void CModelBase::TestLinq() 
//{
//    using namespace cpplinq;
//    int ints[] = { 3, 1, 3, 4, 0, 1, 6 };
//    
//    auto evensum = 
//        from_array(ints) 
//        >> where ([](int i) -> int { return i%2 == 0;})
//        >> sum ()
//        ;
//    
//}
