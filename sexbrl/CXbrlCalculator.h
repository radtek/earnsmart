/* 
 * File:   CXbrlCalculator.h
 * Author: santony
 *
 * Created on February 15, 2014, 6:39 PM
 */

#ifndef CXBRLCALCULATOR_H
#define	CXBRLCALCULATOR_H

#include "sexbrlapi.h"


namespace se
{
    namespace xbrl
    {
        double Calculate(ISession* ps, IFinancialsConsolidated* psrc, const char* mathstring);
    }
}

#endif	/* CXBRLCALCULATOR_H */

