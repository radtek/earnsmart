#include "models.h"

namespace sg
{
    namespace web
    {
        namespace models
        {
            CompanyPicks::CompanyPicks()
            {
            }
            
            CompanyPicks::CompanyPicks(
                cppcms::http::context& ctxt, 
                    string const& subviewName) : BaseContent(ctxt, "CompanyPicksView"),
                    SubViewName(subviewName)
            {
            }

            void CompanyPicks::Init() 
            {
                InternalInit();
            }

            void CompanyPicks::Init(cppcms::http::context& httpctxt) 
            {
                BaseContent::Init(httpctxt, "CompanyPicksView");
                InternalInit();
            }

            
            void CompanyPicks::Init(cppcms::http::context& httpctxt, const string& viewName)
            {
                BaseContent::Init(httpctxt, viewName);
                InternalInit();
            }

            void CompanyPicks::InternalInit() 
            {
                SE_CODE r = XbrlOpen(&_pxbrl);
                if (ISOK(r))
                {
                    r = SecOpen(&_psecapi);
                    if (ISOK(r))
                    {
                        r = _psecapi->get_Exchanges(&_pexchanges);
                    }
                }
                if (FAILED(r))
                {
                    this->set_Error("Failed invocation of gateway interface.");
                }
            }

            
            ISecurityExchangeList* CompanyPicks::get_Exchanges()
            {
                return _pexchanges._ptr;
            }

            
            ///////////////////////Company Screener///////////////////////////
            
            CompanyScreener::CompanyScreener() : GicId(0), ScreeningMode(Default)
            {
                
            }
            
            CompanyScreener::CompanyScreener(cppcms::http::context& httpctxt)
                :   CompanyPicks(httpctxt, "CompanyScreenerView"), GicId(0), ScreeningMode(Default)
            {
                
            }

            void CompanyScreener::Init() 
            {
                CompanyPicks::Init();
            }


            void CompanyScreener::Init(cppcms::http::context& httpctxt) 
            {
                GicId = 0;
                ScreeningMode = Default;
                CompanyPicks::Init(httpctxt);
            }

        }
    }
}
