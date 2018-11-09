/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "../semain/semain.h"
#include "seappmodule.h"
#include "../seglobal/CDirectory.h"
#include "../seglobal/globallogger.h"
using namespace se;

SP<se::IApp> pApp;

int on_module_startup()
{
    const string& path = ::CDirectory::GetCurDir();
    if (ISOK(SEGetApp(path.c_str(), &pApp)))
    {
        BINF << "Started se application instance.";
        return NXWEB_OK;
    }
    else
    {   
        BERR << "Failed to open SE application.";
        return NXWEB_ERROR;
    }
}

void on_module_shutdown() 
{
    if (pApp)
    {
        BINF << "Shutting down the se application instance.";
        pApp.Clear();
    }
}

void on_module_config(const nx_json* js)
{
    
}
