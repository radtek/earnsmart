/* 
 * File:   globallogger.h
 * Author: santony
 *
 * Created on August 31, 2015, 7:38 AM
 */

#ifndef GLOBALLOGGER_H
#define	GLOBALLOGGER_H

#define ELPP_THREAD_SAFE
        
#include "easylogging++.h"

// __FUNCSIG__ in VC++

#define BINF    LOG(INFO) << "[" << __PRETTY_FUNCTION__ << "] "
#define BERR    LOG(ERROR) << "[" << __PRETTY_FUNCTION__ << "] "
#define BWRN    LOG(WARNING) << "[" << __PRETTY_FUNCTION__ << "] "
#define BDBG    LOG(DEBUG) << "[" << __PRETTY_FUNCTION__ << "] "

#define RETURNIFFAILED(func) do { ErrorId ret = func; if (ret != S_Success) return ret; } while(0); 

#define RETIFFAILED(retcode) if (FAILED(retcode)) return ret

#define LOGRETURNIFFAILED(func, message) \
    do { \
        ErrorId ret = func; \
        if (ret != S_Success) \
        { \
            BERR << message; \
            return ret ;\
        }\
    } while(0); 

#define LOGIFFAILEDANDRETURN(func, message) \
    do { \
        ErrorId ret = func; \
        if (ret != S_Success) \
        { \
            BERR << message; \
        }\
        return ret ;\
    } while(0); 
    
#define LOGIFFAILEDANDTHROW(func, message) \
    do { \
        ErrorId ret = func; \
        if (ret != S_Success) \
        { \
            BERR << message; \
            throw seexception(ret, message) ;\
        }\
    } while(0); 

#define LOGANDRETURNPTR(func, message) \
    do { \
        auto ptr = func; \
        if (ptr == nullptr) \
        { \
            BERR << message; \
        }\
        return ptr; \
    } while(0); 
    
  
#define THROWIFFAILED(func, message) \
        do                              \
        {                               \
            ErrorId ret = func;         \
            if (ret != S_Success)       \
            throw se::seexception(ret, message); \
        } while(0); 

#define ERRIFFAILED(ret) if (ret != S_Success) throw se::seexception(ret); 


#endif	/* GLOBALLOGGER_H */

