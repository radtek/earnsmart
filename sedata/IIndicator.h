/* 
 * File:   IIndicator.h
 * Author: santony
 *
 * Created on July 26, 2013, 12:20 AM
 */

#ifndef IINDICATOR_H
#define	IINDICATOR_H

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"

namespace se
{
    namespace data
    {
        struct IPeriodIndication : public IObject, public ISerialization
        {
            virtual DATETIME get_DateTime() = 0;
            virtual R8 get_Value() = 0;
        };
        
//        struct IIndicatorHistory : public IList<IPeriodIndication>
//        {
//            virtual ErrorId get_Indicator(DATETIME period, IPeriodIndication** ppOut) = 0;
//        };
        
//        struct IIndicator : public IObject, public ISerialization
//        {
//            virtual CSTR get_Name() = 0;
//            virtual CSTR get_Description() = 0;
//            virtual R8 get_CurrentValue() = 0;
//            virtual R8 get_Value(DATAPERIOD& period) = 0;
//            virtual ErrorId get_History(IIndicatorHistory** ppOut) = 0;
//        };
//        
//        struct IIndicatorList : public IList<IIndicator>
//        {
//            virtual ErrorId Find(CSTR name, IIndicator** ppOut) = 0;
//            virtual ErrorId Find(UID id, IIndicator** ppOut) = 0;
//        };
    }
}


#endif	/* IINDICATOR_H */

