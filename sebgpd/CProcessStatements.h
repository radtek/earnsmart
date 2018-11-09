/* 
 * File:   CProcessStatements.h
 * Author: aashok
 *
 * Created on June 29, 2013, 7:04 AM
 */

#ifndef CPROCESSSTATEMENTS_H
#define	CPROCESSSTATEMENTS_H

#include "IFunctions.h"
////#include <log4cxx/logger.h>
#include "../sexbrl/sexbrlapi.h"
#include <zthread/Runnable.h>

using namespace ZThread;
using namespace se;
using namespace se::xbrl;
//using namespace log4cxx;

class CProcessStatements : 
        public Runnable,
        public CTimedFunction
{
public:
    CProcessStatements(ISession* ps);
    virtual ~CProcessStatements();
    virtual void run();
private:
    SP<ISession> _pSession;
};

#endif	/* CPROCESSSTATEMENTS_H */

