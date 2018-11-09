/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on January 24, 2015, 12:39 PM
 */

#include <cstdlib>
#include <booster/log.h>
#include <signal.h>
#include <cppcms/applications_pool.h>
#include <cppcms/application.h>
#include "CAppMain.h"
#include "../seglobal/IObject.h"
#include "../semain/semain.h"
#include "CJsonRPCApp.h"

using namespace std;
using namespace booster;
using namespace cppcms;
using namespace se;

//INITIALIZE_EASYLOGGINGPP

bool recvedSighUp = false;
service* srv;

void HandleSignal(int sigNum);

/*
 * 
 */
int main(int argc, char** argv) 
{
    if (signal(SIGSEGV, HandleSignal) == SIG_IGN)
        signal(SIGSEGV, SIG_IGN);

    try
    {
        SP<se::IApp> pApp;
        SEGetApp(argv[0], &pApp);

        while(1)
        {
            srv = new service(argc, argv);
            srv->applications_pool().mount(applications_factory<CAppMain>());
            srv->run();
            if (recvedSighUp)
            {
                recvedSighUp = false;
                continue;
            }
            break;
        }
        
    }
    catch(std::exception const& e)
    {
        cerr << e.what() << endl;
    }
    
    return 0;
}


void HandleSignal(int sigNum)
{
    switch (sigNum)
    {
    case SIGHUP:
        recvedSighUp=true;
        srv->shutdown();
        break;
    }
}