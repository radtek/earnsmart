/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHelpDocTreeTest.cpp
 * Author: santony
 * 
 * Created on May 2, 2016, 6:31 AM
 */

#include "CHelpDocTreeTest.h"
#include "../../seglobal/CDirectory.h"

using namespace se::admin;

CHelpDocTreeTest::CHelpDocTreeTest()
{
}

CHelpDocTreeTest::CHelpDocTreeTest(const CHelpDocTreeTest& orig)
{
}

CHelpDocTreeTest::~CHelpDocTreeTest()
{
}

void CHelpDocTreeTest::setUp()
{
    SP<IApp> p;
    auto ret = SEGetApp(CDirectory::GetCurDir().c_str(), &p);
    CPPUNIT_ASSERT(ret == S_OK);
    pApp = (IAppEx*)p._ptr;
    CPPUNIT_ASSERT(pApp != nullptr);
    pSession = pApp->CreateSession("admin", "963051");
    CPPUNIT_ASSERT(pSession);
}

void CHelpDocTreeTest::tearDown()
{

}



void CHelpDocTreeTest::testGetRootDoc()
{
    RP<IHelpDocTree> pHelpDoc = pSession->get_AdminApi()->get_HelpDocTree();
    CPPUNIT_ASSERT(pHelpDoc);
    Json::Value doc;
    auto retCode = pHelpDoc->get_RootDoc(doc);
    CPPUNIT_ASSERT(retCode == S_OK);
    CPPUNIT_ASSERT(doc != Json::nullValue);
    BINF << doc["docid"] << doc["doctitle"] << doc["docdesc"] << doc["parentdocid"];
    for(const Json::Value& v : doc["subjects"])
    {
        BINF << v["docid"] << v["doctitle"] << v["docdesc"] << v["parentdocid"];
    }
}

void CHelpDocTreeTest::testSaveDocItem()
{
    RP<IHelpDocTree> pHelpDoc = pSession->get_AdminApi()->get_HelpDocTree();
    CPPUNIT_ASSERT(pHelpDoc);
    Json::Value doc;
    doc["docid"] = 1;
    doc["doctitle"] = "Economics";
    doc["docdesc"] = "Economics - Micro and Macro economics";
    auto retCode = pHelpDoc->set_Doc(doc);
    CPPUNIT_ASSERT(retCode == S_OK);
}
