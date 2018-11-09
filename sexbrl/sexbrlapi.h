/* 
 * File:   sexbrlapi.h
 * Author: santony
 *
 * Created on June 30, 2012, 8:53 PM
 */

#ifndef SEXBRLAPI_H
#define	SEXBRLAPI_H

#include <initializer_list>
#include "../seglobal/Object.h"
#include "../seglobal/IObject.h"
#include "../seglobaltypedefs.h"
#include "../sesecurities/sesecuritiesapi.h"
#include <tuple>

#define MYSQLPP_MYSQL_HEADERS_BURIED

#include "../setraining/setrainingapi.h"

#define XBRLEXP __attribute__ ((visibility ("default")))

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

//#define E_DBHostUnreachable                     0x80010000      // db server cannot be reached.
//#define S_SampleInfo                            0x02010000      // a sample info message
//#define W_SampleWarning                         0x01010000      // a sample warning
//#define D_SampleDebug                           0x03010000      // a sample debug message.



using namespace se::securities;
using namespace se::training;

namespace se
{
    namespace xbrl
    {

#define E_CIKFORSYMBOLNOTFOUND                  0x80010001      // cik not found for the given symbol.                     
#define E_FORMINDEXING                          0x80010002      // form indexing failed.
#define E_XBRLDOCNOTCACHED                      0x80010003      // returns when xbrl documents are not cached in user's homepath.
#define E_XPathContextCreate                    0x80010004      // xpath context creation failed.        
#define E_XPathNsRegister                       0x80010005      // xpath register ns failed.        
#define E_XMLParseError                         0x80010006      // Reported an xml parsing error. Need to check logs.

#define S_CalcCompleted                         0x02010001      // used in explicit formulat manager. reports that calculation is completed and nothing else to be done.
        
        
        
        typedef enum XbrlDocClasses
        {
            XDC_Presentation,
            XDC_Calculation
        } XBRLDOCCLASSES, XDC;

        typedef enum XbrlFormTypes 
        {
            XFT_10K = 3,
            XFT_10Q_A = 2,
            XFT_10Q = 1
        } XBRLFORMTYPES, XFT;
        
        typedef enum StatementType
        {
            ST_CashFlow,
            ST_Income,
            ST_BalanceSheet
        } STATEMENTTYPES, XST;
        
        typedef enum StatementDataPeriod
        {
            SDP_Annual,
            SDP_Quarterly
        } STATEMENTDATAPERIOD, SDP;
        
        typedef enum StatementOutputFormat
        {
            SOF_Json,
            SOF_Xml
        } STATEMENTOUTPUTFORMAT, SOF;
        
        
        typedef enum StatementTypeMask
        {
            STM_Presentation = 0x1,
            STM_Calculation = 0x2,
            STM_Definition = 0x4,
            STM_Label = 0x8
        } STATEMENTTYPEMASK, STM;
        
        typedef enum XbrlDataTypes
        {
            XDT_None,
            XDT_Numeric, 
            XDT_Monetary,
            XDT_Text
        } XBRL_DATA_TYPES, XDT;
        
        typedef enum XbrlPeriodTypes
        {
            XPT_None                    =   0x0,
            XPT_Duration                =   0x1,
            XPT_Instant                 =   0x2,
            XPT_PeriodicInstance        =   XPT_Duration | XPT_Instant
        } XBRL_PERIOD_TYPES, XPT;

        /** Listener used in some operations defined in IXbrl interface. */
        template<typename T>
        struct IXbrlCompletionEvent 
        {
            /* Called upon when process is completed. */
            virtual void Completed(T completionData) = 0;
            /* Called upon when any message is passed to the listener. */
            virtual void Inform(CSTR message) = 0;
            /* Called upon when an exception occurs. */
            virtual void Error(ErrorId err, CSTR message) = 0;
        };
        
        struct IXbrlIdList : public IList<UID>
        {
            static SE_CODE XBRLEXP Create(ISession*, IXbrlIdList**);
        };
        
        typedef IXbrlIdList IMatchIdList;
        
        struct IXbrlDocType : public IObject, public ISerialization
        {
            virtual CSTR get_Name() = 0;
            virtual void set_Name(CSTR value) = 0;
            virtual CSTR get_Desc() = 0;
            virtual void set_Desc(CSTR value) = 0;
            virtual PERIOD get_PeriodType() = 0;
            virtual void set_PeriodType(PERIOD value) = 0;
        };
        
