/* 
 * File:   FileSystemMonitor.h
 * Author: santony
 *
 * Created on September 30, 2012, 2:18 AM
 */

#ifndef FILESYSTEMMONITOR_H
#define	FILESYSTEMMONITOR_H


#include "../seglobaltypedefs.h"
#include <sys/inotify.h>
#include <string>
#include <list>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>

using namespace std;

namespace se
{
    namespace io
    {

        
        enum class FSNotifyType : char
        {
            Created = 'c',
            Modified = 'm',
            Deleted = 'd'
        };

        typedef std::function< void (const string&, FSNotifyType) > NotifyFunction;

        /*
         * File or directory change monitor. Only a single path can be specified. For multiple
         * directories or files, create new instances of this type. Also note that 
         * if the path specified is a directory which has subdirectories, those sub directories
         * will not be monitored.
         */
        class FileSystemMonitor 
        {
        public:

            FileSystemMonitor(CSTR path);

            SE_CODE AddListener(NotifyFunction func, int& funcId);
            SE_CODE RemoveListener(int funcId);
            ErrorId Begin();
            ErrorId Stop();
            
            virtual ~FileSystemMonitor();
        private:
            
            typedef map< int, NotifyFunction > Listeners;
            string _path;
            Listeners _listeners;
            int _fd = 0; // inotify filedescriptor
            std::mutex _m;
            bool _exitflag = false;
            thread _th;
            int wd = 0;
            atomic<int> _funcid;
        private:
            FileSystemMonitor(const FileSystemMonitor& orig);
            thread ThreadStart();
            void ReadLoop();
            void Notify(FSNotifyType action, const string& target);
        };
    }
}


#endif	/* FILESYSTEMMONITOR_H */

