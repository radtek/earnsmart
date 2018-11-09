/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CUnitTestBase.cpp
 * Author: santony
 * 
 * Created on March 27, 2016, 1:51 PM
 */

#include "CUnitTestBase.h"
#include "../../seglobal/CDirectory.h"

CUnitTestBase::CUnitTestBase() 
{
}

CUnitTestBase::CUnitTestBase(const CUnitTestBase& orig) 
{
    
}

CUnitTestBase::~CUnitTestBase() 
{
    
}

void CUnitTestBase::setUp() 
{
    SP<IApp> p;
    CSTR curDir = CDirectory::GetCurDir().c_str();
    auto ret = SEGetApp(curDir, &p);
    CPPUNIT_ASSERT(ret == S_OK);
    pApp = (IAppEx*)p._ptr;
    CPPUNIT_ASSERT(pApp != nullptr);
}

RP<ISessionInterface> CUnitTestBase::NonInteractiveAnonLogin() 
{
    pSession = pApp->CreateAnonymousSession();
    CPPUNIT_ASSERT(pSession != nullptr);
    CPPUNIT_ASSERT(pSession->get_UniqueId() != nullptr);
    CPPUNIT_ASSERT(pSession->IsUserHasRole(UserRole::Anonymous));
    return pSession;
}


RP<ISessionInterface> CUnitTestBase::NonInteractiveUserLogin(CSTR userId, CSTR pwd) 
{
    pSession = pApp->CreateSession(userId, pwd);
    CPPUNIT_ASSERT(pSession != nullptr);
    CPPUNIT_ASSERT(pSession->get_UniqueId() != nullptr);
    CPPUNIT_ASSERT(pSession->IsUserRoleHigherThan(UserRole::Anonymous));
    return pSession;
}


void CUnitTestBase::tearDown() 
{
    //pSession->Logout();
}

