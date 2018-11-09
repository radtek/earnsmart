/* 
 * File:   seecomapi.h
 * Author: santony
 *
 * Created on November 18, 2013, 9:50 AM
 */

#ifndef SEECOMAPI_H
#define	SEECOMAPI_H

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"
#include "../seglobalenums.h"
#include "../seglobal/CDate.h"
#include "../sedata/sedataheaders.h"
#include "../semysql/CStoreQueryResult.h"

#define ECONEXP __attribute__ ((visibility ("default")))

namespace se
{
    
    /*
     * Error codes are defined the following way in a 32-bit number (HRESULT)
     * 
     *  7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   
     *-------------------------------------------------------------------------
     *  E| |C|     | T | |     Facility    |               Code                |
     * 
     * Facility code is 1
     * T = {
     *      00 = Error
     *      01 = Warning
     *      10 = Information
     *      11 = Debug
     * }
     * 
     */
    
    //#define S_SampleInfo                            0x02060000      // a sample info message
    //#define W_SampleWarning                         0x01060000      // a sample warning
    //#define D_SampleDebug                           0x03060000      // a sample debug message.
    //#define E_SampleDebug                           0x80060000      // a sample error message.
    
    #define E_TrainingDataNoResult                      0x800C0001
    #define E_TrainingDataSetNotFound                   0x800C0002
    
    
    namespace training
    {
        
        typedef std::function<R4(int /*Index*/)> ANNINPUTCALLBACK;
        typedef std::function<void(int /*array length*/, R4 const* /*values array*/)> ANNOUTPUTCALLBACK;
        typedef std::map<string, string> TrainingTableFieldMapping;
        
        typedef enum tagDeterminer
        {
            DET_None = 0x0,
            DET_Quantity = 0x4,
            DET_Rate = 0x8,
        } DET;
        
        struct ITrainingTable;
        struct IExTrainingTable;
        
        struct ITrainingRecord : public IObject, public ISerialization
        {
            virtual const DATAPERIOD& get_Period() const = 0;
            virtual const OmniType& get_Value() const = 0;
            virtual const OmniType& get_Change() const = 0;

            virtual void set_Period(const DATAPERIOD&) = 0;
            virtual void set_Value(const OmniType&) = 0;
            virtual void set_Change(const OmniType&) = 0;
        };
        
        struct IExTrainingRecord : public IObject, public ISerialization
        {
            virtual const DATAPERIOD &get_Period() const = 0;
            virtual const OmniType& get_Value(SIZE index) const = 0;
            virtual const OmniType& get_Result() const = 0;

            virtual void set_Period(const DATAPERIOD&) = 0;
            virtual void set_Value(SIZE index, const OmniType&) = 0;
            virtual void set_Result(const OmniType&) = 0;

            virtual bool get_IsPredicted() = 0;
            virtual void set_IsPredicted(bool) = 0;
            
        };
        
        struct ITrainingTable : public ISPList<ITrainingRecord>
        {
            virtual CSTR get_Name() = 0;
            // set the field name used for the key training field value. See get_Name for more info.
            virtual void set_Name(CSTR v) = 0;
            //virtual PERIOD get_DataFrequency() = 0;
            //virtual void set_DataFrequency(PERIOD freq) = 0;
            
            // create a new record. it must be added to the table
            virtual SE_CODE AddRecord(DATAPERIOD dp, const OmniType& value) = 0;

            virtual SE_CODE AddRecord(DATAPERIOD dp, const OmniType& value, const OmniType& change) = 0;
            
            virtual SE_CODE get_Record(DATAPERIOD period, ITrainingRecord** ppOut) = 0;

            virtual bool HasRecord(DATAPERIOD period) = 0;
            
            virtual SE_CODE get_FutureRecord(ITrainingRecord** ppOut) = 0;
            
            virtual SE_CODE set_FutureRecord(DATAPERIOD dp, const OmniType& value, const OmniType& change) = 0;

            // Returns S_False when no records available. 
            // to apply on value, set to false, else set to false.
            virtual SE_CODE get_AMean(bool IncludeFutureRecords, R8* pOut, bool ApplyOnValueOrChange = false) = 0;
            
            // Returns S_False when no records available.
            // to apply on value, set to false, else set to false.
            virtual SE_CODE get_GMean(bool IncludeFutureRecords, R8* pOut, bool ApplyOnValueOrChange = false) = 0;
            
            // Returns S_False when no records available.
            // to apply on value, set to false, else set to false.
            virtual SE_CODE get_StdDev(bool IncludeFutureRecords, R8* pOut) = 0;

            // Returns S_False when no records available.
            // to apply on value, set to false, else set to false.
            virtual SE_CODE get_Variance(bool IncludeFutureRecords, R8* pOut) = 0;
            
            static SE_CODE ECONEXP Create(ISession*, ITrainingTable**);
        };
        
