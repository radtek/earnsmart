/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CExceptionTest.cpp
 * Author: santony
 * 
 * Created on January 1, 2016, 1:58 PM
 */

#include "CExceptionTest.h"
#include "../../seglobal/exceptions.h"

using namespace se;

CExceptionTest::CExceptionTest() { }


CExceptionTest::~CExceptionTest() { }

void CExceptionTest::testSeException()
{
    ostringstream os ;
    os << "There is an unknown error with code " << 10 << " which resulted it an error";
    
    seexception ex(E_UnknownError);
    ex << "There is an unknown error with code " << 10 << " which resulted it an error";
    
    CPPUNIT_ASSERT(os.str() == ex.what() );
    
}