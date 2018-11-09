/* 
 * File:   CFinancialPerformanceList.cpp
 * Author: santony
 * 
 * Created on April 1, 2014, 12:19 AM
 */

#include "sexbrlinternal.h"
#include "CFinancialPerformanceList.h"
#include "CXbrlUtilities.h"
#include "../seglobal/CStringUtility.h"

namespace se
{
    namespace xbrl
    {


        CFinancialPerformanceList::CFinancialPerformanceList(ISession* ps) : CROSPList<IFinancialPerformance, IFinancialPerformanceList >(ps)
        {
        }

        CFinancialPerformanceList::~CFinancialPerformanceList() 
        {
            _cikIndex.clear();
        }


        long unsigned int CFinancialPerformanceList::FilterRatioByRange(short year, short quarter, unsigned long ratioId, double rangeStart, double rangeEnd, IFinancialPerformanceList** ppOut) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialPerformanceList::FilterRatioByRange");
           // LOGI(_logger, "List before filtering has " << _cikIndex.size() << " items.");
            SP<IStoreQueryResult> psqr;
            RETURNIFFAILED(CXbrlUtilities::FilterPerformanceRatiosByRange(this->get_Session(), year, quarter, ratioId, rangeStart, rangeEnd, &psqr));
            SP<CFinancialPerformanceList> pl;
            RETURNIFFAILED(CObject<CFinancialPerformanceList>::Create(get_Session(), &pl));
            auto result = psqr->get_Result();
            for (const Row& r : result)
            {
                CIK cik = r["cik"];
                SP<IFinancialPerformance> pratio;
                auto itfind = _cikIndex.find((string)cik);
                if (itfind != _cikIndex.end())
                {
                    pl->Add(_vector.at(itfind->second));
                }
            }
           // LOGI(_logger, "List after filtering has " << pl->get_Count() << " items.");
            pl.CopyTo(ppOut);
            return S_OK;
        }

