/* 
 * File:   CMapAllSymbolsToCIK.h
 * Author: santony
 *
 * Created on October 1, 2012, 9:36 PM
 */

#ifndef CMAPALLSYMBOLSTOCIK_H
#define	CMAPALLSYMBOLSTOCIK_H

#include <zthread/Runnable.h>
#include "../semain/semain.h"

using namespace ZThread;
using namespace se;
using namespace se::xbrl;


class CMapAllSymbolsToCIK : public Runnable, public CTimedFunction
{
public:
    CMapAllSymbolsToCIK(ISession*);
    virtual ~CMapAllSymbolsToCIK();
    void run();
    
private:
    
    //log4cxx::LoggerPtr _logger;
    SP<ISession> _pSession ;

};

#endif	/* CMAPALLSYMBOLSTOCIK_H */

