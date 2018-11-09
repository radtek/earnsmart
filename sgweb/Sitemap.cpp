#include "sgwebinternal.h"
#include "models.h"
#include "../semysql/CStoreQueryResult.h"
#include "../seglobal/exceptions.h"
#include "../semysql/CSEConnections.h"

namespace sg
{
    namespace web
    {
        namespace models
        {
            Sitemap::Sitemap(cppcms::http::context& httpctxt):BaseContent(httpctxt, "SitemapView")
            {
                
            }
            
            void Sitemap::Init() 
            {

            }

            IStoreQueryResult* Sitemap::get_Products() 
            {
                if (!_pProducts)
                {
                    // direct query
                    if (FAILED(IStoreQueryResult::GetDataTable(
                            "select * FROM sa.list_ListedCompanies order by product_name;",
                            &_pProducts)))
                    {
                        //LoggerPtr logger = Logger::getLogger("sg.web.models.sitemap");
                       // LOGE(logger, "Failed to get product list for printing out in sitemap page.");
                    }
                }
                return _pProducts;
            }
        }
    }
}
