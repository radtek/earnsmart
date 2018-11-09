/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on September 5, 2012, 10:29 AM
 */

#include <cstdlib>
#include <glib.h>


#include <boost/filesystem.hpp>
#include <string>
#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "CEventsManager.h"

using namespace std;

// logger initialization
log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("se.bg.main"));


#define gpIdleFuncId = GINT_TO_POINTER(0x1) 
const int MSG_SIZE = 4096;
GMainContext* context;
GMainLoop* mainLoop;
CEventsManager eventsMgr;

void SignalHandler(int sigNo);

/*
GSource* mainSource;
gboolean OnIdle(gpointer userdata);
gboolean OnCallback(gpointer userdata);
gboolean OnPrepare(GSource* src, gint* timeout);
gboolean OnCheck(GSource* src);
gboolean OnDispatch(GSource* source, GSourceFunc callback, gpointer userData);
*/

GSource* timeoutSource ;
gboolean OnTimeout(gpointer data);

/*
 * 
 */
int main(int argc, char** argv) 
{
#ifdef __DEBUG
    std::string log4config = "../log4cxx.conf";
    bool fileCheck = boost::filesystem::exists(log4config);
    if (!fileCheck)
        log4config = "./log4cxx.conf";
#else
    std::string log4config = "/usr/local/lib/smartearn/log4cxx.conf";
#endif
    log4cxx::PropertyConfigurator::configure(log4config);
    LOG4CXX_INFO(logger, "Entering sebgps.");
    
    if (signal(SIGINT, SignalHandler) == SIG_ERR)
        LOG4CXX_ERROR(logger, "Can't catch signals.");
    
    context = g_main_context_new();
    mainLoop = g_main_loop_new(context, TRUE);
    
    timeoutSource = g_timeout_source_new(500);
    auto timeoutId = g_source_attach(timeoutSource, context);
    g_source_set_callback(timeoutSource, OnTimeout, mainLoop, 0);
    
    /* Following for custom event source */
    /*
    GSourceFuncs sourceFuncs = 
        {
            OnPrepare,
            OnCheck,
            OnDispatch,
            0
        };
    mainSource = g_source_new(&sourceFuncs, sizeof(GSource));
    auto mainid = g_source_attach(mainSource, context);
    g_source_set_callback(mainSource, OnCallback, mainLoop, 0);
    */
    g_main_loop_run(mainLoop);
    g_main_loop_ref(mainLoop);
    g_main_context_unref(context);

    LOG4CXX_INFO(logger, "Exiting sebgps.")
    return 0;
}

gboolean OnIdle(gpointer userdata)
{
    return true;
}

void SignalHandler(int sigNo)
{
    cout << "Signal received: " << sigNo;
    if (sigNo == SIGKILL || sigNo == SIGTERM)
    {
        LOG4CXX_INFO(logger, "Process is terminated");
        eventsMgr.Shutdown();
    }
}

/*

gboolean OnCallback(gpointer userdata)
{
    //LOG4CXX_INFO(logger, "Callback is invoked " << ++counter << "times.");
    
    return TRUE;
}

gboolean OnPrepare(GSource* src, gint* timeout)
{
    *timeout = 1000;
    return TRUE;
}

gboolean OnCheck(GSource* src)
{
    return TRUE;
}

gboolean OnDispatch(GSource* source, GSourceFunc callback, gpointer userData)
{
    if (callback != 0)
        return callback(userData);
    return FALSE;
}

*/

gboolean OnTimeout(gpointer data)
{
    if (!eventsMgr.Dispatch())
    {
        g_main_loop_quit(mainLoop);
        return FALSE;
    }
    return TRUE;
}