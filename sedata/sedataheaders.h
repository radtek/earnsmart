/* 
 * File:   sedataheaders.h
 * Author: santony
 *
 * Created on December 8, 2012, 4:25 PM
 */

#ifndef SEDATAHEADERS_H
#define	SEDATAHEADERS_H

#include "../seglobaltypedefs.h"
#include "../seglobal/CObjectRoot.h"
#include "CDataField.h"
#include "CDataFieldList.h"
#include "CDataTable.h"
#include "CDataRow.h"
#include "CDataRowList.h"
#include "CDataset.h"
#include "IDataFormatter.h"
#include "IDataProvider.h"
#include "IDataFormattedOutput.h"
#include "IIndicator.h"

namespace se
{
    namespace data
    {
        extern "C" 
        {
            ErrorId ConvertDataTableToJson(ISession*, CDataTable* datatable, IDataFormattedOutput** jsondata);
        }
    }
}



#endif	/* SEDATAHEADERS_H */

