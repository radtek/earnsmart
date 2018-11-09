/* 
 * File:   IndicatorTrainingClasses.cpp
 * Author: santony
 * 
 * Created on April 16, 2014, 10:41 AM
 */

#include "internal.h"
#include "IndicatorTrainingClasses.h"
#include "../seglobal/exceptions.h"
#include "CTrainingAPI.h"
#include <fstream>
#include "../seglobal/CMisc.h"
#include <unordered_map>
#include "../semath/CStatistics.h"
#include "../seglobal/globallogger.h"

using namespace std;
using namespace se::math;

namespace se
{
    namespace training
    {
        
//Following are lowest and biggest percentage rates
        // that will be considered when calculating mean, gmean, variance, and stddev, etc.
#define MINPCT  -200.
#define MAXPCT  200.
        
        ////////////////////////CTrainingRecord//////////////////
        CTrainingRecord::CTrainingRecord(ISession* ps): CObjectRoot(ps)
        {
            
        }
        
        CTrainingRecord::~CTrainingRecord()
        {
            
        }

        long unsigned int CTrainingRecord::Init(CTrainingTable* table, DATAPERIOD dp, const OmniType& value) 
        {
            _table = table;
            _period = dp;
            _value = value;
            return S_OK;
        }


        const DATAPERIOD& CTrainingRecord::get_Period() const
        {
            return _period;
        }
        
        const OmniType& CTrainingRecord::get_Value() const
        {
            return _value;
        }

        const OmniType& CTrainingRecord::get_Change() const 
        {
            return _change;
        }


        void CTrainingRecord::set_Period(const DATAPERIOD& v)
        {
            _period = v;
        }
        
        void CTrainingRecord::set_Value(const OmniType& v)
        {
            _value = v;
            _table->UpdateChanges();
        }

        void CTrainingRecord::set_Change(const OmniType& v)  
        {
            _change = v;
        }

        
        void CTrainingRecord::Serialize(ISerializedData<IString>* pIn) 
        {
        }
        
        ////////////////////////CExTrainingRecord//////////////////
        CExTrainingRecord::CExTrainingRecord(ISession* ps): CObjectRoot(ps)
        {
            
        }
        
        CExTrainingRecord::~CExTrainingRecord()
        {
            
        }

        long unsigned int CExTrainingRecord::Init(CExTrainingTable* table) 
        {
            _table = table;
            for (int i = 0; i != _table->get_AttributeFieldsCount(); ++i)
                _values.push_back(OmniType());
            return S_OK;
        }
        
        const DATAPERIOD& CExTrainingRecord::get_Period() const
        {
            return _period;
        }
        
        const OmniType& CExTrainingRecord::get_Value(long unsigned int index) const
        {
            if (index < 0 || index >= this->_table->get_AttributeFieldsCount())
                throw seexception("Index is out of the range.");
            
            if (index < this->_table->get_AttributeFieldsCount() && index >= _values.size())
                throw seexception("Index is out of the range.");
            
            return _values[index];
        }

        const OmniType& CExTrainingRecord::get_Result() const
        {
            return _result;
        }


        void CExTrainingRecord::set_Period(const DATAPERIOD& v)
        {
            _period = v;
        }

        void CExTrainingRecord::set_Value(long unsigned int index, const OmniType& v)
        {
            if (index < 0)
                throw seexception("Index is out of the range.");
            _values[index] = v;
        }
        
        void CExTrainingRecord::set_Result(const OmniType& v)
        {
            _result = v;
            // set recentresultperiod
            if (_table->_recentResultPeriod < _period)
                _table->_recentResultPeriod = _period;
        }
        void CExTrainingRecord::set_IsPredicted(bool b) 
        {
            _isPredicted = b;
        }
        bool CExTrainingRecord::get_IsPredicted() 
        {
            return _isPredicted;
        }

        
        void CExTrainingRecord::Serialize(ISerializedData<IString>* pIn) 
        {
            
        }

        ////////////////////////CTrainingTable//////////////////

