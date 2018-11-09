/* 
 * File:   CDirectory.cpp
 * Author: santony
 * 
 * Created on November 3, 2012, 2:29 PM
 */

#include "../seglobaltypedefs.h"
#include "CDirectory.h"
#include "Object.h"
#include "CString.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <pwd.h>
#include <string>
#include <sys/param.h>
#include "globallogger.h"


string CDirectory::processpath;

CDirectory::CDirectory() 
{
}

CDirectory::CDirectory(const CDirectory& orig) 
{
}

CDirectory::~CDirectory() 
{
}

bool CDirectory::Exists(CSTR directoryPath)
{
    // try to open directory
    auto dir = opendir(directoryPath);
    if (!dir)
    {
        return false;
    }
    closedir(dir);
    return true;
}

bool CDirectory::Create(CSTR directoryPath)
{
    if (!Exists(directoryPath))
    {
        std::string sCmd = "mkdir -p -m 0770 ";
        sCmd += directoryPath;
        int ret = system(sCmd.c_str());
        return ret != -1;
    }
    return false;
}


bool CDirectory::Delete(CSTR directoryPath)
{
    string path = directoryPath;
    if (path.substr(path.length()-1, 1)!="/")
        path.append("/");
    // ensure directory exists, if not create one.
    if (!CDirectory::Exists(path.c_str()))
    {
        char cmd[PATH_MAX];
        sprintf(cmd, "rm -R %s", directoryPath);
        int ret = system(cmd);
        return ret != -1;
    }
    return false;
}

string CDirectory::GetCurDir()
{
   char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
       return cwd;
   else
       throw runtime_error(strerror(errno));
}

string CDirectory::GetHome()
{
    struct passwd* pPwd = getpwuid(getuid());
    if (!pPwd)
        throw runtime_error("GetHome: getpwuid error");
    return pPwd->pw_dir;
}

ErrorId CDirectory::GetHome(se::IString **ppDir)
{
    try
    {
        return CObject<CString>::Create( nullptr, GetHome().c_str(), ppDir );

    }
    catch (const std::exception& ex)
    {
        BERR << ex.what();
        return E_NotFound;
    }
}

const string& CDirectory::GetExePath()
{
    if (processpath.empty())
    {
        auto pid = getpid();
        char path[PATH_MAX];
        char buf[PATH_MAX];
        sprintf(path, "/proc/%d/exe", pid);
        auto len = readlink(path, buf, PATH_MAX);
        if (len)
        {
            buf[len] = '\0';
            processpath = buf;
            auto lastof = processpath.find_last_of("/", processpath.length()-1);
            processpath = processpath.substr(0, lastof + 1);
            BINF << "Process Path: " << processpath;
        }
        else
            throw std::runtime_error(strerror(errno));
    }
    return processpath;
}

ErrorId CDirectory::GetExePath(se::IString** ppDir)
{
    string _rootDir = GetExePath();
    ErrorId ret = IString::Create(_rootDir.c_str(), ppDir);
    return ret;
}

bool CDirectory::EnumDir(CSTR dirName, FEnum function, POINTER state)
{
    struct stat stDirInfo;
    struct dirent * stFiles;
    DIR * stDirIn;
    char szFullName[MAXPATHLEN];
    struct stat stFileInfo;
    short int mode;

    if (lstat( dirName, &stDirInfo) < 0)
    {
        perror (dirName);
        return false;
    }
    if (!S_ISDIR(stDirInfo.st_mode))
        return false;
    
    if ((stDirIn = opendir( dirName )) == NULL)
    {
        perror( dirName );
        return false;
    }
    
    while (( stFiles = readdir(stDirIn)) != NULL)
    {
        sprintf(szFullName, "%s/%s", dirName, stFiles -> d_name );

        if (lstat(szFullName, &stFileInfo) < 0)
           perror ( szFullName );

        bool bRet ;
        /* is the file a directory? */
        if (S_ISDIR(stFileInfo.st_mode))
        {
            BDBG << "Directory: " << szFullName ;
            bRet = function(szFullName, true, state);
        }
        else
        {
            BDBG << "Filename: " << szFullName;
            bRet = function(szFullName, false, state);
        }
        if (!bRet)
            goto EXIT;
/*
        if ( S_ISLNK( stFileInfo.st_mode ))
           printf("is link\n");
*/
    }  // end while

EXIT:
    closedir(stDirIn);
    return true;
}

string CDirectory::BuildPath(std::initializer_list<CSTR> parts, CSTR sep)
{
    auto it = parts.begin();
    string outpath;
    for(int i=0; i != parts.size(); ++i)
    {
        outpath += (string)*(it++);
        if (i < parts.size()-1)
            outpath += (string)sep;
    }
    return outpath;
}

vector<string> CDirectory::BuildParts(const char* path, char sep)
{
    typedef vector<string> Parts;
    Parts parts;
    stringstream s;
    s << path;
    string spart;
    while (getline(s, spart, sep))
    {
        parts.emplace_back(spart);
    }
    return move(parts);
}
