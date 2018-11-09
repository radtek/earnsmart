/*
 * File:   main.cpp
 * Author: santony
 *
 * Created on September 5, 2012, 10:29 AM
 */

#include <cstdlib>
#include "../seglobal/segvcatch.h"
#include <zmq.h>
#include <boost/filesystem.hpp>
#include <string>
#include "CEventsManager.h"
#include "MainController.h"
#include <unistd.h>
#include <sys/dir.h>
#include "../seglobaltypedefs.h"
#include "../ReturnCodes.h"
#include "CFunctionHandler.h"
#include <getopt.h>
#include "../semysql/CSEConnections.h"
#include "../semain/semain.h"
#include <vector>
#include <jsoncpp/json/json.h>
#include <zthread/Thread.h>
#include "../seglobal/CFile.h"
#include "CProcessStatements.h"
#include "CMapAllSymbolsToCIK.h"
#include "CParseFormIndexFunctionJson.h"
#include "CPriceHistoryProcessor.h"
#include "CSecurityPriceProcessor.h"
#include <map>
#include <zthread/Runnable.h>
#include <zthread/PoolExecutor.h>
#include <zthread/Mutex.h>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <zthread/Guard.h>
#include "../seglobal/globallogger.h"
#include "CRepeatedDBAction.h"
#include "CProcessEodData.h"
#include "../seglobal/CTimer.h"


using namespace std;
using namespace se;
using namespace se::data;
using namespace se::config;

using namespace Json;
using namespace ZThread;

using namespace boost::date_time;

#define SLEEPTIME 1000

CTimer timer(1, 5);

//void myCrashHandler(int sig) 
//{
//    BWRN << "Crashed situation is being handled by shutting timer off.";
//    timer.StopTimer();
//    el::Helpers::crashAbort(sig);
//}

/*
 * Command arguments:
 * a. -z            : run as a zmq server
 * b. -f funcname   : run a function from the console.
 * Both -z and -f are mutually exclusive, ie., if -zf specified, -f ignored and -z is considered.
 * 
 * If either -z or -p is specified, then program exit with exit code E_InvalidArg.
 */

// tested

void configureLogger()
{
//    el::Configurations defaultConf;
//    defaultConf.setToDefault();
//    // Values are always std::string
//    defaultConf.set(el::Level::Info, el::ConfigurationType::Format, "%datetime %level %msg");
//    
//    // default logger uses default configurations
//    el::Loggers::reconfigureLogger("default", defaultConf);
//    LOG(INFO) << "Log using default file";
//    // To set GLOBAL configurations you may use
//    defaultConf.setGlobally(
//            el::ConfigurationType::Format, "%date %msg");
//    el::Loggers::reconfigureLogger("default", defaultConf);

//    // Load configuration from file
//    el::Configurations conf("easylogging.conf");
//    // Reconfigure single logger
//    el::Loggers::reconfigureLogger("default", conf);
//    // Actually reconfigure all loggers instead
//    el::Loggers::reconfigureAllLoggers(conf);
//    // Now all the loggers will use configuration from file
    
    el::Loggers::configureFromGlobal("easylogging.conf");
}

