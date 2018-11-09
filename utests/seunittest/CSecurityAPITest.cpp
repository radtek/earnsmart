/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CSecurityAPITest.cpp
 * Author: santony
 * 
 * Created on September 17, 2016, 11:25 AM
 */

#include "CSecurityAPITest.h"

CSecurityAPITest::CSecurityAPITest() {
}

CSecurityAPITest::CSecurityAPITest(const CSecurityAPITest& orig) {
}

CSecurityAPITest::~CSecurityAPITest() {
}

void CSecurityAPITest::setUp() 
{
    CUnitTestBase::setUp();
    NonInteractiveUserLogin("admin", "963051");
}

void CSecurityAPITest::tearDown() 
{
    CUnitTestBase::tearDown();
}

void CSecurityAPITest::testgetSecurity() 
{
    RP<ISecurityAPI> psapi = pSession->get_SecurityApi();
    CPPUNIT_ASSERT(psapi);;
    Json::Value security ;
    Json::Value secDesc ;
    secDesc["Exch"] = "AMEX";
    secDesc["Sym"] = "TIS";
    secDesc["Include"]["Fundamentals"] = false;
    secDesc["Include"]["Technicals"] = true;
    SE_CODE ret = psapi->get_Security(secDesc, security);
    CPPUNIT_ASSERT(ret == S_OK);
    cout << security;
}

void CSecurityAPITest::testlistExchangeSecurities() 
{
    RP<ISecurityAPI> psapi = pSession->get_SecurityApi();
    CPPUNIT_ASSERT(psapi);;
    Json::Value p;
    p["Exch"] = "AMEX";
    p["Include"]["Fundamentals"] = false;
    p["Include"]["Technicals"] = true;
    Json::Value output;
    auto ret = psapi->get_ExchangeSecurities(p, output);
    CPPUNIT_ASSERT(ISOK(ret));
    for (auto& j : output["Securities"])
    {
        cout << j;
    }
}
