/* 
 * File:   exceptions.h
 * Author: santony
 *
 * Created on January 5, 2013, 9:23 PM
 */

#ifndef EXCEPTIONS_H
#define	EXCEPTIONS_H

#include <exception>
#include <stdexcept>
#include <string>
#include "../seglobaltypedefs.h"

using namespace std;

namespace se
{

    class seexception : public std::runtime_error
    {
    public:
        explicit seexception(ErrorId err);
        explicit seexception(const string& msg);
        explicit seexception(ErrorId err, const string& msg) ;
        virtual ~seexception() _GLIBCXX_USE_NOEXCEPT;
        ErrorId error() const ;
        seexception& operator<<(const int i);
        seexception& operator<<(const string& s);
        const char* what() const _GLIBCXX_USE_NOEXCEPT override;

        
    private:
        ErrorId _err;
        string _msg;
    };
    
    class BadConversion : 
    public std::runtime_error
    {
    public:
        BadConversion(std::string const& s);
    };
    

}

#endif	/* EXCEPTIONS_H */

