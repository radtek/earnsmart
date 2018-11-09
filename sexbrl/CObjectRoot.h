/* 
 * File:   CObjectRoot.h
 * Author: santony
 *
 * Created on July 2, 2012, 7:28 AM
 */

#ifndef COBJECTROOT_H
#define	COBJECTROOT_H

#include "sexbrlapi.h"


class CObjectRoot 
{
public:
    CObjectRoot();
    CObjectRoot(const CObjectRoot& orig);
    virtual ~CObjectRoot();
private:

protected:
    void _AddRef();
    void _DecRef();
    
    virtual ErrorId Init();
};

#endif	/* COBJECTROOT_H */

