/* 
 * File:   IDataFormattedOutput.h
 * Author: santony
 *
 * Created on January 19, 2013, 9:27 PM
 */

#ifndef IDATAFORMATTEDOUTPUT_H
#define	IDATAFORMATTEDOUTPUT_H

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"

namespace se
{
    namespace data
    {
        typedef enum enDataFormatOutputTypes
        {
            DFOT_JSON       // output formatted as json.
        } DATAFORMATOUTPUTTYPES;
        
        struct IDataFormattedOutput : public IObject
        {
            virtual ErrorId ToString(IString** outVal) = 0;
        };
    }
}


#endif	/* IDATAFORMATTEDOUTPUT_H */

