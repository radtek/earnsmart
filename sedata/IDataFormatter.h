/* 
 * File:   IDataFormatter.h
 * Author: santony
 *
 * Created on January 19, 2013, 9:25 PM
 */

#ifndef IDATAFORMATTER_H
#define	IDATAFORMATTER_H

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"
#include "IDataFormattedOutput.h"
#include "CDataField.h"
#include "IDataProvider.h"

namespace se
{
    namespace data
    {
        struct IDataFormatter : public IObject
        {
            virtual ErrorId Convert(IDataProvider* input, IDataFormattedOutput** output) = 0;
        };
    }
}



#endif	/* IDATAFORMATTER_H */

