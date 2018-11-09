/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CUtilityTest.h
 * Author: santony
 *
 * Created on January 1, 2016, 4:27 PM
 */

#ifndef CUTILITYTEST_H
#define CUTILITYTEST_H

#include "utcommon.h"
#include "CUnitTestBase.h"

class CUtilityTest : public CUnitTestBase
{
    CPPUNIT_TEST_SUITE(CUtilityTest);
    //CPPUNIT_TEST(testNewRandomFunctions);
    CPPUNIT_TEST(testConfigItems);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CUtilityTest();
    CUtilityTest(const CUtilityTest& orig);
    virtual ~CUtilityTest();
    
    void testNewRandomFunctions();
    
    void VerityRolesTest();
    
    void testConfigItems();
    
private:

};

#endif /* CUTILITYTEST_H */