        struct IXbrlAbstractRecord;
        
        struct IXbrlStatement : public IIdName
        {
            virtual SE_CODE get_Tree(IXbrlAbstractRecord** ppOut) = 0;
            virtual I16 get_LowerYear() = 0;
            virtual I16 get_HighestYear() = 0;
        };
        
        struct IXbrlStatementList : public IROSPList<IXbrlStatement>
        {
            
        };
        
       struct IXbrlDataNode : public IObject, public ISerialization
        {
            virtual XDT get_Type() = 0;
            virtual R8 get_Monetary() = 0;
            virtual I64 get_Numeric() = 0;
            virtual CSTR get_Text() = 0;
            virtual CSTR get_Name() = 0;
            virtual CSTR get_Label() = 0;
            virtual ErrorId get_Children(IROSPList<IXbrlDataNode>** ppOut) = 0;
            virtual R8 get_Total() = 0;
            virtual XPT get_PeriodType() = 0;
            virtual IROSPList<IXbrlDataNode>* get_ChildItems() = 0;
            
        };
        
        struct IXbrlDataTree : public IObject, public ISerialization
        {
            virtual ErrorId get_Statements(IROSPList<IXbrlDataNode>** ppOut) = 0;
            virtual RP<se::securities::ISecurityDescription> get_SecurityDescription() = 0;
            
           
        };
        
        struct IFinancialsConsolidated;
        
        struct IXbrlField : public IObject, public ISerialization
        {
            virtual const OmniType&  get_Q1() const = 0;
            virtual const OmniType&  get_Q2() const = 0;
            virtual const OmniType&  get_Q3() const = 0;
            virtual const OmniType&  get_Q4() const = 0;
            virtual const OmniType&  get_HY1() const = 0;
            virtual const OmniType&  get_F3Q() const = 0;
            virtual const OmniType&  get_FY() const = 0;
            virtual const OmniType&  get_Q(I16 quarter) const = 0;
        };
        
        struct IXbrlAbstractRecord;
        
        struct IXbrlRecordList : public IROSPList<IXbrlAbstractRecord> 
        {
            virtual I16 get_LowerYear() = 0;
            virtual I16 get_HighestYear() = 0;
            // quarter to quarter data is added from lowest to highest available years.
            virtual SE_CODE FillEmptyFields() = 0;
        };
        
        
        struct IXbrlAbstractRecord : public IIdName
        {
            virtual bool get_IsAbstract() = 0;
            virtual const char* get_XbrlLabel() = 0;
            virtual RP<IXbrlAbstractRecord> get_Parent() = 0;
            virtual void set_Parent(IXbrlAbstractRecord*) = 0;
            virtual SE_CODE get_Children(IXbrlRecordList**) = 0;
        };
        
        struct IXbrlRecord : public IXbrlAbstractRecord
        {
            virtual XPT get_PeriodType() = 0;
            virtual I16 get_LowerYear() = 0;
            virtual I16 get_HighestYear() = 0;
            virtual I16 get_LatestQuarter() = 0;
            virtual SE_CODE get_Financials(I16 year, IXbrlField** ppOut) = 0;
            // quarter to quarter data is added from lowest to highest available years.
            virtual SE_CODE FillEmptyFields() = 0;
            //// set to true if the numbers for each quarter is a sum of previous quarter and real value of current quarter. In this case the last quarter value will be equal to full year value.
            virtual bool get_IsProgression() = 0;
            // removed from interface (the law of interface immutability is broker
            // in this smartearn libraries many times. but I still consider
            // this api as version 0.xxx pre-alpha)
            //virtual SE_CODE get_ConsolidatedView(IFinancialsConsolidated**) = 0;
        };
        
        typedef UID FNR;
        
        struct IFinancialRatioList;
        
        struct IFinancialRatioGroup : public IIdName
        {
            virtual bool get_IsHidden() = 0;
            virtual SE_CODE get_Ratios(IFinancialRatioList** ppOut) = 0;
        };
        
        enum class RatioComplexity 
        {
            SingleVariable = 0,
            MultiVariable = 1
        };
        
        struct IFinancialRatio : public IIdName
        {
            enum PerfValuePosition
            {
                Unknown,
                Good,
                Ok,
                Bad
            };
            
