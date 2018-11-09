#ifndef SESECURITIESAPI_H_INCLUDED
#define SESECURITIESAPI_H_INCLUDED


#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"
#include "../setraining/setrainingapi.h"
#include "../seglobalenums.h"
#include "../seglobal/CDate.h"
#include "../sedata/sedataheaders.h"

#define SECEXP __attribute__ ((visibility ("default")))

using namespace se::data;

namespace se
{
    
    /*
     * Error codes are defined the following way in a 32-bit number (HRESULT)
     * 
     *  7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   
     *-------------------------------------------------------------------------
     *  E| |C|     | T | |     Facility    |               Code                |
     * 
     * Facility code is 5
     * T = {
     *      00 = Error
     *      01 = Warning
     *      10 = Information
     *      11 = Debug
     * }
     * 
     */
    
    //#define S_SampleInfo                            0x02050000      // a sample info message
    //#define W_SampleWarning                         0x01050000      // a sample warning
    //#define D_SampleDebug                           0x03050000      // a sample debug message.

    /* Zmq related return codes */
    #define E_SecHistoryPriceUnavailable              0x80050001      // Historical price for security not found in db.
    #define E_ExchNotFound                              0x80050002      // Exchange code or name is not found in our repository. 
    
    
    
    namespace securities
    {
        
        typedef enum enHistoricalPriceAveragingMethod
        {
            HPAM_Monthly,
            HPAM_Weekly,
            HPAM_Daily
        } HISTORICALPRICEAVERAGINGMETHOD, HPAM;
        
        
        struct IEconomicallyAdjustedPrices : public IObject
        {
            // operator.
            virtual R8 get_Value(EAM economicAdjustor) = 0;
        };
        
        typedef std::tuple<MONTHYEAR, R8> PeriodPricePair;
        typedef std::tuple<MONTHYEAR, IEconomicallyAdjustedPrices*> PeriodPriceAdjustedPair;

        struct IPublicCompanyInfo;        
        
        struct IPublicCompanyInfoList : public IROSPList<IPublicCompanyInfo>
        {
        };
        
        /* Get global industry classification standard info. */
        struct IGICInfo : public IObject, public ISerialization
        {
            enum QUERYFLDS
            {
                Id,
                Sector,
                Industry
            };
            
            virtual CSTR get_Sector() = 0;
            virtual CSTR get_Industry() = 0;
            static SE_CODE SECEXP Create(ISession*, CSTR sector, CSTR industry, IGICInfo** ppOut) ;
            virtual SE_CODE get_Companies(IPublicCompanyInfoList** ppOut) = 0;
        };
        
      
        /* information about the public company. */
        struct IPublicCompanyInfo : public IObject, public ISerialization
        {
            enum QUERYFLDS
            {
                Id,
                Name,
                GicId,
                Sector,
                Industry,
                Cik,
                Ticker,
                State,
                Country,
                ExchangeId,
                ExchangeName
            };
            
            virtual CSTR get_Name() = 0;
            virtual CIK get_CIK() = 0;
            virtual CSTR get_Summary() = 0;
            virtual ErrorId get_GICInfo(IGICInfo** ppOut) = 0;
            virtual CSTR get_SICInfo() = 0;
            virtual CSTR get_Ticker() = 0;
            virtual ErrorId get_Address(IAddress** ppOut) = 0;
            virtual CSTR get_UrlNews() = 0;
            virtual void set_Name(CSTR) = 0;
            virtual void set_CIK(CSTR) = 0;
            virtual void set_Summary(CSTR) = 0;
            virtual void set_GICInfo(IGICInfo*) = 0;
            virtual void set_SICInfo(CSTR) = 0;
            virtual void set_Address(IAddress*) = 0;
            
            virtual CDate& get_FinancialYearEnd() = 0;
            
            static SE_CODE SECEXP Create(ISession*, ISecurityDescription*, IPublicCompanyInfo**);
        };

        struct ISecurityPrice : public IObject, public ISerialization
        {
            virtual CDate& get_Period() = 0;
            virtual R4 get_Price()  = 0;
            virtual R4 get_Volume()  = 0; //open
        };
        
        
        struct ISecurityPriceEx : public ISecurityPrice
        {
            virtual R4 get_High()  = 0;
            virtual R4 get_Low()  = 0;
            virtual R4 get_Close()  = 0;
            virtual R4 get_adjClose()  = 0;
        };
        
