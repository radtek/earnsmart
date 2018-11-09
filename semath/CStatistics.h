/* 
 * File:   CStatistics.h
 * Author: santony
 *
 * Created on June 16, 2014, 8:00 PM
 */

#ifndef CSTATISTICS_H
#define	CSTATISTICS_H

#include <vector>
#include <cmath>
using namespace std;

namespace se
{
    namespace math
    {
        typedef std::vector<R8> FloatVector;
        typedef std::vector<U32> UInt32Vector;
        typedef std::vector<I32> IInt32Vector;
        typedef std::vector<U64> UInt64Vector;
        typedef std::vector<I64> IInt64Vector;
        
        class CStatistics 
        {
        public:
            template<typename T>
            static R8 GetGeometricMean(const std::vector<T>& list)
            {
                R8 len = (R8)list.size();
                R8 logsum = 0.0;
                //for (auto it = list.begin(); it != list.end(); ++it)
                for (auto item : list)
                {
                    logsum += log( 1 + ((R8)item/100.00) );
                }
                logsum /= len;
                // inverse 
                R8 nroot = exp(logsum);
                return  (nroot - 1) * 100;
            }
        };
        
    }
}


#endif	/* CSTATISTICS_H */

