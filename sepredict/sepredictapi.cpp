/* 
 * File:   sepredictapi.cpp
 * Author: santony
 *
 * Created on May 9, 2015, 3:48 PM
 */

#include <cstdlib>
#include "sepredictinternal.h"
#include "CPredictor.h"
#include "CProductDescription.h"

#define CONN se::data::CSEConnections::GetMain()

using namespace std;

namespace se
{
    namespace predict
    {
        
        extern "C"
        {
            SE_CODE PREDICTEXP Create(ISession* ps, IProductDescription* pd, IPredictor** ppGateway)
            {
                return CObject<CPredictor>::Create(ps, pd, ppGateway);
            }
         
            SE_CODE PREDICTEXP ListAttributes(ISession* ps, IAttributeDescriptionList** ppOut, QUERYFUNC(IAttributeDescriptionBase) filter)
            {
                return E_NotImpl;
            }
        }
    }
}