/* 
 * File:   CBaseContent.h
 * Author: santony
 *
 * Created on September 18, 2013, 9:19 PM
 */

#ifndef CBASECONTENT_H
#define	CBASECONTENT_H

#include <cppcms/view.h>
#include <cppcms/application.h>
#include <string>
#include <cppcms/form.h>

#include "CSession.h"

using namespace cppcms;
using namespace std;

namespace sg
{
    namespace web
    {
        namespace models
        {
            class BaseContent : public base_content
            {
            public:
                string viewName;
                bool hasError;
                
            protected:
                static string _errorNotify, _adminNotify;
                cppcms::http::context* httpctxt;
                string _error;
                SP<IWebSession> _pUserSession;

            public:
                cppcms::http::context& getContext();

            public:
                BaseContent();
                BaseContent(const BaseContent& cp);
                BaseContent(cppcms::http::context& httpctxt);
                BaseContent(cppcms::http::context& httpctxt, string const& viewName);
                
                virtual void Init();
                virtual void Init(cppcms::http::context& httpctxt);
                virtual void Init(cppcms::http::context& httpctxt, const string& viewName);
                
                void set_Error(const string& error, bool email = true);
                const string& get_Error();
                void ModifyView(const string& newViewName);
                IWebSession* get_Session();
                void set_Session(IWebSession* p);
                void WriteFormattedDouble(std::ostream& os, const OmniType& value, int precision = 4) const;
                void WriteFormattedDouble(std::ostream& os, R8 value, int precision = 4) const;
                
            };
        }
    }
}

#endif	/* CBASECONTENT_H */

