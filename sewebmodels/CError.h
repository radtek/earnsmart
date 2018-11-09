/* 
 * File:   CError.h
 * Author: santony
 *
 * Created on July 8, 2015, 3:20 PM
 */

#ifndef CERROR_H
#define	CERROR_H


#include "CModelBase.h"
#include "CMaster.h"

using namespace se;

class CError : public CMaster
{
public:
    CError(const string& message);
    CError(const seexception& ex);
    CError(const CError& orig);
    virtual ~CError();
    
    const seexception& get_Error();
    void set_Error(const seexception& ex);
    
private:
    seexception _ex;
};

#endif	/* CERROR_H */