            virtual CSTR get_Description() = 0;
            virtual CSTR get_Importance() = 0;
            // returns a value between 0.0 and 1.0 which is considered a middle
            // point from bad to good. An value beteen middle point and 1.0 is
            // considered average to excellent. Values in between can be considered
            // average, good, better, best.
            // any value between 0.0 and middle point is considered worst to average.
            // values between are considerred worst, worse, bad and average.
            virtual R8 get_MiddlePoint() = 0;
            virtual R8 get_MinimumPoint() = 0;  // return MINR8 if not set
            virtual R8 get_MaximumPoint() = 0;  // return MAXR8 when not set.
            
            virtual double get_QMiddlePoint() = 0;
            virtual double get_QMaximumPoint() = 0;

            // returns latest annual value
            virtual R8 get_Value() = 0;

            // returns latest quarterly value.
            virtual R8 get_QValue() = 0;
            
            // returns annual value for the given year.
            virtual R8 get_Value(I16 year) = 0;
            
            virtual R8 get_Value(I16 year, I16 quarter) = 0;

            // returns latest quarterly value.
            virtual R8 get_Q1Value() = 0;
            // returns latest quarterly value.
            virtual R8 get_Q2Value() = 0;
            // returns latest quarterly value.
            virtual R8 get_Q3Value() = 0;
            // returns latest quarterly value.
            virtual R8 get_Q4Value() = 0;
            
            // returns latest quarterly value for the given year.
            virtual R8 get_Q1Value(I16 year) = 0;
            // returns latest quarterly value for the given year.
            virtual R8 get_Q2Value(I16 year) = 0;
            // returns latest quarterly value for the given year.
            virtual R8 get_Q3Value(I16 year) = 0;
            // returns latest quarterly value for the given year.
            virtual R8 get_Q4Value(I16 year) = 0;
            
            // get latest annual predicted value
            virtual R8 get_PredictedValue() = 0;
            
            // get the most recent future quarter predicted value.
            virtual R8 get_PredictedQuarterlyValue() = 0;

            // get a future year predicted value
            virtual R8 get_PredictedValue(I16 year) = 0;
            
            // get the future quarterly predicted value.
            virtual R8 get_PredictedValue(I16 year, I16 quarter) = 0;


            
            virtual I16 get_FloorYear() = 0;
            virtual I16 get_CeilYear() = 0;
            virtual I16 get_LastReportedQuarter() = 0;  // last reported quarter of get_CeilYear.
            
            virtual bool get_IsFree() = 0;
            virtual bool get_IsHigherBetter() = 0;
            
            virtual RatioComplexity get_Complexity() = 0;
            
            virtual IFinancialRatio::PerfValuePosition get_ValuePosition(I16 year, I16 quarter = 0) = 0;

            virtual IFinancialRatio::PerfValuePosition get_ValuePosition(R8 value) = 0;
            
            
            //--------------------------------------------------------------------------
            //virtual void set_Description(CSTR) = 0;
            //virtual void set_Importance(CSTR) = 0;
            //virtual void set_MiddlePoint(R8) = 0;
            //virtual void set_Value(R8) = 0;

            virtual SE_CODE Persist() = 0;
        };
        
        struct IFinancialRatioGroupList : public IROSPList<IFinancialRatioGroup>
        {
            virtual SE_CODE get_Ratios(UID groupId, IFinancialRatioList** ppOut) = 0;
        };
        
        struct IFinancialRatioList : public IROSPList<IFinancialRatio>
        {
            virtual SE_CODE get_Ratio(FNR ratio, IFinancialRatio** ppOut) = 0;
        };
        
        struct IFinancialPerformance : public IObject, public ISerialization
        {
            virtual CIK get_CIK() = 0;
            virtual R8 get_Ratio(FNR ratio) = 0;
            virtual SE_CODE get_Ratio(FNR ratio, IFinancialRatio** ppOut) = 0;
            virtual SE_CODE get_Ratios(IFinancialRatioList**) = 0;
            virtual SE_CODE get_RatioGroups(IFinancialRatioGroupList**) = 0;
        };
        
        struct IFinancialPerformanceList : public IROSPList<IFinancialPerformance>
        {
            enum QUERYFLDS
            {
                SectorIndustryId,   // gic id column in product table.
                ExchangeId,         // exchange field from product table.
            };

