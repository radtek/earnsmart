/* 
 * File:   CAsyncProcessor.h
 * Author: santony
 *
 * Created on July 12, 2012, 6:51 AM
 */

#ifndef CASYNCPROCESSOR_H
#define	CASYNCPROCESSOR_H

#include "sexbrlinternal.h"

template<typename T>
class CAsyncProcessor 
{
protected:
    T* _listener;
public:
    CAsyncProcessor()
    {
    
    }
    CAsyncProcessor(const CAsyncProcessor& orig)
    {
        
    }
    virtual ~CAsyncProcessor()
    {
        
    }

    virtual ErrorId Process(T* listener) = 0;
    
private:

};

#endif	/* CASYNCPROCESSOR_H */

