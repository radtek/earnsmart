/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEmailTemplateTest.cpp
 * Author: santony
 * 
 * Created on March 26, 2016, 5:40 PM
 */

#include "CEmailTemplateTest.h"
#include "../../senet/senetapi.h"
#include "../../senet/CEmailTemplate.h"
#include <fstream>
#include <map>
#include <regex>
#include "../../semain/semain.h"
#include "../../seglobal/CDirectory.h"
#include "../../seglobal/CFile.h"


using namespace std;

CEmailTemplateTest::CEmailTemplateTest() 
{
}

CEmailTemplateTest::CEmailTemplateTest(const CEmailTemplateTest& orig) {
}

CEmailTemplateTest::~CEmailTemplateTest() {
}

void CEmailTemplateTest::testApply() 
{
    SP<IApp> p;
    auto ret = SEGetApp(CDirectory::GetCurDir().c_str(), &p);
    CPPUNIT_ASSERT(ret == S_OK);
    string emailFileIn = (string)p->get_Configurator()->get_ExecutableDir() + "/emailtemplates/resetpassword.html";
    string emailFileOut;
    se::net::TemplateVaribles vars;
    vars.insert(make_pair("emailAddress", "sajiantony@hotmail.com"));
    vars.insert(make_pair("TempPassword", "xixixixix"));
    vars.insert(make_pair("signInUrl", "https://u-workhorse:8081/#/signon"));

    RP<ISessionInterface> pSession = ((IAppEx*)p._ptr)->OpenSession("");
    
    se::net::CEmailTemplate tmpl( pSession , emailFileIn, vars);
    try
    {
        emailFileOut = tmpl.Apply();
        CPPUNIT_ASSERT(emailFileOut == emailFileIn + "." + pSession->get_UniqueId() );
    
        tmpl.Close();
        CPPUNIT_ASSERT( !CFile::Exists(emailFileOut.c_str()) );
    
    }
    catch(const std::exception& ex)
    {
        CPPUNIT_ASSERT_MESSAGE(ex.what(), false);
    }
    
}


