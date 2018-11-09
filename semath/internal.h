/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   internal.h
 * Author: santony
 *
 * Created on December 18, 2016, 10:44 AM
 */

#ifndef INTERNAL_H
#define INTERNAL_H

#include "../seglobal/IObject.h"
#include "../seglobal/CObjectRoot.h"
#include "../seglobal/Object.h"
#include "semathapi.h"

#include "../semysql/CSEConnections.h"
#include "../seglobal/exceptions.h"
#include "../seglobal/globallogger.h"

#define SEDBCONN CSEConnections::GetMain()

namespace se
{
    namespace math
    {
        typedef void (*GSLERRHANDLER) (
            const char * , 
            const char * , 
            int , 
            int 
        );
        
        extern "C" void Gsl_ErrorHandler (
            const char * reason, 
            const char * file, 
            int line, 
            int gsl_errno
        );
        
        extern GSLERRHANDLER currentGslErrHandler;

    }
}

#endif /* INTERNAL_H */

