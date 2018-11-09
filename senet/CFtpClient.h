/* 
 * File:   CFtpClient.h
 * Author: santony
 *
 * Created on November 1, 2012, 1:33 AM
 */

#ifndef CFTPCLIENT_H
#define	CFTPCLIENT_H

#include <vector>
#include <functional>
#include <algorithm>
#include <curl/curl.h> // this breaks the idea of hiding nanoftp, but we have limited time now.
//#include <log4cxx/logger.h>
#include "../seglobal/CTimeStamp.h"
#include <string>

namespace se
{
    namespace net
    {

        #define IRemoteDir void 

        struct FileInfo
        {
            std::string FileName;
            se::CTimeStamp Time;
            U32 Permissions;
            I32 UserId;
            I32 GroupId;
        };

        /* 
         * A wrapper over curl's ftp library.
         */
        class CFtpClient 
        {
        public:
            CFtpClient();
            CFtpClient(const CFtpClient& orig);
            virtual ~CFtpClient();

            ErrorId BrowseRemoteFolders(CSTR url, std::function<void (CSTR) > callback);

            /* Obtain a file from the remote ftp directory. 
             * The url must be provided as complete upto a file name.
             * eg: to download a file readme.txt in doc folder in the host ftp.host.com, 
             * url will be ftp://ftp.host.com/doc/readme.txt             
             */
            ErrorId  GetFile(CSTR url, CSTR destination);

            /* Obtain multiple files from the remote ftp directory. 
             * The url must be provided as complete upto a wild card.
             * eg: to download all files in 'doc' folder in the host ftp.host.com, 
             * url will be ftp://ftp.host.com/doc/*. Similarly if only trying to
             * download all txt files, url must be specified as ftp://ftp.host.com/doc/*.txt
             * The "destination" parameter is the directory where the files are downloaded. 
             * The "filterPattern" parameter can be a wild card (*.txt) or 
             * a regex pattern.
             * The downloaded files will be named as at the source.
             */
            ErrorId  GetFiles(CSTR url, CSTR destination, CSTR filterPattern = 0, std::function<void(CSTR)> cb = 0);
            
        private:
            /* 
             * Open the url. the url must be provided as complete upto a file name.
             * eg: to download a file readme.txt in doc folder in the host ftp.host.com, 
             * url will be ftp://ftp.host.com/doc/readme.txt
             */
            ErrorId Open(CSTR url);
            
            ErrorId Close();

            
        private:
            CURL* pCurl ;
        };

    }
}
#endif	/* CFTPCLIENT_H */

