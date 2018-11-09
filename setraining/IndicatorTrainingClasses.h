/* 
 * File:   IndicatorTrainingClasses.h
 * Author: santony
 *
 * Created on April 16, 2014, 10:41 AM
 */

#ifndef INDICATORTRAININGCLASSES_H
#define	INDICATORTRAININGCLASSES_H

#include "setrainingapi.h"
#include "../seglobal/CList.h"

namespace se
{
    namespace training
    {
        class CExTrainingTable;
        class CTrainingTable;
        
        class CTrainingRecord : public CObjectRoot, public ITrainingRecord
        {
            friend class CExTrainingTable;

        public:
            CTrainingRecord(ISession*);

            virtual ~CTrainingRecord();
            
            SE_CODE Init(CTrainingTable* table, DATAPERIOD dp, const OmniType& value);
            
            virtual const DATAPERIOD& get_Period() const;

            virtual const OmniType& get_Value() const;

            virtual const OmniType& get_Change() const;


            virtual void set_Period(const DATAPERIOD&);

            virtual void set_Value(const OmniType&);

            virtual void set_Change(const OmniType&);


            virtual void Serialize(ISerializedData<IString>* pIn);
            
            
        private:
            DATAPERIOD _period;
            OmniType _value, _change;
            CTrainingTable* _table;
        };
        
        class CExTrainingRecord : public CObjectRoot, public IExTrainingRecord
        {
            typedef vector<OmniType> Values;
            
        public:
            CExTrainingRecord(ISession*);

            virtual ~CExTrainingRecord();
            
            SE_CODE Init(CExTrainingTable* table);

            virtual const DATAPERIOD& get_Period() const;

            virtual const OmniType& get_Value(long unsigned int index) const;

            virtual const OmniType& get_Result() const;

            virtual void set_Period(const DATAPERIOD&);

            virtual void set_Value(long unsigned int index, const OmniType&);

            virtual void set_Result(const OmniType&);
            

            virtual bool get_IsPredicted();

            virtual void set_IsPredicted(bool);


            
            virtual void Serialize(ISerializedData<IString>* pIn);
            
        public:
            bool _isPredicted = false;
            DATAPERIOD _period;
            Values _values;
            OmniType _result;
            CExTrainingTable* _table;

        };
        
        
        class CTrainingTable : public CSPList<ITrainingRecord, ITrainingTable>
        {
            friend class CExTrainingTable;
            friend class CTrainingRecord;
            
        public:
            CTrainingTable(ISession*);
            virtual ~CTrainingTable();
            
            virtual void Serialize(ISerializedData<IString>* pIn);

            virtual const char* get_Name();
            
            virtual void set_Name(CSTR);

            virtual PERIOD get_DataFrequency();

            virtual void set_DataFrequency(PERIOD freq);

            virtual long unsigned int AddRecord(DATAPERIOD dp, const OmniType& value);
            

            virtual long unsigned int AddRecord(DATAPERIOD dp, const OmniType& value, const OmniType& change);


            virtual long unsigned int get_Record(DATAPERIOD period, ITrainingRecord** ppOut);

            virtual bool HasRecord(DATAPERIOD period);

            virtual void Add(ITrainingRecord* item);
            
            virtual SE_CODE get_FutureRecord(ITrainingRecord** ppOut);
            
            virtual SE_CODE set_FutureRecord(DATAPERIOD dp, const OmniType& value, const OmniType& change);
            
            virtual SE_CODE get_AMean(bool IncludeFutureRecords, R8* pOut, bool ApplyOnValueOrChange = false);
            
            virtual SE_CODE get_GMean(bool IncludeFutureRecords, R8* pOut, bool ApplyOnValueOrChange = false);
            
            virtual SE_CODE get_StdDev(bool IncludeFutureRecords, R8* pOut);
            
            virtual SE_CODE get_Variance(bool IncludeFutureRecords, R8* pOut) ;

        private:
            void UpdateChanges();

        private:
            typedef map<string, SIZE> Fields;
            Fields _fields;
            PERIOD _datafreq;
            string _name;
            UID _key;
            typedef map<DATAPERIOD, SIZE> RecordPositions;
            RecordPositions _pos;
            SP<CTrainingRecord> _pfutrec;
            R8 _mean;
            bool _bapplyonvalueorchange;
            
        };
        
        class CTrainingTableList : public CSPList<ITrainingTable, ITrainingTableList>
        {
        public:
            CTrainingTableList(ISession*);
            virtual ~CTrainingTableList();
        };
        
        class CExTrainingTable : public CSPList<IExTrainingRecord, IExTrainingTable>
        {
            friend class CExTrainingRecord;
            
        public:
            CExTrainingTable(ISession*);
            virtual ~CExTrainingTable();
            virtual void Serialize(ISerializedData<IString>* pIn);
            
            virtual SE_CODE get_Record(DATAPERIOD period, IExTrainingRecord** ppOut);
            virtual bool HasRecord(DATAPERIOD period);
            virtual SE_CODE AddRecord(DATAPERIOD dp, SIZE index, const OmniType& value);
            virtual void Add(IExTrainingRecord* item);
            virtual SIZE get_AttributeFieldsCount();
            virtual const char* get_AttributeFieldName(long unsigned int index);
            virtual void set_AttributeFieldName(SIZE index, CSTR name);
            virtual long unsigned int CreateRecord(IExTrainingRecord** ppOut);
            virtual const DATAPERIOD& get_RecentResultPeriod() const;
            
            virtual SE_CODE get_FutureRecord(IExTrainingRecord** ppOut);
            virtual SE_CODE set_FutureRecord(int index, DATAPERIOD dp, const OmniType& value);
            virtual SE_CODE Write(CSTR file);

        private:
            typedef vector<string> Fields;
            typedef map<DATAPERIOD, SIZE> RecordPositions;
            Fields _fields;
            RecordPositions _pos;
            DATAPERIOD _recentResultPeriod ;
            SP<CExTrainingRecord> pfutrec;
            
        };
        
    }
}


#endif	/* INDICATORTRAININGCLASSES_H */

