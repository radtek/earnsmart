/* 
 * File:   CFinancialCalculator.h
 * Author: santony
 *
 * Created on September 6, 2014, 10:06 AM
 */

#ifndef CFINANCIALCALCULATOR_H
#define	CFINANCIALCALCULATOR_H

namespace se
{
    namespace math
    {

        class CFinancialCalculator 
        {
        public:
            // Find the future value at period t1 of a single cash flow (a single lump-sum investment) at period t0.
            // The interest rate must be the rate per compounding period. 
            // eg: if rate is annual, then numberOfPeriods is the number of years.
            // if the rate is daily, then numberOfPeriods = 365 * Number o Years.
            // Note: interest rate is a rate, not a percentage (ie., if interest rate is 8%, the value passed
            // as argument is .08).
            static SE_CODE FutureValueOfSingleCashflow(R8 presentValue, R8 interestRate, R8 numberOfPeriods, R8* futureValue); 

            // Get the future value of a single lump-sum investment after given number of periods and interest rate.
            // Note that period is converted based on the given interest rate the period type.
            // Eg: if interest rate is monthly, then periodType is set to Monthly. The numberOfYears will be 
            // scaled to the required number of periods, ie., 12 * numberOfYears. Since the interest rate is 
            // quoted in stated annual interest rate, the statedAnnualInterestRate is divided by 12.
            // Note: interest rate is a rate, not a percentage (ie., if interest rate is 8%, the value passed
            // as argument is .08).
            static SE_CODE FutureValueOfSingleCashflow(R8 presentValue, R8 statedAnnualInterestRate, R8 numberOfYears, PERIOD periodType, R8* futureValue); 
            
            // Same as above, but the compounding period is infinitely large (such compounding every milli-second!)
            // Note: interest rate is a rate, not a percentage (ie., if interest rate is 8%, the value passed
            // as argument is .08).
            static SE_CODE FutureValueOfSingleCashflowContinuousCompounding(
                R8 presentValue, R8 statedAnnualInterestRate, R8 numberOfYears, R8* futureValue);
            
            // calculate an effective annual interest rate from quoted or stated annual interest rate.
            // example if quoted rate is 8% for an investment compounded quarterly, the effective 
            // annual rate is not same as PV * (1+0.08), it is PV * (1+)
            // Note: interest rate is a rate, not a percentage (ie., if interest rate is 8%, the value passed
            // as argument is .08).
            SE_CODE GetEffectiveRate(R8 quotedInterestRate, PERIOD compoundingPeriods, R8* earValue);

        private:

        };

    }
    
}
        
#endif	/* CFINANCIALCALCULATOR_H */

