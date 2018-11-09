/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CXbrlProcessingTests.cpp
 * Author: santony
 * 
 * Created on November 29, 2017, 9:33 PM
 */

#include "CXbrlProcessingTests.h"
#include "../../sexbrl/sexbrlapi.h"

using namespace se::xbrl;

CXbrlProcessingTests::CXbrlProcessingTests() 
{

}

CXbrlProcessingTests::CXbrlProcessingTests(const CXbrlProcessingTests& other) :
CUnitTestBase(other)
{

}


CXbrlProcessingTests::~CXbrlProcessingTests() 
{
}

void CXbrlProcessingTests::setUp() 
{
    CUnitTestBase::setUp();
    NonInteractiveUserLogin("admin", "963051");
}

void CXbrlProcessingTests::tearDown() 
{
    CUnitTestBase::tearDown();
}

void CXbrlProcessingTests::testXbrlFileProcessing() 
{
    auto xbrlApi = pSession->get_XbrlApi();
    CPPUNIT_ASSERT(xbrlApi->ProcessStatement(277043) == S_OK);
}

