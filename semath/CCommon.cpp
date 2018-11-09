/* 
 * File:   CCommon.cpp
 * Author: santony
 * 
 * Created on November 14, 2013, 9:16 AM
 */

#include "CCommon.h"
#include <gsl/gsl_math.h>
#include <sys/param.h>
#include <cmath>
#include <complex>

namespace se
{
    namespace math
    {

        CCommon::CCommon() 
        {
        }

        CCommon::CCommon(const CCommon& orig) 
        {
        }

        CCommon::~CCommon() 
        {
        }

        double CCommon::RoundNear(double value, short positions) 
        {
            I64 pos = std::pow(10, positions);
            I64  ivalue = value * pos;
            return floor(ivalue + 0.5) / pos;
        }
        
        double CCommon::RoundUp(double value, short positions) 
        {
            I64  pos = std::pow(10, positions);
            return ceil(value * pos) / pos;

        }

        double CCommon::RoundDown(double value, short positions) 
        {
            I64 pos = std::pow(10, positions);
            return floor(value * pos) / pos;
        }
        
    }
}
