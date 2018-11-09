/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CSecurityAPITest.h
 * Author: santony
 *
 * Created on September 17, 2016, 11:25 AM
 */

#ifndef CSECURITYAPITEST_H
#define CSECURITYAPITEST_H

#include "CUnitTestBase.h"


class CSecurityAPITest : public CUnitTestBase
{
    CPPUNIT_TEST_SUITE(CSecurityAPITest);
    CPPUNIT_TEST(testgetSecurity);
    //CPPUNIT_TEST(testlistExchangeSecurities);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CSecurityAPITest();
    CSecurityAPITest(const CSecurityAPITest& orig);
    virtual ~CSecurityAPITest();
    
    void setUp() override;
    void tearDown() override;

    void testgetSecurity();
    void testlistExchangeSecurities();

    
private:
    

};

#endif /* CSECURITYAPITEST_H */

