/* 
 * File:   CDirectory.h
 * Author: santony
 *
 * Created on November 3, 2012, 2:29 PM
 */

#ifndef CDIRECTORY_H
#define	CDIRECTORY_H

#include "IObject.h"
#include <string>
#include <initializer_list>

using namespace std;

typedef bool (*FEnum)(CSTR name, bool IsDir, POINTER state);

class CDirectory 
{
public:
    CDirectory();
    CDirectory(const CDirectory& orig);
    virtual ~CDirectory();

    static bool Exists(CSTR directoryPath);
    static bool Create(CSTR directoryPath);
    static bool Delete(CSTR direcotryPath);
    static string GetHome();
    static string GetCurDir();
    static ErrorId GetHome(se::IString **ppDir);
    static ErrorId GetExePath(se::IString** ppDir);
    static const string& GetExePath();
    
    /* Enumerate directory. TFEnum function must be 
     * in the form of bool (f)(CSTR name, bool IsDir) */
    static bool EnumDir(CSTR dirName, FEnum function, POINTER state);
    
    static string BuildPath(std::initializer_list<CSTR> parts, CSTR sep = "/"); // build a path
    static vector<string> BuildParts(CSTR path, char sep='/'); // get individual levels in path as parts 
    
public:
    static string processpath;

private:

};

#endif	/* CDIRECTORY_H */

