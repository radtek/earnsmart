/* 
 * File:   seeoddata.cpp
 * Author: santony
 *
 * Created on February 1, 2015, 7:46 PM
 */

#include <cstdlib>

#include "seeoddataint.h"
#include "CEodData.h"

using namespace std;
using namespace se;
using namespace se::eoddata;

extern "C" SE_CODE EODDATAEXP EodDataOpen(ISession* ps, IEODData** ppGateway)
{

    if (!ps)
    {
        BERR << "Session not provided";
        return E_InvalidArg;
    }
           

    return CObject<CEodData>::Create(ps, ppGateway);

}


#include <stdsoap2.h>
extern "C" {
SOAP_NMAC struct Namespace namespaces[] = { { NULL, NULL} };
}