        struct ITrainingTableList : public ISPList<ITrainingTable>
        {
            static SE_CODE ECONEXP Create(ISession*, ITrainingTableList**);
        };
        
        struct IExTrainingTable : public ISPList<IExTrainingRecord>
        {
            static SE_CODE ECONEXP ExtendTables(ISession*, ITrainingTableList* pTables, ITrainingTable* pResultTable, IExTrainingTable** ppOut);
            
            virtual SE_CODE CreateRecord(IExTrainingRecord** ppOut) = 0;
            virtual SE_CODE get_Record(DATAPERIOD period, IExTrainingRecord** ppOut) = 0;
            virtual bool HasRecord(DATAPERIOD period) = 0;
            virtual SE_CODE AddRecord(DATAPERIOD dp, SIZE index, const OmniType& value) = 0;
            virtual SIZE get_AttributeFieldsCount() = 0;
            virtual CSTR get_AttributeFieldName(SIZE index) = 0;
            virtual void set_AttributeFieldName(SIZE index, CSTR name) = 0;
            
            virtual const DATAPERIOD& get_RecentResultPeriod() const = 0;

            virtual SE_CODE get_FutureRecord(IExTrainingRecord** ppOut) = 0;
            virtual SE_CODE set_FutureRecord(int index, DATAPERIOD dp, const OmniType& value) = 0;
            
            // write the records as CSV records to a file.
            virtual SE_CODE Write(CSTR file) = 0;
            
        };
        
        typedef struct __attrib
        {
            char Source;        // 'E', 'P', ....
            UID Identifier;     // Training set member id (the primary key of economic indicator, performance or similarly used item.)
            
            bool operator<(const __attrib& right) const;
            bool operator>(const __attrib& right) const;
            bool operator<=(const __attrib& right) const;
            bool operator>=(const __attrib& right) const;
            bool operator==(const __attrib& right) const;
            
        } TRAINING_TATTRIB, TRAINING_RESULT;
        
        struct IPredictionSetup : public IObject, public ISerialization
        {
            virtual CSTR get_Name() = 0;
            virtual CSTR get_Algo() = 0;
            virtual CSTR get_Desc() = 0;
            
            virtual void set_Name(CSTR v) = 0;
            virtual void set_Algo(CSTR v) = 0;
            virtual void set_Desc(CSTR v) = 0;
            virtual const TRAINING_RESULT& get_ResultInfo() = 0;
            virtual void set_ResultInfo(const TRAINING_RESULT& v) = 0;
            virtual SE_CODE get_TrainingData(DATAPERIOD periodFrom, DATAPERIOD periodTo, IExTrainingTable** ppOut) = 0;
            
            static SE_CODE ECONEXP Create(ISession*, IPredictionSetup**);
        };
        
        struct IPredictionSetupList : public ISPList<IPredictionSetup>
        {
            virtual SE_CODE FindByResult(const TRAINING_RESULT& result, IPredictionSetup** ppOut) = 0;
            
            static SE_CODE ECONEXP Create(ISession*, IPredictionSetupList**);
        };
        
        
        struct ITrainingAPI : public IObject
        {
//            // Get CPI inflation for country, state, year and month.
//            // Set parameters country, state, year and/or month to zero if no respective filtering is required.
//            virtual ErrorId get_InflationCPI(UID countryId, UID stateId, I16 year, I16 month, IROList<InflationCPIData> **ppOut) = 0;
            
            // Train data by loading data based on trainingId from training tree table.
            virtual ErrorId AnnTrainData(UID trainingId) = 0;
            
            // Get a result from trained data using training id. provided new values and get predicted results.
            virtual ErrorId AnnPredictResult(UID trainingId, ANNINPUTCALLBACK inputCallback, ANNOUTPUTCALLBACK outputCallback) = 0;
            
            
            // get training records for given training id
            // StoreResult when true, the ResultBase is of StoreQueryResult type. Else it is UseQueryResult type.
            virtual ErrorId get_TrainingRecords(UID trainingId, se::data::IStoreQueryResult** result) = 0;
            
            
        };
        
        extern "C"
        {
            SE_CODE ECONEXP TrainingOpen(ISession* ps, ITrainingAPI** ppOut);
            
            SE_CODE ECONEXP CreateTrainingTable(ISession*, PERIOD datafreq, PERIOD requiredDataFreq, ITrainingTable** ppOut);
            SE_CODE ECONEXP CreateExTrainingTable(ISession*, IExTrainingTable** ppOut);
            SE_CODE ECONEXP CreateTrainingTableRecord(ISession*, ITrainingTable* table, DATAPERIOD dp, OmniType value, ITrainingRecord** ppOut);
            SE_CODE ECONEXP CreateExTrainingTableRecord(ISession*, IExTrainingTable* table, IExTrainingRecord** ppOut);
            
        }
    }
}


#endif	/* SEECOMAPI_H */

