/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPredictionsTest.h
 * Author: santony
 *
 * Created on September 3, 2016, 11:22 AM
 */

#ifndef CPREDICTIONSTEST_H
#define CPREDICTIONSTEST_H

#include "utcommon.h"
#include "CUnitTestBase.h"

class CPredictionsTest : public CUnitTestBase
{
    CPPUNIT_TEST_SUITE(CPredictionsTest);
    //CPPUNIT_TEST(testPredictUsingMVLR);
    CPPUNIT_TEST(testConvertGDPData);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CPredictionsTest();
    CPredictionsTest(const CPredictionsTest& orig);
    virtual ~CPredictionsTest();
    
    void testPredictUsingMVLR();
    void testConvertGDPData(); // converts gdp data from files into a json file.
    
private:

};

#endif /* CPREDICTIONSTEST_H */