        CTrainingTable::CTrainingTable(ISession* ps): CSPList<ITrainingRecord, ITrainingTable>(ps)
        {
            
        }

        CTrainingTable::~CTrainingTable() 
        {
        }

        void CTrainingTable::Serialize(ISerializedData<IString>* pIn) 
        {

        }

        const char* CTrainingTable::get_Name() 
        {
            return _name.c_str();
        }

        void CTrainingTable::set_Name(CSTR v) 
        {
            _name = v;
        }

        
        PERIOD CTrainingTable::get_DataFrequency()
        {
            return _datafreq;
        }
        
        void CTrainingTable::set_DataFrequency(PERIOD freq)
        {
            _datafreq = freq;
        }

        void CTrainingTable::UpdateChanges() 
        {
            for (auto it = _vector.begin(); it != _vector.end(); ++it)
            {
                if (it == _vector.begin())
                    (*it)->set_Change((R8)0);
                else
                {
                    const OmniType& oprev = (*(it-1))->get_Value();
                    const OmniType& ocur = (*(it))->get_Value();
                    if (!oprev.IsEmpty() && !ocur.IsEmpty())
                    {
                        R8 chg = ((R8)ocur - (R8)oprev)/(R8)oprev;
                        (*it)->set_Change(chg * 100);
                    }
                }
            }
        }


        SE_CODE CTrainingTable::AddRecord(DATAPERIOD dp, const OmniType& value)
        {
            _mean = 0.0;
            SP<ITrainingRecord> rec;
            RETURNIFFAILED(CObject<CTrainingRecord>::Create(get_Session(), this, dp, value, &rec));
            Add(rec);
            return S_OK;
        }

        long unsigned int CTrainingTable::AddRecord(DATAPERIOD dp, const OmniType& value, const OmniType& change) 
        {
            _mean = 0.0;
            SIZE pos = get_Count();
            SP<ITrainingRecord> rec;
            RETURNIFFAILED(CObject<CTrainingRecord>::Create(get_Session(), this, dp, value, &rec));
            rec->set_Change(change);
            _pos.insert(make_pair(rec->get_Period(), pos));
            CSPList<ITrainingRecord, ITrainingTable>::Add(rec);
        }

        void CTrainingTable::Add(ITrainingRecord* item) 
        {
            _mean = 0.0;
            SIZE pos = get_Count();
            if (!pos)
                item->set_Change((R8)0.0);
            else
            {
                const OmniType& prevVal = _vector[pos-1]->get_Value();
                if (!prevVal.IsEmpty() && !item->get_Value().IsEmpty())
                {
                    R8 rprev = prevVal;
                    R8 rnew = item->get_Value();
                    item->set_Change(100 * (rnew - rprev)/rprev );
                }
            }
            _pos.insert(make_pair(item->get_Period(), pos));
            CSPList<ITrainingRecord, ITrainingTable>::Add(item);
        }

        
        SE_CODE CTrainingTable::get_Record(DATAPERIOD period, ITrainingRecord** ppOut)
        {
            RecordPositions::const_iterator it = _pos.find(period);
            if ( it  != _pos.end())
            {
                this->get_Item(it->second, ppOut);
                return S_OK;
            }
            return E_NotFound;
            
        }
        
        bool CTrainingTable::HasRecord(DATAPERIOD period)
        {
            RecordPositions::const_iterator it = _pos.find(period);
            return (it != _pos.end());
        }
        
        SE_CODE CTrainingTable::get_FutureRecord(ITrainingRecord** ppOut)
        {
            _pfutrec.CopyTo(ppOut);
            return _pfutrec ? S_OK : S_False;
        }
        
        SE_CODE CTrainingTable::set_FutureRecord(DATAPERIOD dp, const OmniType& value, const OmniType& change)
        {
            _mean = 0.0;
            RETURNIFFAILED(CObject<CTrainingRecord>::Create(get_Session(), this, dp, value, &_pfutrec));
            _pfutrec->set_Change(change);
            return S_OK;
        }
        
