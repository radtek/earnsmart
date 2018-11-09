/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on August 15, 2013, 10:33 PM
 */

//#include "../seglobal/segvcatch.h"
#include <cstdlib>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/application.h>
#include <cppcms/applications_pool.h>


#include "CSmartearnApp.h"
#include "CFundamentalsApp.h"
#include "CAdminApp.h"
#include "../semain/semain.h"

#include <signal.h>
//#include <log4cxx/logger.h>

using namespace cppcms;
using namespace std;
using namespace sg::web;
using namespace se;

void HandleSignal(int sigNum);

/*
 * 
 */
int main(int argc, char** argv) 
{
    signal(SIGINT, HandleSignal);

    /*
    // intialize logging
    // Load configuration from file
    el::Configurations conf("./easylogging.cfg");
    // Reconfigure single logger
    el::Loggers::reconfigureLogger("default", conf);
    // Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(conf);
    // Now all the loggers will use configuration from file
    */
    
    try
    {
        SP<se::IApp> pApp;
        SEGetApp(argv[0], &pApp);

        service srv(argc, argv);
        if (argc == 4 && !strcmp(argv[3], "-a"))
        {
            srv.applications_pool().mount(applications_factory<CAdminApp>());
        }
        else
        {
            srv.applications_pool().mount(applications_factory<CSmartearnApp>());
        }
        srv.run();
    }
    catch(std::exception const& e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}

void HandleSignal(int sigNum)
{
    //using namespace log4cxx;
    //LoggerPtr logger = Logger::getLogger("sgweb.main.HandleSignal");
    switch (sigNum)
    {
    case 34:
        //LOG4CXX_ERROR(logger, "Received signal " << sigNum << ". Don't know what to do. Allow to pass.");
        break;
    default:
        //LOG4CXX_ERROR(logger, "Received signal " << sigNum << ". Don't know what to do. Allow to pass.");
        break;
    }
}