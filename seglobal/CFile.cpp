/* 
 * File:   CFile.cpp
 * Author: santony
 * 
 * Created on October 19, 2013, 9:31 AM
 */

#include "CFile.h"
#include <fstream>
#include <cstdio>

using namespace std;

namespace se
{

    CFile::CFile() 
    {
    }

    CFile::CFile(const CFile& orig) 
    {
    }

    CFile::~CFile() 
    {
    }

    bool CFile::Exists(const char* file)
    {
        // attempt to open
        fstream f(file, fstream::in);
        if (f.good())
        {
            f.close();
            return true;
        }
        return false;
    }

    bool CFile::Delete(const char* file) 
    {
        return 0 == std::remove(file);
    }

    std::string CFile::ReadFile(const char* file) 
    {
        std::ifstream in(file, std::ios::in | std::ios::binary);
        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return std::move(contents);
        }
        throw(errno);
    }


}