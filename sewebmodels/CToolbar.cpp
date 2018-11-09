/* 
 * File:   CToolbar.cpp
 * Author: santony
 * 
 * Created on July 3, 2015, 9:08 PM
 */

#include <fstream>

#include "common.h"
#include "CToolbar.h"


/*
 * Menus are defined in json in the below structure
 * {
 *      Menus : [
 *          { Name : "Home", Url : "/" , Secured : false },
 *          { 
 *              Name : "Investment Products", 
 *              Url : "/investmentproducts" , 
 *              Secured : false, 
 *              Children : [ ..... ] 
 *          },
 *          { Name : "News", Url : "/News" , Secured : false },
 *          { Name : "About Us", Url : "/aboutus" , Secured : false },
 *      ]
 * }
 */

bool CToolbar::IsLoaded = false;

CToolbar::CToolbar() : CModelBase("ToolbarView")
{
}

CToolbar::~CToolbar() 
{
    
}

void CToolbar::InitMenus() 
{
    if (!IsLoaded)
    {
        Json::Reader reader;
        ifstream fs;
        SP<IApp> papp;
        THROWIFFAILED( SEOpenApp(&papp), "Failed to open app." );
        string exePath = papp->get_Configurator()->get_ExecutableDir();
        fs.open(exePath +  "/txt/menus.json");
        if (!fs.good())
            throw seexception("Menus couldn't be loaded (file not found).");
        if (!reader.parse(fs, RootMenu, false))
            throw seexception("Menus couldn't be loaded (parse error).");
        IsLoaded = true;
    }
    
}


void CToolbar::serialize(archive& a) 
{
    CModelBase::serialize(a);
    a & IsLoaded ;
}


