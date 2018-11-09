/* 
 * File:   CError.cpp
 * Author: santony
 * 
 * Created on July 8, 2015, 3:20 PM
 */

#include "common.h"
#include "CError.h"

CError::CError(const string& message) : _ex(E_HttpError, message.c_str())
{
    
}

CError::CError(const seexception& ex) : _ex(ex)
{
    
}

CError::CError(const CError& orig) : _ex(orig._ex)
{
    
}

CError::~CError() {
}

const seexception& CError::get_Error() 
{
    return _ex;
}

void CError::set_Error(const seexception& ex) 
{
    _ex = ex;
}



