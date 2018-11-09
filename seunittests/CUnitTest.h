/* 
 * File:   CUnitTest.h
 * Author: santony
 *
 * Created on August 26, 2015, 9:17 AM
 */

#ifndef CUNITTEST_H
#define	CUNITTEST_H

class CUnitTest 
{
public:
    CUnitTest();
    CUnitTest(const CUnitTest& orig) = delete;
    virtual ~CUnitTest();
    
    virtual void Run() = 0;
    
private:

};

#endif	/* CUNITTEST_H */

