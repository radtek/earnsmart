#ifndef INTERNAL_H_INCLUDED
#define INTERNAL_H_INCLUDED

#include "sesecuritiesapi.h"
#include "../seglobal/Object.h"
#include "../seglobal/Object.h"
#include "../semysql/CSEConnections.h"
#include "../external/mariadbpp/mysql++.h"
#include "../external/mariadbpp/ssqls.h"
#include "../seglobal/globallogger.h"

#ifdef __DEBUG__
#define RTRY    
#else
#define RTRY try {
#endif


#define CATCHCLAUSE             \
    catch(seexception& ex)      \
    {                           \
        BERR << "Reported seexception error as " << ex.what();  \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        BERR << "Reported std error as " << ex.what();  \
        return E_DBQueryException;  \
    }                           \
    catch(SE_CODE errorCode)    \
    {   \
        BERR << get_CodeMessage(errorCode); \
        return errorCode;   \
    }   \
    catch(...)                  \
    {                           \
        BERR << "Reported an unknown error";  \
        return E_UnknownError;  \
    }                           

#define RBCATCHCLAUSE(fname)             \
    catch(seexception& ex)      \
    {                           \
        BDBG << "Rolling back"; \
        conn.RollbackTransaction(); \
        BERR << "Reported seexception error as " << ex.what();  \
        return ex.error();      \
    }                           \
    catch(std::exception& ex)   \
    {                           \
        BDBG << "Rolling back"; \
        conn.RollbackTransaction(); \
        BERR << "Reported std error as " << ex.what();  \
        return E_UnknownError;  \
    }                           \
    catch(SE_CODE errorCode)    \
    {   \
        BERR << get_CodeMessage(errorCode); \
        return errorCode;   \
    }   \
    catch(...)                  \
    {                           \
        BDBG << "Rolling back"; \
        conn.RollbackTransaction(); \
        BERR << "Reported an unknown error";  \
        return E_UnknownError;  \
    }                           


#ifdef __DEBUG__
#define RCATCH(fname)
#else
#define RCATCH(fname) } CATCHCLAUSE(fname)
#endif


namespace se
{
    namespace securities
    {
        extern "C"
        {

        /* Internally : close and clean up the resources used by the gateway. this method is called from destructor of CSecurityAPI instance */
        ErrorId SecClose(se::securities::ISecurityAPI* ppGateway) ;

        }
    }
}
#endif // INTERNAL_H_INCLUDED
