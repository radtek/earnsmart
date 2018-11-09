
#include "models.h"

namespace sg
{
    namespace web
    {
        namespace models
        {
            About::About(cppcms::http::context& ctxt) : BaseContent(ctxt, "AboutView"), Done(false), StatusLabelClass("statuslabelhidden")
            {
            }
        }
    }
}


