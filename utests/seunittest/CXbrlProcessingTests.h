/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CXbrlProcessingTests.h
 * Author: santony
 *
 * Created on November 29, 2017, 9:33 PM
 */

#ifndef CXBRLPROCESSINGTESTS_H
#define CXBRLPROCESSINGTESTS_H

#include "CUnitTestBase.h"

class CXbrlProcessingTests : public CUnitTestBase
{
    
    CPPUNIT_TEST_SUITE(CXbrlProcessingTests);
    CPPUNIT_TEST(testXbrlFileProcessing);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CXbrlProcessingTests();
    CXbrlProcessingTests(const CXbrlProcessingTests& other);
    virtual ~CXbrlProcessingTests();

    void setUp() override;
    void tearDown() override;


    void testXbrlFileProcessing();
    
private:

};

#endif /* CXBRLPROCESSINGTESTS_H */

