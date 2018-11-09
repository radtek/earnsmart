/* 
 * File:   CTestFredDataSource.h
 * Author: santony
 *
 * Created on September 11, 2015, 11:01 AM
 */

#ifndef CTESTFREDDATASOURCE_H
#define	CTESTFREDDATASOURCE_H

#include "CUnitTest.h"
#include "../seeconomy/seeconomapi.h"

using namespace se;
using namespace se::economy;

class CTestFredDataSource : public CUnitTest
{
public:
    CTestFredDataSource();
    CTestFredDataSource(const CTestFredDataSource& orig) = delete;
    virtual ~CTestFredDataSource();
    

    virtual void Run();

private:

};

#endif	/* CTESTFREDDATASOURCE_H */

