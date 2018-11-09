/* 
 * File:   exceptions.cpp
 * Author: santony
 * 
 * Created on January 5, 2013, 9:23 PM
 */

#include "../ReturnCodes.h"
#include "../seglobaltypedefs.h"
#include "exceptions.h"
#include <sstream>

namespace se
{
    
    seexception::seexception(ErrorId err)  : runtime_error("Unknown error"),  _err(err)
    {
        std::ostringstream s;
        s << err;
    }

    seexception::seexception(const string& message)  : runtime_error(message), _err(E_UnknownError)
    {
        
    }

    const char* seexception::what() const _GLIBCXX_USE_NOEXCEPT
    {
 
        return _msg.c_str();
    }

    
    seexception& seexception::operator <<(const int i)
    {
        ostringstream os;
        os << this->_msg;
        os << i;
        _msg = os.str();
        return *this;
    }
    
        
    
    seexception& seexception::operator <<(const string& s)
    {
        ostringstream os;
        os << this->_msg;
        os << s;
        _msg = os.str();
        return *this;
    }
    
    seexception::seexception(ErrorId err, const string& msg)  : runtime_error(msg), _err(err)
    {
    }

    seexception::~seexception()  _GLIBCXX_USE_NOEXCEPT
    {
    }
    
    ErrorId seexception::error() const
    {
        return _err;
    }
    

        // badconversion
    BadConversion::BadConversion(std::string const&s) : std::runtime_error(s)
    {
        
    }
    
}