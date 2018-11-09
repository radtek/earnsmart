/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEmailTemplateTest.h
 * Author: santony
 *
 * Created on March 26, 2016, 5:40 PM
 */

#ifndef CEMAILTEMPLATETEST_H
#define CEMAILTEMPLATETEST_H

#include "utcommon.h"

class CEmailTemplateTest : public TestFixture 
{
    CPPUNIT_TEST_SUITE(CEmailTemplateTest);
    CPPUNIT_TEST(testApply);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CEmailTemplateTest();
    CEmailTemplateTest(const CEmailTemplateTest& orig);
    virtual ~CEmailTemplateTest();
    
    void testApply();
    
private:

};

#endif /* CEMAILTEMPLATETEST_H */

