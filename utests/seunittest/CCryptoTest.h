/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CCryptoTest.h
 * Author: santony
 *
 * Created on December 31, 2015, 2:07 PM
 */

#ifndef CCRYPTOTEST_H
#define CCRYPTOTEST_H

#include "utcommon.h"

using namespace CppUnit;

class CCryptoTest : public TestFixture 
{
    
    CPPUNIT_TEST_SUITE(CCryptoTest);
    CPPUNIT_TEST(testEncryption);
    CPPUNIT_TEST_SUITE_END();
    
protected:

    void testEncryption();
    
public:
    CCryptoTest();
    CCryptoTest(const CCryptoTest& orig);
    virtual ~CCryptoTest();
    void setUp() override;
    void tearDown() override;
    
private:

};

#endif /* CCRYPTOTEST_H */

