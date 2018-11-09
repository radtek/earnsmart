/* 
 * File:   seeoddataint.h
 * Author: santony
 *
 * Created on February 1, 2015, 7:45 PM
 */

#ifndef SEEODDATAINT_H
#define	SEEODDATAINT_H
#include "../seglobal/Object.h"
#include "../semysql/CSEConnections.h"
#include "../seglobal/globallogger.h"
#include "seeoddata.h"
#include "../seglobal/exceptions.h"

#define SEDBCONN CSEConnections::GetMain()

#define CATCHCLAUSE             \
    catch(seexception& ex)      \
    {                           \
        BERR << "Reported std error as " << ex.what();   \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        BERR << "Reported std error as " << ex.what();   \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        BERR << "Reported unknown error";   \
        return E_UnknownError;  \
    }                           


#ifdef __DEBUG__
#define MAYTRC CATCHCLAUSE
#else
#define MAYTRC 
#endif

#ifdef __DEBUG__
#define MAYTRY try
#else
#define MAYTRC 
#endif


#endif	/* SEEODDATAINT_H */

