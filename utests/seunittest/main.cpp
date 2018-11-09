/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on December 30, 2015, 5:59 PM
 */

#include <cstdlib>

#include <cppunit/ui/text/TestRunner.h>
#include "CCryptoTest.h"
#include "CEodDataTest.h"
#include "utcommon.h"
#include "CExceptionTest.h"
#include "CUtilityTest.h"
#include "CAdminAPITest.h"
#include "CEmailTemplateTest.h"
#include "CHelpDocTreeTest.h"
#include "CSqlAstTest.h"
#include "CMonteCarloTests.h"
#include "CPredictionsTest.h"
#include "CSecurityAPITest.h"
#include "CXbrlProcessingTests.h"


using namespace std;
using namespace se;
using namespace se::eoddata;

//CPPUNIT_TEST_SUITE_REGISTRATION(CAdminAPITest);
//CPPUNIT_TEST_SUITE_REGISTRATION(CUtilityTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(CEodDataTest); 
//CPPUNIT_TEST_SUITE_REGISTRATION(CCryptoTest); 
//CPPUNIT_TEST_SUITE_REGISTRATION(CExceptionTest); 
//CPPUNIT_TEST_SUITE_REGISTRATION(CEmailTemplateTest); 
//CPPUNIT_TEST_SUITE_REGISTRATION(CHelpDocTreeTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(CSqlAstTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(CMonteCarloTests);
//CPPUNIT_TEST_SUITE_REGISTRATION(CPredictionsTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(CSecurityAPITest);
CPPUNIT_TEST_SUITE_REGISTRATION(CXbrlProcessingTests);
/*
 * 
 */
int main(int argc, char** argv) 
{
    
    //IMPORTANT NOTE: Set the working directory to Linux/phase3/web folder 
    // in netbeans debug or run environment, or run this app in that folder.
    
    CppUnit::TextUi::TestRunner runner;
    TestFactoryRegistry& registry = TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());
    auto wasSuccesful = runner.run("", false);
    return wasSuccesful;
}

