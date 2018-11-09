/* 
 * File:   CMisc.h
 * Author: santony
 *
 * Created on June 21, 2014, 10:10 AM
 */

#ifndef CMISC_H
#define	CMISC_H

#include <list>

namespace se
{

    class CMisc 
    {
    public:
        typedef std::list<R8> DoubleList;
    
    public:

        static R8 GetAverageForMissingData(const DoubleList&);
        
        static void ManageValuesForMissingData(DoubleList&, R8 value);
        
        static std::ostream& FormatFinancialNumber(std::ostream& os, R8 value);

    private:

    };

}
#endif	/* CMISC_H */