int main(int argc, char** argv)
{
    configureLogger();

    //el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime %level : %msg");
    //el::Helpers::setCrashHandler(myCrashHandler);

    ErrorId retValue = E_InvalidArg;
    SP<se::IApp> pApp;
    string progname = argv[0];
    SEGetApp(progname.c_str(), &pApp);

    SP<se::config::IConfigurator> pConfig;
    pApp->get_Configurator(&pConfig);

    SP<ISession> pSession;
    if (FAILED(pApp->CreateSession("admin", "963051", &pSession))) {
        BERR << "Session creation failed. Check you user id and password.";
        return 1;
    }
    auto configFile = pConfig->get_ConfigFilePath();
    ifstream filestrm;
    filestrm.open(configFile);
    if (!filestrm.good()) {
        BERR << "Configuration file sebgpd.config.json doesn't exist in executable directory.";
        return E_FileNotFound;
    }

    //LOG4CXX_INFO(logger, "Parsing configuration file.");

    // read configuration info from json file.
    Value configRoot;
    Reader jsonreader;
    if (!jsonreader.parse(filestrm, configRoot)) {
        BERR << "CoProcessSecDailyPricenfiguration file sebgpd.config.json is invalid and cannot be parsed as a json file.";
        return E_ReadError;
    }

    typedef multimap<string, Runnable*> Functions;
    Functions funcs;

    //using namespace boost::date_time;
    //auto time_duration<class T,typename rep_type> parse_undelimited_time_duration()
    if (!configRoot.isMember("sebgpd"))
        return E_ConfigurationError;

    configRoot = configRoot["sebgpd"];
    // get a list of functions
    const Value& functions = configRoot["functions"];
    for (int index = 0; index != functions.size(); ++index) {
        // populate individual functions into a map.
        const Value& v = functions[index];
        if (v["active"] == "1") {
            Interval inv;
            string szFuncName = v["name"].asString();
            string params = v["params"].asString();
            const Value& interval = v["interval"];
            if (interval["freq"].asString() == "daily")
                inv.Frequency = FREQ_Daily;
            else if (interval["freq"].asString() == "continuous")
                inv.Frequency = FREQ_Continuous;
            else if (interval["freq"].asString() == "weekly")
                inv.Frequency = FREQ_Weekly;
            else if (interval["freq"].asString() == "monthly")
                inv.Frequency = FREQ_Monthly;
            else if (interval["freq"].asString() == "once")
                inv.Frequency = FREQ_Once;
            else if (interval["freq"].asString() == "static")
                inv.Frequency == FREQ_Static;
            else {
                BERR << "Configuration file sebgpd.config.json specifies an unknown 'freq' value.";
                return E_ConfigurationError;
            }

            inv.Factor = interval["factor"].asString();
            int instances = v.get("instances", 1).asInt();
            Runnable* pFunc = 0;
            for (int i = 0; i != instances; ++i) {
                if (szFuncName == "processstatements")
                    pFunc = new CProcessStatements(pSession);
                else if (szFuncName == "eoddataquotes")
                    pFunc = new CProcessEodDataQuotes(pSession, params);
                else if (szFuncName == "eoddatahistory")
                    pFunc = new CProcessEodDataHistory(pSession, params);
                else if (szFuncName == "mapsymboltocik")
                    pFunc = new CMapAllSymbolsToCIK(pSession);
                else if (szFuncName == "parseformindex")
                    pFunc = new CParseFormIndexFunction(pSession, params.c_str());
                else if (szFuncName == "pricehistory")
                    pFunc = new CSecurityPriceProcessor(pSession, atoi(params.c_str()));
                else if (szFuncName == "repeateddbaction")
                    pFunc = new CRepeatedDBAction(pSession, params);
                if (pFunc) 
                {
                    dynamic_cast<CTimedFunction*> (pFunc)->SetInterval(v);
                    funcs.insert(make_pair(szFuncName, pFunc));
                }
            }
        }
    }

    BINF << "Completed Parsing configuration file.";

    BINF << "Setting up thread pool executor.";

    if (!funcs.size()) {
        BWRN << "No function is set to active. Check configuration file.";
        return 0;
    }

    // create pool executor
    PoolExecutor threadpool(funcs.size());

    class tricktask : public Runnable
    {
        Runnable* _pother;
    public:

        tricktask(Runnable* pother) : _pother(pother)
        {
        }

        void run()
        {
            _pother->run();
        }
    };

    BINF << "Loading One time tasks.";

    for (auto kv : funcs) {
        CTimedFunction* f = dynamic_cast<CTimedFunction*> (kv.second);
        if (f->GetFreq() == FREQ_Once) {
            Task task(new tricktask(kv.second));
            threadpool.execute(task);
        }
        else if (f->GetFreq() == FREQ_Static) {
            kv.second->run(); // assuming that run method is not blocking....
        }
    }

    BINF << "Loading other tasks.";

    //Mutex lock;



    auto timerFunc = [&funcs, &threadpool](CTimer const* timer, LPVOID state) -> void
    {
        // enumerate push the map entries into a thread-pool
        // we will have timer check look.
        // if function is busy, don't requeue
        // else push again.
        for (auto kv : funcs) 
        {
            BINF << "Attempting to run task functions .... ";
            CTimedFunction* f = dynamic_cast<CTimedFunction*> (kv.second);
            if (f != nullptr && f->GetFreq() != FREQ_Once) 
            {
                if (!f->IsBusy()) 
                {
                    BINF << "Function is not busy. Starting it .... ";
                    f->SetBusy();
                    //Guard<Mutex> g(lock);
                    if (f->HasExpired()) 
                    {
                        Task task(new tricktask(kv.second));
                        threadpool.execute(task);
                    }
                    else
                    {
                        f->UnsetBusy();
                    }
                }
                else
                {
                    BINF << "Function is busy...";
                }
            }
        }
    };

    timer.SetCallback(timerFunc);
    timer.StartTimer();


    while(true)
    {
        sleep(SLEEPTIME);
    }
    

    timer.StopTimer();
    
    BINF << "Exiting sebgpd.";

    return retValue;
}
