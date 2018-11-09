/* 
 * File:   CWSClientSession.h
 * Author: santony
 *
 * Created on February 1, 2015, 8:19 PM
 */

#ifndef CWSCLIENTSESSION_H
#define	CWSCLIENTSESSION_H

#include "seeoddata.h"
#include "nsDataSoapProxy.h"

using namespace ns;

namespace se
{
    namespace eoddata
    {
        
        typedef std::vector<ns1__QUOTE * > Quotes;

        class CWSClientSession : public CObjectRoot, public IWSClientSession
        {
        private:
            
            DataSoapProxy* _pproxy = nullptr;
            string _token;
            string _user, _pass;        
            I16 _reloginretry = 0;
        private:
            void Destroy();
            
        public:
            CWSClientSession(ISession*);
            CWSClientSession(const CWSClientSession& orig) = delete;
            virtual ~CWSClientSession();
            

            long unsigned int Init(CSTR userId, CSTR password);


            virtual bool IsLoggedIn();

            virtual SE_CODE Login();

            virtual const char* get_Version();


            virtual long unsigned int GetNews(const char* symbol, const char* exchange, INewsList** ppOut);

            virtual long unsigned int GetExchanges(IExchangeList** ppOut);
            
            virtual long unsigned int GetSymbols(const char* exchange, ISymbolList** ppOut);

            //TODO: Move to se::securities library and read from db.
            virtual long unsigned int GetSecurityDetails(se::securities::ISecurityDescription* pdesc, ISecurityDetails** ppOut);

            virtual long unsigned int GetSymbolChanges(const char* exchange, ISymbolChangeList** ppOut);

            virtual long unsigned int GetSymbolHistoryDaily(se::securities::ISecurityDescription* pdesc, const CDate& datestart, const CDate& dateend, ISecurityQuoteList** ppOut);

            virtual long unsigned int GetSymbolHistoryMonthly(se::securities::ISecurityDescription* pdesc, const CDate& datestart, const CDate& dateend, ISecurityQuoteList** ppOut);
            
            virtual SE_CODE GetSymbolEODHistory(se::securities::ISecurityDescription* pdesc, const CDate& datestart, ISecurityQuoteList** ppOut) ;

            virtual long unsigned int GetQuotesForDay(const CDate& date, ISecurityQuoteList** ppOut);

            virtual long unsigned int GetQuotesForMonth(char Month, ISecurityQuoteList** ppOut);

            virtual long unsigned int PublishSecurityData(CSTR exchange, DATETIME* pDate = nullptr);
            

            virtual long unsigned int PublishCommonData(DATETIME* pDate);

            virtual SE_CODE FindSymbolId(const string& symbol, const string& exchange);
            
            // Updates the latest fundamentals in the database.
            virtual SE_CODE UpdateFundamentals(const string& exchange);
            
            // Updates latest technicals in the database.
            virtual SE_CODE UpdateTechnicals(const string& exchange);

            // load exchanges
            virtual SE_CODE UploadExchanges();
            
            // load country data from eoddata
            virtual SE_CODE UploadCountries();
            
            // Upload symbols
            virtual SE_CODE UploadSymbols(const string& exchange);
            
            virtual SE_CODE UploadSymbolsEODHistoryByExchange(const string& exchange);
            
            virtual SE_CODE UpdateSymbolEODHistory(const string& exchange, const string& symbol, const CDate& datestart);
            

            virtual long unsigned int UploadExchangeQuotes(const string& exchange, const CDate& startdate);

            virtual long unsigned int UploadExchangeQuotesCurrent(const string& exchange);
            
            
            virtual SE_CODE GetSymbolHistoryPeriod(se::securities::ISecurityDescription* pdesc, const CDate& datestart, const CDate& dateend, const string& period, ISecurityQuoteList** ppOut);



        };

    }
}

#endif	/* CWSCLIENTSESSION_H */

