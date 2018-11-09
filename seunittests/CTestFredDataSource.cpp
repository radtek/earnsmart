/* 
 * File:   CTestFredDataSource.cpp
 * Author: santony
 * 
 * Created on September 11, 2015, 11:01 AM
 */

#include "CTestFredDataSource.h"

CTestFredDataSource::CTestFredDataSource() {
}

CTestFredDataSource::~CTestFredDataSource() {
}

void CTestFredDataSource::Run() 
{
    SP<IFredDataSource> pFred;
    IFredDataSource::Create(&pFred);
    SP<IIndicatorCategory> pCatRoot;
    auto rCode = pFred->get_Category(0, &pCatRoot);
    BINF << rCode << endl;
    if (FAILED(rCode))
        return ;
    
    BINF << pCatRoot->get_Name();
    
    return;
}


