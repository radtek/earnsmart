/* 
 * File:   semain.h
 * Author: santony
 *
 * Created on October 30, 2012, 7:55 PM
 */

#ifndef SEMAIN_H
#define	SEMAIN_H

#define SEMAINEXP __attribute__ ((visibility ("default")))

#include "../seglobal/IObject.h"
#include "../seglobaltypedefs.h"
#include "ISessionInterface.h"



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

    //#define E_SampleError                           0x80030000      // a sample info message
    //#define S_SampleInfo                            0x02030000      // a sample info message
    //#define W_SampleWarning                         0x01030000      // a sample warning
    //#define D_SampleDebug                           0x03030000      // a sample debug message.

    /* Zmq related return codes */
#define E_LogConfigFile                      0x80030001      // Configuration error - log configuration file is missing.
#define E_DBConnFile                         0x80030002      // Configuration error - DB connnection file is missing.

    struct IAppEx : public IApp
    {
        /*
         * Open a session. If session id is provided, then the session is queried in the database
         * and the session will be open. The session's user role type will be set to anonymous 
         * if the session is not provided, or the session identified (from the database) is an anonymous type.
         * Similarly it will be set to standard or licensed if the session id is found to be registered respectively.
         */
        virtual RP<ISessionInterface> OpenSession(CSTR sessionId) = 0;

        /*
         * Open a new session. Use this method create a session when no session id is available, but
         * both the session user is not anonymous.
         * Use the session Id stored in the returned interface to reopen the session quickly (get_Id()).
         */
        virtual RP<ISessionInterface> CreateSession(CSTR userId, CSTR pwd) = 0;

        virtual RP<ISessionInterface> CreateAnonymousSession() = 0;
    
        virtual SE_CODE CloseSession(CSTR sessionId) = 0;
        
    
        virtual ISessionInterface* GetSessionNoLock(CSTR sessionId) = 0;
    
    };
    

    extern "C" 
    {
        ErrorId SEMAINEXP SEGetApp(CSTR process, IApp** ppOut);
        ErrorId SEMAINEXP SEOpenApp(IApp** ppOut);
        //ErrorId SEMAINEXP SECloseApp(IApp* pIn);
    }

}

#endif	/* SEMAIN_H */

