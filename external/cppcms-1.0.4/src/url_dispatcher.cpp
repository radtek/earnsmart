///////////////////////////////////////////////////////////////////////////////
//                                                                             
//  Copyright (C) 2008-2012  Artyom Beilis (Tonkikh) <artyomtnk@yahoo.com>     
//                                                                             
//  See accompanying file COPYING.TXT file for licensing details.
//
///////////////////////////////////////////////////////////////////////////////
#define CPPCMS_SOURCE
#include <cppcms/url_dispatcher.h>
#include <cppcms/application.h>
#include <booster/shared_ptr.h>
#include <vector>

#include "booster/log.h"

namespace cppcms 
{

    option::option(std::string expr):
        expr_(expr)
    {

    }

    option::~option()
    {
        
    }
    
    bool option::matches(const std::string& path)
    {
        return std::regex_match(path, match_, expr_);
    }

    template<typename H>
    struct base_handler : public option 
    {
            base_handler(std::string expr,H handle,int a=0,int b=0,int c=0,int d=0)
                    : option(expr),handle_(handle)
            {
                    select_[0]=a;
                    select_[1]=b;
                    select_[2]=c;
                    select_[3]=d;
            }
            virtual bool dispatch(std::string url)
            {
                if(matches(url)) 
                {
                    execute_handler(handle_);
                    return true;
                }
                return false;
            }
    private:
            void execute_handler(url_dispatcher::handler const &h)
            {
                    h();
            }

            void execute_handler(url_dispatcher::handler1 const &h)
            {
                    h(match_[select_[0]]);
            }

            void execute_handler(url_dispatcher::handler2 const &h)
            {
                    h(match_[select_[0]],match_[select_[1]]);
            }
            void execute_handler(url_dispatcher::handler3 const &h)
            {
                    h(match_[select_[0]],match_[select_[1]],match_[select_[2]]);
            }
            void execute_handler(url_dispatcher::handler4 const &h)
            {
                    h(match_[select_[0]],match_[select_[1]],match_[select_[2]],match_[select_[3]]);
            }

            int select_[4];
            H handle_;
    };


    template<typename H>
    booster::shared_ptr<option> make_handler(std::string expr,H const &handler,int a=0,int b=0,int c=0,int d=0)
    {
        return booster::shared_ptr<option>(new base_handler<H>(expr,handler,a,b,c,d));
    }

    // Meanwhile nothing
    url_dispatcher::url_dispatcher() :
                    d(new url_dispatcher::_data())
    {
    }
    url_dispatcher::~url_dispatcher()
    {
    }

    bool url_dispatcher::dispatch(std::string url)
    {
        unsigned i;
        for(i=0;i<d->options.size();i++) {
            if(d->options[i]->dispatch(url))
                return true;
        }
        return false;
    }

    void url_dispatcher::mount(std::string const &match,application &app,int select)
    {
            d->options.push_back(booster::shared_ptr<option>(new mounted(match,select,&app)));
    }



    void url_dispatcher::assign(std::string const &expr,handler h)
    {
            d->options.push_back(make_handler(expr,h));
    }

    void url_dispatcher::assign(std::string const &expr,handler1 h,int p1)
    {
            d->options.push_back(make_handler(expr,h,p1));
    }

    void url_dispatcher::assign(std::string const &expr,handler2 h,int p1,int p2)
    {
            d->options.push_back(make_handler(expr,h,p1,p2));
    }

    void url_dispatcher::assign(std::string const &expr,handler3 h,int p1,int p2,int p3)
    {
            d->options.push_back(make_handler(expr,h,p1,p2,p3));
    }

    void url_dispatcher::assign(std::string const &expr,handler4 h,int p1,int p2,int p3,int p4)
    {
            d->options.push_back(make_handler(expr,h,p1,p2,p3,p4));
    }
        

} // namespace cppcms
