#include "models.h"

namespace sg
{
    namespace web
    {
        namespace models
        {
            Header::Header()
            {
                
            }

            Header::Header(cppcms::http::context& ctxt) : BaseContent(ctxt), compSearch(ctxt)
            {
                
            }
            
            void Header::Init()
            {
                compSearch.Init();
            }

            void Header::Init(cppcms::http::context& httpctxt) 
            {
                compSearch.Init(httpctxt);
            }
        }
    }
}

