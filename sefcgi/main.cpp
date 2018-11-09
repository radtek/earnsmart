/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on August 28, 2012, 1:25 PM
 */

#include <exception>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include "../seglobaltypedefs.h"
#include <fcgiapp.h>

#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include "FCgiIO.h"

#include "CAPIGateway.h"

#include <memory.h>

#include "../semain/semain.h"

using namespace std;
using namespace cgicc;
using namespace se::gw;


log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("se.fcgi.main"));


#define MTAPPROACH
//#define STAPPROACH
//#define JSONTESTS

#if defined(STAPPROACH)



int main(int argc, char** argv)
{
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);
    
    while (FCGX_Accept_r(&request) == 0)
    {
        try
        {

            FCGX_FPrintF(request.out,
                "Content-type: text/html\r\n"
                "\r\n"
                "<title>FastCGI Hello! (multi-threaded C, fcgiapp library)</title>"
                "<h1>FastCGI Hello! (multi-threaded C, fcgiapp library)</h1>");


//            FCgiIO io(request);
//            Cgicc cgi(&io);
//            
//            io << HTTPHTMLHeader() << 
//                    HTMLDoctype( HTMLDoctype::eStrict )
//                    << endl;
//            
//            io << head() << endl
//                    << title() << "GNU cgicc v"
//                    << cgi.getVersion() 
//                    << title() 
//                    << endl
//                    << head()
//                    << endl;
//            
//            io << body() << endl;
//            io << h1("cgicc/FastCgi Test") << endl
//                    << "PID: " << getpid() << br() << endl
//                    << "count: " << ++count << br() << endl;
//            io << "Form Elements:" << br() << endl;
//            for(const_form_iterator i = cgi.getElements().begin();
//                    i != cgi.getElements().end(); 
//                    ++i)
//            {
//                io << i->getName() << " = " << i->getValue() << br() << endl;
//            }
//            io << body() << html();
        }
        catch(const exception& )
        {
            // log
        }

        FCGX_Finish_r(&request);
    }
    return 0;
}


#elif defined(MTAPPROACH)
using namespace se;
using namespace se::gw;


static void *doit(void *a);

#define THREAD_COUNT 20
static int counts[THREAD_COUNT];
static U64 count = 0;

int main(int argc, char** argv) 
{
    SP<se::IApp> pApp;
    SEGetApp(&pApp);
    
    SP<se::config::IConfigurator> pConfig;
    pApp->get_Configurator(&pConfig);
    pConfig->InitLog();
    pConfig->InitDb();
    
    LOG4CXX_INFO(logger, "Entering se fcgi gw.");

    try
    {
        PSIZE i;
        pthread_t id[THREAD_COUNT];

        FCGX_Init();

        for (i = 1; i < THREAD_COUNT; i++)
            pthread_create(&id[i], NULL, doit, (void*)i);

        doit(0);
        
        LOG4CXX_INFO(logger, "Exiting se fcgi gw.");
        
    }
    catch(exception& ex)
    {
        LOG4CXX_ERROR(logger, "Exiting se fcgi gw with error " << ex.what());
    }
    catch (...)
    {
        LOG4CXX_ERROR(logger, "Exiting se fcgi gw with an unknown error ");
    }
    return 0;    
}

#define GATEWAY

static void *doit(void *a)
{
    PSIZE rc, i, thread_id = (PSIZE)a;
    pid_t pid = getpid();
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);
    CAPIGateway gw ;

    for (;;)
    {
        static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;
        static pthread_mutex_t counts_mutex = PTHREAD_MUTEX_INITIALIZER;

        /* Some platforms require accept() serialization, some don't.. */
        pthread_mutex_lock(&accept_mutex);
        rc = FCGX_Accept_r(&request);
        pthread_mutex_unlock(&accept_mutex);

        if (rc < 0)
            break;

        FCgiIO io(request);

        try
        {
            gw.InvokeMethod(io.getenv("REQUEST_URI"), io);
        }
        catch(std::exception& ex)
        {
            
            io.err() << "Exception thrown by the invoked method. Please contact administrator.";
        }
        
        FCGX_Finish_r(&request);
    }

    return NULL;
}

#elif defined(JSONTESTS)

#include <sstream>
#include <iosfwd>
#include "JsonWrappers.h"


using namespace std;

using namespace se;
using namespace se::gw;

int main(int argc, char** argv) 
{
    
    SP<se::IApp> pApp;
    SEGetApp(&pApp);
    
    SP<se::config::IConfigurator> pConfig;
    pApp->get_Configurator(&pConfig);
    pConfig->InitLog();
    pConfig->InitDb();
    
    LOG4CXX_INFO(logger, "Entering se fcgi gw.");

    try
    {

        stringstream io;
        CAPIGateway gw ;
        gw.InvokeMethod("http://localhost/segw.jac/1/1/goog", io);
        cout << io.str() << endl;
        gw.InvokeMethod("http://localhost/segw.jac/1/2/goog", io);
        cout << io.str() << endl;
        LOG4CXX_INFO(logger, "Exiting se fcgi gw.");
    }
    catch (std::exception& ex)
    {
        LOG4CXX_ERROR(logger, "Exited sefcgi gw with error " << ex.what());
    }
    return 0;
}

#else   // queued approach

int main(int argc, char** argv) 
{
    FCGX_Init();
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);
    while (FCGX_Accept_r(&request) == 0)
    {
        PSIZE threadid;
        pthread_create(&threadid, NULL, doit, (void*)request.out);
        doit(request.out);
    }
    return 0;
}

static void *doit(void *a)
{
    pthread_t tid = pthread_self();
    FCGX_Stream* out = (FCGX_Stream*)a;
    FCGX_FPrintF(out,
        "Content-type: text/html\r\n"
        "\r\n"
        "<title>FastCGI Hello! (multi-threaded C, fcgiapp library)</title>"
        "<h1>FastCGI Hello! (multi-threaded C, fcgiapp library)</h1>"
        "Thread %u <p>", tid);
}

#endif