/* 
 * File:   senetinternal.h
 * Author: santony
 *
 * Created on November 1, 2012, 1:45 AM
 */

#ifndef SENETINTERNAL_H
#define	SENETINTERNAL_H

#include "senetapi.h"
#include "../seglobaltypedefs.h"
#include "../ReturnCodes.h"
#include <string>
#include "../seglobal/globallogger.h"
#include "../seglobal/CObjectRoot.h"

using namespace std;


#define CATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
        BERR << " Reported std error as " << ex.what();     \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        BERR << " Reported std error as " << ex.what();     \
        return E_UnknownError; \
    }                           \
    catch(...)                  \
    {                           \
        BERR << " Unknown error reported.";                     \
        return E_UnknownError;  \
    }                           


#endif	/* SENETINTERNAL_H */

