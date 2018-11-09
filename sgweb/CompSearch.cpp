#include "models.h"
#include "../sesecurities/sesecuritiesapi.h"
#include "../seglobal/exceptions.h"

using namespace se;
using namespace se::securities;

namespace sg
{
    namespace web
    {
        namespace models
        {
            CompSearch::CompSearch()
            {
                
            }

            CompSearch::CompSearch(cppcms::http::context& ctxt) : BaseContent(ctxt)
            {
            }
            
            void CompSearch::Init()
            {
                
            }

            void CompSearch::Init(cppcms::http::context& httpctxt) 
            {
                this->httpctxt = &httpctxt;
            }

            ISecurityDescriptionList* CompSearch::getRecentPicks(IAdminAPI* papi) 
            {
                if (!papi)
                {
                    THROWIFFAILED(AdminOpen(&papi), "Failed to open admin gateway.");
                }
                SP<IWebSession> pWebSession;
                papi->get_WebSession(this->httpctxt->session().get_session_cookie().c_str(), &pWebSession);
                if (pWebSession)
                {
                    SP<ISecurityDescriptionList> plist;
                    if (FAILED(pWebSession->get_ProductFinds(&plist, 20)))
                        return nullptr;
                    return plist;
                }
                return nullptr;
            }
        }
    }
}


