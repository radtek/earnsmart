/* 
 * File:   CCommon.h
 * Author: santony
 *
 * Created on November 14, 2013, 9:16 AM
 */

#ifndef CCOMMON_H
#define	CCOMMON_H
#include <vector>
#include "../seglobaltypedefs.h"
#include <cmath>

using namespace std;

namespace se
{
    namespace math
    {
        class CCommon 
        {
        public:
            CCommon();
            CCommon(const CCommon& orig);
            virtual ~CCommon();
            
            R8 RoundUp(R8 value, I16 positions);
            R8 RoundNear(R8 value, I16 positions);
            R8 RoundDown(R8 value, I16 positions);
            
            template<typename T>
            bool CompareFloats(T left, T right, I16 precision)
            {
                I64 ival = left * std::pow<I64>(10, precision);
                I64 inval = right * std::pow<I64>(10, precision);
                return ival == inval;
            }
            
            template<typename iter>
            R8 HarmonicMean(iter from, iter to)
            {
                // compile error when attempting to do an operation ot supported
                //typename iterator_traits<iter>::value_type t;
                static_assert(std::is_floating_point<typename iter::value_type>::value, "The type is not a floating point");
                
                // loop
                R8 size = 0;
                R8 inverseTotal ;
                for (iter it = from; it != to; ++it)
                {
                    inverseTotal += 1 / *it;
                    size++;
                }
                return size / inverseTotal;
            }
            
        private:

        };
        
    }
}


#endif	/* CCOMMON_H */

