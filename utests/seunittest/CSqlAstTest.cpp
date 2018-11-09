/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CSqlAstTest.cpp
 * Author: santony
 * 
 * Created on May 26, 2016, 6:34 AM
 */

#include "CSqlAstTest.h"
#include "../../seglobal/CAst.h"
#include "../../sedata/QueryFields.h"

using namespace se;
using namespace se::data;

CSqlAstTest::CSqlAstTest() 

{
    
}

CSqlAstTest::CSqlAstTest(const CSqlAstTest& orig) 
{
    
}

CSqlAstTest::~CSqlAstTest() 
{
    
}

void CSqlAstTest::testBuildWhereClause() 
{
    CSqlAst ast = AST_CONDITION(0, AST_OPERATOR::Equal, "Joe");
    ast & AST_CONDITION(1, AST_OPERATOR::Equal, "Hola");
    ast | AST_CONDITION(2, AST_OPERATOR::Equal, "HELLO");
    ast | AST_CONDITION("CountryCode", AST_OPERATOR::Equal, 1);
    
    // print
    BINF << ast;
    
}

void CSqlAstTest::testRealWhereClause()
{
    CSqlAst ast = AST_CONDITION(QUERYFIELDS::State::State::Id, AST_OPERATOR::Equal, 30);
    ast & AST_CONDITION(QUERYFIELDS::Country::Id, AST_OPERATOR::Equal, 1);
    BINF << ast;
}
