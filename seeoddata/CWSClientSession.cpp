/* 
 * File:   CWSClientSession.cpp
 * Author: santony
 * 
 * Created on February 1, 2015, 8:19 PM
 */

#include "seeoddataint.h"
#include "CWSClientSession.h"
#include "CEODDataWrappers.h"

#include "../seglobal/CDate.h"
#include "../semysql/CMyqlUtilities.h"
#include "../external/mariadbpp/ssqls.h"

using namespace mysqlpp;
using namespace se::data;
using namespace ns;

namespace se
{
    namespace eoddata
    {

#define LOGGER __FUNCTION__

        CWSClientSession::CWSClientSession(ISession* ps):CObjectRoot(ps)
        {
        }

        CWSClientSession::~CWSClientSession()
        {
        }

        void CWSClientSession::Destroy() 
        {
            if (_pproxy)
            {
                _pproxy->destroy();
                delete _pproxy;
                _pproxy = nullptr;
            }
        }


        long unsigned int CWSClientSession::Init(CSTR userId, CSTR password)
        {

            _user = userId;
            this->_pass = password;

            return Login();
        }

        long unsigned int CWSClientSession::Login()
        {
            // attempt to login and capture the token
            _pproxy = new DataSoapProxy();
            _ns1__Login lg;
            lg.Username = &_user;
            lg.Password = &_pass;
            _ns1__LoginResponse lgresp;
            int resp = _pproxy->Login(&lg, lgresp);
            if (resp != 0)
            {
                if (lgresp.LoginResult)
                    BERR << lgresp.LoginResult->Message;
                return E_SOAPError;
            }
            if (!lgresp.LoginResult->Token)
                return E_SOAPError;

            _token = *lgresp.LoginResult->Token;

            return S_OK;

        }

        bool CWSClientSession::IsLoggedIn()
        {
            //  we will invoke DataClientLatestVersion method using token.
            // if rejected, we will return false.

            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return false;
            }
            _ns1__DataClientLatestVersion inVer;
            _ns1__DataClientLatestVersionResponse outVer;
            inVer.Token = &_token;
            int ret = _pproxy->DataClientLatestVersion(&inVer, outVer);
            if (ret == 0 && !outVer.DataClientLatestVersionResult->VERSION || outVer.DataClientLatestVersionResult->VERSION->empty())
            {
                BERR << "Failed to check login status. Error response: " << outVer.DataClientLatestVersionResult->Message;
                _reloginretry++;
                if (_reloginretry == 5)
                    return false;
                sleep(10);
                Login();
            }
            else if (ret != 0)
            {
                BERR << "Failed to check login status. EODData Return code " << ret;
                _reloginretry++;
                if (_reloginretry == 5)
                    return false;
                sleep(10);
                Login();
            }
            return true;
        }

        const char* CWSClientSession::get_Version()
        {
            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return "Unknown";
            }

            _ns1__DataClientLatestVersion inVer;
            _ns1__DataClientLatestVersionResponse outVer;
            inVer.Token = &_token;
            int ret = _pproxy->DataClientLatestVersion(&inVer, outVer);
            if (ret != 0 || !outVer.DataClientLatestVersionResult->VERSION || outVer.DataClientLatestVersionResult->VERSION->empty())
            {
                BERR << "Failed to check login status. Error response: " << outVer.DataClientLatestVersionResult->Message;
                return "Unknown";
            }

