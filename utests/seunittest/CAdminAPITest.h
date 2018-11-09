/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CAdminAPITest.h
 * Author: santony
 *
 * Created on January 1, 2016, 5:24 PM
 */

#ifndef CADMINAPITEST_H
#define CADMINAPITEST_H

#include "utcommon.h"
#include "../../seadmin/seadminapi.h"
#include "../../semain/semain.h"
#include "CUnitTestBase.h"

using namespace se;
using namespace se::admin;

class CAdminAPITest : public CUnitTestBase
{
    CPPUNIT_TEST_SUITE(CAdminAPITest);
    //CPPUNIT_TEST(testAnonymousRegistration);
    //CPPUNIT_TEST(testLogin);
    //CPPUNIT_TEST(testLicensedUserRegistrationCycle);
    //CPPUNIT_TEST(testQueryStates);
    CPPUNIT_TEST(testStandardUserRegistration);
    //CPPUNIT_TEST(testInternalUserRegistration);
    //CPPUNIT_TEST(testPositions);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CAdminAPITest();
    CAdminAPITest(const CAdminAPITest& orig);
    virtual ~CAdminAPITest();
    
    void testAnonymousRegistration();
    
    void testStandardUserRegistration();
    
    void testLicensedUserRegistrationCycle();
    
    void testInternalUserRegistration();
    
    void testLogin();
    
    //////////// Global api tests ////////////
    void testQueryStates();
    
    ////////////////// Portfolios and Positions
    void testPositions();
    

private:
    SP<IAppEx> pApp;

};

#endif /* CADMINAPITEST_H */

