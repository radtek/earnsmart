/* 
 * File:   CStringToNumber.h
 * Author: santony
 *
 * Created on December 16, 2012, 9:55 AM
 */

#ifndef CSTRINGTONUMBER_H
#define	CSTRINGTONUMBER_H

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "exceptions.h"
#include "../seglobaltypedefs.h"

namespace se
{
    namespace conv
    {
        class CConvertString 
        {
        public:
            template<typename R>
            static R To(std::string const& s, bool FailIfLeftOverChars = false)
            {
                std::istringstream i(s);
                R x;
                CHR c;
                if (!(i >> x ) || (FailIfLeftOverChars && i.get(c)))
                    throw BadConversion("CConvertString::To(" + s + ")");
                return x;
            }
        };
        
    }
}


#endif	/* CSTRINGTONUMBER_H */

