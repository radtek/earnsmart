
#include "seecconinternal.h"
#include "CEconomyAPI.h"

namespace se
{
    namespace economy
    {

        SE_CODE ECONOMYEXP EconOpen(ISession* ps, IEconomyApi** ppGateway)
        {
            if (!ps)
            {
                BERR << "Session not provided";
                return E_InvalidArg;
            }
            return CObject<CEconomyAPI>::Create(ps, ppGateway);
                
        }
        
    }
}

