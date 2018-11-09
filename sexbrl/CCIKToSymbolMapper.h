/* 
 * File:   CCIKToSymbolMapper.h
 * Author: santony
 *
 * Created on July 8, 2012, 11:49 PM
 */

#ifndef CCIKTOSYMBOLMAPPER_H
#define	CCIKTOSYMBOLMAPPER_H

#include "../seglobaltypedefs.h"
#include <string>
#include "../seglobal/IObject.h"

using namespace se;
using namespace std;
using namespace se::xbrl;
////using namespace log4cxx;

/** Contains Arun's two tasks put together.
 1. Download a specific webpage from SEC website
 2. Using regex, capture the CIK from the page.*/

class CCIKToSymbolMapper 
{
public:
    CCIKToSymbolMapper();
    CCIKToSymbolMapper(const CCIKToSymbolMapper& orig);
    virtual ~CCIKToSymbolMapper();
    
public:
    // Always read company info from edgar website.
    ErrorId GetCompanyDetailsFromEdgar(CSTR symbolOrCik, IPublicCompanyInfo** ppOut);
    SE_CODE GetCIKFromSymbol(CSTR symbol, IString** ppOut, bool cache=false);
    bool GetSymbolFromCIK(CIK cik, se::IString** symbol);
    void UpdateCIKForAllSymbols() ;
    
private:
    
    /* reads the cik for the symbol (1st param) from the local 
     * table. If found, returns true and outCik will be set 
     * with the cik value. If not, returns false, Cik will be
     * set to zero.
     */
    bool CheckGetCachedCIK(const string& symbol, string& outCik);
    
    /* saves the CIK into the table for the security symbol. */
    void CacheCIKForSymbol(const string& symbol, CIK cik);

private:
//    //LoggerPtr _logger;

};

#endif	/* CCIKTOSYMBOLMAPPER_H */

