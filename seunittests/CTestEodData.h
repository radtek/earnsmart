/* 
 * File:   CTestEodData.h
 * Author: santony
 *
 * Created on August 26, 2015, 9:15 AM
 */

#ifndef CTESTEODDATA_H
#define	CTESTEODDATA_H

#include "../seeoddata/seeoddata.h"
#include "CUnitTest.h"

using namespace se;
using namespace se::eoddata;

class CTestEodData : public CUnitTest
{
public:
    CTestEodData();
    CTestEodData(const CTestEodData& orig) = delete;
    virtual ~CTestEodData();
    

    virtual void Run();


private:

};

#endif	/* CTESTEODDATA_H */

