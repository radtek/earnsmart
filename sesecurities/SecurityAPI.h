#ifndef SECURITYAPI_H
#define SECURITYAPI_H

#include "sesecuritiesapi.h"
#include "../seglobal/CDate.h"
#include "../semysql/CSEConnections.h"
#include "CMarketDataProvider.h"
#include <Poco/Mutex.h>
#include <Poco/ScopedLock.h>
#include <unordered_map>
#include <mutex>

using namespace mysqlpp;
using namespace se::securities;
using namespace se;

class CSecurityAPI : public CObjectRoot, public ISecurityAPI
{
public:
    CSecurityAPI(ISession*);
    virtual ~CSecurityAPI();

    NamedObjectNamingType get_TypeId() override;


    virtual long unsigned int Init(MARKETDATAPROVIDERS defmdprov);
    
    MARKETDATAPROVIDERS get_DefaultMarketDataProvider() override;

    /* Create a clean security description. it could be used for
    setting information of a security */
    virtual ErrorId CreateSecurityDescription(ISecurityDescription** ppOut) ;

    virtual long unsigned int get_SecurityDescriptions(const char* symbol, ISecurityDescriptionList** ppOut);

    virtual long unsigned int get_SecurityDescriptionsByName(const char* namepattern, ISecurityDescriptionList** ppOut);

    virtual long unsigned int get_Exchanges(IExchangeList** ppOut);
    
    long unsigned int get_Exchanges(Json::Value& jsOut) override;


    /* get information about a security */
    virtual ErrorId get_Security(ISecurityDescription* pSecDesc, ISecurity** ppOut) ;

    /* get a public company's information */
    virtual ErrorId get_Company(ISecurityDescription* pSecDesc, IPublicCompanyInfo** ppOut) ;

    /* Ensures that if a security information of a security description 
     * is not already found in our systems, it will be fixed. */
    virtual ErrorId SynchronizeSecurityDescription(ISecurityDescription* pIn);

    /* Check if the given date is a public holiday for the given exchange. */
    virtual bool IsHoliday(se::EXCHANGEID exchangeId, se::CDate& date);

    virtual CDate LastTradingDay();

    virtual long unsigned int SearchModule(const char* searchText, SearchMethod searchMethod, Json::Value& output);


    virtual long unsigned int SearchSymbols(const char* searchText, SearchMethod searchMethod, Json::Value& output);

    virtual long unsigned int SearchProducts(const char* searchText, SearchMethod searchMethod, Json::Value& output);



    // historical price processing.
    virtual ErrorId ProcessSecurityHistoricalPrices(ISecurityDescription* pIn);

    // process daily statements
    virtual ErrorId ProcessSecurityDailyPrice(ISecurityDescription* pIn);

    // get historica prices between given period
    virtual ErrorId get_SecurityHistoricalPrices(ISecurityDescription* pIn, CDate start, CDate end, IROSPList<ISecurityPriceEx> **ppOut);

    virtual ErrorId get_SecurityHistoricalPrices(ISecurityDescription* pIn, CDate start, CDate end, EAM economicAdjustments, HPAM averagingMethod, IROList<PeriodPriceAdjustedPair> **ppOut);

    // get a day's security price.
    virtual ErrorId get_SecurityPrice(ISecurityDescription* pIn, ISecurityPriceEx **ppOut, CDate date);

    //for store security daily price of different companies 
    virtual ErrorId ProcessSecDailyPrice(I32 WaitPerSymbol);

    virtual SE_CODE get_GICS(IGICList** ppOut, QUERYFUNC(IGICInfo) criteria = nullptr);

    long unsigned int get_GICS(Json::Value& ojs) override;

    
    virtual SE_CODE get_PublicCompanies(IPublicCompanyInfoList** ppOut, QUERYFUNC(IPublicCompanyInfo) criteria = nullptr);

    virtual long unsigned int get_GICHierarchy(unsigned long gicId, IIdNameList** ppOut);

