/* 
 * File:   IDataProvider.h
 * Author: santony
 *
 * Created on January 19, 2013, 10:55 PM
 */

#ifndef IDATAPROVIDER_H
#define	IDATAPROVIDER_H

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"

namespace se
{
    namespace data
    {
        struct IDataProvider // : public IObject
        {
            // temporary virtual function to enable polymorphism
            virtual CSTR ProviderName() = 0;
        };
    }
}



#endif	/* IDATAPROVIDER_H */

