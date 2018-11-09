/* 
 * File:   seglobalinternal.h
 * Author: santony
 *
 * Created on December 29, 2013, 5:23 PM
 */

#ifndef SEGLOBALINTERNAL_H
#define	SEGLOBALINTERNAL_H

#include "IObject.h"

#define CATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
        /*LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what());*/      \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        /*LOGE(_logger, "(" << fname << ") : Reported std error as " << ex.what());*/      \
    }                           \
    catch(...)                  \
    {                           \
        /*LOGE(_logger, "(" << fname << ") : Unknown error reported."); */                         \
        return E_UnknownError;  \
    }                           



#endif	/* SEGLOBALINTERNAL_H */

