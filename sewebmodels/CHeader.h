/* 
 * File:   CHeader.h
 * Author: santony
 *
 * Created on July 3, 2015, 9:08 PM
 */

#ifndef CHEADER_H
#define	CHEADER_H

#include "CModelBase.h"
#include "CAppSearch.h"
#include "CLoginOrRegister.h"

class CHeader : public CModelBase 
{
public:
    CAppSearch AppSearch ;
    CLoginOrRegister LoginOrRegister;
    
public:
    CHeader();
    CHeader(const CHeader& orig);
    CHeader(CHeader&& orig);
    virtual ~CHeader();
    

};

#endif	/* CHEADER_H */

