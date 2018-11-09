/* 
 * File:   semaininternal.h
 * Author: santony
 *
 * Created on November 6, 2012, 6:23 AM
 */

#ifndef SEMAININTERNAL_H
#define	SEMAININTERNAL_H

#include "semain.h"
#include "../seglobal/Object.h"
#include "../seglobal/CObjectRoot.h"
#include "../seglobal/exceptions.h"
#include <boost/filesystem.hpp>
#include "../seglobal/globallogger.h"
#include "../seadmin/seadminapi.h"



#define CATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
        BERR << "Reported std error as " << ex.what();\
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        BERR << "Reported std error as " << ex.what();\
        return E_UnknownError; \
    }                           \
    catch(...)                  \
    {                           \
        BERR << "Unknown error reported." ;\
        return E_UnknownError;  \
    }                           



#endif	/* SEMAININTERNAL_H */

