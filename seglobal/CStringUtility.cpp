/* 
 * File:   CStringUtility.cpp
 * Author: santony
 * 
 * Created on December 30, 2012, 6:28 PM
 */

#include <sstream>

#include "CStringUtility.h"
#include <regex>

using namespace std;

namespace se
{
    namespace utils
    {
        string CStringUtility::Duplicate(CHR chr, I16 times)
        {
            stringstream os;
            for (auto i = 0; i != times; ++i)
            {
                os << chr;
            }
            return os.str();
        }        

        void CStringUtility::LeftTrim(string& str, char chr) 
        {
            std::string::size_type pos =  str.find_first_not_of(chr,0);
            if(pos > 0)
                str.erase(0,pos);     
        }

        bool CStringUtility::ValidateEmail(const string& email) 
        {
            std::regex rgpattern(R"(^[a-zA-Z0-9!#$%&'*+=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?$)");
            std::smatch matches;
            return std::regex_match(email, matches, rgpattern);
        }
        
        string CStringUtility::GenTempString(U32 len)
        {
            srand(time(NULL));
            char freeBuffer[len];
            const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
            for (U32 i = 0; i < len ; ++i)
            {
                freeBuffer[i] = alphanum[rand() % sizeof(alphanum)-1];
            }
            freeBuffer[len] = 0;
            return freeBuffer;
        }

        bool CStringUtility::ChangeCase(char* str, size_t size, bool lower) 
        {
            if (size != strlen(str))
                return false;
            if (lower)
            {
                for (int i=0; i<size; ++i)
                {
                    str[i] = tolower(str[i]);
                }
            }
            else
            {
                for (int i=0; i<size; ++i)
                {
                    str[i] = toupper(str[i]);
                }
            }
        }
        
    }
}
