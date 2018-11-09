/* 
 * File:   CFinancialRatio.cpp
 * Author: santony
 * 
 * Created on February 14, 2014, 3:04 PM
 */

#include "sexbrlinternal.h"
#include "CFinancialRatio.h"
#include "../seglobal/exceptions.h"
#include "CFinancialRatioList.h"
#include "CXbrlCalculator.h"
#include "CExplicitFormulaManager.h"
#include "CPersistPerformanceData.h"
#include <boost/regex.hpp>

using namespace se::data;

#define SEDBCONN        se::data::CSEConnections::GetMain()


namespace se
{
    namespace xbrl
    {

        CFinancialRatio::CFinancialRatio(ISession* ps) : CObjectRoot(ps) ,
            _floorYear(2999), _ceilYear(0), _middlePoint(ER8), _minPoint(MINR8), _maxPoint(MAXR8), _qmaxpoint(MAXR8), _qmidpoint(MAXR8), _bIsFree(false), _bishigherbetter(true), _lastq(0), _complexity(RatioComplexity::SingleVariable)
        {
        }

        CFinancialRatio::~CFinancialRatio() 
        {
            
        }

        long unsigned int CFinancialRatio::Init() 
        {
            return S_OK;
        }
        

        long unsigned int CFinancialRatio::Init(UID perfId, IFinancialsConsolidated* p) 
        {
            _pcon = p;
            set_Id(perfId);
            
            return BuildRatios();
        }

        SE_CODE CFinancialRatio::Init(unsigned long perfId, CIK cik)
        {
            set_Id(perfId);
            return BuildCachedRatios(cik, perfId);
        }
        
        long unsigned int CFinancialRatio::BuildRatios() 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialRatio::BuildRatios");
            CSmartConnection conn(SEDBCONN);
            Query q = conn->query();
            try
            {
                q << "call get_AllowedPerformanceFormulae(" << _pcon->get_Cik() << ", " << get_Id() << ")";
                auto sqr = q.store();
                if (!sqr.size())
                    throw seexception(E_NoDataToReturn, "Performance id not found.");

                const Row& row = sqr[0];
                
                this->set_Id(row["perf_id"]);
                this->set_Name(row["perf_name"]);
                this->set_Description(row["perf_desc"]);
                this->set_Importance(row["perf_imp"]);

                sql_double_null dval = row["perf_middlepoint"];
                this->set_MiddlePoint(dval.is_null ? ER8 : dval.data);
                sql_double_null dval0 = row["perf_qmiddlepoint"];
                this->set_QMiddlePoint(dval0.is_null ? ER8 : dval0.data);
                sql_double_null dval1 = row["perf_maxpoint"];
                this->set_MaximumPoint(dval1.is_null ? MAXR8 : dval1.data);
                sql_double_null dval11 = row["perf_qmaxpoint"];
                this->set_QMaximumPoint(dval11.is_null ? MAXR8 : dval11.data);
                sql_double_null dval2 = row["perf_minpoint"];
                this->set_MinimumPoint(dval2.is_null ? MINR8 : dval2.data);
                this->set_IsFree(row["perf_isfree"]=="1"?true:false);
                this->set_IsHigherBetter(row["perf_higherbetter"]=="1"?true:false);                
                set_Complexity((RatioComplexity)(int)row["perf_complexity"]);
                sql_varchar_null qformula = row["perf_qformula"];
                THROWIFFAILED(CalculateAndSetRatio(
                        get_Id(), 
                        (CSTR)row["perf_formula"], 
                        qformula.is_null | qformula.data == "" ? nullptr : qformula.data.c_str()
                        ), "Failed to calculate performance formula values.");

                while (q.more_results())
                    q.store_next();
                
                return S_OK;
            }
            catch(const std::exception& ex)
            {
                if (q)
                {
                    while (q.more_results())
                        q.store_next();
                }
               // LOGE(_logger, "Error: " << ex.what()) ;
                return  E_DBQueryException;
            }
        }

