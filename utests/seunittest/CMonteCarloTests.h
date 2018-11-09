/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CMonteCarloTests.h
 * Author: santony
 *
 * Created on August 7, 2016, 9:27 PM
 */

#ifndef CMONTECARLOTESTS_H
#define CMONTECARLOTESTS_H

#include "utcommon.h"

class CMonteCarloTests : public TestFixture
{
    
    CPPUNIT_TEST_SUITE(CMonteCarloTests);
    CPPUNIT_TEST(testPlain);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CMonteCarloTests();
    CMonteCarloTests(const CMonteCarloTests& orig);
    virtual ~CMonteCarloTests();
    
    void testPlain();
    
    void setUp() override;
    void tearDown() override;


    
    
private:

    double g(double* k, size_t dim, void* params);
    void displayResults(char* title, double result, double error);
};

#endif /* CMONTECARLOTESTS_H */