        struct ISecurityNewsList;
        
        /* a security instrument. */
        struct ISecurity : public IObject, public ISerialization
        {
            virtual ErrorId get_SecurityDescription(ISecurityDescription** ppOut) = 0;

            virtual CSTR get_Name() = 0;

            virtual CSTR get_LongName() = 0;
            
            virtual ErrorId get_Company(IPublicCompanyInfo** ppOut) = 0;

            virtual R8 get_Marketcap() = 0;

            virtual R8 get_IndustryMarketcapShare() = 0;

            virtual SE_CODE get_News(ISecurityNewsList**) = 0;
            
            static SE_CODE SECEXP Create(ISession*, ISecurityDescription* pIn, ISecurity** ppOut);
            
        };
        
        struct IGICList : public IROSPList<IGICInfo>
        {
        };
        
        // NOTE: All methods that return an R8 will be set to ER8 to represent invalid/null.
        struct ISecuritySelectionStatistics : public IObject, public ISerialization
        {
            enum QUERYFLDS
            {
                ExchangeIds,    // A comma separated list of exchange Identifiers.
            };

            
            virtual R8 get_AMeanOfReturnRate() const = 0;
            virtual R8 get_GMeanOfReturnRate() const = 0;
            virtual R8 get_ExpectedReturnRate() const = 0;
            virtual R8 get_ExpectedReturnValue() const = 0;
            virtual R8 get_ReturnRisk() const = 0; // stddev
            virtual R8 get_Volatility() const = 0; // variance
            virtual R8 get_Alpha() const = 0;  //TBD
            virtual R8 get_Beta() const = 0;   //TBD

            virtual void set_AMeanOfReturnRate(R8 val)= 0;
            virtual void set_GMeanOfReturnRate(R8 val)= 0;
            virtual void set_ExpectedReturnRate(R8 val)= 0;
            virtual void set_ExpectedReturnValue(R8 val)= 0;
            virtual void set_ReturnRisk(R8 val)= 0;
            virtual void set_Volatility(R8 val)= 0;
            virtual void set_Alpha(R8 val)= 0;
            virtual void set_Beta(R8 val)= 0;
            
            virtual SE_CODE get_SecurityDescription(ISecurityDescription** ppOut) = 0;
        };
        
        struct ISecuritySelectionStatisticsList : public IROSPList<ISecuritySelectionStatistics>
        {
            virtual SE_CODE Reset() = 0;
            virtual SE_CODE RenewList() = 0;
        };
        
        struct IOptionFeeder : public IObject
        {
            enum Feeders
            {
                Yahoo,
            };
            // returns currently setup option feeder.
            static SE_CODE SECEXP GetFeeder(ISession*, IOptionFeeder** ppOut);
            // returns a specific option feeder.
            static SE_CODE SECEXP GetFeeder(ISession*, Feeders feeder, IOptionFeeder** ppOut);
        };
        
        typedef enum class SecurityNewsSource
        {
            Nasdaq
        } SNS;
        
        struct ISecurityNews : public IObject, public ISerialization
        {
            virtual const DATETIME& get_Date() = 0;
            virtual CSTR get_Title(bool* pbIshtml = 0) = 0;   // this could contain html tags.
            virtual CSTR get_Link() = 0;
            virtual CSTR get_NewsPrecis(bool* pbIshtml = 0) = 0;
            virtual CSTR get_NewsContent(bool* pbIshtml = 0) = 0; // can contain html. pbIsHtml is set to true if html
            virtual SNS get_Source() = 0;
        };
        
        struct ISecurityNewsList : public ISPList<ISecurityNews>
        {
            static SE_CODE SECEXP Create(ISession*, ISecurityNewsList**);
        };
        
        typedef enum class SecurityPriceSource
        {
            Nasdaq,
            Yahoo
        } SPS;
        
        struct ISecurityPriceRSSFeed : public IObject
        {
            virtual CSTR get_Feed() = 0;
            virtual SPS get_Source() = 0;
        };

        static NamedObjectNamingType ISecurityAPIName = 0xF;
        
