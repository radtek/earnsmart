/* 
 * File:   internal.h
 * Author: santony
 *
 * Created on March 17, 2013, 1:47 PM
 */

#ifndef INTERNAL_H
#define	INTERNAL_H

#include "../seglobal/Object.h"
#include "seadminapi.h"
#include "../seglobal/exceptions.h"
#include "../seglobal/globallogger.h"

using namespace se;

#define CATCHCLAUSEAUTO             \
    catch(const seexception& ex)      \
    {                           \
        BERR << ex.what() ; \
        return ex.error();      \
    }                           \
    catch(const std::exception& ex)   \
    {                           \
        BERR << ex.what() ; \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        BERR << "Unknown error" ; \
        return E_UnknownError;  \
    }                           

#define CATCHCLAUSE(fname)             \
    catch(const seexception& ex)      \
    {                           \
        BERR << ex.what() ; \
        return ex.error();      \
    }                           \
    catch(const std::exception& ex)   \
    {                           \
        BERR << ex.what() ; \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        BERR << "Unknown error" ; \
        return E_UnknownError;  \
    }                           

#define CATCHLOG()             \
    catch(const seexception& ex)      \
    {                           \
        BERR << ex.what() ; \
        return ex.error();      \
    }                           \
    catch(const std::exception& ex)   \
    {                           \
        BERR << ex.what() ; \
        return E_UnknownError;  \
    }                           \
    catch(...)                  \
    {                           \
        BERR << "Unknown error" ; \
        return E_UnknownError;  \
    }                           

#define RBCATCHCLAUSE(fname)             \
    catch(const seexception& ex)      \
    {                           \
        conn.RollbackTransaction(); \
        BERR << ex.what() ; \
        return ex.error();      \
    }                           \
    catch(const mysqlpp::Exception& ex)   \
    {                           \
        conn.RollbackTransaction(); \
        BERR << ex.what() ; \
        return E_DBUpdateError;  \
    }                           \
    catch(const std::exception& ex)   \
    {                           \
        conn.RollbackTransaction(); \
        BERR << ex.what() ; \
        return E_DBUpdateError;  \
    }                           \
    catch(...)                  \
    {                           \
        conn.RollbackTransaction(); \
        BERR << "Unknown error" ; \
        return E_UnknownError;  \
    }


#define THRWITHBOOSTERLOG( func, message)  \
    if (auto ecode = FAILED(func))  \
    {   \
        seexception ex(ecode, message); \
        BERR << ex.what(); \
        throw ex;   \
    }   
    

#endif	/* INTERNAL_H */

