/* 
 * File:   COptionPricing.h
 * Author: santony
 *
 * Created on January 8, 2013, 11:55 AM
 */

#ifndef COPTIONPRICING_H
#define	COPTIONPRICING_H

class COptionPricing 
{
public:
    COptionPricing();
    COptionPricing(const COptionPricing& orig);
    virtual ~COptionPricing();
    
    double dOne(double underlyingPrice, double exercisePrice, double time, double interest, double volatility, double dividend);
    
private:

};

#endif	/* COPTIONPRICING_H */

