/* 
 * File:   seeoddata.h
 * Author: santony
 *
 * Created on February 1, 2015, 7:45 PM
 */

#ifndef SEEODDATA_H
#define	SEEODDATA_H

#define EODDATAEXP __attribute__ ((visibility ("default")))

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"
#include "../seglobalenums.h"
#include "../sesecurities/sesecuritiesapi.h"

namespace se {
    namespace eoddata {

        /*
         * Error codes are defined the following way in a 32-bit number (HRESULT)
         * 
         *  7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0
         *------------------------------------------------------------------------------------------
         *  E| |C|          |     Facility     | T |                    Code                        |
         * 
         * Facility code of this library is 11
         * 
         */

#define E_SOAPError                                     0x800E0001
        //#define E_UserLoginExists                               0x800B0005
        //#define S_NewUserAddress                                0x000B0001      // success, but a new user address record is created

        struct INews : public IObject {
            virtual CSTR get_Symbol() = 0;
            virtual CSTR get_Exchange() = 0;
            virtual CSTR get_Source() = 0;
            virtual DATETIME& get_When() = 0;
            virtual CSTR get_Title() = 0;
            virtual CSTR get_Intro() = 0;
            virtual CSTR get_Story() = 0;
            virtual CSTR get_Caption() = 0;
            virtual I32 get_Views() = 0;
        };

        struct INewsList : public IEnumerable1<INews> {
        };

        struct IExchange : public se::IIdName {
            virtual CSTR get_Code() = 0;
            virtual CSTR get_Country() = 0;
            virtual CSTR get_TimeZone() = 0;
        };

        struct IExchangeList :
        public se::IEnumerable1<IExchange>,
        public IFinder<IExchange>,
        public IROList1<IExchange> {
        };

        struct ISecurityQuote : public IObject {
            virtual DATETIME& get_DateTime() = 0;
            virtual R8 get_Open() const = 0;
            virtual R8 get_High() const = 0;
            virtual R8 get_Low() const = 0;
            virtual R8 get_Close() const = 0;
            virtual U64 get_Volume() const = 0;
            virtual I64 get_Openinterest() const = 0;
            virtual R8 get_Prev() const = 0;
            virtual R8 get_Change() const = 0;
            virtual R8 get_Bid() const = 0;
            virtual R8 get_Ask() const = 0;
            virtual R8 get_PreviousClose() const = 0;
            virtual R8 get_NextOpen() const = 0;
            virtual DATETIME& get_Modified() = 0;
        };

        struct ISecurityQuoteList : public IEnumerable1<ISecurityQuote> {
        };

        struct ISecurityTechnicals : public IObject {
            virtual DATETIME& get_DateTime() = 0;
            virtual R8 get_Previous() const = 0;
            virtual R8 get_Change() const = 0;
            virtual I32 get_MA1() const = 0;
            virtual I32 get_MA2() const = 0;
            virtual I32 get_MA5() const = 0;
            virtual I32 get_MA20() const = 0;
            virtual I32 get_MA100() const = 0;
            virtual I32 get_MA200() const = 0;
            virtual R8 get_MAPercent() const = 0;
            virtual R8 get_MAReturn() const = 0;
            virtual I64 get_VolumeChange() const = 0;
            virtual R8 get_ThreeMonthChange() const = 0;
            virtual R8 get_SixMonthChange() const = 0;
            virtual R8 get_WeekHigh() const = 0;
            virtual R8 get_WeekLow() const = 0;
            virtual R8 get_AvgWeekChange() const = 0;
            virtual R8 get_AvgWeekVolume() const = 0;
            virtual R8 get_WeekVolume() const = 0;
            virtual R8 get_MonthHigh() const = 0;
            virtual R8 get_MonthLow() const = 0;
            virtual R8 get_MonthChange() const = 0;
            virtual R8 get_AvgMonthChange() const = 0;
            virtual R8 get_AvgMonthVolume() const = 0;
            virtual R8 get_MonthVolume() const = 0;
            virtual R8 get_YearHigh() const = 0;
            virtual R8 get_YearLow() const = 0;
            virtual R8 get_YearChange() const = 0;
            virtual R8 get_AvgYearChange() const = 0;
            virtual R8 get_AvgYearVolume() const = 0;
            virtual R8 get_YTDChange() const = 0;
            virtual R8 get_RSI14() const = 0;
            virtual R8 get_STO9() const = 0;
            virtual R8 get_WPR14() const = 0;
            virtual R8 get_MTM14() const = 0;
            virtual R8 get_ROC14() const = 0;
            virtual I64 get_PTC() const = 0;
            virtual R8 get_SAR() const = 0;
            virtual R8 get_Volatility() const = 0;
            virtual R8 get_Liquidity() const = 0;
        };

        struct ISecurityFundamentals : public IObject {
            virtual DATETIME& get_DateTime() = 0;
            virtual CSTR get_Industry() const = 0;
            virtual CSTR get_Sector() const = 0;
            virtual U64 get_NumOfShares() const = 0;
            virtual U64 get_MarketCap() const = 0;
            virtual R8 get_PricePerEarnings() const = 0;
            virtual R8 get_EPS() const = 0;
            // Net tangible assets or net asset value.
            virtual R8 get_NTA() const = 0;
            // annual dividends per share / price per share
            virtual R8 get_DivYield() const = 0;
            virtual R8 get_Dividend() const = 0;
            virtual DATETIME& get_DividendDate() = 0;
            virtual R8 get_DividendPerShare() const = 0;
            // credits given by certain governments against dividend double taxation.
            virtual R8 get_ImputationCredits() const = 0;
            virtual R8 get_EBITDA() const = 0;
            // Price per earnings to growth ration.
            virtual R8 get_PEG() const = 0;
            virtual R8 get_PtS() const = 0;
            virtual R8 get_PtB() const = 0;
            virtual R8 get_Yield() const = 0;
        };

