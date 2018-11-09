/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on August 6, 2013, 6:22 PM
 */

#include <cstdlib>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <iostream>
#include "content.h"
#include <fstream>
#include <assert.h>

using namespace std;
using namespace se::model;


class hello : public cppcms::application
{
    ifstream _fcss;
    ifstream _fjs;
    
public:
    hello(cppcms::service& srv) : cppcms::application(srv)
    {
        
        dispatcher().assign("", &hello::intro, this);
        mapper().assign("");

        dispatcher().assign("/news", &hello::news, this);
        mapper().assign("news", "/news");

        dispatcher().assign("/page", &hello::page, this);
        mapper().assign("page", "/page");

        dispatcher().assign("/style", &hello::css, this);
        mapper().assign("style", "/style");

        dispatcher().assign("/script", &hello::script, this);
        mapper().assign("script", "/script");

        dispatcher().assign("/images/(([\\w\\W]+)(\\.)(jpg|ico|png|bmp))", &hello::image, this, 1);
        mapper().assign("images", "/images/{1}");
        
        mapper().root("/hello");
    }
    
    ~hello()
    {
        _fcss.close();
    }
    
    void ini(master& c)
    {
        c.title = "My web site";
    }
    
    void intro()
    {
        master c(&this->session());
        ini(c);
        render("intro", c);
    }
    
    void page()
    {
        se::model::page c(&this->session());
        ini(c);
        c.page_title = "About";
        c.page_content = "<p>A page about this website</p>";
        c.test.teststr = "This is an nested type property.";
        render("page", c);
        
    }
    
    
    void news()
    {
        se::model::news c(&this->session());
        ini(c);
        c.news_list.push_back("India sggreelaunches an aircraft carrier.");
        c.news_list.push_back("India's nuclear submarine reaches critical.");
        c.news_list.push_back("And other news.");
        render("news", c);
    }
    
    void css()
    {
        if (!_fcss.is_open())
        {
            _fcss.open("sggreen.css");
            assert(_fcss.is_open());
        }
        OutputStream(_fcss);
    }
    
    void script()
    {
        if (!_fjs.is_open())
        {
            _fjs.open("sgscript.js");
            assert(_fjs.is_open());
        }
        OutputStream(_fjs);
    }

    void image(string filename)
    {
        ifstream img;
        img.open(filename.c_str());
        assert(img.is_open());
        OutputStream(img);
        img.close();
        //response().content_type("application/octet-stream");
        //response().set_header("X-Lighttpd-Send-File",filename);
        
    }
    
    void OutputStream(istream& strm)
    {
        strm.clear();
        strm.seekg(0, ios::beg);
        response().out() << strm.rdbuf();
    }
    
};


/*
 * 
 */
int main(int argc, char** argv) 
{
    
    try
    {
        cppcms::service srv(argc, argv);
        srv.applications_pool().mount(cppcms::applications_factory<hello>());
        srv.run();
    }
    catch(std::exception const &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

