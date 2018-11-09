// The functions contained in this file are pretty dummy
// and are included only as a placeholder. Nevertheless,
// they *will* get included in the shared library if you
// don't remove them :)
//
// Obviously, you 'll have to write yourself the super-duper
// functions to include in the resulting library...
// Also, it's not necessary to write every function in this file.
// Feel free to add more files in this project. They will be
// included in the resulting library.


#include "internal.h"
#include "SecurityAPI.h"
#include "../seglobal/SecurityDescription.h"
#include <Poco/Mutex.h>
#include <Poco/ScopedLock.h>

namespace se
{
    namespace securities
    {
        extern "C"
        {
            ErrorId SECEXP SecOpen(ISession* ps, MARKETDATAPROVIDERS defMarketDataProvider, ISecurityAPI** ppGateway)
            {
                CSecurityAPI::__loadMessages__(
                {
                    CSecurityAPI::CodeMessageItem(E_SecHistoryPriceUnavailable, "Historical price for security not found in db"),
                    CSecurityAPI::CodeMessageItem(E_ExchNotFound, "Exchange code or name is not found in our repository.")
                });
                if (!ps)
                {
                    BERR << "Session not provided";
                    return E_InvalidArg;
                }
                return CObject<CSecurityAPI>::Create(ps, defMarketDataProvider, ppGateway);
                
            }
           
            SE_CODE SECEXP CreateSecurityDescription(ISession* ps, ISecurityDescription** ppout)
            {
                return ISecurityDescription::Create(ps, ppout);
            }
        }
    }
}

