/* 
 * File:   sepredictinternal.h
 * Author: santony
 *
 * Created on May 9, 2015, 11:58 AM
 */

#ifndef SEPREDICTINTERNAL_H
#define	SEPREDICTINTERNAL_H

#include "../seglobal/Object.h"
#include "../semysql/CSEConnections.h"
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#include "sepredictapi.h"
#include "../seglobal/exceptions.h"


#define CATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
        /*LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what()); */     \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        /*LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what());*/      \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        /*LOGE(_logger, "(" << fname << ") : Unknown error reported.");                          */\
        return E_UnknownError;  \
    }                           
#endif	/* SEPREDICTINTERNAL_H */

