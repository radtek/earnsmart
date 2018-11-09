/* 
 * File:   CXbrl.h
 * Author: santony
 *
 * Created on July 1, 2012, 9:41 PM
 */

#ifndef CXBRL_H
#define	CXBRL_H

#include "../seglobal/CObjectRoot.h"
#include <string>
#include "CXbrlDocType.h"
#include "../seglobal/CROList.h"
#include "../seglobal/CList.h"
#include "../semysql/CStoreQueryResult.h"
#include <Poco/Mutex.h>
#include <unordered_set>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace se;
using namespace se::xbrl;
////using namespace log4cxx;
using namespace se::data;

class CXbrl : public CObjectRoot, public IXbrl
{

    friend class CFinancialsConsolidated;
    friend class CProcessEdgarStatements;
    
public:
    CXbrl(ISession*);
    CXbrl(const CXbrl& orig)=delete;
    virtual ~CXbrl();

    NamedObjectNamingType get_TypeId() override;

    
    virtual unsigned long GetProductId(CIK cik);
    
    virtual SE_CODE GetCIK(CSTR symbol, IString** ppOut);
    
    virtual bool GetSymbol(CIK inCIK, IString** outSymbol);
    
    virtual ErrorId UpdateCIKs(IXbrlCompletionEvent<int>* listener);
    
    //virtual ErrorId ProcessEdgarFormIdx(const YEARQUARTER& period, CSTR formType) ;
   
    virtual ErrorId ProcessStatements();
    
    virtual ErrorId ProcessStatement(UID recid);
    

    virtual long unsigned int PersistPerformanceData();

    virtual long unsigned int PersistPerformanceData(CIK cik, UID PerfId = 0);

    // Returns a edgar financial reports for the given security description.
    virtual ErrorId GetStatements(se::securities::ISecurityDescription* pIn, DATAPERIOD& period, XFT formType, XDC xbrltype, IXbrlDataTree** ppOut) ;

    // Returns a edgar financial reports for the given Edgar CIK.
    virtual ErrorId GetStatements(CIK cik, DATAPERIOD& period, XFT formType, XDC xbrltype, IXbrlDataTree** ppOut) ;
    
    virtual ErrorId GetDocumentTypes(IROSPList<IXbrlDocType>** ppOut);
    
    virtual ErrorId GetPeriods(UID uidProdId,IROList<I16>** ppOut);
    
    virtual ErrorId GetStatementNames(UID uidProdId, DATAPERIOD& period, XFT formType, IROSPList<IXbrlStatement>** ppOut);

    virtual ErrorId GetStatement(UID statementId, XDC, IXbrlDataNode** ppOut);
     
    virtual SE_CODE GetConsolidatedFinancials(CIK cik, IFinancialsConsolidated** ppOut, bool fillemptyValues = false);

    virtual SE_CODE get_Label(IString** ppOut, unsigned long xbrlId, CIK cik = nullptr);

    virtual long unsigned int GetJsonOutputController(IJsonOutputController**);

    virtual SE_CODE get_CIK(unsigned long ProdId, IString** ppOut);

    virtual long unsigned int GetEdgarCompanyInfo(const char* symbolOrCIK, IPublicCompanyInfo** ppOut);

    virtual SE_CODE FindCompany(CSTR criteria, ISecurityDescriptionList** ppOut);

    virtual long unsigned int GetFinancialPerformance(CIK cik, IFinancialPerformance** ppOut);
    
    virtual long unsigned int GetFinancialPerformance(CIK cik, unsigned long perfId, IFinancialRatio** ppOut);
    
    virtual long unsigned int GetCachedFinancialPerformance(CIK cik, bool OnlyPubliclyDisplayed, IFinancialPerformance** ppOut);

    virtual long unsigned int GetCachedFinancialPerformance(CIK cik, unsigned long perfId, IFinancialRatio** ppOut);
    
    virtual long unsigned int GetCachedFinancialPerformances(IFinancialPerformanceList** ppOut);

    virtual long unsigned int GetFinancialStatements(CIK cik, IXbrlStatementList** ppOut);

    virtual long unsigned int GetSelectedAccounts(CIK cik, const XbrlMatchId& xids, XbrlMatchId& availableMids, IXbrlRecordList** ppOut);
    
    virtual SE_CODE GetLatestFilingDates(CIK cik, CDate* pDateAnnual, CDate* pDateQuarterly);

    virtual SE_CODE GetFinancialYearEnd(CIK cik, CDate* pdate);
    
    // for a given financial year end, corresponding quarters' start and end dates are provided in a map
    // so for a company that reports 4th quarterly earnings on 3/31/year, map[4] = 1/1/year -3/31/year
    SE_CODE MapFinancialQuartersToMonths(const CDate& finyearEnd, QuarterMonthMap* map);
    
