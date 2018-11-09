#include "models.h"
#include "../seglobal/exceptions.h"

namespace sg
{
    namespace web
    {
        namespace models
        {
            Fundamentals::Fundamentals(cppcms::http::context& ctxt, string const& subViewName) 
                : BaseContent(ctxt, "FundamentalsView"), SubViewName(subViewName)
            {
            }
            
            void Fundamentals::Init(UID productId)
            {
                ProductId = productId;
                SP<ISecurityAPI> pApi;
                SecOpen(&pApi);
                pApi->get_Security(productId, &_pSec);
            }
            
            ISecurityDescription * Fundamentals::get_SecurityInfo()
            {
                if (_pSec && !_pSecDesc)
                    THROWIFFAILED(_pSec->get_SecurityDescription(&_pSecDesc), "Failed to obtain security description.");
                return _pSecDesc;
            }

            IPublicCompanyInfo* Fundamentals::get_CompanyInfo() 
            {
                get_SecurityInfo();
                if (_pSec && !_pCo)
                    THROWIFFAILED(_pSec->get_Company(&_pCo), "Failed to obtain company details.");
                return _pCo;
            }

            ISecurity* Fundamentals::get_Security() 
            {
                return _pSec;
            }

            IGICInfo* Fundamentals::get_GicInfo() 
            {
                if (!_pgic)
                {
                    get_CompanyInfo();
                    if (_pCo)
                        THROWIFFAILED(_pCo->get_GICInfo(&_pgic) , "Failed to obtain company sector/industry details.");
                }
                return _pgic;
            }

            
            //////////////////////////CompanyNews///////////////////////////////
            CompanyNews::CompanyNews(cppcms::http::context& httpctxt) : Fundamentals(httpctxt, "CompanyNewsView")
            {
                
            }

            void CompanyNews::Init(UID ProdId)
            {
                Fundamentals::Init(ProdId);
                _pNews.Clear();
                HasNews = false;
                if (!_pNews)
                {
                    ISecurity* pSec = get_Security();
                    if (pSec)
                    {
                        if (ISOK(pSec->get_News(&_pNews)) && _pNews->get_Count())
                            HasNews = true;
                    }
                }
            }

            ISecurityNewsList* CompanyNews::get_News()
            {
                return _pNews;
            }
        }
    }
}

