/* 
 * File:   CStringUtility.h
 * Author: santony
 *
 * Created on December 30, 2012, 6:28 PM
 */

#ifndef CSTRINGUTILITY_H
#define	CSTRINGUTILITY_H

#include <string>
#include "../seglobaltypedefs.h"

using namespace std;

namespace se
{
    namespace utils
    {
        class CStringUtility 
        {
        public:
            /* duplicate a character as many times as 'times' parameters
             and return the new string */
            static string Duplicate(CHR chr, I16 times);
            
            static void LeftTrim(string& str, char chr);
            
            static bool ValidateEmail(const string& email);

            static string GenTempString(U32 len = 10);
            
            static bool ChangeCase(STR str, size_t size, bool lower);
            
        };
        
    }
}


#endif	/* CSTRINGUTILITY_H */