    virtual long unsigned int get_SecurityPredictionSetups(unsigned long productId, UID matchId, se::training::IPredictionSetupList** ppOut);

    virtual long unsigned int ProcessEarningCalendar(PERIOD processPeriod);
    

    virtual long unsigned int get_RandomListOfBestQuarterlySecurities(
        ISecuritySelectionStatisticsList** ppOut, 
        SIZE listSize = 100,
        QUERYFUNC(ISecuritySelectionStatistics) criteria = nullptr
        );
    
    virtual SE_CODE get_SecuritySelectionStatistics(UID productId, ISecuritySelectionStatistics** ppOut);


    virtual long unsigned int get_SecurityNews(ISecurityDescription *pDesc, ISecurityNewsList** ppOut);

    virtual SE_CODE get_SecurityPriceRSSFeedCurrent(ISecurityDescription* pDesc, SPS source, ISecurityPriceRSSFeed** ppOut);
    

    virtual long unsigned int get_SecurityHistoricalPrices(ISecurityDescription* pDesc, Json::Value& root);

    
    ///////////////////////// IErrorDescription /////////////////////    

    virtual const char* get_CodeMessage(long unsigned int errorCode);

    
protected:
    
#ifdef __DEBUG__
    void OnAdd(unsigned short rc) override
    {
        BINF << rc ;
    }
    void OnDec(unsigned short rc) override
    {
        BINF << rc ;
    }
#endif

    
private:
    CDate LastTradingDay(CSmartConnection& conn, bool OnlyCompletedDays = true);
    ErrorId ProcessSecurityHistoricalPrices(CSmartConnection& conn, ISecurityDescription* pIn);
    ErrorId DownloadHistoricalPrices(CSmartConnection& conn, ISecurityDescription* pSec);
    ErrorId DownloadHistoricalPrices(CSmartConnection& conn, ISecurityDescription* pSec, CDate& dtFrom, CDate& dtTo);
    ErrorId DailyPrice(CSTR symbol);
    ErrorId GetCSV(CSTR symbol);
    ErrorId GetCSV(CSTR symbol, CDate& from, CDate& to);
    ErrorId ReadCsv() ;
    ErrorId SaveHistoricalPrices(CSmartConnection& conn, int productid,std::string date, std::string open, std::string high, std::string low, std::string close, std::string volume, std::string adj_close);
    CSTR dateConversion(CSTR date);
    ErrorId SetSecurityPriceEx(Row const& row, ISecurityPriceEx** ppOut);
    SE_CODE LoadGic(const Row& row, IGICInfo** ppOut);
    SE_CODE get_SecurityDetails(CSmartConnection& conn, ISecurityDescription *pSecDesc, ISecurity**);
    void SetPredictionSetupDetails(CSmartConnection& conn, const Row& r, se::training::IPredictionSetup* p);
    
    SE_CODE CreatePredicationInstance(unsigned long productId, UID matchId, se::training::IPredictionSetupList* p);
    
    void LoadGICs(CSmartConnection& conn, Json::Value& parent);
    
private:
    int _pID;
    std::string _csvHtml;
    std::string _csvLink;
    std::string _symbol;
    typedef std::unordered_map<UID, ISecurity*> SecurityMap;
    SecurityMap _securities;
    std::recursive_mutex _lock;
    MARKETDATAPROVIDERS _defProvider ;
    CMarketDataProvider* _dataProvider;
    
public:
    
    static Poco::Mutex securityLock;
    
    
    
    
    
    
    
public:
////////////////////// JSON METHODS BEGINS /////////////////////////////////
    SE_CODE get_Security(const Json::Value securityDesc, Json::Value& output) override;
    SE_CODE get_ExchangeSecurities(const Json::Value input, Json::Value& output) override;


///////////////////////JSON METHODS ENDS ///////////////////////////    
    
};

#endif // SECURITYAPI_H
