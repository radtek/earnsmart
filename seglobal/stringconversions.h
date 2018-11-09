/* 
 * File:   stringconversions.h
 * Author: santony
 *
 * Created on September 16, 2012, 6:51 AM
 */

#ifndef STRINGCONVERSIONS_H
#define	STRINGCONVERSIONS_H

#include "../seglobaltypedefs.h"
#include <string>

//CWSTR STRTOWSTR(CSTR in) __attribute__ ((always_inline));
//CSTR WSTRTOSTR(CWSTR in) __attribute__ ((always_inline));

void stringconvert(std::string& from, std::wstring& to) __attribute__ ((always_inline));
void stringconvert(std::wstring& from, std::string& to) __attribute__ ((always_inline));


#endif	/* STRINGCONVERSIONS_H */