            // filter ratios using a value with +/- tolerance of certain percentage.
            // eg: Say value = 20, tolerance is 5%, then ratios within
            // the range of 19 - 21. 
            // set the range to zero for no tolerance.
            // set quarter to zero to get annual results for the given year. The value 
            // of the year should be above 2000
            // returns E_InvalidArg if year and/or ratioId are not given.
            virtual SE_CODE FilterRatioByValue(I16 year, I16 quarter, UID ratioId, R8 value, R8 tolerance, IFinancialPerformanceList** ppOut) = 0;
            
            // Filter a ratio by range
            // set quarter to zero to get annual results for the given year. The value 
            // of the year should be above 2000
            // returns E_InvalidArg if year, ratioId and/or rangeStart are not given.
            virtual SE_CODE FilterRatioByRange(I16 year, I16 quarter, UID ratioId, R8 rangeStart, R8 rangeEnd, IFinancialPerformanceList** ppOut) = 0;
            
            virtual long unsigned int FilterLatestAnnualRatioByRange(unsigned long ratioId, double rangeStart, double rangeEnd, IFinancialPerformanceList** ppOut) = 0;

            virtual long unsigned int FilterLatestQuarterlyRatioByRange(unsigned long ratioId, double rangeStart, double rangeEnd, IFinancialPerformanceList** ppOut) = 0;

            virtual long unsigned int FilterLatestAnnualRatioByValue(unsigned long ratioId, double value, double tolerance, IFinancialPerformanceList** ppOut) = 0;

            virtual long unsigned int FilterQuarterlyRatioByValue(unsigned long ratioId, double value, double tolerance, IFinancialPerformanceList** ppOut) = 0;
            
            virtual bool TryGet(CIK cik, IFinancialPerformance** ppOut) = 0;
            
        };
        
        struct IFinancialsConsolidated : 
                public IObject,
                public ISerialization
        {
            virtual CIK get_Cik() = 0;
            virtual I16 get_LowestYear() = 0;
            virtual I16 get_HighestYear() = 0;
            virtual I16 get_LatestQuarter() = 0;
            virtual const CDate& get_LatestQuarterlyFilingDate() = 0;
            virtual const CDate& get_LatestAnnualFilingDate() = 0;
            // get financial month/day of the company
            virtual const CDate& get_FinancialYearEnd() = 0;
            virtual SE_CODE get_Records(std::initializer_list<UID> xids, IXbrlRecordList** ppOut) = 0;
            virtual SE_CODE get_Record(UID xbrlId, IXbrlRecord** ppOut) = 0;
            virtual bool get_HasRecord(UID xbrlId) = 0;
            virtual SE_CODE get_Record(UID xbrlId, UID segmentId, IXbrlRecord** ppOut) = 0;
            virtual SE_CODE get_Performance(IFinancialPerformance** ppOut) = 0;
            virtual SE_CODE get_PerformanceRatio(UID perfId, IFinancialRatio** ppOut) = 0;
            virtual SE_CODE get_FiscalPeriodMappings(IFinancialPeriod** ppOut) = 0;
            
            // get account record based on match account approach
            virtual SE_CODE get_MatchedAccountRecord(UID matchId, IXbrlRecord** ppOut) = 0;
            
        };
        
        struct IFinancialStatementPresentation : public IObject, public ISerialization
        {
            virtual SE_CODE get_Statement(STATEMENTTYPES statementType, IXbrlStatement**) = 0;
            virtual SE_CODE get_Statements(IXbrlStatementList**) = 0;
        };
        
        struct IJsonOutputController : public IObject
        {
            virtual SE_CODE get_YearlyData(CIK cik, std::initializer_list<UID> xids, std::ostream& outputstream) = 0;
        };
        
        //typedef IROSPList<ISecurityDescription> ISecurityDescriptionList;
        typedef IROSPList<IXbrlDocType> IXbrlDocTypeList;
        
        typedef std::vector<U32> XbrlMatchId;
        typedef std::map<int, std::pair<CDate, CDate>> QuarterMonthMap;
        
        typedef std::tuple<DATAPERIOD, OmniType> FINPRDDATA, *PFINPRDDATA;
        
