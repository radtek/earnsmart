/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on November 24, 2017, 10:51 AM
 */

#include <cstdlib>
#include "Common.h"
#include "SEApplication.h"

using namespace std;
using namespace Wt;

void myCrashHandler(int sig);

std::unique_ptr<WApplication> CreateApp(const WEnvironment& env);

/*
 * 
 */
int main(int argc, char** argv)
{

    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime %level : %msg");
    el::Helpers::setCrashHandler(myCrashHandler);

    return WRun(argc, argv, CreateApp);


}

std::unique_ptr<WApplication> CreateApp(const WEnvironment& env)
{
    auto app = std::make_unique<SEApplication>(env);
    const string* themePtr = env.getParameter("theme");
    string theme;
    if (!themePtr)
        theme = "bootstrap3";
    else
        theme = *themePtr;
    if (theme == "bootstrap3")
    {
        auto bootstrapTheme = make_shared<WBootstrapTheme>();
        bootstrapTheme->setVersion(BootstrapVersion::v3);
        bootstrapTheme->setResponsive(true);
        app->setTheme(bootstrapTheme);
        app->useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
    }
    else if (theme == "bootstrap2")
    {
        auto bootstrapTheme = std::make_shared<WBootstrapTheme>();
        bootstrapTheme->setResponsive(true);
        app->setTheme(bootstrapTheme);
    }
    else
        app->setTheme(std::make_shared<WCssTheme>(theme));

    app->messageResourceBundle().use(app->appRoot() + "htmldata");

    app->useStyleSheet("earnsmartdef.css");

    return app;
}

void myCrashHandler(int sig)
{
    BWRN << "Crashed situation is being handled by shutting timer off.";
    el::Helpers::crashAbort(sig);
}
