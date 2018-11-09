/* 
 * File:   COptionPricing.cpp
 * Author: santony
 * 
 * Created on January 8, 2013, 11:55 AM
 */

#include "COptionPricing.h"
#include <cmath>
#include <complex>

COptionPricing::COptionPricing() {
}

COptionPricing::COptionPricing(const COptionPricing& orig) {
}

COptionPricing::~COptionPricing() {
}

double COptionPricing::dOne(double underlyingPrice, double exercisePrice, double time, double interest, double volatility, double dividend)
{
    return (std::log(underlyingPrice / exercisePrice) + (interest - dividend + .5 * std::pow(volatility, 2)) * time) / (volatility * time * time);
}

