/* 
 * File:   CVariadicTemplates.h
 * Author: santony
 *
 * Created on September 4, 2015, 4:11 PM
 */

#ifndef CVARIADICTEMPLATES_H
#define	CVARIADICTEMPLATES_H

#include "CUnitTest.h"


class CVariadicTemplates : public CUnitTest
{
public:
    CVariadicTemplates();
    CVariadicTemplates(const CVariadicTemplates& orig) = delete;
    virtual ~CVariadicTemplates();
    

    virtual void Run();

    
private:

};

#endif	/* CVARIADICTEMPLATES_H */

