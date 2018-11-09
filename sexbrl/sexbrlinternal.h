/* 
 * File:   sexbrlinternal.h
 * Author: santony
 *
 * Created on July 1, 2012, 5:33 PM
 */

#ifndef SEXBRLINTERNAL_H
#define	SEXBRLINTERNAL_H

#include "../ReturnCodes.h"
#include "sexbrlapi.h"
#include "../seglobal/Object.h"
#include "../secommon.h"
#include "../seglobal/IObject.h"
#include "../seglobal/globallogger.h"
#include "../semysql/CSEConnections.h"
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"

using namespace se::data;

////#include <log4cxx/logger.h>
////using namespace log4cxx;

#define  LOGI    //LOG4CXX_INFOINFO
#define LOGW    //LOG4CXX_INFOWARN
#define LOGE    //LOG4CXX_INFOERROR
#define LOGF    //LOG4CXX_INFOFATAL


#define LOGIP(str)    //LOG4CXX_INFOINFO(_logger, str)
#define LOGWP(str)    //LOG4CXX_INFOWARN(_logger, str)
#define LOGEP(str)    //LOG4CXX_INFOERROR(_logger, str)
#define LOGFP(str)    //LOG4CXX_INFOFATAL(_logger, str)


#ifdef __DEBUG__
#define RTRY    
#else
#define RTRY try {
#endif


#define CATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
        BERR << "(" << fname << ") : Reported std error as " << ex.what();  \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        BERR << "(" << fname << ") : Reported std error as " << ex.what();  \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        BERR << "(" << fname << ") : Unknown error reported.";          \
        return E_UnknownError;  \
    }                           

#define RBCATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
       /* LOGIP("Rolling back");  */\
        conn.RollbackTransaction(); \
        BERR << "(" << fname << ") : Reported std error as " << ex.what();  \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
       /* LOGIP("Rolling back");  */\
        conn.RollbackTransaction(); \
        BERR << "(" << fname << ") : Reported std error as " << ex.what(); \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
       /* LOGIP("Rolling back");  */\
        conn.RollbackTransaction(); \
        BERR << "(" << fname << ") : Unknown error reported."; \
        return E_UnknownError;  \
    }                           


#ifdef __DEBUG__
#define RCATCH(fname)
#else
#define RCATCH(fname) } CATCHCLAUSE(fname)
#endif

namespace se
{
    namespace xbrl
    {
        const int YearModulus = 2000;
    }
}


#endif	/* SEXBRLINTERNAL_H */