        long unsigned int CFinancialRatio::BuildCachedRatios(CIK cik, UID PerfId) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialRatio::BuildCachedRatios");
            CSmartConnection conn(SEDBCONN);
            Query q = conn->query();
            try
            {
                q << "select * from list_PerfRatioValues where cik = " << cik << " and perf_id = " << PerfId << " order by year, quarter";
               // LOGIP(q.str());
                StoreQueryResult sqr = q.store();
                if (!sqr.size())
                {
                    LOGRETURNIFFAILED(E_NotFound, "Performance ratios not avialable for this company..");
                }
                
                
                I16 lastyear = 0;
                for (const Row& row : sqr)
                {
                    I16 year = row["year"];
                    if (year != lastyear)
                    {
                        set_Id(row["perf_id"]);
                        set_Name(row["perf_name"]);
                        set_Description(row["perf_desc"]);
                        set_Importance(row["perf_imp"]);
                        sql_double_null dval = row["perf_middlepoint"];
                        set_MiddlePoint(dval.is_null ? ER8 : dval.data);
                        sql_double_null dval0 = row["perf_qmiddlepoint"];
                        set_QMiddlePoint(dval0.is_null ? ER8 : dval0.data);
                        sql_double_null dval1 = row["perf_maxpoint"];
                        set_MaximumPoint(dval1.is_null ? MAXR8 : dval1.data);
                        sql_double_null dval11 = row["perf_qmaxpoint"];
                        set_QMaximumPoint(dval11.is_null ? MAXR8 : dval11.data);
                        sql_double_null dval2 = row["perf_minpoint"];
                        set_MinimumPoint(dval2.is_null ? MINR8 : dval2.data);
                        set_IsFree(row["perf_isfree"]=="1"?true:false);
                        set_IsHigherBetter(row["perf_higherbetter"]=="1"?true:false);                
                        set_Complexity((RatioComplexity)(int)row["perf_complexity"]);
                    }
                    
                    I16 quarter = row["quarter"];
                    
#ifdef __DEBUG__
                if (PerfId == 21 && year == 2014)
                {
                    string s= "break";
                }
#endif
                    
                    
                    sql_double_null sqldperfvalue = row["perf_value"];
                    R8 dperfvalue = sqldperfvalue.is_null ? ER8 : sqldperfvalue.data;
                    if (dperfvalue != ER8 && lastyear != year)
                        _lastq = 0;

                    SetValue(year, quarter, false, dperfvalue);

                    sql_double_null sqldperfpredvalue = row["perf_pred_value"];
                    dperfvalue = sqldperfpredvalue.is_null ? ER8 : sqldperfpredvalue.data;

                    SetValue(year, quarter, true, dperfvalue);
                    
                    
                    lastyear = year;
                }
#ifdef __DEBUG__
                if (PerfId == 21)
                {
                    string s= "break";
                }
                auto val = this->get_QValue();
               // LOGIP(val);
#endif
                
                return S_OK;
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "Error: " << ex.what()) ;
                return  E_DBQueryException;
            }

        }

        void CFinancialRatio::SetValue(short year, short quarter, bool pred, double value) 
        {
            if (quarter == 0)
            {
                if (pred)
                    set_PredictedValue(year, value);
                else
                {
                    set_Value(year, value);
                }
            }
            else
            {
                if (pred)
                {
                    set_PredictedValue(year, quarter, value);
                }
                else
                {
                    switch (quarter)
                    {
                        case 1:
                            set_Q1Value(year, value);
                            break;
                        case 2:
                            set_Q2Value(year, value);
                            break;
                        case 3:
                            set_Q3Value(year, value);
                            break;
                        case 4:
                            set_Q4Value(year, value);
                            break;
                    }
                }
            }     

            if (!quarter)
                _lastq = 0;
            
            if (!pred && value != ER8)
            {
                _floorYear = std::min(year, _floorYear);
                _ceilYear = std::max(year, _ceilYear);
                _lastq = std::max(_lastq, quarter);
            }
        }
        

        long unsigned int CFinancialRatio::Persist() 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialRatio::Persist");
            CSmartConnection conn(SEDBCONN);
            Query q = conn->query();
            try
            {
                return CPersistPerformanceData::ProcessRatio(conn, _pcon->get_Cik(), this);
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "Error: " << ex.what()) ;
                return  E_DBQueryException;
            }

        }
        
        #define HASANYFORMULA ( (formula && strlen(formula))  || (qformula && strlen(qformula)))

        SE_CODE CFinancialRatio::CalculateAndSetRatio(FNR ratio, const char* formula, const char* qformula) 
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.CFinancialRatio::CalculateAndSetRatio");
            try
            {
                if (!HASANYFORMULA)
                {
                   // LOGW(_logger, "No formula defined for the performance ratio #" << ratio);
                    return E_InvalidArg;
                }
                
#ifdef __DEBUG__
                if (ratio == 21)
                {
                    string s = "break";
                }
#endif
                
                _ceilYear = _pcon->get_HighestYear();
                _floorYear = _pcon->get_LowestYear();
                _lastq = _pcon->get_LatestQuarter();
                //for (I16 year = _pcon->get_HighestYear(); year >= _pcon->get_LowestYear(); --year)
                SP<CExplicitFormulaManager> explFunc;
                SP<CExplicitFormulaManagerState> explState;
                bool explicitFunction = false;
                if (formula && strlen(formula) && strcasecmp(formula, "N/A"))
                {
                    const boost::regex rg("[A-Za-z]{1, 50}");
                    explicitFunction = boost::regex_match(formula, rg);
                    if (explicitFunction)
                    {
                        RETURNIFFAILED(CExplicitFormulaManagerState::Instance(get_Session(),  &explState));
                        RETURNIFFAILED(CExplicitFormulaManager::Instance(get_Session(), &explFunc));
                        // add this instance as a state value
                        explState->set_Property("FinancialRatio", (POINTER)this );
                    }
                }
                string qfmla = qformula ? qformula : "";
                if (qfmla.empty())
                    qfmla = formula;
                if (!qfmla.empty() && qfmla !=  "N/A")
                {
                    const boost::regex rg("[A-Za-z]{1, 50}");
                    explicitFunction = boost::regex_match(qfmla, rg);
                    if (explicitFunction && !explFunc)
                    {
                        RETURNIFFAILED(CExplicitFormulaManagerState::Instance(get_Session(), &explState));
                        RETURNIFFAILED(CExplicitFormulaManager::Instance(get_Session(), &explFunc));
                        // add this instance as a state value
                        explState->set_Property("FinancialRatio", this);
                    }                        
                }
                for (I16 year = _floorYear; year <= _ceilYear; year++)
                {
                    _pcon->set_Property("Year", (I32)year);
                    _pcon->set_Property("Quarter", (I32)0);

                    // if formula is a simple string, then we send the task to ExplicitFormulaManager.
                    // if a formula is not defined for full year analysis, then we have to move forward to quarter.

                    double d = ER8;
                    if (explicitFunction)
                    {
                        SE_CODE ret = explFunc->Run(explState, _pcon, formula, year, &d);
                        if (ret == S_CalcCompleted)
                            return S_OK;

                        else if (FAILED(ret))
                        {
                           // LOGW(_logger, "Failed to get explict formula value for perfid " << get_Id() << " for full year " << year);
                        }
                    }
                    else
                    {
                        d = Calculate(this->get_Session(), this->_pcon, formula);
                    }
                    set_Value(year, d);
                   // LOGI(_logger, "Ratio (" << year << " FY): " << get_Name() << " = " << d);
                    for (auto q = 1; q <=4; ++q)
                    {
                        d = ER8;
                        if (explicitFunction)
                        {
                            SE_CODE ret = explFunc->Run(explState, _pcon, qfmla.c_str(), year, q, &d);
                            if (ret == S_CalcCompleted)
                                return S_OK;

                            else if (FAILED(ret))
                            {
                               // LOGW(_logger, "Failed to get explict formula value for perfid " << get_Id() << " for year " << year << " and quarter " << q);
                            }

                        }
                        else
                        {
                            _pcon->set_Property("Quarter", (I32)q);
                            d = Calculate(get_Session(), this->_pcon, qfmla.c_str());
                        }                            
                       // LOGI(_logger, "Ratio (" << year << " Q" << q << "): " << get_Name() << " = " << d);
                        switch (q)
                        {
                            case 1:
                                set_Q1Value(year, d);
                                break;
                            case 2:
                                set_Q2Value(year, d);
                                break;
                            case 3:
                                set_Q3Value(year, d);
                                break;
                            case 4:
                                set_Q4Value(year, d);
                                break;
                        }

//                        if (year == _ceilYear && d == ER8)
//                        {
//                            _lastq = q;
//                            break;
//                        }
                        
                        if (year == _ceilYear && q == _lastq)
                            break;
                        
                    }
                }
                return S_OK;
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "Error: " << ex.what());
                return E_UnknownError;
            }
            catch(...)
            {
               // LOGE(_logger, "Unknown error.");
                return E_UnknownError;
            }
        }

        CFinancialRatio::VALUES& CFinancialRatio::YearValue(short year) 
        {
            auto find = _values.find(year);
            if (find == _values.end())
            {
                VALUES v(CFinancialRatio::__Size, ER8);
                auto it = _values.insert(make_pair(year, v));
                if (it.second)
                {
                    return it.first->second;
                }
                else
                    throw seexception("Failed to insert a financial ratio node.");
            }
            else
                return find->second;
        }


        const char* CFinancialRatio::get_Description() 
        {
            return _desc.c_str();
        }

        const char* CFinancialRatio::get_Importance() {
            
            return _importance.c_str();

        }

        double CFinancialRatio::get_MiddlePoint() {
            return _middlePoint;

        }

        double CFinancialRatio::get_QMiddlePoint() {
            return _qmidpoint;
        }

        double CFinancialRatio::get_MinimumPoint() {
            return _minPoint;
        }

        double CFinancialRatio::get_MaximumPoint() {
            return _maxPoint;
        }

        double CFinancialRatio::get_QMaximumPoint() {
            return _qmaxpoint;
        }
        

        bool CFinancialRatio::get_IsFree() 
        {
            return _bIsFree;
        }

        bool CFinancialRatio::get_IsHigherBetter() {
            return _bishigherbetter;
        }
        

        const char* CFinancialRatio::get_Name() {
            return _name.c_str();
        }

        short CFinancialRatio::get_FloorYear() 
        {
            return _floorYear;
        }

        short CFinancialRatio::get_CeilYear() 
        {
            return _ceilYear;
        }

        short CFinancialRatio::get_LastReportedQuarter() 
        {
            return !_lastq ? 4 : _lastq;
        }

        IFinancialRatio::PerfValuePosition CFinancialRatio::get_ValuePosition(double yrv) 
        {
            if (yrv == ER8)
                return IFinancialRatio::Unknown;
            else
            {
                if (_bishigherbetter && yrv >= _middlePoint)
                    return IFinancialRatio::Good;
                
                if (!_bishigherbetter && yrv <= _middlePoint)
                    return IFinancialRatio::Good;
                
                R8 less10 ;
                if (_bishigherbetter)
                {
                    less10 = _middlePoint - (_middlePoint * .1);
                    if (yrv >= less10)
                        return IFinancialRatio::Ok;
                    else if (yrv < less10)
                        return IFinancialRatio::Bad;
                }
                else
                {
                    less10 = _middlePoint + (_middlePoint * .1);
                    if (yrv <= less10)
                        return IFinancialRatio::Ok;
                    else if (yrv > less10)
                        return IFinancialRatio::Bad;
                }
            }
            return IFinancialRatio::Unknown;
        }


        IFinancialRatio::PerfValuePosition CFinancialRatio::get_ValuePosition(I16 year, I16 quarter) 
        {
            if (quarter < 0 || quarter > 4)
                throw seexception("Invalid quarter. Valid range between 0 and 4");
            
            R8 yrv;
            if (quarter == 0)
                 yrv = get_Value(year);
            else
                yrv = get_Value(year, quarter);
            return get_ValuePosition(yrv);
        }
        

        double CFinancialRatio::get_Value() 
        {
            auto it = _values.rbegin();
            if (it != _values.rend())
                return it->second[Year];
            return ER8;
        }
        
        double CFinancialRatio::get_PredictedValue()
        {
            auto it = _values.rbegin();
            if (it != _values.rend())
                return it->second[PYear];
            return ER8;
        }

        double CFinancialRatio::get_PredictedQuarterlyValue() 
        {
            auto it = _values.rbegin();
            if (it != _values.rend())
            {
                I16 year = it->first;
                R8 ret = ER8;
                for (int q = 4; q >= 1; --q)
                {
                    ret = get_PredictedValue(year, q);
                    if (ret == ER8)
                        continue;
                }
                return ret;
            }
            return ER8;
        }


        double CFinancialRatio::get_QValue() 
        {
            switch (_lastq)
            {
                case 1:
                    return get_Q1Value(_ceilYear);
                case 2:
                    return get_Q2Value(_ceilYear);
                case 3:
                    return get_Q3Value(_ceilYear);
                case 4: 
                    return get_Q4Value(_ceilYear);
            }
            return ER8;
        }

        void CFinancialRatio::set_Description(const char* value) 
        {
            _desc = value;
        }

        void CFinancialRatio::set_Importance(const char* value) 
        {
            _importance = value;
        }

        void CFinancialRatio::set_MiddlePoint(double value) 
        {
            _middlePoint = value;
        }

        void CFinancialRatio::set_QMiddlePoint(double v) {
            _qmidpoint = v;
        }

        void CFinancialRatio::set_MaximumPoint(double v) 
        {
                _maxPoint = v;
        }

        void CFinancialRatio::set_QMaximumPoint(double v) {
            _qmaxpoint = v;
        }

        
        void CFinancialRatio::set_MinimumPoint(double v) 
        {
            _minPoint = v;
        }

        void CFinancialRatio::set_IsFree(bool v) {
            _bIsFree = v;
        }
        void CFinancialRatio::set_IsHigherBetter(bool v) 
        {
            _bishigherbetter = v;
        }


        void CFinancialRatio::set_Name(const char* value) 
        {
            _name = value;
        }

        RatioComplexity CFinancialRatio::get_Complexity() 
        {
            return _complexity;
        }
        
        void CFinancialRatio::set_Complexity(RatioComplexity value) 
        {
            _complexity = value;
        }

        double CFinancialRatio::get_Value(short year, short quarter) 
        {
            if (quarter < 0 || quarter > 4)
                throw seexception("Invalid quarter. Valid range between 0 and 4");
            if (!quarter)
                return get_Value(year);
            else
            {
                switch (quarter)
                {
                    case 1:
                        return get_Q1Value(year);
                    case 2:
                        return get_Q2Value(year);
                    case 3:
                        return get_Q3Value(year);
                    case 4:
                        return get_Q4Value(year);
                }
            }
            return ER8;
        }


        double CFinancialRatio::get_Value(short year) 
        {
            return YearValue(year)[Year];
        }

        double CFinancialRatio::get_PredictedValue(short year) 
        {
            return YearValue(year)[PYear];
        }

        double CFinancialRatio::get_PredictedValue(short year, short quarter) 
        {
            switch (quarter)
            {
                case 1:
                    return YearValue(year)[PQ1];
                case 2:
                    return YearValue(year)[PQ2];
                case 3:
                    return YearValue(year)[PQ3];
                case 4:
                    return YearValue(year)[PQ4];
            }
            return ER8;
        }
        
        
        double CFinancialRatio::get_Q1Value() 
        {
            auto it = _values.rbegin();
            if (it != _values.rend())
                return it->second[Q1];
            return 0.0;
        }

        double CFinancialRatio::get_Q1Value(short year) 
        {
            return YearValue(year)[Q1];
        }

        double CFinancialRatio::get_Q2Value() 
        {
            auto it = _values.rbegin();
            if (it != _values.rend())
                return it->second[Q2];
            return ER8;
        }

        double CFinancialRatio::get_Q2Value(short year) 
        {
            return YearValue(year)[Q2];
        }

        double CFinancialRatio::get_Q3Value() 
        {
            auto it = _values.rbegin();
            if (it != _values.rend())
                return it->second[Q3];
            return ER8;
        }

        double CFinancialRatio::get_Q3Value(short year) 
        {
            return YearValue(year)[Q3];
        }

        double CFinancialRatio::get_Q4Value() 
        {
            auto it = _values.rbegin();
            if (it != _values.rend())
                return it->second[Q4];
            return ER8;
        }

        double CFinancialRatio::get_Q4Value(short year) 
        {
            return YearValue(year)[Q4];
        }
        
        

        void CFinancialRatio::set_Value(short year, double value) 
        {
            YearValue(year)[Year] = value;
        }
        
        void CFinancialRatio::set_Value(short year, short quarter, double value)
        {
            switch (quarter)
            {
            case 1:
                YearValue(year)[Q1] = value;
                break;
            case 2:
                YearValue(year)[Q2] = value;
                break;
            case 3:
                YearValue(year)[Q3] = value;
                break;
            case 4:
                YearValue(year)[Q4] = value;
                break;
            }
        }

        void CFinancialRatio::set_Q1Value(short year, double value) 
        {
            YearValue(year)[Q1] = value;
        }

        void CFinancialRatio::set_Q2Value(short year, double value) 
        {
            YearValue(year)[Q2] = value;
        }

        void CFinancialRatio::set_Q3Value(short year, double value) 
        {
            YearValue(year)[Q3] = value;
        }

        void CFinancialRatio::set_Q4Value(short year, double value) 
        {
            YearValue(year)[Q4] = value;
        }

        void CFinancialRatio::set_PredictedValue(short year, const R8 value)
        {
            YearValue(year)[PYear] = (R8)value;
        }

        void CFinancialRatio::set_PredictedValue(short year, short quarter, const double value) 
        {
            switch (quarter)
            {
            case 1:
                YearValue(year)[PQ1] = value;
                break;
            case 2:
                YearValue(year)[PQ2] = value;
                break;
            case 3:
                YearValue(year)[PQ3] = value;
                break;
            case 4:
                YearValue(year)[PQ4] = value;
                break;
            }
        }
        
        
        void CFinancialRatio::Serialize(ISerializedData<IString>* pIn) 
        {
            pIn->Add("Id", get_Id());
            pIn->Add("Description", _desc.c_str());
            pIn->Add("Name", _name.c_str());
            pIn->Add("Importance", _importance.c_str());
            ostringstream str;
            
            str << _middlePoint;
            pIn->Add("MiddlePoint", _middlePoint == ER8 ? "-" : str.str().c_str() );
            str.str("");

            str << _qmaxpoint;
            pIn->Add("QMiddlePoint", _qmidpoint == ER8 ? "-" : str.str().c_str() );
            str.str("");

            str << _minPoint;
            pIn->Add("MinPoint", _minPoint == MINR8 ? "-" : str.str().c_str() );
            str.str("");

            str << _maxPoint;
            pIn->Add("MaxPoint", _maxPoint == MAXR8 ? "-" : str.str().c_str() );
            str.str("");

            str << _qmaxpoint;
            pIn->Add("QMaxPoint", _qmaxpoint == MAXR8 ? "-" : str.str().c_str() );
            str.str("");
            
            double v = get_Value();
            str << v;
            pIn->Add("LatestAnnualValue", v == ER8  ?  "-" : str.str().c_str() );

            v = get_PredictedValue();
            str << v;
            pIn->Add("LatestAnnualPredictedValue", v == ER8  ?  "-" : str.str().c_str() );

            v = get_PredictedQuarterlyValue();
            str << v;
            pIn->Add("LatestQuarterlyPredictedValue", v == ER8  ?  "-" : str.str().c_str() );
            
            v = get_Q1Value();
            str.str("");
            str << v;
            pIn->Add("LatestQ1Value", v == ER8  ?  "-" : str.str().c_str() );

            v = get_Q2Value();
            str.str("");
            str << v;
            pIn->Add("LatestQ2Value", v == ER8  ?  "-" : str.str().c_str() );
            
            v = get_Q3Value();
            str.str("");
            str << v;
            pIn->Add("LatestQ3Value", v == ER8  ?  "-" : str.str().c_str()  );
            
            v = get_Q4Value();
            str.str("");
            str << v;
            pIn->Add("LatestQ41Value", v == ER8  ?  "-" : str.str().c_str()  );
            
            pIn->Add("CeilYear", _ceilYear);
            pIn->Add("FloorYear", _floorYear);
            
            pIn->Add("HigherBetter", _bishigherbetter);
            
        }
        
        
        ///////////////////////////////CFinancialRatioGroup/////////////////////////////

        CFinancialRatioGroup::CFinancialRatioGroup(ISession* ps) :
                CObjectRoot(ps)
        {

        }

        
        CFinancialRatioGroup::~CFinancialRatioGroup()
        {
            
        }

        const char* CFinancialRatioGroup::get_Name() 
        {
            return _sname.c_str();
        }

        void CFinancialRatioGroup::set_Name(const char* name) 
        {
            _sname = name;
        }

        bool CFinancialRatioGroup::get_IsHidden() {
            return _bhidden;
        }

        void CFinancialRatioGroup::set_IsHidden(bool v) {
            _bhidden = v;
        }
        

        void CFinancialRatioGroup::Add(IFinancialRatio* pIn) 
        {
            if (!_ratios)
                CObject<CFinancialRatioList>::Create(get_Session(), &_ratios);
            _ratios->Add(pIn);
        }


        long unsigned int CFinancialRatioGroup::get_Ratios(IFinancialRatioList** ppOut) 
        {
            _ratios.CopyTo(ppOut);
            return S_OK;
        }

        void CFinancialRatioGroup::Serialize(ISerializedData<IString>* pIn) 
        {
            pIn->Add("Id", get_Id());
            pIn->Add("Name", get_Name());
            pIn->Add("Ratios", _ratios._ptr);
            SP<IFinancialRatioList> pratios;
            if (SUCCEEDED(get_Ratios(&pratios)) && pratios)
            {
                pIn->Add("Ratios of this group", pratios);
            }
        }
    }
}