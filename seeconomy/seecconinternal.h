/* 
 * File:   seecconinternal.h
 * Author: santony
 *
 * Created on January 23, 2014, 4:44 AM
 */

#ifndef SEECCONINTERNAL_H
#define	SEECCONINTERNAL_H

#include "../seglobal/Object.h"
#include "seeconomapi.h"

#include "../semysql/CSEConnections.h"
#include "../seglobal/exceptions.h"
#include "../seglobal/globallogger.h"

#define SEDBCONN CSEConnections::GetMain()

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
        BERR << "(" << fname << ") : Reported std error as " << ex.what();      \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        BERR << "(" << fname << ") : Reported std error as " << ex.what();     \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        BERR << "(" << fname << ") : Unknown error reported.";                         \
        return E_UnknownError;  \
    }                           

#define RBCATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
        BINF << "Rolling back";  \
        conn.RollbackTransaction(); \
        BERR << "(" << fname << ") : Reported std error as " << ex.what();     \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        BINF << "Rolling back";  \
        conn.RollbackTransaction(); \
        BERR << "(" << fname << ") : Reported std error as " << ex.what();     \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        BINF << "Rolling back";  \
        conn.RollbackTransaction(); \
        BERR << "(" << fname << ") : Unknown error reported.";                         \
        return E_UnknownError;  \
    }                           

#ifdef __DEBUG__
#define RCATCH(fname)
#else
#define RCATCH(fname) } CATCHCLAUSE(fname)
#endif


#endif	/* SEECCONINTERNAL_H */