        long unsigned int CFinancialPerformanceList::FilterLatestAnnualRatioByRange(unsigned long ratioId, double rangeStart, double rangeEnd, IFinancialPerformanceList** ppOut) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialPerformanceList::FilterLatestAnnualRatioByRange");
           // LOGI(_logger, "List before filtering has " << _cikIndex.size() << " items.");
            SP<IStoreQueryResult> psqr;
            RETURNIFFAILED(CXbrlUtilities::FilterPerformanceLatestAnnualRatiosByRange(get_Session(), ratioId, rangeStart, rangeEnd, &psqr));
            SP<CFinancialPerformanceList> pl;
            RETURNIFFAILED(CObject<CFinancialPerformanceList>::Create(get_Session(), &pl));
            auto result = psqr->get_Result();
            for (const Row& r : result)
            {
                CIK cik = r["cik"];
                SP<IFinancialPerformance> pratio;
                auto itfind = _cikIndex.find((string)cik);
                if (itfind != _cikIndex.end())
                {
                    pl->Add(_vector.at(itfind->second));
                }
            }
           // LOGI(_logger, "List after filtering has " << pl->get_Count() << " items.");
            pl.CopyTo(ppOut);
            return S_OK;
        }

        long unsigned int CFinancialPerformanceList::FilterLatestQuarterlyRatioByRange(unsigned long ratioId, double rangeStart, double rangeEnd, IFinancialPerformanceList** ppOut) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialPerformanceList::FilterLatestQuarterlyRatioByRange");
           // LOGI(_logger, "List before filtering has " << _cikIndex.size() << " items.");
            SP<IStoreQueryResult> psqr;
            RETURNIFFAILED(CXbrlUtilities::FilterPerformanceLatestQuarterlyRatiosByRange(get_Session(), ratioId, rangeStart, rangeEnd, &psqr));
            SP<CFinancialPerformanceList> pl;
            RETURNIFFAILED(CObject<CFinancialPerformanceList>::Create(get_Session(), &pl));
            auto result = psqr->get_Result();
            for (const Row& r : result)
            {
                CIK cik = r["cik"];
                SP<IFinancialPerformance> pratio;
                auto itfind = _cikIndex.find((string)cik);
                if (itfind != _cikIndex.end())
                {
                    pl->Add(_vector.at(itfind->second));
                }
            }
           // LOGI(_logger, "List after filtering has " << pl->get_Count() << " items.");
            pl.CopyTo(ppOut);
            return S_OK;
        }
        

        long unsigned int CFinancialPerformanceList::FilterRatioByValue(short year, short quarter, unsigned long ratioId, double value, double tolerance, IFinancialPerformanceList** ppOut) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialPerformanceList::FilterRatioByValue");
           // LOGI(_logger, "List before filtering has " << _cikIndex.size() << " items.");
            SP<IStoreQueryResult> psqr;
            RETURNIFFAILED(CXbrlUtilities::FilterPerformanceRatiosByValue(get_Session(), year, quarter, ratioId, value, tolerance, &psqr));
            SP<CFinancialPerformanceList> pl;
            RETURNIFFAILED(CObject<CFinancialPerformanceList>::Create(get_Session(), &pl));
            auto result = psqr->get_Result();
            for (const Row& r : result)
            {
                CIK cik = r["cik"];
                SP<IFinancialPerformance> pratio;
                auto itfind = _cikIndex.find((string)cik);
                if (itfind != _cikIndex.end())
                {
                    pl->Add(_vector.at(itfind->second));
                }
            }
           // LOGI(_logger, "List after filtering has " << pl->get_Count() << " items.");
            pl.CopyTo(ppOut);
            return S_OK;
        }

        long unsigned int CFinancialPerformanceList::FilterLatestAnnualRatioByValue(unsigned long ratioId, double value, double tolerance, IFinancialPerformanceList** ppOut) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialPerformanceList::FilterLatestAnnualRatioByValue");
           // LOGI(_logger, "List before filtering has " << _cikIndex.size() << " items.");
            SP<IStoreQueryResult> psqr;
            RETURNIFFAILED(CXbrlUtilities::FilterPerformanceLatestAnnualRatiosByValue(get_Session(), ratioId, value, tolerance, &psqr));
            SP<CFinancialPerformanceList> pl;
            RETURNIFFAILED(CObject<CFinancialPerformanceList>::Create(get_Session(), &pl));
            auto result = psqr->get_Result();
            for (const Row& r : result)
            {
                CIK cik = r["cik"];
                SP<IFinancialPerformance> pratio;
                auto itfind = _cikIndex.find((string)cik);
                if (itfind != _cikIndex.end())
                {
                    IFinancialPerformance* p = _vector.at(itfind->second);
                    p->set_Property("year", (I32)r["year"]);
                    pl->Add(p);
                }
            }
           // LOGI(_logger, "List after filtering has " << pl->get_Count() << " items.");
            pl.CopyTo(ppOut);
            return S_OK;

        }

        long unsigned int CFinancialPerformanceList::FilterQuarterlyRatioByValue(unsigned long ratioId, double value, double tolerance, IFinancialPerformanceList** ppOut) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialPerformanceList::FilterQuarterlyRatioByValue");
           // LOGI(_logger, "List before filtering has " << _cikIndex.size() << " items.");
            SP<IStoreQueryResult> psqr;
            RETURNIFFAILED(CXbrlUtilities::FilterPerformanceLatestQuarterlyRatiosByValue(get_Session(), ratioId, value, tolerance, &psqr));
            SP<CFinancialPerformanceList> pl;
            RETURNIFFAILED(CObject<CFinancialPerformanceList>::Create(get_Session(), &pl));
            auto result = psqr->get_Result();
            for (const Row& r : result)
            {
                CIK cik = r["cik"];
                SP<IFinancialPerformance> pratio;
                auto itfind = _cikIndex.find((string)cik);
                if (itfind != _cikIndex.end())
                {
                    pl->Add(_vector.at(itfind->second));
                }
            }
           // LOGI(_logger, "List after filtering has " << pl->get_Count() << " items.");
            pl.CopyTo(ppOut);
            return S_OK;

        }
        

        void CFinancialPerformanceList::Add(IFinancialPerformance* item) 
        {
            SIZE length = _vector.size();
            string cik = item->get_CIK();
            se::utils::CStringUtility::LeftTrim(cik, '0');
            _cikIndex.insert(make_pair(cik, length));
            CROSPList<IFinancialPerformance, IFinancialPerformanceList >::Add(item);
        }

        bool CFinancialPerformanceList::TryGet(CIK cik, IFinancialPerformance** ppOut) 
        {
            string strcik = cik;
            se::utils::CStringUtility::LeftTrim(strcik, '0');
            auto itfind = _cikIndex.find(strcik);
            if (itfind != _cikIndex.end())
                return get_Item(itfind->second, ppOut) == S_OK;
            return false;
        }

    }
}