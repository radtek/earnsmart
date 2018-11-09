/* 
 * File:   CSecuritySelectionStatistics.cpp
 * Author: santony
 * 
 * Created on July 2, 2014, 4:14 PM
 */

#include "internal.h"

#define EXPAND_MY_SSQLS_STATICS 
#include "CSecuritySelectionStatistics.h"
#include "../seglobal/exceptions.h"

namespace se
{
    namespace securities
    {


        CSecuritySelectionStatistics::CSecuritySelectionStatistics(ISession* ps) : CObjectRoot(ps) 
        {
        }
        
        CSecuritySelectionStatistics::~CSecuritySelectionStatistics() 
        {
        }

        long unsigned int CSecuritySelectionStatistics::Init(const Row& r) 
        {
            _r = r;
            set_Id((UID)_r["product_id"]);
            return S_OK;
        }


        R8 CSecuritySelectionStatistics::get_AMeanOfReturnRate() const 
        {
            sql_decimal_null d = _r["return_amean"];
            if (d.is_null)
                return ER8;
            else
                return d.data;
        }

        R8 CSecuritySelectionStatistics::get_Alpha() const 
        {
            return ER8;
        }

        R8 CSecuritySelectionStatistics::get_Beta() const 
        {
            return ER8;
        }

        R8 CSecuritySelectionStatistics::get_ExpectedReturnRate() const 
        {
            sql_decimal_null d = _r["expected_return_rate"];
            if (d.is_null)
                return ER8;
            else
                return d.data;
        }

        R8 CSecuritySelectionStatistics::get_ExpectedReturnValue() const 
        {
            sql_double_null d = _r["expected_return_value"];
            if (d.is_null)
                return ER8;
            else
                return d.data;
        }

        R8 CSecuritySelectionStatistics::get_GMeanOfReturnRate() const 
        {
            sql_decimal_null d = _r["return_gmean"];
            if (d.is_null)
                return ER8;
            else
                return d.data;
        }

        R8 CSecuritySelectionStatistics::get_ReturnRisk() const 
        {
            sql_decimal_null d = _r["stddev"];
            if (d.is_null)
                return ER8;
            else
                return d.data;
        }

        long unsigned int CSecuritySelectionStatistics::get_SecurityDescription(ISecurityDescription** ppOut) 
        {
            if (!_pdesc)
            {
                //SP<ISecurityAPI> papi;
                //RETURNIFFAILED(SecOpen(&papi));
                //RETURNIFFAILED(papi->get_SecurityDescription(get_Id(), &_pdesc));
            }
            _pdesc.CopyTo(ppOut);
            return S_OK;
        }

        R8 CSecuritySelectionStatistics::get_Volatility() const 
        {
            sql_decimal_null d = _r["variance"];
            if (d.is_null)
                return ER8;
            else
                return d.data;
        }

        void CSecuritySelectionStatistics::set_AMeanOfReturnRate(R8 val) {

            throw seexception(E_NotImpl, "Not implemented");
        }

        void CSecuritySelectionStatistics::set_Alpha(R8 val) {
            throw seexception(E_NotImpl, "Not implemented");

        }

        void CSecuritySelectionStatistics::set_Beta(R8 val) {
            throw seexception(E_NotImpl, "Not implemented");

        }

        void CSecuritySelectionStatistics::set_ExpectedReturnRate(R8 val) {
            throw seexception(E_NotImpl, "Not implemented");

        }

        void CSecuritySelectionStatistics::set_ExpectedReturnValue(R8 val) {
            throw seexception(E_NotImpl, "Not implemented");

        }

        void CSecuritySelectionStatistics::set_GMeanOfReturnRate(R8 val) {
            throw seexception(E_NotImpl, "Not implemented");

        }

        void CSecuritySelectionStatistics::set_ReturnRisk(R8 val) 
        {
            throw seexception(E_NotImpl, "Not implemented");
        }

        void CSecuritySelectionStatistics::set_Volatility(R8 val) 
        {
            throw seexception(E_NotImpl, "Not implemented");
            
        }
        
        void CSecuritySelectionStatistics::Serialize(ISerializedData<IString>* pIn)
        {
            throw seexception(E_NotImpl, "Not implemented");
        }

        SE_CODE CSecuritySelectionStatistics::PopulateInstance(ISession* ps, const Row& row, ISecuritySelectionStatistics** ppOut)
        {
            CSecuritySelectionStatistics *p;
            RETURNIFFAILED(CObject<CSecuritySelectionStatistics>::Create(ps, row , &p));
            *ppOut = p;
            return S_OK;
        }
        
        /////////////////////////////////CSecuritySelectionStatisticsList///////////////////////////////
        CSecuritySelectionStatisticsList::CSecuritySelectionStatisticsList(ISession* ps) : CROSPList<ISecuritySelectionStatistics, ISecuritySelectionStatisticsList>(ps)
        {
            
        }
        
        CSecuritySelectionStatisticsList::~CSecuritySelectionStatisticsList()
        {
            
        }

        long unsigned int CSecuritySelectionStatisticsList::Init(long unsigned int selectionSize, const StoreQueryResult& sqr) 
        {
            _selSize = selectionSize;
            _fullList = sqr;
            RenewList();
            return S_OK;
        }

        long unsigned int CSecuritySelectionStatisticsList::Reset() 
        {
            _selectedItems.clear();
            _vector.clear();
            return RenewList();
        }


        long unsigned int CSecuritySelectionStatisticsList::RenewList() 
        {
            _vector.clear();
            if (_selSize == 0)
            {
                for (const Row& r : _fullList)
                {
                    SP<ISecuritySelectionStatistics> p;
                    THROWIFFAILED(CSecuritySelectionStatistics::PopulateInstance(get_Session(), r, &p), "Failed to create an instance of CSecuritySelectionStatistics");
                    Add(p);
                }
            }
            else
            {
                SIZE selSize = min(_selSize, _fullList.size());
                //then just generate the integer like this:
                SIZE randomVal;
                std::mt19937 engine(seeder());
                std::uniform_int_distribution<SIZE> dist(0, _fullList.size()-1);
                for (SIZE i = 0; i != selSize; ++i)
                {
                    //while (_selectedItems.find( randomVal = (SIZE)dist(engine)) != _selectedItems.end());
                    randomVal = (SIZE)dist(engine);
                    //_selectedItems.insert(randomVal);

                    const Row& r = _fullList[randomVal];
                    SP<ISecuritySelectionStatistics> p;
                    THROWIFFAILED(CSecuritySelectionStatistics::PopulateInstance(get_Session(), r, &p), "Failed to create an instance of CSecuritySelectionStatistics");
                    Add(p);
                }
            }
            return S_OK;
        }
    }
}
