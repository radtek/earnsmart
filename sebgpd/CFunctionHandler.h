/* 
 * File:   CFunctionHandler.h
 * Author: santony
 *
 * Created on October 1, 2012, 8:49 PM
 */

#ifndef CFUNCTIONHANDLER_H
#define	CFUNCTIONHANDLER_H

#include <string>
#include "IFunctions.h"

using namespace std;

class CFunctionHandler : public IFunction
{
public:
    CFunctionHandler(CSTR funcName, CSTR funcArg = 0);
    virtual ~CFunctionHandler();

    virtual ErrorId Execute();
    virtual void Interrupt();
    
private:
    CFunctionHandler(const CFunctionHandler& orig);
    
private:
    string _funcName ;
    string _funcArg;
    bool _bExecInSync;
    IFunction* pFunc;
};

#endif	/* CFUNCTIONHANDLER_H */

