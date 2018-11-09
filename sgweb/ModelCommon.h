/* 
 * File:   ModelCommon.h
 * Author: santony
 *
 * Created on September 10, 2013, 8:18 PM
 */

#ifndef MODELCOMMON_H
#define	MODELCOMMON_H
#include <cppcms/view.h>
#include <cppcms/application.h>
#include <string>
#include <cppcms/form.h>
#include "commontypes.h"
#include <vector>
#include "../sesecurities/sesecuritiesapi.h"

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
            private:
                cppcms::http::context* httpctxt;
                
            protected:
                cppcms::http::context& getContext();
                
            public:
                BaseContent(cppcms::http::context& httpctxt);
                virtual void Init() = 0;
                
            };
            
            class Footer : public BaseContent
            {
            public:
                Footer(cppcms::http::context& httpctxt);
                
                virtual void Init();
                
            };
            
            class Header : public BaseContent
            {
            public:
                std::list<std::string> menu_list;

            public:
                Header(cppcms::http::context& httpctxt);
                virtual void Init();
                
            };

        }
    }
}

#endif	/* MODELCOMMON_H */

