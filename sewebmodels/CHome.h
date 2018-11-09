/* 
 * File:   CHome.h
 * Author: santony
 *
 * Created on June 30, 2015, 10:38 PM
 */

#ifndef CHOME_H
#define	CHOME_H

#include "CModelBase.h"
#include "CMaster.h"


class CHome : public CMaster
{
public:
    CHome();
    CHome(const CHome& orig);
    CHome(CHome&& orig);
    virtual ~CHome();
private:

};

#endif	/* CHOME_H */