        struct IFinPeriodList : public IList<FINPRDDATA> 
        {
            virtual SE_CODE get(DATAPERIOD period, OmniType* pOut) = 0;
        };
        
        struct ICompetitionProfile
        {
            virtual CSTR get_Cik() = 0;
            virtual bool get_IsComplementor() = 0;  // when true, competition corelation is -ve, else corelation is +ve.
        };
        
        typedef IList<ICompetitionProfile> ICompetionProfileList;
        
        static NamedObjectNamingType IXbrlName = 0xE;
        
        /** The facade interface of this API. */
        struct IXbrl : public INamedObject
        {
            // Returns product id. if not found returns zero.
            virtual UID GetProductId(CIK cik) = 0;
            /** Returns the CIK of a company security symbol.
             Returns zero if not found.*/
            virtual SE_CODE GetCIK(CSTR symbol, IString** ppOut) = 0;
            
            virtual bool GetSymbol(CIK inCIK, IString** outSymbol) = 0;
            
            virtual SE_CODE GetEdgarCompanyInfo(CSTR symbolOrCIK, IPublicCompanyInfo** ppOut) = 0;
            
            virtual SE_CODE GetFinancialYearEnd(CIK cik, CDate* pdate) = 0;
            
            /* 
             * Read symbols from the symbol master table, and update CIK mapping table if not already done so.
             */
            virtual ErrorId UpdateCIKs(IXbrlCompletionEvent<int>* listener) = 0;
            
            virtual SE_CODE get_Label(IString** ppOut, UID xbrlId, CIK cik = nullptr) = 0;
            
            virtual ErrorId ProcessStatements() = 0;

            virtual ErrorId ProcessStatement(UID recid) = 0;
            
            // enumerates through all CIKs and persist performance ratios and other statistics.
            virtual ErrorId PersistPerformanceData() = 0;

            //For single CIK persist performance ratios and other statistics.
            virtual ErrorId PersistPerformanceData(CIK cik, UID PerfId = 0) = 0;
            
            
            // Returns a edgar financial reports for the given security description.
            virtual ErrorId GetStatements(se::securities::ISecurityDescription* pIn , DATAPERIOD& period, XFT formType, XDC xbrltype, IXbrlDataTree** ppOut) = 0;
            
            // Returns a edgar financial reports for the given Edgar CIK.
            virtual ErrorId GetStatements(CIK cik, DATAPERIOD& period, XFT formType, XDC xbrltype, IXbrlDataTree** ppOut) = 0;
            
            virtual ErrorId GetDocumentTypes(IROSPList<IXbrlDocType>** ppOut) = 0 ;
            
            virtual ErrorId GetPeriods(UID uidProdId,IROList<I16>** ppOut) = 0 ;
            
            virtual ErrorId GetStatementNames(UID uidProdId, DATAPERIOD& period, XFT formType, IROSPList<IXbrlStatement>** ppOut)=0;
            
            virtual ErrorId GetStatement(UID statementId, XDC, IXbrlDataNode** ppOut) = 0;
            
            // get consolidated financial statements
            // the last variadic parameter is a list of xbrlids
            virtual SE_CODE GetConsolidatedFinancials(CIK cik, IFinancialsConsolidated** ppOut, bool fillemptyValues = false) = 0;
            
            virtual SE_CODE GetFinancialPerformance(CIK cik, IFinancialPerformance** ppOut) = 0;

            virtual SE_CODE GetFinancialPerformance(CIK cik, UID perfId, IFinancialRatio** ppOut) = 0;
            
            // returns a collection of financial performances of all companies
            // based on the criteria.
            virtual SE_CODE GetCachedFinancialPerformances(IFinancialPerformanceList** ppOut) = 0;
            
            virtual SE_CODE GetCachedFinancialPerformance(CIK cik, unsigned long perfId, IFinancialRatio** ppOut) = 0;

            virtual SE_CODE GetCachedFinancialPerformance(CIK cik, bool OnlyPubliclyDisplayed, IFinancialPerformance** ppOut) = 0;
            
            virtual SE_CODE GetFinancialStatements(CIK cik, IXbrlStatementList** ppOut) = 0;

            virtual SE_CODE GetSelectedAccounts(CIK cik, const XbrlMatchId& mids, XbrlMatchId& availableMids, IXbrlRecordList** ppOut) = 0;
            
