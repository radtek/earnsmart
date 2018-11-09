#include "models.h"
#include "commontypes.h"

namespace sg
{
    namespace web
    {
        namespace models
        {
            Subscriptions::Subscriptions(cppcms::http::context& ctxt) : BaseContent(ctxt, "SubscriptionsView")
            {
                
            }
            
            void Subscriptions::Init()
            {
            }
        }
    }
}