        SE_CODE CTrainingTable::get_AMean(bool IncludeFutureRecords, R8* pOut, bool ApplyOnValueOrChange)
        {
            _bapplyonvalueorchange = ApplyOnValueOrChange;
            SIZE ulCount = _vector.size();
            if (!ulCount)
                return S_False;
            
            ulCount = 0;
            R8 tot;
            DATATYPES dt;
            if (ApplyOnValueOrChange)
                dt = DT_R8;
            else
                dt = _vector[0]->get_Value().dt;

            switch (dt)
            {
                case DT_I32 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& v = *pv;
                        if (!v.IsEmpty())
                        {
                            tot += (R8)(I32)v;
                            ulCount++;
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    tot += (R8)(I32)_pfutrec->get_Change();
                                    ulCount++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    tot += (R8)(I32)_pfutrec->get_Value();
                                    ulCount++;
                                }
                            }
                        }
                    }
                    *pOut = tot /= ulCount;
                    break;
                }
                case DT_U32 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& v = *pv;
                        if (!v.IsEmpty())
                        {
                            tot += (R8)(U32)v;
                            ulCount++;
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    tot += (R8)(U32)_pfutrec->get_Change();
                                    ulCount++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    tot += (R8)(U32)_pfutrec->get_Value();
                                    ulCount++;
                                }
                            }
                        }
                    }
                    *pOut = tot /= ulCount;
                    break;
                }
                case DT_I64 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& v = *pv;
                        if (!v.IsEmpty())
                        {
                            tot += (R8)(I64)v;
                            ulCount++;
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    tot += (R8)(I64)_pfutrec->get_Change();
                                    ulCount++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    tot += (R8)(I64)_pfutrec->get_Value();
                                    ulCount++;
                                }
                            }
                        }
                    }
                    *pOut = tot /= ulCount;
                    break;
                }
                case DT_U64 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& v = *pv;
                        if (!v.IsEmpty())
                        {
                            tot += (R8)(U64)v;
                            ulCount++;
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    tot += (R8)(U64)_pfutrec->get_Change();
                                    ulCount++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    tot += (R8)(U64)_pfutrec->get_Value();
                                    ulCount++;
                                }
                            }
                        }
                    }
                    *pOut = tot /= ulCount;
                    break;
                }
                case DT_R8 :
                {
                    int i=0;
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& v = *pv;
                        if (!v.IsEmpty())
                        {
                            R8 rv = (R8)v;
                            if (!i && !rv)
                                continue;
                            if (rv < MINPCT or rv > MAXPCT)
                                continue;

                            tot += rv;
                            ulCount++;
                            
                        }
                        i++;
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    R8 v = (R8)_pfutrec->get_Change();
                                    tot += v;
                                    ulCount++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    R8 v = (R8)_pfutrec->get_Value();
                                    tot += v;
                                    ulCount++;
                                }
                            }
                        }
                    }
                    *pOut = tot /= ulCount;
                    break;
                }
                case DT_R4 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& v = *pv;
                        if (!v.IsEmpty())
                        {
                            tot += (R8)(R4)v;
                            ulCount++;
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    tot += (R8)(R4)_pfutrec->get_Change();
                                    ulCount++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    tot += (R8)(R4)_pfutrec->get_Value();
                                    ulCount++;
                                }
                            }
                        }
                    }
                    *pOut = tot /= ulCount;
                    break;
                }
            }
            _mean = *pOut;
            return S_OK;
        }

        SE_CODE CTrainingTable::get_GMean(bool IncludeFutureRecords, R8* pOut, bool ApplyOnValueOrChange)
        {
            _bapplyonvalueorchange = ApplyOnValueOrChange;
            SIZE ulCount = _vector.size();
            if (!ulCount)
                return S_False;

            DATATYPES dt;
            if (ApplyOnValueOrChange)
                dt = DT_R8;
            else
                dt = _vector[0]->get_Value().dt;

            FloatVector vec;
            switch (dt)
            {
                case DT_I32 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            vec.push_back((R8)(I32)val);
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    vec.push_back((R8)(I32)_pfutrec->get_Change());
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    vec.push_back((R8)(I32)_pfutrec->get_Value());
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_U32 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            vec.push_back((R8)(U32)val);
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    vec.push_back((R8)(U32)_pfutrec->get_Change());
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    vec.push_back((R8)(U32)_pfutrec->get_Value());
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_I64 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            vec.push_back((R8)(I64)val);
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    vec.push_back((R8)(I64)_pfutrec->get_Change());
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    vec.push_back((R8)(I64)_pfutrec->get_Value());
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_U64 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            vec.push_back((R8)(U64)val);
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    vec.push_back((R8)(U64)_pfutrec->get_Change());
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    vec.push_back((R8)(U64)_pfutrec->get_Value());
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_R8 :
                {
                    int i=0;
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            R8 v = (R8)val;
                            if (!i && !v)
                                continue;
                            if (v < MINPCT or v > MAXPCT)
                                continue;
                            vec.push_back(v);
                        }
                        i++;
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    R8 v = (R8)_pfutrec->get_Change();
                                    vec.push_back(v);
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    auto v =(R8)_pfutrec->get_Value();
                                    vec.push_back(v);
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_R4 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (ApplyOnValueOrChange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            vec.push_back((R8)(R4)val);
                        }
                    }
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (ApplyOnValueOrChange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    vec.push_back((R8)(R4)_pfutrec->get_Change());
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    vec.push_back((R8)(R4)_pfutrec->get_Value());
                                }
                            }
                        }
                    }                    
                    break;
                }
            }
            *pOut = CStatistics::GetGeometricMean(vec);

            _mean = *pOut;

            return S_OK;
        }

        long unsigned int CTrainingTable::get_Variance(bool IncludeFutureRecords, double* pOut) 
        {
            if (!_vector.size())
                return S_False;
            if (!_mean)
            {
                if (S_False == get_AMean(IncludeFutureRecords, &_mean))
                    return S_False;
            }
            
            R8 tot = 0.0;
            R8 size = 0.0;
            switch (_vector[0]->get_Value().dt)
            {
                case DT_I32 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (_bapplyonvalueorchange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            R8 v = (R8)(I32)val;
                            v = v - _mean;
                            v *= v;
                            tot += v;
                            size++;
                        }
                    }            
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (_bapplyonvalueorchange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    R8 v = (R8)(I32)_pfutrec->get_Change();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    R8 v = (R8)(I32)_pfutrec->get_Value();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_U32 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (_bapplyonvalueorchange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            R8 v = (R8)(U32)val;
                            v = v - _mean;
                            v *= v;
                            tot += v;
                            size++;
                        }
                    }            
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (_bapplyonvalueorchange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    R8 v = (R8)(U32)_pfutrec->get_Change();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    R8 v = (R8)(U32)_pfutrec->get_Value();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_I64 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (_bapplyonvalueorchange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            R8 v = (R8)(I64)val;
                            v = v - _mean;
                            v *= v;
                            tot += v;
                            size++;
                        }
                    }            
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (_bapplyonvalueorchange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    R8 v = (R8)(I64)_pfutrec->get_Change();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    R8 v = (R8)(I64)_pfutrec->get_Value();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_U64 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (_bapplyonvalueorchange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            R8 v = (R8)(U64)val;
                            v = v - _mean;
                            v *= v;
                            tot += v;
                            size++;
                        }
                    }            
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (_bapplyonvalueorchange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    R8 v = (R8)(U64)_pfutrec->get_Change();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    R8 v = (R8)(U64)_pfutrec->get_Value();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_R8 :
                {
                    int i=0;
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (_bapplyonvalueorchange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            R8 v = (R8)val;
                            if (!i && !v)
                                continue;
                            if (v < MINPCT or v > MAXPCT)
                                continue;

                            v = v - _mean;
                            v *= v;
                            tot += v;
                            size++;
                        }
                        i++;
                    }            
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (_bapplyonvalueorchange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    R8 v = (R8)_pfutrec->get_Change();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    R8 v = (R8)_pfutrec->get_Value();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                        }
                    }                    
                    break;
                }
                case DT_R4 :
                {
                    for (ITrainingRecord* p : _vector)
                    {
                        OmniType* pv ;
                        if (_bapplyonvalueorchange)
                            pv = const_cast<OmniType*>(&p->get_Change());
                        else
                            pv = const_cast<OmniType*>(&p->get_Value());
                        const OmniType& val = *pv;
                        if (!val.IsEmpty())
                        {
                            R8 v = (R8)(R4)val;
                            v = v - _mean;
                            v *= v;
                            tot += v;
                            size++;
                        }
                    }            
                    if (IncludeFutureRecords)
                    {
                        if (_pfutrec)
                        {
                            if (_bapplyonvalueorchange)
                            {
                                if (!_pfutrec->get_Change().IsEmpty())
                                {
                                    R8 v = (R8)(R4)_pfutrec->get_Change();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                            else
                            {
                                if (!_pfutrec->get_Value().IsEmpty())
                                {
                                    R8 v = (R8)(R4)_pfutrec->get_Value();
                                    v = v - _mean;
                                    v *= v;
                                    tot += v;
                                    size++;
                                }
                            }
                        }
                    }                    
                    break;
                }
            }           
            
            *pOut = tot / size;
            
            return S_OK;
            
        }


        SE_CODE CTrainingTable::get_StdDev(bool IncludeFutureRecords, R8* pOut)
        {
            R8 var;
            RETURNIFFAILED(get_Variance(IncludeFutureRecords, &var));
            *pOut = sqrt(var);   // population stddev (not sample - if so, then denominator will be _vector.size() - 1)
            return S_OK;
        }

        SE_CODE ECONEXP ITrainingTable::Create(ISession* ps, ITrainingTable** ppOut)
        {
            return CObject<CTrainingTable>::Create(ps, ppOut);
        }

        
        ////////////////////////////CTrainingTableList//////////////////////
        CTrainingTableList::CTrainingTableList(ISession* ps):CSPList<ITrainingTable, ITrainingTableList>(ps)
        {
            
        }
        
        CTrainingTableList::~CTrainingTableList()
        {
            
        }
        
        SE_CODE ECONEXP ITrainingTableList::Create(ISession* ps, ITrainingTableList** ppOut)
        {
            return CObject<CTrainingTableList>::Create(ps, ppOut);
        }
        
        /////////////////////////////CExTrainingTable////////////////////////////
        
        CExTrainingTable::CExTrainingTable(ISession* ps): CSPList<IExTrainingRecord, IExTrainingTable>(ps)
        {
            _recentResultPeriod.Day = 1;
            _recentResultPeriod.Month = 1;
            _recentResultPeriod.Year = 1;
        }
        
        CExTrainingTable::~CExTrainingTable()
        {
            
        }

        void CExTrainingTable::Serialize(ISerializedData<IString>* pIn) 
        {

        }

        long unsigned int CExTrainingTable::AddRecord(DATAPERIOD dp, long unsigned int index, const OmniType& value) 
        {
            SP<CExTrainingRecord> prec;
            RETURNIFFAILED(CObject<CExTrainingRecord>::Create(get_Session(), this, &prec));
            prec->set_Period(dp);
            prec->set_Value(index, value);
            Add(prec);
            return S_OK;
        }

        void CExTrainingTable::Add(IExTrainingRecord* item) 
        {
            SIZE pos = get_Count();
            _pos.insert(make_pair(item->get_Period(), pos));
            CSPList<IExTrainingRecord, IExTrainingTable>::Add(item);
        }
        long unsigned int CExTrainingTable::CreateRecord(IExTrainingRecord** ppOut) 
        {
            return CObject<CExTrainingRecord>::Create(get_Session(), this, ppOut);
        }

        
        SE_CODE CExTrainingTable::get_Record(DATAPERIOD period, IExTrainingRecord** ppOut)
        {
            RecordPositions::const_iterator it = _pos.find(period);
            if ( it  != _pos.end())
            {
                this->get_Item(it->second, ppOut);
                return S_OK;
            }
            return E_NotFound;
        }

        bool CExTrainingTable::HasRecord(DATAPERIOD period) 
        {
            RecordPositions::const_iterator it = _pos.find(period);
            return (it != _pos.end());
        }

        long unsigned int CExTrainingTable::get_AttributeFieldsCount() 
        {
            return _fields.size();
        }

        const char* CExTrainingTable::get_AttributeFieldName(long unsigned int index) 
        {
            if (index < 0)
                throw seexception("Invalid range.");
            
            if (index < _fields.size())
            {
                return _fields[index].c_str();
            }
            
            std::stringstream str;
            str << "ATTR_" << index;
            _fields.insert(_fields.begin() + index, str.str());

            return _fields[index].c_str();
        }

        const DATAPERIOD& CExTrainingTable::get_RecentResultPeriod() const 
        {
            return _recentResultPeriod;
        }


        void CExTrainingTable::set_AttributeFieldName(long unsigned int index, const char* name) 
        {
            if (index < 0)
                throw seexception("Invalid range.");
            
            if (index >= _fields.size())
                _fields.insert(_fields.begin() + index, name);
            else
                _fields[index] = name;
        }

        long unsigned int CExTrainingTable::get_FutureRecord(IExTrainingRecord** ppOut) 
        {
            pfutrec.CopyTo(ppOut);
            return pfutrec ? S_OK : S_False;
        }
        

        long unsigned int CExTrainingTable::set_FutureRecord(int index, DATAPERIOD dp, const OmniType& value) 
        {
            if (!pfutrec)
            {
                RETURNIFFAILED(CObject<CExTrainingRecord>::Create(get_Session(), this, &pfutrec));
                pfutrec->set_Period(dp);
            }
            pfutrec->set_Value(index, value);
            return S_OK;
        }
        

        long unsigned int CExTrainingTable::Write(const char* file) 
        {
            if (!this->get_Count())
                return E_NoDataToReturn;

            try
            {
                string strfiledata = file;
                strfiledata += ".data";
                
                string strfilenames = file;
                strfilenames += ".names";
                
                ofstream ofs, ofsnames;
                
                ofs.open(strfiledata);
                if (!ofs.is_open())
                    return E_ReadError;
                
                ofsnames.open(strfilenames);
                if (!ofsnames.is_open())
                    return E_ReadError;
                
                ofsnames << "| Prediction Analysis " << endl;
                ofsnames << endl;
                ofsnames << "Result."   << "    |target" << endl;
                auto fc = this->get_AttributeFieldsCount();
                for (int i=0; i != fc; ++i)
                {
                    ofsnames << get_AttributeFieldName(i) << ": " << "continuous" << "." << endl;
                }
                ofsnames << "Result: continuous" << endl;
                ofsnames.close();
                
                for (IExTrainingRecord* prec : *this)
                {
                    for (int i=0; i != fc; ++i)
                    {
                        const OmniType& ot = prec->get_Value(i);
                        if (ot.IsEmpty())
                            ofs << "?";
                        else
                            ofs << ot;
                        ofs << ", ";
                    }
                    const OmniType& result = prec->get_Result();
                    if(result.IsEmpty())
                        ofs << "?";
                    else
                        ofs << result;
                    ofs << std::endl;
                }            
                ofs.close();
                
                return S_OK;
            }
            catch(const std::exception& ex)
            {
                return E_UnknownError;
            }
        }
        
        
        long unsigned int ECONEXP IExTrainingTable::ExtendTables(ISession* ps, ITrainingTableList* pTables, ITrainingTable* pResultTable, IExTrainingTable** ppOut) 
        {
            //LoggerPtr _logger = Logger::getLogger("IExTrainingTable::ExtendTables");
            if (!pTables || !pResultTable)
            {
               // LOGEP("Invalid arguments. Both pTables and pResultTable must be provided.");
                return E_InvalidArg;
            }
            
            if (!pResultTable->get_Count())
            {
               // LOGEP("Result table is empty. Cannot proceed.");
                return E_TrainingDataNoResult;
            }

            SP<IExTrainingTable> pExTable;
            RETURNIFFAILED(CObject<CExTrainingTable>::Create(ps, &pExTable));

            for (int i=0; i != pTables->get_Count(); ++i)
            {
                SP<ITrainingTable> pTable;
                pTables->get_Item(i, &pTable);
                pExTable->set_AttributeFieldName(i, pTable->get_Name());
               // LOGI(_logger, "Setting attribute name " << pTable->get_Name() << " at attrib position " << i);
            }
            
            
            try
            {
                
//                DATAPERIOD start, end;
//                if (pResultTable->get_Count())
//                    start = (*pResultTable->begin())->get_Period();
//                
//                for (int i=0; i != pTables->get_Count(); ++i)
//                {
//                    SP<ITrainingTable> pTable;
//                    pTables->get_Item(i, &pTable);
//                    for (ITrainingRecord* prec : *pTable)
//                    {
//                        DATAPERIOD dp = prec->get_Period();
//                        if (dp < start)
//                            continue;
//                        
//                        OmniType attrVal = prec->get_Change();
//                        SP<IExTrainingRecord> pexrecord;
//                        if (ISOK(pExTable->get_Record(dp, &pexrecord)))
//                        {
//                            pexrecord->set_Value(i, attrVal);
//                        }
//                        else
//                        {
//                            pExTable->AddRecord(dp, i, attrVal);
//                        }
//                       // LOGI(_logger, "Added extended record for " << pTable->get_Name() << " for Period: " << dp);
//                    }
//                }
//                
//                for (ITrainingRecord* prec : *pResultTable)
//                {
//                    DATAPERIOD dp = prec->get_Period();
//                    const OmniType& attrVal = prec->get_Value();
//                    const OmniType& attrChg = prec->get_Change();
//                    SP<IExTrainingRecord> pexrecord;
//                    if (ISOK(pExTable->get_Record(dp, &pexrecord)))
//                        pexrecord->set_Result(attrVal);
//                    else
//                        pexrecord->set_Result(OmniType::Empty());
//
//                   // LOGI(_logger, "Added extended record for Result for Period: " << dp << " with a value = " << prec->get_Value() << " and change = " << prec->get_Change());
//                }                
                DATAPERIOD now;
                CDate dtNow = CDate::Now();
                now.Quarter = dtNow.get_Quarter();
                now.Year = dtNow.get_Year();
                switch (now.Quarter)
                {
                    case 1:
                        now.Month = 1;
                        break;
                    case 2:
                        now.Month = 4;
                        break;
                    case 3:
                        now.Month = 7;
                        break;
                    case 4:
                        now.Month = 10;
                        break;
                }
                now.Day = 0;
                
                typedef std::unordered_map<int, CMisc::DoubleList> AverageValues;
                AverageValues avgs;
                // we need only the records that matches the period defined in result table.
                for (ITrainingRecord* prec : *pResultTable)
                {
                    DATAPERIOD dp = prec->get_Period();
                   // LOGI(_logger, "Adding extended record for Period : " << dp);
                    for (int i=0; i != pTables->get_Count(); ++i)
                    {
                        SP<ITrainingTable> pTable;
                        pTables->get_Item(i, &pTable);

                        se::training::DET determiner = se::training::DET_None ;
                        const OmniType& ot = pTable->get_Property("Determiner");
                        if (!ot.IsEmpty())
                        {
                            determiner = (se::training::DET)(I32)ot;
                        }
                        
                        OmniType attrVal;
                        SP<ITrainingRecord> prec;
                        if (FAILED(pTable->get_Record(dp, &prec)))
                        {
                           // LOGE(_logger, "No record in the attribute table " << pTable->get_Name() << " for period " << dp << ". Attempting to put in an average value.");
                            auto itfind = avgs.find(i);
                            if ( itfind != avgs.end())
                            {
                                CMisc::DoubleList& dbllist = itfind->second;
                                if (dbllist.size())
                                {
                                    attrVal = CMisc::GetAverageForMissingData(dbllist);
                                }
                            }                            
                        }
                        else
                        {
                            if (determiner == DET_Quantity)
                                attrVal = prec->get_Change();
                            else //if(det == DET_Rate)
                                attrVal = prec->get_Value();
                            
                            auto itfind = avgs.find(i);
                            if ( itfind == avgs.end())
                            {
                                auto pr = avgs.insert(make_pair(i, CMisc::DoubleList()));
                                itfind = pr.first;
                            }
                            CMisc::DoubleList& dbllist = itfind->second;
                            CMisc::ManageValuesForMissingData(dbllist, attrVal);
                        }

                        // fetch record for the period from extended table.
                        SP<IExTrainingRecord> pexrecord;
                        if (ISOK(pExTable->get_Record(dp, &pexrecord)))
                        {
                            pexrecord->set_Value(i, attrVal);
                        }
                        else
                        {
                            pExTable->AddRecord(dp, i, attrVal);
                        }
                        
                       // LOGI(_logger, "Setting attrib value " << attrVal << " for attrib position " << i);

                    }
                    // fetch record for the period from extended table.
                    SP<IExTrainingRecord> pexrecord;
                    if (ISOK(pExTable->get_Record(dp, &pexrecord)))
                    {
                        const OmniType& v = prec->get_Value();
                        if(v.IsEmpty())
                            pexrecord->set_Result(OmniType::Empty());
                        else
                            pexrecord->set_Result(v);
                    }
                   // LOGI(_logger, "Added extended record for Period : " << dp);
                }
                
                for (int i=0; i != pTables->get_Count(); ++i)
                {
                    SP<ITrainingTable> pTable;
                    pTables->get_Item(i, &pTable);
                    se::training::DET determiner = se::training::DET_None ;
                    const OmniType& ot = pTable->get_Property("Determiner");
                    if (!ot.IsEmpty())
                    {
                        determiner = (se::training::DET)(I32)ot;
                    }
                    SP<ITrainingRecord> pfut;
                    OmniType futVal;
                    if (ISOK(pTable->get_FutureRecord(&pfut)))
                    {
                        if (determiner == DET_Quantity)
                            futVal = pfut->get_Change();
                        else //if(det == DET_Rate)
                            futVal = pfut->get_Value();
                        if (futVal.IsEmpty())
                        {
                            SP<ITrainingRecord> lastrecord;
                            if (ISOK(pTable->get_Item(pTable->get_Count()-1, &lastrecord)))
                            {
                                if (determiner == DET_Quantity)
                                    futVal = lastrecord->get_Change();
                                else //if(det == DET_Rate)
                                    futVal = lastrecord->get_Value();
                            }
                        }
                        pExTable->set_FutureRecord(i, pfut->get_Period(), futVal);
                    }
                    else
                    {
                        SP<ITrainingRecord> lastrecord;
                        if (ISOK(pTable->get_Item(pTable->get_Count()-1, &lastrecord)))
                        {
                            if (determiner == DET_Quantity)
                                futVal = lastrecord->get_Change();
                            else //if(det == DET_Rate)attrVal
                                futVal = lastrecord->get_Value();
                            pExTable->set_FutureRecord(i, now, futVal);
                        }
                        else
                            pExTable->set_FutureRecord(i, now, OmniType());
                    }
                }
            }
            catch(seexception& ex)
            {
               // LOGE(_logger, "Error: " << ex.what());
                return ex.error();
            }
            catch(std::exception& ex)
            {
               // LOGE(_logger, "Error: " << ex.what());
                return E_UnknownError;
            }
            pExTable.CopyTo(ppOut);
            return S_OK;
        }
        
    }
}