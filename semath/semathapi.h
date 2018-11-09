/* 
 * File:   semathapi.h
 * Author: santony
 *
 * Created on November 27, 2013, 8:43 AM
 */

#ifndef SEMATHAPI_H
#define	SEMATHAPI_H

#include "../seglobal/IObject.h"

#define MATHEXP __attribute__ ((visibility ("default")))


namespace se
{
    
    /*
     * Error codes are defined the following way in a 32-bit number (HRESULT)
     * 
     *  7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   
     *-------------------------------------------------------------------------
     *  E| |C|     | T | |     Facility    |               Code                |
     * 
     * Facility code is 1
     * T = {
     *      00 = Error
     *      01 = Warning
     *      10 = Information
     *      11 = Debug
     * }
     * 
     */
    //#define S_SampleInfo                            0x02070000      // a sample info message
    //#define W_SampleWarning                         0x01070000      // a sample warning
    //#define D_SampleDebug                           0x03070000      // a sample debug message.
    //#define E_SampleDebug                           0x80070000      // a sample error message.

#   define E_ANNTRAINEDRECORDNOTFOUND                   0x80070001      // a ANN trained record not found.
#   define E_GslMemoryError                             0x80070002  // Gsl memory creation exception.

    namespace math
    {
        enum class InterPolationTypes
        {
            Linear,
        };
        
        struct IInterpolations : public IObject
        {
            /*
             * Obtain a linearly interpolated value of y. 
             * @param y1: The start of y range
             * @param y2: The end of y range
             * @param x1: The start of x range
             * @param x2: The end of x range
             * @param x: The intermediate value of x
             * @returned: The intermediate interpolated value of y 
             */
            virtual R8 get_y(R8 y1, R8 y2, R8 x1, R8 x2, R8 x) = 0;
        };
        
        struct IMathApi : public IObject, public IErrorDescription
        {
            virtual SE_CODE get_InterpolationMethods(InterPolationTypes type, SIZE dimension, IInterpolations** ppOut) = 0;
        };
        
        SE_CODE MATHEXP MathOpen(ISession*, IMathApi** ppOut);        
    }
}

#endif	/* SEMATHAPI_H */

