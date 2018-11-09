/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHelpDocTreeTest.h
 * Author: santony
 *
 * Created on May 2, 2016, 6:31 AM
 */

#ifndef CHELPDOCTREETESTER_H
#define CHELPDOCTREETESTER_H

#include "utcommon.h"

class CHelpDocTreeTest : public TestFixture
{
    CPPUNIT_TEST_SUITE(CHelpDocTreeTest);
    CPPUNIT_TEST(testGetRootDoc);
    //CPPUNIT_TEST(testSaveDocItem);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CHelpDocTreeTest();
    CHelpDocTreeTest(const CHelpDocTreeTest& orig);
    virtual ~CHelpDocTreeTest();

    void setUp() override;
    void tearDown() override;
    
    void testGetRootDoc();
    
    void testSaveDocItem();
    
private:
    SP<IAppEx> pApp;
    SP<ISessionInterface> pSession ;
};

#endif /* CHELPDOCTREETESTER_H */

