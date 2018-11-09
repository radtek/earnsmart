#include "models.h"
#include "sgwebinternal.h"
#include "../seglobal/exceptions.h"

namespace sg
{
    namespace web
    {
        namespace models
        {
            CompanyPerformance::CompanyPerformance(cppcms::http::context& ctxt) : Fundamentals(ctxt, "CompanyPerformanceView")
            {
                //_logger = Logger::getLogger("sg.web.companyperformance");
            }

            void CompanyPerformance::Init(unsigned long prodId) 
            {
                try
                {
                    Fundamentals::Init(prodId);
                    SP<IXbrl> papi;
                    THROWIFFAILED(XbrlOpen(&papi), "xbrl api cannot be obtained.");
                    SP<IString> pcik;
                    if (S_False == papi->get_CIK(prodId, &pcik))
                        throw seexception(E_NotFound, "CIK not found.");
                    SP<IFinancialPerformance> pperf;
                    THROWIFFAILED(papi->GetCachedFinancialPerformance(pcik->get_Buffer(), true, &pperf), "Failed to get performance instance.");
                    if (ISOK(pperf->get_Ratios(&_pratiolist)))
                        FinYearEnd = this->get_CompanyInfo()->get_FinancialYearEnd().get_Year();
                    if (!_pratiolist || !_pratiolist->get_Count())
                    {
                        SMTPWRITE("Performance ratios not found for cik " << pcik->get_Buffer());
                    }
                }
                catch(const seexception& ex)
                {
                   // LOGE(_logger, "CompanyPerformance::Init failed. Unknown error. " << ex.what());
                    throw ex;
                }
                catch(const std::exception& ex)
                {
                   // LOGE(_logger, "CompanyPerformance::Init failed. Unknown error. " << ex.what());
                    throw ex;
                }
            }
            
            IFinancialRatioList* CompanyPerformance::get_Ratios() 
            {
                return _pratiolist;
            }
        }
    }
}