        /* a gateway to this api */
        struct ISecurityAPI : 
            public INamedObject, 
            public IErrorDescription, 
            public ISearcheable
        {
            
            virtual MARKETDATAPROVIDERS get_DefaultMarketDataProvider() = 0;
            
            /* Get all securities with the symbol as first argument*/
            virtual SE_CODE get_SecurityDescriptions(CSTR symbol, ISecurityDescriptionList** ppOut) = 0;

            /* Get all securities with matching names given first argument. The name pattern can be a few text chars or text chars mixed with %.*/
            virtual SE_CODE get_SecurityDescriptionsByName(CSTR namepattern, ISecurityDescriptionList** ppOut) = 0;
            
            /* Get all exchanges */
            virtual SE_CODE get_Exchanges(IExchangeList** ppOut) = 0;
            
            virtual SE_CODE get_Exchanges(Json::Value& jsOut) = 0;

            /* Create a clean security description. it could be used for
            setting information of a security */
            virtual ErrorId CreateSecurityDescription(ISecurityDescription** ppOut) = 0;
            
            /* get information about a security */
            virtual ErrorId get_Security(ISecurityDescription* pSecDesc, ISecurity** ppOut) = 0;
            
            /* get a public company's information */
            virtual ErrorId get_Company(ISecurityDescription* pSecDesc, IPublicCompanyInfo** ppOut) = 0;
            
            /* Ensures that the security is properly enabled in the security master. */
            virtual ErrorId SynchronizeSecurityDescription(ISecurityDescription* pIn) = 0;

            // is the given date a holiday for the given echange ?
            virtual bool IsHoliday(EXCHANGEID exchangeId, CDate& date) = 0;
            
            /* Get the last trading day. Returns only the last day for which a closing security price is available. */
            virtual CDate LastTradingDay() = 0;
            
            /*
             * searchText: A search text which can be part of a symbol or name.
             * output: Json output in the following structure (category
             * [
             *      {"Ticker" : [ { "Id" : xx, "Symbol" : "xx", "Name" : "xx", "Exchange" : "xx", "Type" : "xx" }, {...} ],
             *      {"Product" : [ { "Id" : xx, "Symbol" : "xx", "Name" : "xx" , "Exchange" : "xx", "Type" : "xx"}, {...} ],
             *      ....
             * ]
             */
            //virtual long unsigned int SearchModule(const char* searchText, SearchMethod searchMethod, Json::Value& output);
            
            virtual long unsigned int SearchSymbols(const char* searchText, SearchMethod searchMethod, Json::Value& output) = 0;
            virtual long unsigned int SearchProducts(const char* searchText, SearchMethod searchMethod, Json::Value& output) = 0;

            // historical price processing.
            virtual ErrorId ProcessSecurityHistoricalPrices(ISecurityDescription* pIn) = 0;
            
            // process daily statements
            virtual ErrorId ProcessSecurityDailyPrice(ISecurityDescription* pIn) = 0;
            
            // get historica prices between given period
            virtual ErrorId get_SecurityHistoricalPrices(ISecurityDescription* pIn, CDate start, CDate end, IROSPList<ISecurityPriceEx> **ppOut) = 0;
  
            // get a day's security price.
            virtual ErrorId get_SecurityPrice(ISecurityDescription* pIn, ISecurityPriceEx **ppOut, CDate date = CDate::Now()) = 0;
            
            // This method updates security prices of all symbols we have.
            virtual ErrorId ProcessSecDailyPrice(I32 WaitPerSymbol = 1  ) = 0;
            
            // get security price as timer series data. This could be daily, weekly or monthly. For weekly and monthly data, 
            // a harmonic mean is taken for the respective period.
            virtual ErrorId get_SecurityHistoricalPrices(ISecurityDescription* pIn, CDate start, CDate end, ECONOMICADJMASKS economicAdjustments, HISTORICALPRICEAVERAGINGMETHOD averagingMethod, IROList<PeriodPriceAdjustedPair> **ppOut) = 0;
            
            // List companies by GICS
            virtual SE_CODE get_GICS(IGICList** ppOut, QUERYFUNC(IGICInfo) criteria = nullptr) = 0;
            
            
            virtual SE_CODE get_GICS(Json::Value& ojs)=0;
            
            // get a list of gics with 'gicId' as parent. Send zero to get a list of parents.
            virtual SE_CODE get_GICHierarchy(UID gicId, IIdNameList** ppOut) = 0;
            
            virtual SE_CODE get_PublicCompanies(IPublicCompanyInfoList** ppOut, QUERYFUNC(IPublicCompanyInfo) criteria = nullptr) = 0;
          
            virtual SE_CODE get_SecurityPredictionSetups(UID productId, UID matchId, se::training::IPredictionSetupList **ppOut) = 0;
            
            virtual SE_CODE ProcessEarningCalendar(PERIOD processPeriod) = 0;
            
            // return a list of securities which is by default order by return/risk ratio (low to high)
            virtual SE_CODE get_RandomListOfBestQuarterlySecurities(
                ISecuritySelectionStatisticsList** ppOut, 
                SIZE listSize = 100,
                QUERYFUNC(ISecuritySelectionStatistics) criteria = nullptr) = 0;
            
            virtual SE_CODE get_SecuritySelectionStatistics(UID productId, ISecuritySelectionStatistics** ppOut) = 0;
            
            // get security news. 
            virtual SE_CODE get_SecurityNews(ISecurityDescription *pDesc, ISecurityNewsList** ppOut) = 0;
            
            // get a rss feed that provides current security price.
            virtual SE_CODE get_SecurityPriceRSSFeedCurrent(ISecurityDescription* pDesc, SPS source, ISecurityPriceRSSFeed** ppOut) = 0;
           
            // Historical prices are return in json format { "Daily Prices" : [{"date" : "2015-04-30", "close" : 489.6300}, {...}] }
            virtual SE_CODE get_SecurityHistoricalPrices(ISecurityDescription* pDesc, Json::Value& root) = 0;
            
            ///////////////////// operations that support json /////////
            // Get security info as json. Provide the security description.
            // @securityDesc: The security desc has same features and ISecurityDescription
            /* 
                { 
                    "Exch" : "AMEX", 
                    "Sym" : "TIS", 
                    "Country" : "US", 
                    "AssetType" : "Equity" ,
                    "Include" : { "Fundamentals" : true/false, "Technicals" : true/false }
                } 
             */
            // Note that "Country" and "AssetType" are optional fields.
            // @returnvalue: 
            /*  An array of following object ([...])
                { 
                    "Exch" : "AMEX", 
                    "Sym" : "TIS",  
                    "Name" : "Orchid Paper Company",
                    "LName" : "Orchid Paper Company",
                    "Country" : "US",
                    "AssetType" : "Equity",
                    "AssetSubType" : "Stock",
                }
             * TODO: More will be added later such as technical and fundamentals.
             * Remarks:
             * AssetTypes are "Equity", "Index", Future", "Option", "Currency", "Fixed".
             * AssetSubTypes are "Stock, "EquitIndex", (TBD)
            */
            
            virtual SE_CODE get_Security(const Json::Value securityDesc, Json::Value& output) = 0;
            
            /* 
             * Get all securities listed in an exchange.
             * @input: 
             * { "Exch" : "AMEX", "Include" : { "Fundamentals" : true/false, "Technicals" : true/false }}
             * @output
             * { "Exch" : "AMEX", 
             *   "Securities" : [ 
             *      { "Sym" : "TIS", "Name" : "Orchard Paper company", "LName" : "...", "Country" : "US", "AssetType" : "Equity", "AssetSubType" : "Stock",
             *          "Technicals" : { Optional, see get_Technicals for details },
             *          "Fundamentals" : { Optional, see get_Fundametnals for details }
             *      }
             *  ]
             * }
             */
            virtual SE_CODE get_ExchangeSecurities(const Json::Value input, Json::Value& output) = 0;
            
            
        };
        
        extern "C"
        {
            /* Open and obtain a reference to the api gateway */
            ErrorId SECEXP SecOpen(ISession* ps, MARKETDATAPROVIDERS defaultProvider, ISecurityAPI** ppGateway) ;

            SE_CODE SECEXP CreateSecurityDescription(ISession* ps, ISecurityDescription**);
        }
    }
}


#endif // SESECURITIESAPI_H_INCLUDED
