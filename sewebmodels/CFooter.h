/* 
 * File:   CFooter.h
 * Author: santony
 *
 * Created on July 3, 2015, 9:08 PM
 */

#ifndef CFOOTER_H
#define	CFOOTER_H

#include "CModelBase.h"


class CFooter : public CModelBase 
{
public:
    CFooter();
    CFooter(const CFooter& orig);
    CFooter(CFooter&& orig);
    virtual ~CFooter();
private:

};

#endif	/* CFOOTER_H */