        struct ISecurityDetails : public IObject {
            virtual RP<se::securities::ISecurityDescription> get_Desc() = 0;
            virtual RP<ISecurityFundamentals> get_Fundamentals() = 0;
            virtual RP<ISecurityTechnicals> get_Technicals() = 0;
            virtual RP<ISecurityQuote> get_Quote() = 0;
            virtual RP<INewsList> get_News() = 0;
        };

        struct ISymbolList : public IEnumerable1<se::securities::ISecurityDescription> {
        };

        struct ISymbolChange : public IObject {
            virtual CSTR get_OldSymbol() = 0;
            virtual CSTR get_NewSymbol() = 0;
            virtual CSTR get_OldExchangeCode() = 0;
            virtual CSTR get_NewExchangeCode() = 0;
        };

        struct ISymbolChangeList : public IEnumerable1<ISymbolChange> {
        };

        struct IWSClientSession : public IObject {
            virtual bool IsLoggedIn() = 0;
            virtual CSTR get_Version() = 0;

            virtual SE_CODE GetExchanges(IExchangeList** ppOut) = 0;

            virtual SE_CODE GetNews(CSTR symbol, CSTR exchange, INewsList** ppOut) = 0;

            virtual SE_CODE GetSymbols(CSTR exchange, ISymbolList** ppOut) = 0;

            //TODO: Move to se::securities library and read from db.
            /// Returns most recent EOD quote, technicals and fundamentals.
            virtual SE_CODE GetSecurityDetails(se::securities::ISecurityDescription* pdesc, ISecurityDetails** ppOut) = 0;

            virtual SE_CODE GetSymbolChanges(CSTR exchange, ISymbolChangeList** ppOut) = 0;

            virtual SE_CODE GetSymbolHistoryDaily(se::securities::ISecurityDescription* pdesc, const CDate& datestart, const CDate& dateend, ISecurityQuoteList** ppOut) = 0;

            virtual SE_CODE GetSymbolHistoryMonthly(se::securities::ISecurityDescription* pdesc, const CDate& datestart, const CDate& dateend, ISecurityQuoteList** ppOut) = 0;

            virtual SE_CODE GetSymbolEODHistory(se::securities::ISecurityDescription* pdesc, const CDate& datestart, ISecurityQuoteList** ppOut) = 0;
            
            virtual SE_CODE GetQuotesForDay(const CDate& date, ISecurityQuoteList** ppOut) = 0;

            virtual SE_CODE GetQuotesForMonth(I8 Month, ISecurityQuoteList** ppOut) = 0;

            // Publishes daily data from EOD to our storage. Not implemented.
            virtual SE_CODE PublishSecurityData(CSTR exchange, DATETIME* pDate = nullptr) = 0;

            // Publish common data such as list of countries, exchanges and their properties.
            virtual SE_CODE PublishCommonData(DATETIME* pDate = nullptr) = 0;

            virtual SE_CODE FindSymbolId(const string& symbol, const string& exchange) = 0;

            // Updates the latest fundamentals in the database.
            virtual SE_CODE UpdateFundamentals(const string& exchange) = 0;

            // Updates latest technicals in the database.
            virtual SE_CODE UpdateTechnicals(const string& exchange) = 0;

            // load exchanges
            virtual SE_CODE UploadExchanges() = 0;

            // load country data from eoddata
            virtual SE_CODE UploadCountries() = 0;

            // Upload symbols
            virtual SE_CODE UploadSymbols(const string& exchange) = 0;

	    virtual SE_CODE UploadSymbolsEODHistoryByExchange(const string& exchange) = 0;
            
            virtual long unsigned int UploadExchangeQuotesCurrent(const string& exchange)=0;
            // upload quotes (min date available is 2013/01/02)
            virtual SE_CODE UploadExchangeQuotes(const string& exchange, const CDate& startdate) = 0;

            // upload symbol prices from a past date to today.
            virtual SE_CODE UpdateSymbolEODHistory(const string& exchange, const string& symbol, const CDate& datestart) = 0;

            virtual SE_CODE GetSymbolHistoryPeriod(se::securities::ISecurityDescription* pdesc, const CDate& datestart, const CDate& dateend, const string& period, ISecurityQuoteList** ppOut) = 0;

        };

        /* The interface gateway*/
        static NamedObjectNamingType IEODDataName = 0xB;

        enum EODFileSubject
        {
            Exchanges,
            SecurityList,
            SecurityEODHistory,
            SymbolChange,
        };
        
        enum EODContentType
        {
            Xml,
            SpaceSeperated
        };
        
        struct EODFileTraits
        {
            EODFileSubject FileType;
            // Required if the exchange name is ambiguous in the downloaded file.
            string ExchangeName;
            
            EODContentType ContentType;
        };
        
        // Getway to EODDATA 
        struct IEODData : public INamedObject 
        {
            //Login and create a client session with EODDATA.
            virtual SE_CODE LoginToWSClient(CSTR userName, CSTR password, IWSClientSession** ppOut) = 0;

            // Create a client session with EODDATA utilizing cached userid and password.
            virtual SE_CODE OpenDefault(IWSClientSession** ppOut) = 0;
            
            // Read an EODData downloaded file. The characteristics of the file 
            // is passed as second parameter.
            virtual SE_CODE ReadEODFile(const string& fileName, const EODFileTraits& traits) = 0;
            
        };

        extern "C" SE_CODE EODDATAEXP EodDataOpen(ISession*, se::eoddata::IEODData** ppOut);

    }

}

#endif	/* SEEODDATA_H */

