/* 
 * File:   CSecurityPriceProcessor.h
 * Author: santony
 *
 * Created on October 19, 2013, 6:58 PM
 */

#ifndef CSECURITYPRICEPROCESSOR_H
#define	CSECURITYPRICEPROCESSOR_H

#include <zthread/Runnable.h>
#include "IFunctions.h"

using namespace ZThread;

class CSecurityPriceProcessor : public Runnable, public CTimedFunction
{
public:
    CSecurityPriceProcessor(ISession* ps, int waitpersymbol);
    virtual ~CSecurityPriceProcessor();
    void run();
private:
    //LoggerPtr _logger;
    int _waitpersymbol;
     SP<ISession> _pSession;
   
};

#endif	/* CSECURITYPRICEPROCESSOR_H */

