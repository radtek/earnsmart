#include "models.h"
#include "commontypes.h"

namespace sg
{
    namespace web
    {
        namespace models
        {

            Home::Home(cppcms::http::context& ctxt) : BaseContent(ctxt, "HomeView")
            {
                
            }
            
            void Home::Init(UID ProdId)
            {
                ProductId = ProdId;
            }
        }
    }
}


