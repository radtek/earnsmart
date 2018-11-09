/* 
 * File:   CFinancialCalculator.cpp
 * Author: santony
 * 
 * Created on September 6, 2014, 10:06 AM
 */


#include "semathapi.h"
#include "CFinancialCalculator.h"
#include <cmath>

using namespace std;

namespace se
{
    namespace math
    {

        long unsigned int CFinancialCalculator::FutureValueOfSingleCashflow(double presentValue, double interestRate, double numberOfPeriods, double* futureValue) 
        {
            // formula
            // FV = PV * (1+r)^n
            if (presentValue <= 0 || interestRate <= 0 || numberOfPeriods <= 0)
                return E_InvalidArg;
            
            *futureValue = presentValue * std::pow( (1 + interestRate), interestRate );
           
            return S_OK;
            
        }

        long unsigned int CFinancialCalculator::FutureValueOfSingleCashflow(double presentValue, double statedAnnualInterestRate, double numberOfYears, PERIOD periodType, double* futureValue) 
        {
            switch(periodType)
            {
                case PERIOD_Annually:
                    return FutureValueOfSingleCashflow(presentValue, statedAnnualInterestRate, numberOfYears, futureValue);
                case PERIOD_SemiAnnually:
                {
                    R8 numPer = numberOfYears * 2;
                    R8 rate = statedAnnualInterestRate / 2;
                    return FutureValueOfSingleCashflow(presentValue, rate, numPer, futureValue);
                }
                case PERIOD_Quarterly:
                {
                    R8 numPer = numberOfYears * 4;
                    R8 rate = statedAnnualInterestRate / 4;
                    return FutureValueOfSingleCashflow(presentValue, rate, numPer, futureValue);
                }
                case PERIOD_Monthly:
                {
                    R8 numPer = numberOfYears * 12;
                    R8 rate = statedAnnualInterestRate / 12;
                    return FutureValueOfSingleCashflow(presentValue, rate, numPer, futureValue);
                }
                case PERIOD_Weekly:
                {
                    R8 numPer = numberOfYears * 52;
                    R8 rate = statedAnnualInterestRate / 52;
                    return FutureValueOfSingleCashflow(presentValue, rate, numPer, futureValue);
                }
                case PERIOD_Daily:
                {
                    R8 numPer = numberOfYears * 365;
                    R8 rate = statedAnnualInterestRate / 365;
                    return FutureValueOfSingleCashflow(presentValue, rate, numPer, futureValue);
                }
                default:
                    return E_NotSupported;
            }
            
        }
        
        SE_CODE CFinancialCalculator::FutureValueOfSingleCashflowContinuousCompounding(R8 presentValue, R8 statedAnnualInterestRate, R8 numberOfYears, R8* futureValue)
        {
            // formula used FV = PV * e^(rN)
            if (presentValue <= 0 or statedAnnualInterestRate <= 0 || numberOfYears <= 0)
                return E_InvalidArg;

            *futureValue = presentValue * exp(statedAnnualInterestRate * numberOfYears);

            return S_OK;
        }

        long unsigned int CFinancialCalculator::GetEffectiveRate(double quotedInterestRate, PERIOD compoundingPeriods, R8* earValue) 
        {
            if (quotedInterestRate <= 0)
                return E_InvalidArg;
                
            R8 power;
            switch (compoundingPeriods)
            {
                case PERIOD_Annually:
                    power = 1;
                    break;
                case PERIOD_SemiAnnually:
                    power = 2;
                    break;
                case PERIOD_Quarterly:
                    power = 4;
                    break;
                case PERIOD_Monthly:
                    power = 12;
                    break;
                case PERIOD_Weekly:
                    power = 52;
                    break;
                case PERIOD_Daily:
                    power = 365;
                    break;
                case PERIOD_Continuous:
                    *earValue = exp(quotedInterestRate) - 1;
                    return S_OK;
                default:
                    return E_NotSupported;
            }
            *earValue = pow(quotedInterestRate, power);
            return S_OK;
        }
    }
    
}