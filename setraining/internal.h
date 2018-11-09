/* 
 * File:   internal.h
 * Author: santony
 *
 * Created on November 18, 2013, 9:54 AM
 */

#ifndef INTERNAL_H
#define	INTERNAL_H

#include "setrainingapi.h"
#include "../seglobal/Object.h"
////#include <log4cxx/logger.h>
#include "../seglobal/Object.h"

//using namespace log4cxx;

#define LOGI    //LOG4CXX_INFO
#define LOGW    //LOG4CXX_WARN
#define LOGE    //LOG4CXX_ERROR
#define LOGF    //LOG4CXX_FATAL


#define LOGIP(str)    //LOG4CXX_INFO(_logger, str)
#define LOGWP(str)    //LOG4CXX_WARN(_logger, str)
#define LOGEP(str)    //LOG4CXX_ERROR(_logger, str)
#define LOGFP(str)    //LOG4CXX_FATAL(_logger, str)

#ifdef __DEBUG__
#define RTRY    
#else
#define RTRY try {
#endif


#define CATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
        /*LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what());    */  \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        /*LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what());     */ \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        /*LOGE(_logger, "(" << fname << ") : Unknown error reported.");                        */  \
        return E_UnknownError;  \
    }                           

#define RBCATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
        /*LOGIP("Rolling back"); */ \
        conn.RollbackTransaction(); \
        /*LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what());     */ \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        /*LOGIP("Rolling back");  */\
        conn.RollbackTransaction(); \
        /*LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what());      */\
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        /*LOGIP("Rolling back");  */\
        conn.RollbackTransaction(); \
        /*LOGE(_logger, "(" << fname << ") : Unknown error reported.");                         */ \
        return E_UnknownError;  \
    }                           

#ifdef __DEBUG__
#define RCATCH(fname)
#else
#define RCATCH(fname) } CATCHCLAUSE(fname)
#endif

#endif	/* INTERNAL_H */