            return outVer.DataClientLatestVersionResult->VERSION->c_str();
        }

        long unsigned int CWSClientSession::GetNews(const char* symbol, const char* exchange, INewsList** ppOut)
        {

            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }

            _ns1__NewsListBySymbol in;
            _ns1__NewsListBySymbolResponse out;
            string sex = exchange;
            string sym = symbol;
            in.Exchange = &sex;
            in.symbol = &sym;
            in.token = &_token;

            int resp = _pproxy->NewsListBySymbol(&in, out);
            if (!resp)
            {
                if (*out.NewsListBySymbolResult->Message != "Success")
                {
                    BERR << "Error: " << *out.NewsListBySymbolResult->Message;
                    return E_SOAPError;
                }
            }
            else
                return E_SOAPError;

            SP<CNewsList> pl;
            RETURNIFFAILED(CObject<CNewsList>::Create(
                    get_Session(), 
                    &out.NewsListBySymbolResult->NEWSLIST->NEWS,
                    &pl));

            pl.CopyTo(ppOut);

            return S_OK;



        }

        long unsigned int CWSClientSession::GetExchanges(IExchangeList** ppOut)
        {

            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }



            _ns1__ExchangeList in;
            in.Token = &_token;
            _ns1__ExchangeListResponse out;
            int resp = _pproxy->ExchangeList(&in, out);

            if (!resp)
            {
                if (*out.ExchangeListResult->Message != "Success")
                {
                    BERR << "Error: " << *out.ExchangeListResult->Message;
                    return E_SOAPError;
                }
            }
            else
                return E_SOAPError;

            SP<CExchangeList> pl;
            RETURNIFFAILED(CObject<CExchangeList>::Create(
                    get_Session(), 
                    &out.ExchangeListResult->EXCHANGES->EXCHANGE,
                    &pl));

            pl.CopyTo(ppOut);

            return S_OK;

        }

        long unsigned int CWSClientSession::GetSymbolChanges(const char* exchange, ISymbolChangeList** ppOut)
        {
            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }

            _ns1__SymbolChangesByExchange in;
            _ns1__SymbolChangesByExchangeResponse out;
            in.Token = &_token;
            string exch = exchange;
            in.Exchange = &exch;
            int resp = _pproxy->SymbolChangesByExchange(&in, out);
            if (resp == 0)
            {
                if (*out.SymbolChangesByExchangeResult->Message != "Success")
                {
                    BERR << "Error: " << *out.SymbolChangesByExchangeResult->Message;
                    return E_SOAPError;
                }
            }
            else
                return E_SOAPError;

            return CObject<CSymbolChangeList>::Create(get_Session(), &out.SymbolChangesByExchangeResult->SYMBOLCHANGES->SYMBOLCHANGE, ppOut);

        }

        long unsigned int CWSClientSession::GetSecurityDetails(se::securities::ISecurityDescription* pdesc, ISecurityDetails** ppOut)
        {

            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }

            _ns1__SymbolGet in;
            _ns1__SymbolGetResponse out;
            in.Token = &_token;
            string sym, exch;
            sym = pdesc->get_Symbol();
            exch = pdesc->get_ExchCode();
            in.Symbol = &sym;
            in.Exchange = &exch;
            int resp = _pproxy->SymbolGet(&in, out);
            if (resp == 0)
            {
                if (*out.SymbolGetResult->Message != "Success")
                {
                    BERR << "Error: " << *out.SymbolGetResult->Message;
                    return E_SOAPError;
                }
            }
            else
                return E_SOAPError;



            return E_NotImpl;


        }

        long unsigned int CWSClientSession::GetSymbols(const char* exchange, ISymbolList** ppOut)
        {
            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }
            _ns1__SymbolList in;
            _ns1__SymbolListResponse out;
            in.Token = &_token;
            string exch = exchange;
            in.Exchange = &exch;
            int resp = _pproxy->SymbolList(&in, out);
            if (resp == 0)
            {
                if (*out.SymbolListResult->Message != "Success")
                {
                    BERR << "Error: " << *out.SymbolListResult->Message;
                    return E_SOAPError;
                }
            }
            else
                return E_SOAPError;



            return CObject<CSymbolList>::Create(get_Session(), &out.SymbolListResult->SYMBOLS->SYMBOL, ppOut);

        }

        long unsigned int CWSClientSession::GetSymbolHistoryPeriod(se::securities::ISecurityDescription* pdesc, const CDate& datestart, const CDate& dateend, const string& period, ISecurityQuoteList** ppOut)
        {
            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }

            // five years of data won't be any longer than 200 * 5, 1000 or 1500 records.
            // simply logic is more than enough to populate records.

            string exch = pdesc->get_ExchCode();
            string sym = pdesc->get_Symbol();

            ostringstream ostr;
            string sdate, edate;
            ostr << datestart.get_Year() << setw(2) << setfill('0') << datestart.get_Month() << setw(2) << setfill('0') << datestart.get_Day();
            sdate = ostr.str();
            ostr.str("");
            ostr << dateend.get_Year() << setw(2) << setfill('0') << dateend.get_Month() << setw(2) << setfill('0') << dateend.get_Day();
            edate = ostr.str();

            _ns1__SymbolHistoryPeriodByDateRange in;
            in.StartDate = &sdate;
            in.EndDate = &edate;
            in.Exchange = &exch;
            in.Period = const_cast<string*> (&period);
            in.Token = &_token;
            in.Symbol = &sym;
            _ns1__SymbolHistoryPeriodByDateRangeResponse out;
            int resp = _pproxy->SymbolHistoryPeriodByDateRange(&in, out);
            if (resp == 0)
            {
                if (*out.SymbolHistoryPeriodByDateRangeResult->Message != "Success")
                {
                    BERR << "Error: " << *out.SymbolHistoryPeriodByDateRangeResult->Message;
                    return E_SOAPError;
                }




            }
            else
                return E_SOAPError;
            return CObject<CSecurityQuoteList>::Create(get_Session(), &out.SymbolHistoryPeriodByDateRangeResult->QUOTES->QUOTE, ppOut);

        }

        long unsigned int CWSClientSession::GetSymbolHistoryMonthly(se::securities::ISecurityDescription* pdesc, const CDate& datestart, const CDate& dateend, ISecurityQuoteList** ppOut)
        {

            return GetSymbolHistoryPeriod(pdesc, datestart, dateend, "m", ppOut);
        }

        long unsigned int CWSClientSession::GetSymbolHistoryDaily(se::securities::ISecurityDescription* pdesc, const se::CDate& datestart, const se::CDate& dateend, ISecurityQuoteList** ppOut)
        {
            return GetSymbolHistoryPeriod(pdesc, datestart, dateend, "d", ppOut);
        }

        SE_CODE CWSClientSession::GetSymbolEODHistory(se::securities::ISecurityDescription* pdesc, const CDate& datestart, ISecurityQuoteList** ppOut)
        {


        }

        long unsigned int CWSClientSession::GetQuotesForDay(const CDate& date, ISecurityQuoteList** ppOut)
        {

        }

        long unsigned int CWSClientSession::GetQuotesForMonth(char Month, ISecurityQuoteList** ppOut)
        {

        }

        long unsigned int CWSClientSession::PublishCommonData(DATETIME* pDate)
        {
            SE_CODE ret = UploadCountries();
            if (FAILED(ret))
                return ret;

            ret = UploadExchanges();
            if (FAILED(ret))
                return ret;

            return ret;
        }

        long unsigned int CWSClientSession::PublishSecurityData(CSTR exchange, DATETIME* pDate)
        {
            /*
             * Things to publish (web apis):
             * 1. Country List: Update only changes
             * 2. ExchangeList: Update only changes
             * 6. SymbolList : all exchanges, changes only
             * 3. Fundamentals: (dated records for each exchange)
             * 4. Technicals: (dated records for each exchange)
             * 5. QuoteList:(dated records for each exchange)
             * 7. SymbolChangesByExchange
             * 8. SplitListByExchange
             */

            SE_CODE ret = E_UnknownError;

            ret = UploadSymbols(exchange);
            if (FAILED(ret))
            {
                BERR << "Failed to upload symbols for exchange " << exchange;
                return ret;
            }

            ret = UpdateFundamentals(exchange);
            if (FAILED(ret))
            {
                BERR << "Failed to upload fundamentals for exchange " << exchange;
                return ret;
            }

            ret = UpdateTechnicals(exchange);
            if (FAILED(ret))
            {
                BERR << "Failed to upload technicals for exchange " << exchange;
                return ret;
            }

            return S_OK;

        }

        /*
         eod_countries
        +--------------+--------------+------+-----+---------+----------------+
        | Field        | Type         | Null | Key | Default | Extra          |
        +--------------+--------------+------+-----+---------+----------------+
        | country_id   | int(11)      | NO   | PRI | NULL    | auto_increment |
        | country_name | varchar(150) | YES  |     | NULL    |                |
        | country_code | varchar(5)   | YES  | MUL | NULL    |                |
        +--------------+--------------+------+-----+---------+----------------+
         */

        //        sql_create_3(
        //            eod_countries, 
        //            1, 
        //            3, 
        //            sql_varchar, country_code,
        //            sql_varchar, country_name,
        //            sql_int, country_id
        //        );

        //        typedef set<eod_countries> EodCountries;

        /////////////////////////////////Private Methods//////////////////////////////////////////

        long unsigned int CWSClientSession::UploadCountries()
        {
            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }

            try
            {

                _ns1__CountryList in;
                _ns1__CountryListResponse out;
                in.Token = &_token;

                int resp = _pproxy->CountryList(&in, out);
                if (resp == 0)
                {
                    if (*out.CountryListResult->Message != "Success")
                    {
                        BERR << "Error: " << *out.CountryListResult->Message;
                        return E_SOAPError;
                    }
                }
                else
                    return E_SOAPError;

                se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
                auto& countries = out.CountryListResult->COUNTRIES->CountryBase;
                for (ns1__CountryBase* c : countries)
                {
                    // Check if country already exists, if not
                    // insert.
                    // If there is a change in country name update.
                    string scode = *c->Code;
                    string sname = *c->Name;
                    Query q = conn->query();
                    q << "select * from eod_countries where country_code = " << quote << scode;
                    if (!q.store().size())
                    {
                        q = conn->query();
                        q << "insert into eod_countries (country_code, country_name) "
                                << "values (" << quote << scode << ", " << quote << sname << ")";
                        q.execute();
                    }
                }

                return S_OK;

            }
            CATCHCLAUSE

        }

        /*        
        eod_exchanges;
        +-----------+--------------+------+-----+---------+----------------+
        | Field     | Type         | Null | Key | Default | Extra          |
        +-----------+--------------+------+-----+---------+----------------+
        | eid       | int(11)      | NO   | PRI | NULL    | auto_increment |
        | ecode     | varchar(12)  | NO   |     | NULL    |                |
        | ename     | varchar(250) | YES  |     | NULL    |                |
        | ecountry  | varchar(150) | YES  |     | NULL    |                |
        | etimezone | varchar(100) | YES  |     | NULL    |                |
        +-----------+--------------+------+-----+---------+----------------+
         */

        long unsigned int CWSClientSession::UploadExchanges()
        {
            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }

            try
            {

                _ns1__ExchangeList in;
                _ns1__ExchangeListResponse out;
                in.Token = &_token;

                int resp = _pproxy->ExchangeList(&in, out);
                if (resp == 0)
                {
                    if (*out.ExchangeListResult->Message != "Success")
                    {
                        BERR << "Error: " << *out.ExchangeListResult->Message;
                        return E_SOAPError;
                    }
                }
                else
                    return E_SOAPError;

                se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
                auto& exchanges = out.ExchangeListResult->EXCHANGES->EXCHANGE;
                for (ns1__EXCHANGE* e : exchanges)
                {
                    // Check if country already exists, if not
                    // insert.
                    // If there is a change in country name update.
                    string scode = *e->Code;
                    string sname = *e->Name;
                    string scountry = *e->Country;
                    string stz = *e->TimeZone;
                    Query q = conn->query();
                    q << "select * from eod_exchanges where ecode = " << quote << scode;
                    if (!q.store().size())
                    {
                        q = conn->query();
                        q << "insert into eod_exchanges (ecode,ename, ecountry, etimezone) "
                                << "values ("
                                << quote << scode << ", "
                                << quote << sname << ", "
                                << quote << scountry << ", "
                                << quote << stz
                                << ")";
                        q.execute();
                    }
                }

                return S_OK;

            }
            CATCHCLAUSE


        }

        /*
        +-------+--------------+------+-----+---------+----------------+
        | Field | Type         | Null | Key | Default | Extra          |
        +-------+--------------+------+-----+---------+----------------+
        | sid   | bigint(20)   | NO   | PRI | NULL    | auto_increment |
        | ssym  | varchar(50)  | NO   |     | NULL    |                |
        | sname | varchar(500) | NO   |     | NULL    |                |
        | stype | smallint(6)  | NO   |     | NULL    |                |
        +-------+--------------+------+-----+---------+----------------+
        4 rows in set (0.18 sec)

        MariaDB [saeoddata]> show columns in eod_securityexchanges;
        +-------+------------+------+-----+---------+-------+
        | Field | Type       | Null | Key | Default | Extra |
        +-------+------------+------+-----+---------+-------+
        | sid   | bigint(20) | NO   | PRI | 0       |       |
        | eid   | int(11)    | NO   | PRI | 0       |       |
        +-------+------------+------+-----+---------+-------+
         */

        long unsigned int CWSClientSession::UploadSymbols(const string& ex)
        {

            if (!IsLoggedIn())
                RETURNIFFAILED(Login());

            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }

            string exchange = ex;
            std::transform(exchange.begin(), exchange.end(), exchange.begin(), ::tolower);

            BINF << "Symbols upload started for " << exchange;

            _ns1__SymbolList in;
            _ns1__SymbolListResponse out;
            in.Token = &_token;
            in.Exchange = &const_cast<string&> (exchange);

            int resp = _pproxy->SymbolList(&in, out);
            if (resp == 0)
            {
                if (*out.SymbolListResult->Message != "Success")
                {
                    BERR << "Error: " << *out.SymbolListResult->Message;
                    return E_SOAPError;
                }
            }
            else
                return E_SOAPError;

            UID eid;
            se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
            auto q = conn->query();
            q << "select eid from eod_exchanges where ecode=" << quote << exchange;
            auto sefind = q.store();
            if (sefind.size())
                eid = sefind[0][0];
            else
                throw se::seexception(E_DBQueryException, "Failed to locate eod exchange");

           
            // Ensure that exchange tables are availble
            q = conn->query();
            q << "call create_ExchSecTables(" << quote << exchange << ")";
            q.exec();
            
            auto securityType = 1; //TODO: This is temporary. Need to update all securities at some other point in time
            auto& symbols = out.SymbolListResult->SYMBOLS->SYMBOL;
            for (ns1__SYMBOL* s : symbols)
            {
                string symbol = *s->Code;
                //symbol = symbol.substr(0, symbol.find("."));
                auto sid = FindSymbolId(symbol, exchange);
                if (!sid)
                {

                    BERR << "Symbol " << symbol << " not found in exchange " << exchange;

                    string sName = *s->Name;
                    try
                    {
                        
                        q = conn->query();

                        q << "insert into eod_" << exchange << "_securities (ssym, sname, slname, stype) values (";
                        q << quote << symbol;
                        q << ", " << quote << sName;
                        q << ", " << quote << *s->LongName;
                        q << ", " << securityType;
                        q << ")";
                        q.exec();

                    }
                    catch (const mysqlpp::Exception& ex)
                    {
                        throw se::seexception(E_DBUpdateError, ex.what());
                    }
                }

            }

            BINF << "Symbols upload completed for " << exchange;

            return S_OK;

        }

