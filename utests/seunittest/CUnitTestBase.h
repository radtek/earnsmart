/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CUnitTestBase.h
 * Author: santony
 *
 * Created on March 27, 2016, 1:51 PM
 */

#ifndef CUNITTESTBASE_H
#define CUNITTESTBASE_H

#include "utcommon.h"
#include "../../semain/semain.h"

using namespace se;

class CUnitTestBase : public TestFixture 
{
protected:
    SP<IAppEx> pApp;
    RP<ISessionInterface> pSession;         
    
protected:
    virtual RP<ISessionInterface> NonInteractiveAnonLogin();
    virtual RP<ISessionInterface> NonInteractiveUserLogin(CSTR userId, CSTR pwd);
    
public:
    CUnitTestBase();
    CUnitTestBase(const CUnitTestBase& orig);
    virtual ~CUnitTestBase();
    
    void setUp() override;
    void tearDown() override;
    
private:

};

#endif /* CUNITTESTBASE_H */

