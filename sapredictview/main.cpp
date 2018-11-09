/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on March 12, 2015, 3:46 PM
 */

#include <cstdlib>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include "webapp.h"


using namespace std;
using namespace cppcms;

/*
 * 
 */
int main(int argc, char** argv) 
{
    service srv(argc, argv);
    srv.applications_pool().mount(applications_factory<webapp>());
    srv.run();

    return 0;
}

