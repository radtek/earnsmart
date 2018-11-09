/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CSqlAstTest.h
 * Author: santony
 *
 * Created on May 26, 2016, 6:34 AM
 */

#ifndef CSQLASTTEST_H
#define CSQLASTTEST_H

#include "utcommon.h"

class CSqlAstTest : public TestFixture
{
    
    CPPUNIT_TEST_SUITE(CSqlAstTest);
    CPPUNIT_TEST(testBuildWhereClause);
    CPPUNIT_TEST(testRealWhereClause);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CSqlAstTest();
    CSqlAstTest(const CSqlAstTest& orig);
    virtual ~CSqlAstTest();
    
    void testBuildWhereClause();
    void testRealWhereClause();
    
private:

};

#endif /* CSQLASTTEST_H */

