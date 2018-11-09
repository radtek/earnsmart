/* 
 * File:   CInvestmentProducts.h
 * Author: santony
 *
 * Created on October 20, 2015, 10:42 PM
 */

#ifndef CINVESTMENTPRODUCTS_H
#define	CINVESTMENTPRODUCTS_H

#include <cppcms/application.h>
#include "../sesecurities/sesecuritiesapi.h"

using namespace se;
using namespace se::securities;

class CInvestmentProducts : public cppcms::application
{
public:
    CInvestmentProducts(cppcms::service& srv);
    CInvestmentProducts(const CInvestmentProducts& orig) = delete;
    virtual ~CInvestmentProducts();
    
private:
    
    void Stocks();
    void Options();
    void StockCharts(ISecurityDescription* pDesc);

};

#endif	/* CINVESTMENTPRODUCTS_H */

