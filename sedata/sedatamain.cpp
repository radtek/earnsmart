
#include "sedataheaders.h"
#include "../seglobal/Object.h"
#include "CDataTableToJsonFormatter.h"

using namespace se;
using namespace se::data;

extern "C" 
{
    ErrorId ConvertDataTableToJson(ISession* ps, CDataTable* datatable, IDataFormattedOutput** jsondata)
    {
        SP<IDataFormatter> formatter;
        ErrorId ret;
        ret = CObject<CDataTableToJsonFormatter>::Create(ps, &formatter);

        if (S_Success == ret)
        {
            ret = formatter->Convert(datatable, jsondata);
        }

        return ret;
    }
    
}
