/* 
 * File:   FileSystemMonitor.cpp
 * Author: santony
 * 
 * Created on September 30, 2012, 2:18 AM
 */

#include "FileSystemMonitor.h"
#include <string.h>
#include <stdexcept>
#include "../ReturnCodes.h"
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include "globallogger.h"

using namespace se::io;

#define LOGGER "se.io.FileSystemMonitor"

FileSystemMonitor::FileSystemMonitor(CSTR path) :
    _path(path)
{
    if ( !path || !strlen(path))
        throw std::runtime_error("No path specified");
    
    
}

FileSystemMonitor::FileSystemMonitor(const FileSystemMonitor& orig) 
{
    
}

FileSystemMonitor::~FileSystemMonitor() 
{
    Stop();
}

ErrorId FileSystemMonitor::Begin()
{
    if (_fd > 0)
    {
        BINF << "File descriptor already exists. Stop before calling begin again.";
        return S_False;
    }
    _funcid = 0;
    int wd = 0;
    lock_guard<mutex> l(_m);
    {
        _fd = inotify_init();
        if (_fd < 0)
        {
            BERR << "Failed call to inotify_init.";
            return E_SystemError;
        }

        _th =ThreadStart();
        BINF << "File system initialization succeeded.";

    }
    
    return S_Success;
}

thread FileSystemMonitor::ThreadStart() 
{
    return thread( [this]
    {
        BINF << "Add inotify watcher to monitor file changes.";
        wd = inotify_add_watch(_fd, _path.c_str(), IN_CREATE | IN_MODIFY | IN_DELETE);
        if (wd <= 0)
        {
            BERR << "Error: " << strerror(errno) << ". Failed to add inotify watch. Exiting new thread.";
            return;
        }
        while (!_exitflag)
        {
            this->ReadLoop();
        }
    });
}

void FileSystemMonitor::ReadLoop() 
{
    const ssize_t eventsize = sizeof(struct inotify_event);
    const ssize_t buffsize = 1024 * (eventsize + FILENAME_MAX);
    ssize_t len, i = 0;
    char action[81+FILENAME_MAX] = {0};
    char buff[buffsize] = {0};
    len = read(_fd, buff, buffsize);
    if (_exitflag)
        return;
    while (i < len)
    {
        struct inotify_event* pevent = (struct inotify_event*)&buff[i];
        if (pevent->len)
        {
            string sfile = _path + "/" + pevent->name;
            // check if path need to be appended.
            if (pevent->mask & IN_CREATE)
            {
                Notify(FSNotifyType::Created, sfile);
            }
            else if (pevent->mask & IN_DELETE)
            {
                Notify(FSNotifyType::Deleted, sfile);
            }
            else if (pevent->mask & IN_MODIFY)
            {
                Notify(FSNotifyType::Modified, sfile);
            }
            i += eventsize + pevent->len;
        }   
        else
            break;
    }
}

void FileSystemMonitor::Notify(FSNotifyType action, const string& target) 
{
    if (_fd > 0)
    {
        lock_guard<mutex> l(_m);    // not the best performance, but it is ok for now.
        for (auto p : _listeners)
            (p.second)(target, action);
    }
}


ErrorId FileSystemMonitor::Stop()
{
    // remove all watchers
    if (_fd > 0)
    {
        lock_guard<mutex> l(_m);
        _exitflag = true;
        // write a tmp file 
        ofstream ofs(_path + "/.x.txt", std::ofstream::out);
        ofs << "checking";
        ofs.close();
        _th.join();
        BINF << "Removing listeners.";
        _listeners.clear();
        BINF << "Removed listeners.";
        close(_fd);
        _fd = 0;
        _funcid = 0;
    }
    BINF << "Successfully stopped the monitor.";
    return S_Success;
}

long unsigned int FileSystemMonitor::AddListener(NotifyFunction func, int& funcId) 
{
    BINF << "Adding listener.";
    funcId = _funcid++;
    lock_guard<mutex> l(_m);
    _listeners.insert(make_pair(funcId, func));
    BINF << "Listener added.";
    return S_OK;
}

long unsigned int FileSystemMonitor::RemoveListener(int funcId) 
{
    BINF << "Removing listener.";
    lock_guard<mutex> l(_m);
    _listeners.erase(funcId);
    BINF << "Removed listener.";
    return S_Success;
}
