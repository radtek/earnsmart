#include "models.h"

#include "sgwebinternal.h"
#include "../seglobal/CJsonSerializedData.h"

namespace sg
{
    namespace web
    {
        namespace models
        {
            CompanySummary::CompanySummary(cppcms::http::context& ctxt) : 
                Fundamentals(ctxt, "CompanySummaryView")
            {
                //_logger = Logger::getLogger("sg.web.companysummary");
            }
            
            void CompanySummary::Init(unsigned long productId)
            {
                Fundamentals::Init(productId);
            }
        }
    }
}

