/* 
 * File:   CMisc.cpp
 * Author: santony
 * 
 * Created on June 21, 2014, 10:10 AM
 */

#include "seglobalinternal.h"
#include "CMisc.h"
#include <cmath>
#include <iomanip>

using namespace std;

namespace se
{
    R8 CMisc::GetAverageForMissingData(const DoubleList& list)
    {
        if (!list.size())
             return 0.0;

         R8 sum;
         for (const R8 v : list)
         {
             sum += v;
         }
         return sum / list.size();
        
    }

    void CMisc::ManageValuesForMissingData(DoubleList& list, R8 value)
    {
        if (list.size() == 3)
        {
            // discard item from the top
            list.pop_front();
        }
        list.push_back(value);
    }
    
    
    std::ostream& CMisc::FormatFinancialNumber(std::ostream& os, R8 value)
    {
        int lg = (int)std::log10(std::abs(value));
        if (lg >= 9)
        {
            int prevprec = os.precision();
            os.precision(numeric_limits<double>::digits10);
            value = value / 1e9;
            os << std::setprecision(3) << value << " Billion";
            os.precision(prevprec);
        }
        else if (lg >= 6)
        {
            int prevprec = os.precision();
            os.precision(numeric_limits<double>::digits10);
            value = value / 1e6;
            os << std::setprecision(3) << value << " Million";
            os.precision(prevprec);
        }
        else
        {
            os << value;
        }
        return os;
    }    
}
