/* 
 * File:   CRepeatedDBAction.h
 * Author: santony
 *
 * Created on January 20, 2014, 12:08 PM
 */

#ifndef CREPEATEDDBACTION_H
#define	CREPEATEDDBACTION_H

#include "IFunctions.h"
////#include <log4cxx/logger.h>
#include "../sexbrl/sexbrlapi.h"
#include <zthread/Runnable.h>
#include <string>

using namespace std;
using namespace ZThread;
using namespace se;
using namespace se::xbrl;
//using namespace log4cxx;


class CRepeatedDBAction : 
        public Runnable,
        public CTimedFunction
{
public:
    CRepeatedDBAction(ISession* ps, const string& param);
    virtual ~CRepeatedDBAction();
    void run();
private:
    SP<IXbrl> _pxbrl ;
    string _param;
    SP<ISession> _pSession;

};

#endif	/* CREPEATEDDBACTION_H */

