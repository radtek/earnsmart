/* 
 * File:   CPortfolio.cpp
 * Author: santony
 * 
 * Created on June 28, 2014, 1:52 PM
 */

#include "internal.h"
#include "CPortfolio.h"
#include "CAdminAPI.h"

namespace se
{
    namespace admin
    {

        CPortfolio::CPortfolio(ISession* ps) : CObjectRoot(ps)
        {
        }

        CPortfolio::~CPortfolio() 
        {
        }

        const OmniType& CPortfolio::get_AMean() const 
        {
            return _amean;
        }

        const OmniType& CPortfolio::get_Alpha() const 
        {
            return _alpha;
        }

        const OmniType& CPortfolio::get_Beta() const 
        {
            return _beta;
        }

        const DateTime& CPortfolio::get_CreatedOn() const 
        {
            return _createdon;
        }

        const OmniType& CPortfolio::get_GMean() const 
        {
            return _gmean;
        }

        long unsigned int CPortfolio::get_Items(IPortfolioItemList** ppOut) 
        {
            if (!_items)
            {
                Poco::ScopedLock<Poco::Mutex> g(CAdminAPI::adminLock);
                if (!_items)
                {
                    RETURNIFFAILED(CObject<CPortfolioItems>::Create(get_Session(), &_items));
                }
            }
            _items.CopyTo(ppOut);
            return S_OK;
        }

        const DateTime& CPortfolio::get_ModifiedOn() const {
            return _modifiedon;
        }

        const char* CPortfolio::get_Name() {
            return _name.c_str();
        }

        const char* CPortfolio::get_Notes() const {
            return _notes.c_str();
        }

        const OmniType& CPortfolio::get_ReturnRisk() const {
            return _returnrisk;
        }

        const OmniType& CPortfolio::get_RiskFreeReturn() const {
            return _riskfreereturn;
        }

        const OmniType& CPortfolio::get_Volatility() const {
            return _volatility;
        }

        void CPortfolio::set_AMean(const OmniType& val) 
        {
            _amean = val;
        }

        void CPortfolio::set_Alpha(const OmniType& val) 
        {
            _alpha = val;
        }

        void CPortfolio::set_Beta(const OmniType& val) 
        {
            _beta = val;
        }

        void CPortfolio::set_GMean(const OmniType& val) 
        {
            _gmean = val;
        }

        void CPortfolio::set_Name(const char* name) 
        {
            _name = name;
        }

        void CPortfolio::set_Notes(const char* v) 
        {
            _notes = v;
        }

        void CPortfolio::set_ReturnRisk(const OmniType& val) 
        {
            _returnrisk = val;
        }

        void CPortfolio::set_RiskFreeReturn(const OmniType& val) 
        {
            _riskfreereturn = val;
        }

        void CPortfolio::set_Volatility(const OmniType& val) 
        {
            _volatility = val;
        }

        long unsigned int CPortfolio::Save() 
        {

        }

        void CPortfolio::Serialize(ISerializedData<IString>* pIn) {

        }


    }
}