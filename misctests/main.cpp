/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on November 14, 2012, 7:20 AM
 */

#include <cstdlib>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <iostream>

//#include "CXbrlCalculation.h";
#include <boost/filesystem.hpp>
#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "../sedata/sedataheaders.h"
#include "../seadmin/seadminapi.h"

#include <jsoncpp/json/json.h>

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

#include "../semain/semain.h"
#include "../semysql/CSEConnections.h"
#include "../seglobal/CDirectory.h"
#include "../senet/CFtpClient.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <map>
#include "../senet/senetapi.h"
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "../seglobal/CFunctionCache.h"
#include "CTimer.h"

#define SEDBCONN        CSEConnections::GetMain()

using namespace std;
using namespace se;
using namespace se::net;
using namespace boost::date_time;
using namespace boost::gregorian;
using namespace se::cache;

string text;
log4cxx::LoggerPtr logger;


void FunctionCacheTest(IApp* pApp);

void TestTimer();


/*
 * 
 */
int main(int argc, char** argv) 
{
    SP<se::IApp> pApp;
    SEGetApp(&pApp);
    
    SP<se::config::IConfigurator> pConfig;
    pApp->get_Configurator(&pConfig);
    pConfig->InitLog();
    pConfig->InitDb();    
    logger = log4cxx::Logger::getLogger("misctests::main");
    
    FunctionCacheTest(pApp);

//    time_t clk0 = time(0);
//    while(true)
//    {
//        time_t clk1 = time(0);
//
//        auto d = difftime(clk1, clk0);
//
//        cout << "Num of Secs: " << d << endl;
//        
//        sleep(1);
//    }
    
    while(true)
        sleep(120); // for two hours
    
    return 0;
}

void TestTimer()
{
    char key;
    CTimer timer(1000, 5000);
    timer.SetCallback([](CTimer const* timer, LPVOID state)
    {
        LOG4CXX_INFO(logger, "Timer callback is invoked.");
    });
    timer.StartTimer();
    cout << "Enter to exit" << endl;
    cin.ignore().get();
    timer.StopTimer();
}


void FunctionCacheTest(IApp* pApp)
{
    
    // get configurator two times to test smart pointers as return value.
    {
    SP<se::config::IConfigurator> pConfig;
    pApp->get_Configurator(&pConfig);
    
    auto str = pConfig->get_ExecutableDir();
    cout << str;
    }

    {
    SP<se::config::IConfigurator> pConfig;
    pApp->get_Configurator(&pConfig);
    auto str = pConfig->get_ExecutableDir();
    cout << str;
    }
    
    string retValue = "RETURNEDVALUE";
    if (!CFunctionCache::Instance().TryGetFValue((string)"test", 100, retValue))
    {
        CFunctionCache::Instance().SetFValue((string)"test", 100, retValue);
    }
    string retValue1 ;
    if (CFunctionCache::Instance().TryGetFValue((string)"test", 100, retValue1))
    {
        LOG4CXX_INFO(logger, "Found it.");
    }
    
    if (!CFunctionCache::Instance().TryGetFValue("test", (string)"Test", 100.00, retValue))
    {
        CFunctionCache::Instance().SetFValue("test", (string)"Test", 100.00, retValue);
    }
    if (CFunctionCache::Instance().TryGetFValue("test", (string)"Test", 100.00, retValue1))
    {
        LOG4CXX_INFO(logger, "Found it.");
    }
    
    if (!CFunctionCache::Instance().TryGetFValue("test", 100, 100.00, (string)"testp", retValue))
    {
        CFunctionCache::Instance().SetFValue("test", 100, 100.00,  (string)"testp", retValue);
    }
    if (CFunctionCache::Instance().TryGetFValue("test", 100, 100.00,  (string)"testp", retValue1))
    {
        LOG4CXX_INFO(logger, "Found it.");
    }
    
    R8 rrRetValue = 3e102, rrRetValue1;
    
    if (!CFunctionCache::Instance().TryGetFValue("test", 100, 100.00, false, (string)"testp", rrRetValue))
    {
        CFunctionCache::Instance().SetFValue("test", 100, 100.00, false, (string)"testp", rrRetValue);
    }
    if (CFunctionCache::Instance().TryGetFValue("test", 100, 100.00, false, (string)"testp", rrRetValue1))
    {
        LOG4CXX_INFO(logger, "Found it.");
    }

    if (!CFunctionCache::Instance().TryGetFValue("test",  100, 100.00, false, (string)"testp", 10, pApp))
    {
        CFunctionCache::Instance().SetFValue("test", 100, 100.00, false, (string)"testp", 10, pApp);
    }
    if (CFunctionCache::Instance().TryGetFValue("test", 100, 100.00, false, (string)"testp", 10, pApp))
    {
        LOG4CXX_INFO(logger, "Found it.");
    }
}

ErrorId EdgarFtpDirectoryDownloadTest()
{
        CFtpClient ftp;
    stringstream sList ;
    ftp.BrowseRemoteFolders("ftp://ftp.sec.gov/edgar/full-index/?", [&sList](CSTR data)
        {
        sList << data;
        }
    );
    
    if (sList.fail())
        return E_FtpError;

    string line;
    std::map< string, boost::gregorian::date > directories;
    while (getline(sList, line))
    {
        // capture the file time and directory name
        // this sample line looks like this.
        /*
        1            1416 	25	     38	  43	       56			
        drwxr-xr-x   6 1019     bin          4096 Aug  3 05:00 1993
        drwxr-xr-x   6 1019     bin          4096 Aug  3 05:00 1994
        drwxr-xr-x   6 1019     bin          4096 Aug  3 05:01 1995
        */
        string sFileTime = "2013 ";
        string sDirName;
        const char* szLine = line.c_str();
        for (int i=0; i!= line.size(); ++i)
        {
            if (szLine[0] != 'd')
                break;
            if (i>= 42 && i<55)
                sFileTime += szLine[i];
            if (i>= 55 )
                sDirName += szLine[i];
        }
        if (szLine[0] == 'd')
            directories.insert(make_pair(sDirName, from_string(sFileTime)));
    }
    
    for (auto p : directories)
        LOG4CXX_INFO(logger, "Dir: " << p.first << " - " << to_simple_string(p.second));
    

    return S_Success;
    
}