            virtual SE_CODE GetJsonOutputController(IJsonOutputController**) = 0;
            
            virtual SE_CODE get_CIK(UID ProdId, IString** ppOut) = 0;
            
            virtual SE_CODE FindCompany(CSTR criteria, ISecurityDescriptionList** ppOut) = 0;
        
            virtual SE_CODE GetLatestFilingDates(CIK cik, CDate* pDateAnnual, CDate* pDateQuarterly) = 0;
            
            virtual SE_CODE GetRatioGroups(IFinancialRatioGroupList** ppOut) = 0;

            virtual SE_CODE GetRatiosForGroup(UID groupId, IFinancialRatioList** ppOut) = 0;
            
            //virtual SE_CODE MapGivenQuarterToCompanyQuarter(CDate finyearEnd, DATAPERIOD dp, DATAPERIOD* pout) = 0;

            //virtual SE_CODE MapCompanyQuarterToRegularQuarter(CDate finyearEnd, DATAPERIOD dp, DATAPERIOD* pout) = 0 ;
            
            virtual SE_CODE GetMatchAccounts(UID matchId, IXbrlIdList** ppOut, CIK cik = 0) = 0;
            
            virtual SE_CODE GetMatchedAccountValue(UID productId, UID matchId, IXbrlRecord** ppOut) = 0;

            virtual SE_CODE GetMatchedAccountValue(CIK cik, UID matchId, IXbrlRecord** ppOut) = 0;
            
            virtual SE_CODE SavePredictedFinancialPerformance(CIK cik, UID perfId, DATAPERIOD dp, const OmniType& value) = 0;
          
            // get a training record for a security for prediction purpose.
            // productid : security product id.
            // xbrlmatchid : the field from xbrls match table, which is used to lookup up financial account record
            // of the company from consolidated finances data structure.
            // rebuild : the constructed training data record is cached in the database. set this field
            // to true to clear those records and rebuild.
            // ppOut : returned training record.
            virtual SE_CODE GetPerformanceTrainingData(
                UID securityId, 
                UID perfratioid, 
                DATAPERIOD from,
                DATAPERIOD to,
                se::training::ITrainingTable** ppOut) = 0;

            virtual SE_CODE GetPerformanceTrainingData(
                CIK cik, 
                UID perfratioid, 
                DATAPERIOD from,
                DATAPERIOD to,
                se::training::ITrainingTable** ppOut) = 0;
            
            virtual SE_CODE GetPerformanceTrainingData(
                UID securityId, 
                unsigned long perfratioid, 
                DATAPERIOD from, 
                DATAPERIOD to, 
                se::training::ITrainingTableList** ppOut) = 0;

            // returns a list of period-account values
            // that will include future values after
            // algorithmic predictions are computed.
            // future values are attributed (get_Property("IsFuture") = true);
            virtual SE_CODE GetProspectsView(
                UID productId, 
                UID matchid, 
                PERIOD interval,    // get annual or quarterly values (these are the only supported fields from PERIOD enum by this function)
                se::training::ITrainingTable ** ppOut
                ) = 0;
            
            // Revenue growth of a gic (sector or industry) 
            // recompute : set to true to recalculate growth averages from all companies under the gics id and save it to the database.
            // gics id - param - industry or sector id.
            // interval param : only annual and quarterly currently supported.
            // set ppOut parameter to nullptr if you're using this function only for recomputing purpose
            // not required to return anything.
            virtual SE_CODE GetGicsRevenueGrowth(bool recompute, UID gicId, PERIOD interval, IFinPeriodList** ppOut, OmniType* poutAverage) = 0;
            
            virtual SE_CODE ProcessProductKeyStats() = 0;
            
            virtual SE_CODE ProcessProductKeyStats(UID prodId) = 0;
            
            virtual SE_CODE CaptureFinancialPeriods(UID prodId, IFinancialPeriod** ppOut) = 0;
            
            virtual SE_CODE CaptureFinancialPeriods(CIK cik, IFinancialPeriod** ppOut) = 0;
            
            virtual SE_CODE GetCompetitionProfiles(CIK primary, ICompetionProfileList** ppOut) = 0;
            
        };

        extern "C" 
        {
            ErrorId XBRLEXP XbrlOpen(ISession*, IXbrl** outVal) ;

        }
        
    }
}




#endif	/* SEXBRLAPI_H */

