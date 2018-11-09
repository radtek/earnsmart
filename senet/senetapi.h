/* 
 * File:   senetapi.h
 * Author: santony
 *
 * Created on September 22, 2012, 9:36 PM
 */

#ifndef SENETAPI_H
#define	SENETAPI_H


#include "../seglobaltypedefs.h"
#include "../ReturnCodes.h"
#include "../seglobal/IObject.h"

#define NETEXP __attribute__ ((visibility ("default")))


namespace se
{

/*
 * Error codes are defined the following way in a 32-bit number (HRESULT)
 * 
 *  7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   
 *-------------------------------------------------------------------------
 *  E| |C|     | T | |     Facility    |               Code                |
 * 
 * Facility code is 1
 * T = {
 *      00 = Error
 *      01 = Warning
 *      10 = Information
 *      11 = Debug
 * }
 * 
 */

//#define S_SampleInfo                            0x02020000      // a sample info message
//#define W_SampleWarning                         0x01020000      // a sample warning
//#define D_SampleDebug                           0x03020000      // a sample debug message.

/* Zmq related return codes */
#define E_ZMQError                      0x80020001      // Any zmq error.

/* ftp related return codes */
#define E_FtpError                      0x80021001      // Any ftp client error.
#define E_FtpHost                       0x80021002      // Host not found or not accessible.
#define E_FtpAlreadyClosed              0x80021003      // A connection is already closed.
#define E_FtpDestination                0x80021004        // Destination file is null or invalid.
#define E_FtpDownload                   0x80021005        // Download failed.

#define E_HttpError                     0x80021006      // http connection error.
#define E_SmtpError                     0x80021007      // Smtp connection error.
#define E_RSSError                      0x80021008      // Smtp connection error.
#define E_CurlError                     0x80021009      // General curl connection error.

#define S_FtpHostAlreadyConnected       0x02020001      // A host is already connected. Close connection before reopening.
    
    

}

namespace se
{
    namespace net
    {
        typedef enum class EmailDeliveryProvider
        {
            SparkPost,
            Office365 // not implemented
            
        } EMLPROVD;
        
        SE_CODE NETEXP SendEmail(
                EMLPROVD serviceProvider, 
                CSTR from, 
                CSTR to, 
                CSTR subject, 
                CSTR body,
                IString** retMsg = nullptr
                );
    }
}

//#include "CFtpClient.h"
//#include "CZMQClient.h"

#endif	/* SENETAPI_H */

