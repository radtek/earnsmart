/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CExceptionTest.h
 * Author: santony
 *
 * Created on January 1, 2016, 1:58 PM
 */

#ifndef CEXCEPTIONTEST_H
#define CEXCEPTIONTEST_H

#include "utcommon.h"


class CExceptionTest : public TestFixture
{
    CPPUNIT_TEST_SUITE(CExceptionTest);
    CPPUNIT_TEST(testSeException);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CExceptionTest();
    CExceptionTest(const CExceptionTest& orig) = delete;
    virtual ~CExceptionTest();
    
private:

    void testSeException();
    
};

#endif /* CEXCEPTIONTEST_H */

