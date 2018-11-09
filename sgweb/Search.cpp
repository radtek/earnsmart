#include "models.h"
#include "commontypes.h"

namespace sg
{
    namespace web
    {
        namespace models
        {

            Search::Search(cppcms::http::context& ctxt) : BaseContent(ctxt, "SearchView")
            {
                
            }
            
            void Search::Init()
            {
            }
            
            void Search::SetList(IROSPList<ISecurityDescription>* pList)
            {
                _pList = pList;
            }
            
            IROSPList<ISecurityDescription>* Search::get_List()
            {
                return  _pList._ptr;
            }
            
        }
    }
}