    //virtual SE_CODE MapGivenQuarterToCompanyQuarter(CDate finyearEnd, DATAPERIOD dp, DATAPERIOD* pout) ;

    //virtual SE_CODE MapCompanyQuarterToRegularQuarter(CDate finyearEnd, DATAPERIOD dp, DATAPERIOD* pout) ;
    
    // for a given financial year end, real yearly quarters' start and end dates are provided in a map
    // so for a company that reports 4th quarterly earnings on 3/31/year, map[1] = 1/1/year -3/31/year
    SE_CODE MapFinancialQuartersToRealMonths(const CDate& finyearEnd, QuarterMonthMap* map);

    virtual long unsigned int GetRatioGroups(IFinancialRatioGroupList** ppOut);
    
    virtual SE_CODE GetRatiosForGroup(UID groupId, IFinancialRatioList** ppOut);

    virtual long unsigned int GetMatchAccounts(unsigned long matchId, IXbrlIdList** ppOut, CIK cik = 0);
    
    virtual SE_CODE GetMatchedAccountValue(UID productId, UID matchId, IXbrlRecord** ppOut);
    
    virtual SE_CODE GetMatchedAccountValue(CIK cik, UID matchId, IXbrlRecord** ppOut);

    virtual long unsigned int GetPerformanceTrainingData(unsigned long securityId, unsigned long perfratioid, DATAPERIOD from, DATAPERIOD to, se::training::ITrainingTable** ppOut);

    virtual long unsigned int GetPerformanceTrainingData(CIK cik, unsigned long perfratioid, DATAPERIOD from, DATAPERIOD to, se::training::ITrainingTable** ppOut);

    virtual long unsigned int GetPerformanceTrainingData(unsigned long securityId, unsigned long perfratioid, DATAPERIOD from, DATAPERIOD to, se::training::ITrainingTableList** ppOut);
    
    virtual long unsigned int SavePredictedFinancialPerformance(CIK cik, unsigned long perfId, DATAPERIOD dp, const OmniType& value);

    virtual SE_CODE GetProspectsView(
                UID productId, 
                UID matchid, 
                PERIOD interval,
                se::training::ITrainingTable ** ppOut
                );
    
    virtual SE_CODE GetGicsRevenueGrowth(bool recompute, UID gicId, PERIOD interval, IFinPeriodList** ppOut, OmniType* poutAverage);

    virtual SE_CODE ProcessProductKeyStats();
    
    virtual SE_CODE ProcessProductKeyStats(UID prodId) ;
    

    virtual long unsigned int CaptureFinancialPeriods(unsigned long prodId, IFinancialPeriod** ppOut);

    virtual long unsigned int CaptureFinancialPeriods(CIK cik, IFinancialPeriod** ppOut);


    virtual long unsigned int GetCompetitionProfiles(CIK primary, ICompetionProfileList** ppOut);


    SE_CODE _GetFinancialPerformance(IFinancialsConsolidated* pcon, IFinancialPerformance** ppOut);
    
private:
    SE_CODE get_Label(CSmartConnection& conn,  IString** ppOut, unsigned long xbrlId, CIK cik = nullptr);    
    SE_CODE RunRegressionTree(se::training::IExTrainingTable* extenTable, const DATAPERIOD& newperiod) ;
    SE_CODE _GetProspectsView(
                UID productId, 
                UID matchid, 
                PERIOD interval,
                se::training::ITrainingTable *& ptbl
                );
    SE_CODE _GetProspectsViewFast(
                IFinancialsConsolidated* pcon,
                UID matchid, 
                PERIOD interval,
                se::training::ITrainingTable *& ptbl
                );
    

    SE_CODE GetConfigurator(se::config::IConfigurator** ppOut);

    SE_CODE ProcessProductKeyStats(CSmartConnection& conn, UID prodId, const DATAPERIOD& dp) ;

    SE_CODE ProcessProductKeyStats(CSmartConnection& conn, UID prodId, IFinancialsConsolidated* pcon, const DATAPERIOD& dp) ;
    
    SE_CODE _GetMatchedAccountValue(IFinancialsConsolidated* pCon, UID matchId, IXbrlRecord** ppOut);
    
    SE_CODE _GetMatchAccounts(unsigned long matchId, IXbrlIdList** ppOut);  // when cik = 0;
    
    SE_CODE _PersistPerformanceData(IFinancialsConsolidated* pcon, UID PerfId = 0);
    
private:
     string _userId;
     string _password;
     ////LoggerPtr _logger;     
     Poco::Mutex _lock;
     Poco::Mutex _lock1;
     CSEConnPool* SEDBCONN;
     typedef std::unordered_set<U64> UniqueFinType;
     UniqueFinType _fintypedupcheck;
     std::mutex _mtx;
     std::condition_variable _cv;
     void* statementProcessor;
};

#endif	/* CXBRL_H */

