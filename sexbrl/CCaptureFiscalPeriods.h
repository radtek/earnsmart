/* 
 * File:   CCaptureFiscalPeriods.h
 * Author: santony
 *
 * Created on August 22, 2014, 7:28 PM
 */

#ifndef CCAPTUREFISCALPERIODS_H
#define	CCAPTUREFISCALPERIODS_H

class CCaptureFiscalPeriods 
{
public:
    static SE_CODE Capture(CIK cik, IFinancialPeriod** ppOut);
};

#endif	/* CCAPTUREFISCALPERIODS_H */