//        long unsigned int CWSClientSession::UploadSymbolsEODHistory()
//        {
//            if (!IsLoggedIn())
//                RETURNIFFAILED(Login());
//
//            se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
//            Query q = conn->query();
//            q << "select sid, max(hdate) as maxdate from eod_" << e << "_historydaily group by sid order by sid"
//            q << "SELECT a.sid, ssym, ecode, maxdate FROM saeoddata.list_SymbolsAndExchangesCompact a left join (select sid, max(hdate) as maxdate from saeoddata.eod_historydaily group by sid order by sid)  b on a.sid = b.sid;";
//            auto qstore = q.store();
//            for (const Row& r : qstore)
//            {
//                CDate ddate;
//                sql_date_null maxdate = r["maxdate"];
//                if (!maxdate.is_null)
//                    se::data::CMyqlUtilities::PopulateDateTime(maxdate.data, ddate);
//                if (!ddate.IsEmpty())
//                {
//                    if ((ddate.get_WeekDay() >=1 && ddate.get_WeekDay()<=5) and ddate.IsToday())
//                        continue;
//                }
//                const string sym = (CSTR) r["ssym"];
//                const string exch = (CSTR) r["ecode"];
//                if (FAILED(UpdateSymbolEODHistory(exch, sym, ddate)))
//                {
//                    BERR << "Failed to upate symbol price history for " << sym << " in exch " << exch;
//                }
//            }
//
//            BINF << "Completed successfully.";
//            return S_OK;
//
//            BERR << "No records found. Return as failed.";
//            return E_NotFound;
//
//        }
        
        SE_CODE CWSClientSession::UploadSymbolsEODHistoryByExchange(const string& ex)
        {
            
            if (!IsLoggedIn())
                RETURNIFFAILED(Login());
            
            string exchange = ex;
            std::transform(exchange.begin(), exchange.end(), exchange.begin(), ::tolower);
            

            se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
            Query q = conn->query();
            q << "select sid, max(hdate) as maxdate from eod_" << exchange << "_historydaily group by sid order by sid";
            auto qstore = q.store();
            CDate dtNow = CDate::Now();
            CDate dtLwd ;
            if (dtNow.get_WeekDay() == 0)
                dtLwd = dtNow -= 2;
            else if (dtNow.get_WeekDay() == 7)
                dtLwd = dtNow -= 1;
            else
                dtLwd = dtNow;
            for (const Row& r : qstore)
            {
                CDate ddate;
                sql_date_null maxdate = r["maxdate"];
                if (!maxdate.is_null)
                    se::data::CMyqlUtilities::PopulateDateTime(maxdate.data, ddate);
                if (!ddate.IsEmpty())
                {
                    if (dtLwd == ddate)
                        continue;
                }
                const string sym = (CSTR) r["ssym"];
                if (FAILED(UpdateSymbolEODHistory(exchange, sym, ddate)))
                {
                    BERR << "Failed to upate symbol price history for " << sym << " in exch " << exchange;
                }
            }

            BINF << "Completed successfully.";
            return S_OK;

            BERR << "No records found. Return as failed.";
            return E_NotFound;

        }

        /*
         * Table: eod_historydaily
            Columns:
            hdate date PK 
            sid bigint(20) PK 
            hopen decimal(12,4) 
            hhigh decimal(12,4) 
            hlow decimal(12,4) 
            hclose decimal(12,4) 
            hvol bigint(20) 
            hopeninterest bigint(20) 
            hprev decimal(12,4) 
            hchg decimal(12,4) 
            hbid decimal(12,4) 
            hask decimal(12,4) 
            hnextopen decimal(12,4) 
            hmodified datetime
         */

        sql_create_14(eod_historydaily,
                2,
                1,
                sql_datetime, hdate,
                sql_bigint, sid,
                sql_decimal_null, hopen,
                sql_decimal_null, hhigh,
                sql_decimal_null, hlow,
                sql_decimal_null, hclose,
                sql_bigint, hvol,
                sql_bigint, hopeninterest,
                sql_decimal_null, hprev,
                sql_decimal_null, hchg,
                sql_decimal_null, hbid,
                sql_decimal_null, hask,
                sql_decimal_null, hnextopen,
                sql_datetime, hmodified
                )


        long unsigned int CWSClientSession::UploadExchangeQuotesCurrent(const string& ex)
        {

            if (!IsLoggedIn())
                RETURNIFFAILED(Login());

            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }
            
            string exchange = ex;
            std::transform(exchange.begin(), exchange.end(), exchange.begin(), ::tolower);
            
            BINF << "Uploading started for " << exchange;

            _ns1__QuoteList in;
            _ns1__QuoteListResponse out;
            in.Exchange = &const_cast<string&> (exchange);
            in.Token = &_token;

            int resp = _pproxy->QuoteList(&in, out);
            if (!resp)
            {
                if (*out.QuoteListResult->Message != "Success")
                {
                    BERR << "Error: " << *out.QuoteListResult->Message;
                    return E_SOAPError;
                }
            }
            else
            {
                return E_SOAPError;
            }
            typedef vector<eod_historydaily> Table;
            int index = 1;
            Table table;
            se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
            const auto& quotes = out.QuoteListResult->QUOTES->QUOTE;
            BDBG << "Writing " << quotes.size() << " of quoe records for exchange " << exchange;
            const string tableName = "eod_" + exchange + "_historydaily";
            for (auto p : quotes)
            {
                eod_historydaily r;
                r.instance_table(tableName.c_str());
                se::DateTime hdate = (const time_t&) p->DateTime;
                se::data::CMyqlUtilities::PopulateDateTime(hdate, r.hdate);
                
                auto sid = FindSymbolId(*p->Symbol, exchange);
                        
                r.sid = sid;
                r.hopen = p->Open;
                r.hhigh = p->High;
                r.hlow = p->Low;
                r.hclose = p->Close;
                r.hvol = p->Volume;
                r.hopeninterest = p->OpenInterest;
                r.hprev = p->Previous;
                r.hchg = p->Change;
                r.hbid = p->Bid;
                r.hask = p->Ask;
                r.hnextopen = p->NextOpen;
                hdate = (const time_t&) p->Modified;
                se::data::CMyqlUtilities::PopulateDateTime(hdate, r.hmodified);
                table.emplace_back(r);
                if (0 == (index % 100))
                {
                    // execute
                    Query q = conn->query();
                    q.replace(table.begin(), table.end()).execute();
                    table.clear();
                }
                index++;
            }

            if (table.size())
            {
                // execute
                Query q = conn->query();
                q.replace(table.begin(), table.end()).execute();
            }

            BDBG << "Successfully wrote " << quotes.size() << " quote records for " << exchange;

            BINF << "Successfully Uploaded quote records for " << exchange;


            return S_OK;
        }
                

        long unsigned int CWSClientSession::UploadExchangeQuotes(const string& exchange, const CDate& startdate)
        {
            BINF << "Uploading started for " << exchange;

            if (!IsLoggedIn())
                RETURNIFFAILED(Login());

            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }

            _ns1__QuoteListByDate in;
            _ns1__QuoteListByDateResponse out;
            in.Exchange = &const_cast<string&> (exchange);
            in.Token = &_token;
            CDate tmp = startdate;
            if (tmp.IsEmpty())
            {
                tmp.set_Day(2);
                tmp.set_Month(1);
                tmp.set_Year(2013);
            }

            string date;
            ostringstream ostr;
            ostr << tmp.get_Year() << setw(2) << setfill('0') 
                    << tmp.get_Month() << setw(2) << setfill('0') 
                    << tmp.get_Day();
            date = ostr.str();
            in.QuoteDate = &date;

            int resp = _pproxy->QuoteListByDate(&in, out);
            if (!resp)
            {
                if (*out.QuoteListByDateResult->Message != "Success")
                {
                    BERR << "Error: " << *out.QuoteListByDateResult->Message;
                    return E_SOAPError;
                }
            }
            else
            {
                return E_SOAPError;
            }
            typedef vector<eod_historydaily> Table;
            int index = 1;
            Table table;
            se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
            const auto& quotes = out.QuoteListByDateResult->QUOTES->QUOTE;
            BDBG << "Writing " << quotes.size() << " of quoe records for exchange " << exchange;
            const string tableName = "eod_" + exchange + "_historydaily";
            for (auto p : quotes)
            {
                eod_historydaily r;
                r.instance_table(tableName.c_str());
                se::DateTime hdate = (const time_t&) p->DateTime;
                se::data::CMyqlUtilities::PopulateDateTime(hdate, r.hdate);
                
                auto sid = FindSymbolId(*p->Symbol, exchange);
                        
                r.sid = sid;
                r.hopen = p->Open;
                r.hhigh = p->High;
                r.hlow = p->Low;
                r.hclose = p->Close;
                r.hvol = p->Volume;
                r.hopeninterest = p->OpenInterest;
                r.hprev = p->Previous;
                r.hchg = p->Change;
                r.hbid = p->Bid;
                r.hask = p->Ask;
                r.hnextopen = p->NextOpen;
                hdate = (const time_t&) p->Modified;
                se::data::CMyqlUtilities::PopulateDateTime(hdate, r.hmodified);
                table.emplace_back(r);
                if (0 == (index % 100))
                {
                    // execute
                    Query q = conn->query();
                    q.replace(table.begin(), table.end()).execute();
                    table.clear();
                }
                index++;
            }

            if (table.size())
            {
                // execute
                Query q = conn->query();
                q.replace(table.begin(), table.end()).execute();
            }

            BDBG << "Successfully wrote " << quotes.size() << " quote records for " << exchange;

            BINF << "Successfully Uploaded quote records for " << exchange;


            return S_OK;

        }

        long unsigned int CWSClientSession::UpdateSymbolEODHistory(const string& ex, const string& symbol, const CDate& datestart)
        {


            if (!IsLoggedIn())
                RETURNIFFAILED(Login());

            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }
            
            string exchange = ex;
            std::transform(exchange.begin(), exchange.end(), exchange.begin(), ::tolower);
            BINF << "1. Uploading price history for " << symbol << ":" << exchange;
            

            BINF << "2. Uploading price history for " << symbol << ":" << exchange;

            _ns1__SymbolHistory in;
            _ns1__SymbolHistoryResponse out;

            string date;
            if (!datestart.IsEmpty())
            {
                ostringstream ostr;
                ostr << datestart.get_Year() << setw(2) << setfill('0') << datestart.get_Month() << setw(2) << setfill('0') << datestart.get_Day();
                date = ostr.str();
            }
            in.Exchange = &const_cast<string&> (exchange);
            in.Symbol = &const_cast<string&> (symbol);
            in.Token = &_token;
            in.StartDate = &date;

            int resp = _pproxy->SymbolHistory(&in, out);
            if (!resp)
            {
                if (*out.SymbolHistoryResult->Message != "Success")
                {
                    BERR << "Error: " << *out.SymbolHistoryResult->Message;
                    return E_SOAPError;
                }
            }
            else
            {
                return E_SOAPError;
            }

            int sid = FindSymbolId(symbol, exchange);
            typedef vector<eod_historydaily> Table;
            int index = 1;
            Table table;
            se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
            const auto& quotes = out.SymbolHistoryResult->QUOTES->QUOTE;
            BDBG << "Writing " << quotes.size() << " price records for " << symbol << ":" << exchange;
            const string tableName = "eod_" + exchange + "_historydaily";
            for (auto p : quotes)
            {
                eod_historydaily r;
                r.instance_table(tableName.c_str());
                se::DateTime hdate = (const time_t&) p->DateTime;
                se::data::CMyqlUtilities::PopulateDateTime(hdate, r.hdate);
                r.sid = sid;
                r.hopen = p->Open;
                r.hhigh = p->High;
                r.hlow = p->Low;
                r.hclose = p->Close;
                r.hvol = p->Volume;
                r.hopeninterest = p->OpenInterest;
                r.hprev = p->Previous;
                r.hchg = p->Change;
                r.hbid = p->Bid;
                r.hask = p->Ask;
                r.hnextopen = p->NextOpen;
                hdate = (const time_t&) p->Modified;
                se::data::CMyqlUtilities::PopulateDateTime(hdate, r.hmodified);
                table.emplace_back(r);
                if (0 == (index % 100))
                {
                    // execute
                    Query q = conn->query();
                    q.replace(table.begin(), table.end()).execute();
                    table.clear();
                }
                index++;
            }

            if (table.size())
            {
                // execute
                Query q = conn->query();
                q.replace(table.begin(), table.end()).execute();
            }

            BDBG << "Successfully wrote " << quotes.size() << " price records for " << symbol << ":" << exchange;

            BINF << "Successfully Uploaded price history for " << symbol << ":" << exchange;

            return S_OK;
        }

        /*
        eod_fundamentals;
        +---------------+---------------+------+-----+---------+-------+
        | Field         | Type          | Null | Key | Default | Extra |
        +---------------+---------------+------+-----+---------+-------+
        | fdate         | datetime      | NO   |     | NULL    |       |
        | sid           | bigint(20)    | NO   |     | NULL    |       |
        | findustry     | varchar(350)  | YES  |     | NULL    |       |
        | fsector       | varchar(350)  | YES  |     | NULL    |       |
        | fnumshares    | bigint(20)    | YES  |     | NULL    |       |
        | fmarketcap    | bigint(20)    | YES  |     | NULL    |       |
        | fppe          | decimal(12,4) | YES  |     | NULL    |       |
        | feps          | decimal(12,4) | YES  |     | NULL    |       |
        | fnta          | decimal(12,4) | YES  |     | NULL    |       |
        | fdivyld       | decimal(12,4) | YES  |     | NULL    |       |
        | fdiv          | decimal(12,4) | YES  |     | NULL    |       |
        | fdivdate      | datetime      | YES  |     | NULL    |       |
        | fdivpershare  | decimal(12,4) | YES  |     | NULL    |       |
        | fimputcredits | decimal(12,4) | YES  |     | NULL    |       |
        | febitda       | decimal(12,4) | YES  |     | NULL    |       |
        | fpeg          | decimal(12,4) | YES  |     | NULL    |       |
        | fpts          | decimal(12,4) | YES  |     | NULL    |       |
        | fptb          | decimal(12,4) | YES  |     | NULL    |       |
        | fyld          | decimal(12,4) | YES  |     | NULL    |       |
        +---------------+---------------+------+-----+---------+-------+
         */

        sql_create_19(eod_fundamentals,
                2, 1,
                sql_datetime, fdate,
                sql_bigint, sid,
                sql_varchar_null, findustry,
                sql_varchar_null, fsector,
                sql_bigint, fnumshares,
                sql_bigint, fmarketcap,
                sql_decimal_null, fppe,
                sql_decimal_null, feps,
                sql_decimal_null, fnta,
                sql_decimal_null, fdivyld,
                sql_decimal_null, fdiv,
                sql_datetime_null, fdivdate,
                sql_decimal_null, fdivpershare,
                sql_decimal_null, fimputcredits,
                sql_decimal_null, febitda,
                sql_decimal_null, fpeg,
                sql_decimal_null, fpts,
                sql_decimal_null, fptb,
                sql_decimal_null, fyld
                )

        long unsigned int CWSClientSession::UpdateFundamentals(const string& ex)
        {
            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }
            string exchange = ex;
            std::transform(exchange.begin(), exchange.end(), exchange.begin(), ::tolower);

            BINF << "Fundamental update started for " << exchange;

            _ns1__FundamentalList in;
            _ns1__FundamentalListResponse out;
            in.Token = &_token;
            in.Exchange = &const_cast<string&> (exchange);

            int resp = _pproxy->FundamentalList(&in, out);
            if (resp == 0)
            {
                if (*out.FundamentalListResult->Message != "Success")
                {
                    BERR << "Error: " << *out.FundamentalListResult->Message;
                    return E_SOAPError;
                }
            }
            else
                return E_SOAPError;

            typedef vector<eod_fundamentals> Table;
            Table table;
            auto& fundamentals = out.FundamentalListResult->FUNDAMENTALS->FUNDAMENTAL;
            // push every 100 records
            int index = 1;
            se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
            const string tableName = "eod_" + exchange + "_fundamentals";
            for (ns1__FUNDAMENTAL* f : fundamentals)
            {
                eod_fundamentals rec;
                rec.instance_table(tableName.c_str());
                string symbol = *f->Symbol;
                // strip suffix.
                //symbol = symbol.substr(0, symbol.find("."));
                int sid = 0;
                if (!(sid = FindSymbolId(symbol, exchange)))
                {
                    BWRN << "Symbol record not found for symbol " << symbol;
                    continue;
                }

                try
                {
                    // Check if country already exists, if not
                    // insert.
                    // If there is a change in country name update.
                    se::DateTime fdate = (const time_t&) f->DateTime;
                    se::data::CMyqlUtilities::PopulateDateTime(fdate, rec.fdate);
                    // find sid
                    rec.sid = sid;
                    rec.findustry = *f->Industry;
                    rec.fsector = *f->Sector;
                    rec.fnumshares = f->Shares;
                    rec.fmarketcap = f->MarketCap;
                    rec.fppe = f->PE;
                    rec.feps = f->EPS;
                    rec.fnta = f->NTA;
                    rec.fdivyld = f->DivYield;
                    rec.fdiv = f->Dividend;
                    DateTime divDate = f->DividendDate;
                    se::data::CMyqlUtilities::PopulateDateTime(divDate, rec.fdivdate.data);
                    rec.fdivpershare = f->DPS;
                    rec.fimputcredits = f->ImputationCredits;
                    rec.febitda = f->EBITDA;
                    rec.fpeg = f->PEG;
                    rec.fpts = f->PtS;
                    rec.fptb = f->PtB;
                    rec.fyld = f->Yield;


                    /*
                    // use insert ignore command.
                    Query q = conn->query();
                    q << "INSERT IGNORE INTO `eod_fundamentals` " ;
                    q << "(`fdate`, `sid`, `findustry`, `fsector`, `fnumshares`, ";
                    q << "`fmarketcap`, `fppe`, `feps`, `fnta`, `fdivyld`, ";
                    q << "`fdiv`, `fdivdate`, `fdivpershare`, `fimputcredits`, ";
                    q << "`febitda`, `fpeg`, `fpts`, `fptb`, `fyld`) ";
                    q << "VALUES (" << dtdate << ", " << sid << ", " << quote << industry ;
                    q << ", " << quote << sector;
                    q << ", " << numShares ;
                    q << ", " << marketcap ;
                    q << ", " << ppe ;
                    q << ", " << eps ;
                    q << ", " << nta;
                    q << ", " << divyld;
                    q << ", " << div;
                    q << ", " << dtdivdate;
                    q << ", " << divpshare;
                    q << ", " << imputcredits;
                    q << ", " << ebitda;
                    q << ", " << peg;
                    q << ", " << pts;
                    q << ", " << ptb;
                    q << ", " << yld;
                    q << ")";
                    q.execute();
                     */

                    table.emplace_back(rec);
                }
                catch (const std::exception& ex)
                {
                    BERR << "Symbol " << symbol << " Failed: " << ex.what();
                    rec;
                }

                if (0 == (index % 100))
                {
                    // execute
                    Query q = conn->query();
                    q.replace(table.begin(), table.end()).execute();
                    table.clear();

                }

                index++;
            }

            if (table.size())
            {
                // execute
                Query q = conn->query();
                q.replace(table.begin(), table.end()).execute();
            }
            
            BINF << "Fundamental update completed for " << exchange;
            

            return S_OK;
        }

        /*
        eod_technicals;
        +-------------+---------------+------+-----+---------+-------+
        | Field       | Type          | Null | Key | Default | Extra |
        +-------------+---------------+------+-----+---------+-------+
        | tdate       | datetime      | NO   |     | NULL    |       |
        | sid         | bigint(20)    | NO   |     | NULL    |       |
        | tprev       | decimal(12,4) | YES  |     | NULL    |       |
        | tchg        | decimal(12,4) | YES  |     | NULL    |       |
        | tma1        | int(11)       | YES  |     | NULL    |       |
        | tma2        | int(11)       | YES  |     | NULL    |       |
        | tma5        | int(11)       | YES  |     | NULL    |       |
        | tma20       | int(11)       | YES  |     | NULL    |       |
        | tma100      | int(11)       | YES  |     | NULL    |       |
        | tma200      | int(11)       | YES  |     | NULL    |       |
        | tmapct      | decimal(12,4) | YES  |     | NULL    |       |
        | tmaret      | decimal(12,4) | YES  |     | NULL    |       |
        | tvolchg     | bigint(20)    | YES  |     | NULL    |       |
        | t3mchg      | decimal(12,4) | YES  |     | NULL    |       |
        | t6mchg      | decimal(12,4) | YES  |     | NULL    |       |
        | tweekhigh   | decimal(12,4) | YES  |     | NULL    |       |
        | tweeklow    | decimal(12,4) | YES  |     | NULL    |       |
        | tavgweekchg | decimal(12,4) | YES  |     | NULL    |       |
        | tavgweekvol | decimal(12,4) | YES  |     | NULL    |       |
        | tweekvol    | decimal(12,4) | YES  |     | NULL    |       |
        | tmonhigh    | decimal(12,4) | YES  |     | NULL    |       |
        | tmonlow     | decimal(12,4) | YES  |     | NULL    |       |
        | tmonchg     | decimal(12,4) | YES  |     | NULL    |       |
        | tavgmonchg  | decimal(12,4) | YES  |     | NULL    |       |
        | tavgmonvol  | decimal(12,4) | YES  |     | NULL    |       |
        | tyearhigh   | decimal(12,4) | YES  |     | NULL    |       |
        | tyearlow    | decimal(12,4) | YES  |     | NULL    |       |
        | tyearchg    | decimal(12,4) | YES  |     | NULL    |       |
        | tavgyearchg | decimal(12,4) | YES  |     | NULL    |       |
        | tavgyearvol | decimal(12,4) | YES  |     | NULL    |       |
        | tytdchg     | decimal(12,4) | YES  |     | NULL    |       |
        | trsi14      | decimal(12,4) | YES  |     | NULL    |       |
        | tst09       | decimal(12,4) | YES  |     | NULL    |       |
        | twpr14      | decimal(12,4) | YES  |     | NULL    |       |
        | tmtm14      | decimal(12,4) | YES  |     | NULL    |       |
        | troc14      | decimal(12,4) | YES  |     | NULL    |       |
        | tpic        | bigint(20)    | YES  |     | NULL    |       |
        | tsar        | decimal(12,4) | YES  |     | NULL    |       |
        | tvolatility | decimal(12,4) | YES  |     | NULL    |       |
        | tliquidity  | decimal(12,4) | YES  |     | NULL    |       |
        +-------------+---------------+------+-----+---------+-------+
         */

        long unsigned int CWSClientSession::UpdateTechnicals(const string& ex)
        {

            if (!_pproxy)
            {
                BWRN << "Proxy instance is not initialized.";
                return E_IncorrectOperation;
            }
            
            string exchange = ex;
            std::transform(exchange.begin(), exchange.end(), exchange.begin(), ::tolower);
            
            BINF << "Technicals update started for " << exchange;

            _ns1__TechnicalList in;
            _ns1__TechnicalListResponse out;
            in.Token = &_token;
            in.Exchange = &const_cast<string&> (exchange);

            int resp = _pproxy->TechnicalList(&in, out);
            if (resp == 0)
            {
                if (*out.TechnicalListResult->Message != "Success")
                {
                    BERR << "Error: " << *out.TechnicalListResult->Message;
                    return E_SOAPError;
                }
            }
            else
                return E_SOAPError;

            typedef vector<eod_fundamentals> Table;
            Table table;
            auto& technicals = out.TechnicalListResult->TECHNICALS->TECHNICAL;
            se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
            for (ns1__TECHNICAL* t : technicals)
            {


                sql_datetime tdate;
                DateTime dtDate = t->DateTime;
                se::data::CMyqlUtilities::PopulateDateTime(dtDate, tdate);
                string symbol = *t->Symbol;
                //symbol = symbol.substr(0, symbol.find("."));
                int sid = 0;
                if (!(sid = FindSymbolId(symbol, exchange)))
                {
                    BWRN << "Symbol record not found for symbol " << symbol;
                    continue;
                }
                double tprev, tchg, tmapct, tmaret, t3mchg, t6mchg;
                double tweekhigh, tweeklow, tavgweekchg, tavgweekvol, tweekvol;
                double tmonhigh, tmonlow, tmonchg, tavgmonchg, tavgmonvol;
                double tyearhigh, tyearlow, tyearchg, tavgyearchg, tavgyearvol;
                double tytdchg, trsi14, tst09, twpr14, tmtm14, troc14, tsar;
                double tvolatility, tliquidity;
                I64 tma1, tma2, tma5, tma20, tma100, tma200, tvolchg, tpic;

                tprev = t->Previous;
                tchg = t->Change;
                tmapct = t->MAPercent;
                tmaret = t->MAReturn;
                t3mchg = t->ThreeMonthChange;
                t6mchg = t->SixMonthChange;
                tweekhigh = t->WeekHigh;
                tweeklow = t->WeekLow;
                tavgweekchg = t->AvgWeekChange;
                tavgweekvol = t->AvgWeekVolume;
                tmonhigh = t->MonthHigh;
                tmonlow = t->MonthLow;
                tmonchg = t->MonthChange;
                tavgmonchg = t->AvgMonthChange;
                tavgmonvol = t->AvgMonthVolume;
                tyearhigh = t->YearHigh;
                tyearlow = t->YearLow;
                tyearchg = t->YearChange;
                tavgyearchg = t->AvgYearChange;
                tavgyearvol = t->AvgYearVolume;
                tytdchg = t->YTDChange;
                trsi14 = t->RSI14;
                tst09 = t->STO9;
                twpr14 = t->WPR14;
                tmtm14 = t->MTM14;
                troc14 = t->ROC14;
                tsar = t->SAR;
                tvolatility = t->Volatility;
                tliquidity = t->Liquidity;
                tma1 = t->MA1;
                tma2 = t->MA2;
                tma5 = t->MA5;
                tma20 = t->MA20;
                tma100 = t->MA100;
                tma200 = t->MA200;
                tvolchg = t->VolumeChange;
                tpic = t->PTC;
                /*
                tdate sid tprev tchg  tma1  tma2  tma5  
                tma20 tma100 tma200 tmapct tmaret tvolchg t3mchg t6mchg tweekhigh
                tweeklow tavgweekchg tavgweekvol tweekvol tmonhigh tmonlow 
                tmonchg tavgmonchg tavgmonvol tyearhigh tyearlow 
                tyearchg tavgyearchg tavgyearvol tytdchg trsi14 
                tst09       twpr14      tmtm14      troc14      tpic        
                tsar        tvolatility             tliquidity 
                 */
                Query q = conn->query();
                q << "INSERT IGNORE INTO eod_" << exchange << "_technicals VALUES (";
                q << quote << tdate << ", " << sid << ", " << tprev << ", " << tchg << ", ";
                q << tma1 << ", " << tma2 << ", " << tma5 << ", " << tma20 << ", ";
                q << tma100 << ", " << tma200 << ", " << tmapct << ", " << tmaret << ", " << tvolchg << ", ";
                q << t3mchg << ", " << t6mchg << ", " << tweekhigh << ", " << tweeklow << ", ";
                q << tavgweekchg << ", " << tavgweekvol << ", " << tweekvol << ", ";
                q << tmonhigh << ", " << tmonlow << ", " << tmonchg << ", " << tavgmonchg << ", ";
                q << tavgmonvol << ", " << tyearhigh << ", " << tyearlow << ", ";
                q << tyearchg << ", " << tavgyearchg << ", " << tavgyearvol << ", ";
                q << tytdchg << ", " << trsi14 << ", " << tst09 << ", " << twpr14 << ", ";
                q << tmtm14 << ", " << troc14 << ", " << tpic << ", " << tsar << ", ";
                q << tvolatility << ", " << tliquidity;
                q << ")";
                //BDBG << q.str();
                q.execute();

            }
            
            BINF << "Technicals update completed " << exchange;
            

            return S_OK;

        }

        /*
        eod_securities
        +-------+--------------+------+-----+---------+----------------+
        | Field | Type         | Null | Key | Default | Extra          |
        +-------+--------------+------+-----+---------+----------------+
        | sid   | bigint(20)   | NO   | PRI | NULL    | auto_increment |
        | ssym  | varchar(50)  | NO   |     | NULL    |                |
        | sname | varchar(500) | NO   |     | NULL    |                |
        | stype | smallint(6)  | NO   |     | NULL    |                |
        +-------+--------------+------+-----+---------+----------------+

        eod_securityexchanges;
        +-------+------------+------+-----+---------+-------+
        | Field | Type       | Null | Key | Default | Extra |
        +-------+------------+------+-----+---------+-------+
        | sid   | bigint(20) | NO   | PRI | 0       |       |
        | eid   | int(11)    | NO   | PRI | 0       |       |
        +-------+------------+------+-----+---------+-------+
         */

        long unsigned int CWSClientSession::FindSymbolId(const string& symbol, const string& ex)
        {
            string exchange = ex;
            std::transform(exchange.begin(), exchange.end(), exchange.begin(), ::tolower);
            
            se::data::CSmartConnection conn(se::data::CSEConnections::GetEodData());
            Query q = conn->query();
            q << "select sid from eod_" << exchange << "_securities where ssym=" << quote << symbol;
            BDBG << q.str();
            auto uq = q.use();
            if (uq)
            {
                const Row& r = uq.fetch_row();
                if (r)
                    return r[0];
            }
            return 0;
        }
    }
}