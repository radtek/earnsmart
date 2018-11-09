/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CUtilityTest.cpp
 * Author: santony
 * 
 * Created on January 1, 2016, 4:27 PM
 */

#include "CUtilityTest.h"

#include <random>

CUtilityTest::CUtilityTest() 
{
    setUp();
}

CUtilityTest::CUtilityTest(const CUtilityTest& orig) { }

CUtilityTest::~CUtilityTest() { }

void CUtilityTest::testNewRandomFunctions() 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000000000, 10000000000);
 
    for (U64 n=0; n<10; ++n)
        std::cout << dis(gen) << ' ';
    std::cout << '\n';    

    CPPUNIT_ASSERT(true);
}


void VerityRolesTest()
{
    BINF << "Anonymous Role: " << (U32)se::UserRole::Anonymous;
    BINF << "Standard Role: " << (U32)se::UserRole::Standard;
    BINF << "Licensed Role: " << (U32)se::UserRole::Licensed;
    BINF << "Internal Role: " << (U32)se::UserRole::Internal;
    BINF << "Admin Role: " << (U32)se::UserRole::Admin;
    CPPUNIT_ASSERT(true);
}

void CUtilityTest::testConfigItems() 
{
    using namespace se::config;
    auto pSsn = this->NonInteractiveAnonLogin();
    CPPUNIT_ASSERT(pSsn != nullptr);
    // save a config
    auto pcfg = pApp->get_Configurator();
    SP<IConfigItem> pItem ;
    IConfigItem::Create(pSsn, &pItem);
    CPPUNIT_ASSERT(pItem != nullptr);
    pItem->set_Name("IndexFileProcessingCurrentYear");
    pItem->set_Value(2017);
    auto ret = pcfg->set_Settings(USAREGIONID, "SECSTATEMENTS", pItem);
    CPPUNIT_ASSERT(ret == S_OK);
    pItem->set_Name("IndexFileProcessingCurrentQtr");
    pItem->set_Value(4);
    ret = pcfg->set_Settings(USAREGIONID, "SECSTATEMENTS", pItem);
    CPPUNIT_ASSERT(ret == S_OK);
}

