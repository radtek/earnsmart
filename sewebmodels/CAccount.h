/* 
 * File:   CAccount.h
 * Author: santony
 *
 * Created on August 10, 2015, 12:13 PM
 */

#ifndef CACCOUNT_H
#define	CACCOUNT_H

#include "CMaster.h"

class CAccount : public CMaster 
{
public:
    CAccount();
    CAccount(const CAccount& orig);
    virtual ~CAccount();
    

    virtual void serialize(archive& a);

    
private:

};

#endif	/* CACCOUNT_H */

