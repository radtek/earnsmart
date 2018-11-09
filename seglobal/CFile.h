/* 
 * File:   CFile.h
 * Author: santony
 *
 * Created on October 19, 2013, 9:31 AM
 */

#ifndef CFILE_H
#define	CFILE_H

#include "../seglobaltypedefs.h"
#include <string>

namespace se
{
    class CFile 
    {
    public:
        CFile();
        CFile(const CFile& orig);
        virtual ~CFile();

        static bool Exists(CSTR file);
    
        static bool Delete(CSTR file);
    
        
        // Read entire file to a string. 
        // Use this for smaller files (< 100mb)
        static std::string ReadFile(CSTR file);
        
    private:

    };
}

#endif	/* CFILE_H */

