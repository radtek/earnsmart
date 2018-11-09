/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEodDataTest.h
 * Author: santony
 *
 * Created on December 31, 2015, 2:08 PM
 */

#ifndef CEODDATATEST_H
#define CEODDATATEST_H

#include "utcommon.h"
#include "../../seglobal/IObject.h"
#include "../../semain/semain.h"
#include "../../seeoddata/seeoddata.h"
#include "CUnitTestBase.h"


class CEodDataTest : public CUnitTestBase
{
    CPPUNIT_TEST_SUITE(CEodDataTest);
    CPPUNIT_TEST(testEodDataExchanges);
    CPPUNIT_TEST_SUITE_END();
    
public:
    CEodDataTest();
    CEodDataTest(const CEodDataTest& orig);
    virtual ~CEodDataTest();
    
protected:

    void testEodDataExchanges();
    
};

#endif /* CEODDATATEST_H */

