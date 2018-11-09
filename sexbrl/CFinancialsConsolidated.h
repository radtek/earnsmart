/* 
 * File:   CFinancialsConsolidated.h
 * Author: santony
 *
 * Created on January 27, 2014, 7:08 PM
 */

#ifndef CFINANCIALSCONSOLIDATED_H
#define	CFINANCIALSCONSOLIDATED_H

////#include <log4cxx/logger.h>
#include <set>
#include "CFinancialItem.h"
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include "../seglobal/CFinancialPeriod.h"



using namespace std;
////using namespace log4cxx;
using namespace se;
using namespace se::xbrl;
using namespace boost;
using namespace boost::graph;


typedef FinItemNode<OmniType> FinItemNodeType;

class CFinancialsConsolidated : 
        public CObjectRoot, 
        public IFinancialsConsolidated
{
    typedef map<int, Row> ExRows;
    
public:
    CFinancialsConsolidated(ISession*);
    CFinancialsConsolidated(const CFinancialsConsolidated& orig)=delete;
    virtual ~CFinancialsConsolidated();
    
#ifdef __DEBUG__
    virtual void _AddRef();
    virtual void _DecRef();
#endif
    
    virtual void Serialize(ISerializedData<IString>* pIn);
    
    SE_CODE Init(CIK cik, bool fillEmptyValues = true);
    
    virtual CIK get_Cik();

    virtual const CDate& get_LatestAnnualFilingDate();

    virtual const CDate& get_LatestQuarterlyFilingDate();

    virtual const CDate& get_FinancialYearEnd() ;
    
    virtual bool get_HasRecord(unsigned long xbrlId);

    virtual SE_CODE get_Records(std::initializer_list<UID> xids, IXbrlRecordList** ppOut);

    virtual long unsigned int get_Record(unsigned long xbrlId, IXbrlRecord** ppOut);

    virtual long unsigned int get_Record(unsigned long xbrlId, unsigned long segmentId, IXbrlRecord** ppOut);

    virtual long unsigned int get_Performance(IFinancialPerformance** ppOut);

    virtual long unsigned int get_PerformanceRatio(unsigned long perfId, IFinancialRatio** ppOut);

    virtual short get_LowestYear();

    virtual short get_HighestYear();

    virtual short get_LatestQuarter();

    virtual SE_CODE get_FiscalPeriodMappings(IFinancialPeriod** ppOut) ;
    

    virtual long unsigned int get_MatchedAccountRecord(unsigned long matchId, IXbrlRecord** ppOut);

    
private:
    FinItemNodeType& ManageFinItemNode(UID xid, int& index, bool& newlyCreated);
    FinItemNodeType& ManageFinItemNode(FinItemNodeType& pnode, UID xid, bool& newlyCreated);
    void SubmitToDb();
    void HandleData(CSmartConnection& conn, const StoreQueryResult& qr);
    
    bool SetQuarterlyValue(
        FinItemNodeType& node, 
        UID segId, 
        const CDate& startDate, 
        const CDate& endDate, 
        int numMonths, 
        int numDays, 
        const OmniType& value, 
        XPT contextType);
    
    bool SetDurationValue(
        FinItemNodeType& node, 
        UID segId, 
        FinItemNodeType::Duration duration, 
        const CDate& startDate, 
        const CDate& endDate, 
        int numMonths, 
        int numDays, 
        const OmniType& value, 
        XPT contextType);
    
    bool SetFullYearValue(
        FinItemNodeType& node, 
        UID segId, 
        const CDate& startDate, 
        const CDate& endDate, 
        int numMonths, 
        int numDays, 
        const OmniType& value, 
        XPT contextType);
    
    void Print();
    void Reconcile(bool fillemptyvalues);
    int WhatIsFinEndYear(CDate&, CDate&);
    void ProcessExceptionalRows(ExRows& rows);
    void SetFinancialDates(CSmartConnection& conn);
    void SetYearRange(const DATAPERIOD&);
    bool PeriodMapping(const CDate& endDate, DATAPERIOD* dpOut);
    
private:
    
    string _cik;
    typedef vector<FinItemNodeType> FinItemList; 
    typedef map<I64, int> FinItemListIndex; 
    FinItemList _l;
    FinItemListIndex _m;
    ////LoggerPtr _logger;
    CDate _finyr;
    typedef set<int> DocumentTypes;
    DocumentTypes docyearly, docquarterly;
    CDate _latestAnnualFilingDate, _latestQuarterlyFilingDate;
    I16 _lowestYear, _highestYear;
    DATAPERIOD _ceilPer;
    SP<IXbrl> pXapi;
    SP<IFinancialPeriod> _fper;
};

#endif	/* CFINANCIALSCONSOLIDATED_H */

