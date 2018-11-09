/* 
 * File:   CXbrlListener.h
 * Author: arun
 *
 * Created on 4 August, 2012, 8:07 AM
 */



#ifndef CXBRLLISTENER_H
#define	CXBRLLISTENER_H

#include "../sexbrl/sexbrlapi.h"

using namespace se::xbrl;

class CXbrlListener : public IXbrlCompletionEvent<int>
{
public:
    CXbrlListener();
    CXbrlListener(const CXbrlListener& orig);
    virtual ~CXbrlListener();
    
    virtual void Completed(int completionData) {};
    virtual void Inform(CSTR message){} ;
    virtual void Error(ErrorId err, CSTR message) {};
    
private:

};

#endif	/* CXBRLLISTENER_H */

