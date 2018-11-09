/* 
 * File:   CFinancialRatio.h
 * Author: santony
 *
 * Created on February 14, 2014, 3:04 PM
 */

#ifndef CFINANCIALRATIO_H
#define	CFINANCIALRATIO_H

#include <string>
#include <array>

#include "sexbrlapi.h"

using namespace std;

namespace se
{
    namespace xbrl
    {
        
        class CFinancialRatioList;

        class CFinancialRatio  : public CObjectRoot, public IFinancialRatio
        {
        private:
            enum ValuePostions
            {
                Year,
                Q1,
                Q2,
                Q3,
                Q4,
                PYear,
                PQ1,
                PQ2,
                PQ3,
                PQ4,
                __Size
            };
            typedef std::vector<double> VALUES;
            typedef map<I16, VALUES> ANNUALVALUES;

            friend class CFinancialPerformance;
            
        private:
            SE_CODE BuildRatios();
            SE_CODE BuildCachedRatios(CIK cik, UID PerfId);
            SE_CODE CalculateAndSetRatio(FNR ratio, CSTR formula, CSTR qformula);
            
        public:
            CFinancialRatio(ISession*);
            CFinancialRatio(const CFinancialRatio& orig)=delete;
            virtual ~CFinancialRatio();
            

            virtual long unsigned int Init();

            
            long unsigned int Init(UID perfId, IFinancialsConsolidated* p);

            long unsigned int Init(UID perfId, CIK cik);
            
            virtual const char* get_Description();

            virtual const char* get_Name();

            virtual const char* get_Importance();

            virtual double get_MiddlePoint();
            
            virtual double get_QMiddlePoint();

            virtual double get_MinimumPoint();

            virtual double get_MaximumPoint();
            
            virtual double get_QMaximumPoint();

            virtual bool get_IsFree();
            
            virtual bool get_IsHigherBetter();

            virtual double get_Value();

            virtual double get_QValue();

            virtual double get_Value(short year);

            virtual double get_Q1Value();

            virtual double get_Q1Value(short year);

            virtual double get_Q2Value();

            virtual double get_Q2Value(short year);

            virtual double get_Q3Value();

            virtual double get_Q3Value(short year);

            virtual double get_Q4Value();

            virtual double get_Q4Value(short year);
            
            virtual R8 get_Value(I16 year, I16 quarter);
            
            virtual short get_FloorYear();

            virtual short get_CeilYear();
            
            virtual short get_LastReportedQuarter();
            
            virtual RatioComplexity get_Complexity();
            
            void set_Complexity(RatioComplexity);
            
            virtual IFinancialRatio::PerfValuePosition get_ValuePosition(I16 year, I16 quarter = 0);

            virtual IFinancialRatio::PerfValuePosition get_ValuePosition(R8 value);
            
            void set_Description(const char*);

            void set_Importance(const char*);

            void set_MiddlePoint(double);
            void set_QMiddlePoint(double);
            void set_QMaximumPoint(double);
            
            void set_MinimumPoint(R8);
            void set_MaximumPoint(R8);
            void set_IsFree(bool);
            void set_IsHigherBetter(bool);

            void set_Name(const char* name);

            void set_Value(I16 year, double value);
            void set_Value(I16 year, I16 quarter, double value);
            void set_Q1Value(I16 year, double value);        
            void set_Q2Value(I16 year, double value);
            void set_Q3Value(I16 year, double value);
            void set_Q4Value(I16 year, double value);

            virtual double get_PredictedQuarterlyValue();
            virtual double get_PredictedValue(short year, short quarter);
            virtual double get_PredictedValue();
            virtual double get_PredictedValue(short year);
            
            void set_PredictedValue(I16 year, const R8 value = ER8);
            void set_PredictedValue(I16 year, I16 quarter, const R8 value = ER8);

            virtual void Serialize(ISerializedData<IString>* pIn);
            virtual long unsigned int Persist();

        private:
            VALUES& YearValue(I16 year = 0);
            void SetValue(I16 year, I16 quarter, bool pred, R8 value);
            
        private:
            string _name;
            string _desc;
            string _importance;
            R8 _middlePoint, _maxPoint, _minPoint, _qmaxpoint, _qmidpoint;
            ANNUALVALUES _values;
            I16 _floorYear, _ceilYear;
            bool _bIsFree, _bishigherbetter;
            I16 _lastq;
            IFinancialsConsolidated* _pcon;
            RatioComplexity _complexity;
            
        };
        
        
        
        /////////////////////////////// FinancialRatioGrup////////////////////
        
        class CFinancialRatioGroup  : public CObjectRoot, public IFinancialRatioGroup
        {
        public:
            CFinancialRatioGroup(ISession*);
            CFinancialRatioGroup(const CFinancialRatioGroup& orig)=delete;
            virtual ~CFinancialRatioGroup();
            
            virtual const char* get_Name();

            virtual void set_Name(const char* name);
            

            virtual bool get_IsHidden();
            void set_IsHidden(bool v);

            virtual long unsigned int get_Ratios(IFinancialRatioList** ppOut);
            
            void Add(IFinancialRatio* pIn);

            virtual void Serialize(ISerializedData<IString>* pIn);
            
        private:
            string _sname;
            SP<CFinancialRatioList> _ratios;
            bool _bhidden;
        };        
        
    }
    
}
#endif	/* CFINANCIALRATIO_H */

