/* 
 * File:   CEODDataWrappers.cpp
 * Author: santony
 * 
 * Created on February 4, 2015, 6:31 PM
 */

#include "seeoddataint.h"
#include "CEODDataWrappers.h"

namespace se
{
    namespace eoddata
    {
        
        CExchangeList::CExchangeList(ISession* ps):CEnumerable<IExchange, CExchange, IExchangeList, EODEXCHANGELIST >(ps)
        {
            
        }
        long unsigned int CExchangeList::Init(EODEXCHANGELIST* const eodData) 
        {
            return CEnumerable<IExchange, CExchange, IExchangeList, EODEXCHANGELIST>::Init(eodData);
        }

        
        SE_CODE CExchangeList::Find(std::function< bool (const IExchange&)> func, IExchange* pOut)
        {
            
        }

        long unsigned int CExchangeList::get_Size() 
        {
            return getInternalList()->size();
        }

        
        ////////////////////////////////Exchange////////////////////////////////////////
        long unsigned int CExchange::Init(EODEXCHANGE* const orig) 
        {
            _sName = *orig->Name;
            _sCode = *orig->Code;
            _sCountry = *orig->Country;
            _sTZone = *orig->TimeZone;
            return S_OK;
        }

        const char* CExchange::get_Name() 
        {
            return _sName.c_str();
        }

        const char* CExchange::get_Code() 
        {
            return _sCode.c_str();
        }

        const char* CExchange::get_Country() 
        {
            return _sCountry.c_str();
        }

        const char* CExchange::get_TimeZone() 
        {
            return _sTZone.c_str();
        }
        
        ///////////////////////////News///////////////////////////

        long unsigned int CNewsList::Init(EODNEWSLIST * const p) 
        {
            return CEnumerable<INews,CNews,INewsList,EODNEWSLIST>::Init(p);
        }

        long unsigned int CNews::Init(EODNEWS* pnews) 
        {
            _sSym = *pnews->Symbol;
            _sExch = *pnews->Exchange;
            _sSource = *pnews->SourceKey;
            
            _dtWhen = pnews->DateTime;
            return S_OK;
        }
        const char* CNews::get_Symbol() 
        {
            return _sSym.c_str();
        }
        const char* CNews::get_Exchange() 
        {
            return _sSym.c_str();
        }

        const char* CNews::get_Source() {
            return _sSource.c_str();
        }

        DATETIME& CNews::get_When() {
            return _dtWhen;
        }

        const char* CNews::get_Title() {
            return _sTitle.c_str();
        }

        const char* CNews::get_Intro() {
            return _sIntro.c_str();
        }

        const char* CNews::get_Story() {
            return _sStory.c_str();
        }

        const char* CNews::get_Caption() {
            return _sCaption.c_str();

        }

        signed int CNews::get_Views() {
            return _iViews;

        }
        
        //////////////////////////Fundamentals//////////////////////////////

        long unsigned int CSecurityFundamentals::Init(EODFUNDAMENTALS* fund) 
        {
            _dt = fund->DateTime;
            _industry = *fund->Industry;
            _sector = *fund->Sector;
            _numOfShares = fund->Shares;
            _marketCap = fund->MarketCap;
            _pe = fund->PE;
            _eps = fund->EPS;
            _nta = fund->NTA;
            _divYld = fund->DivYield;
            _div = fund->Dividend;
            _dtDivDate = fund->DividendDate;
            _divPShare = fund->DPS;
            _impCredits = fund->ImputationCredits;
            _ebitda = fund->EBITDA;
            _peg = fund->PEG;
            _pts = fund->PtS;
            _yld = fund->Yield;
            
            return S_OK;
        }

        DATETIME& CSecurityFundamentals::get_DateTime() {
            return _dt;
        }

        const char* CSecurityFundamentals::get_Industry() const {
            return _industry.c_str();
        }

        const char* CSecurityFundamentals::get_Sector() const {
            return _sector.c_str();
        }

        unsigned long CSecurityFundamentals::get_NumOfShares() const {
            return _numOfShares;
        }

        unsigned long CSecurityFundamentals::get_MarketCap() const {
            return _marketCap;
        }

        double CSecurityFundamentals::get_PricePerEarnings() const {
            return _pe;
        }

        double CSecurityFundamentals::get_EPS() const {
            return _eps;
        }

        double CSecurityFundamentals::get_NTA() const {
            return _nta;
        }

        double CSecurityFundamentals::get_DivYield() const {
            return _divYld;
        }

        double CSecurityFundamentals::get_Dividend() const {
            return _div;
        }

        DATETIME& CSecurityFundamentals::get_DividendDate(){
            return _dtDivDate;
        }

        double CSecurityFundamentals::get_DividendPerShare() const {
            return _divPShare;
        }

        double CSecurityFundamentals::get_ImputationCredits() const {
            return _impCredits;
        }

        double CSecurityFundamentals::get_EBITDA() const {
            return _ebitda;
        }

        double CSecurityFundamentals::get_PEG() const {
            return _peg;
        }

        double CSecurityFundamentals::get_PtS() const {
            return _pts;
        }

        double CSecurityFundamentals::get_PtB() const {
            return _ptb;
        }


        double CSecurityFundamentals::get_Yield() const {
            return _yld;
        }

        ///////////////////////////quote////////////////////////////

        long unsigned int CSecurityQuote::Init(EODQUOTE* quote) 
        {
            _dt = quote->DateTime;
            _open = quote->Open;
            _high = quote->High;
            _low = quote->Low;
            _close = quote->Close;
            _vol = quote->Volume;
            _openint = quote->OpenInterest;
            _prev = quote->Previous;
            _change = quote->Change;
            _bid = quote->Bid;
            _ask = quote->Ask;
            _prevClose = quote->PreviousClose;
            _nextOpen = quote->NextOpen;
            _dtmod = quote->Modified;
            return S_OK;
        }

        DATETIME& CSecurityQuote::get_DateTime() 
        {
            return _dt;
        }


        double CSecurityQuote::get_Open() const {
            return _open;
        }

        double CSecurityQuote::get_High() const {
            return _high;
        }

        double CSecurityQuote::get_Low() const {
            return _low;
        }

        double CSecurityQuote::get_Close() const {
            return _close;
        }

        unsigned long CSecurityQuote::get_Volume() const {
            return _vol;
        }

        signed long CSecurityQuote::get_Openinterest() const {
            return _openint;
        }

        double CSecurityQuote::get_Prev() const {
            return _prev;
        }

        double CSecurityQuote::get_Change() const {
            return _change;
        }

        double CSecurityQuote::get_Bid() const {
            return _bid;
        }

        double CSecurityQuote::get_Ask() const {
            return _ask;
        }

        double CSecurityQuote::get_PreviousClose() const {
            return _prevClose;
        }

        double CSecurityQuote::get_NextOpen() const {
            return _nextOpen;
        }

        DATETIME& CSecurityQuote::get_Modified() {
            return _dtmod;
        }
        

        long unsigned int CSecurityQuoteList::Init(EODSECURITYQUOTELIST * const qlist) 
        {
            return CEnumerable<ISecurityQuote, CSecurityQuote, ISecurityQuoteList, EODSECURITYQUOTELIST>::Init(qlist);
        }

        
        
        ////////////////////////Fundamentals//////////////////////////////////////

        long unsigned int CSecurityTechnicals::Init(EODTECHNICAL* tec) 
        {
            _dt = tec->DateTime;
            _prev = tec->Previous;
            _change = tec->Change;
            _ma1 = tec->MA1;
            _ma2 = tec->MA2;
            _ma5 = tec->MA5;
            _ma20 = tec->MA20;
            _ma100 = tec->MA100;
            _ma200 = tec->MA200;
            _mapct = tec->MAPercent;
            _maret = tec->MAReturn;
            _volchg = tec->VolumeChange;
            _3mchg = tec->ThreeMonthChange;
            _6mchg = tec->SixMonthChange;
            _weekhigh = tec->WeekHigh;
            _weeklow = tec->WeekLow;
            _avgweekchg = tec->AvgWeekChange;
            _avgweekvol = tec->AvgWeekVolume;
            _weekvol = tec->WeekVolume;
            _mhigh = tec->MonthHigh;
            _mlow = tec->MonthLow;
            _mchg = tec->MonthChange;
            _avgmchg = tec->AvgMonthChange;
            _avgmvol = tec->AvgMonthVolume;
            _mvol = tec->MonthVolume;
            _yhigh = tec->YearHigh;
            _ylow = tec->YearLow;
            _ychg = tec->YearChange;
            _avgychg = tec->AvgYearChange;
            _avgyvol = tec->AvgYearVolume;
            _ytdchg = tec->YTDChange;
            _rsi14 = tec->RSI14;
            _st09 = tec->STO9;
            _wpr14 = tec->WPR14;
            _mtm14 = tec->MTM14;
            _roc14 = tec->ROC14;
            _ptc = tec->PTC;
            _sar = tec->SAR;
            _volatility = tec->Volatility;
            _liquidity = tec->Liquidity;
            
            return S_OK; 
        }


        DATETIME& CSecurityTechnicals::get_DateTime() {
            return _dt;
        }

        double CSecurityTechnicals::get_Previous() const {
            return _prev;
        }

        double CSecurityTechnicals::get_Change() const {
            return _change;
        }

        signed int CSecurityTechnicals::get_MA1() const {
            return _ma1;
        }

        signed int CSecurityTechnicals::get_MA2() const {
            return _ma2;
        }

        signed int CSecurityTechnicals::get_MA5() const {
            return _ma5;
        }
        signed int CSecurityTechnicals::get_MA20() const {
            return _ma20;
        }

        signed int CSecurityTechnicals::get_MA100() const {
            return _ma100;
        }

        signed int CSecurityTechnicals::get_MA200() const {
            return _ma200;
        }

        double CSecurityTechnicals::get_MAPercent() const {
            return _mapct;
        }

        double CSecurityTechnicals::get_MAReturn() const {
            return _maret;
        }

        signed long CSecurityTechnicals::get_VolumeChange() const {
            return _volchg;
        }

        double CSecurityTechnicals::get_ThreeMonthChange() const {
            return _3mchg;
        }

        double CSecurityTechnicals::get_SixMonthChange() const {
            return _6mchg;
        }

        double CSecurityTechnicals::get_WeekHigh() const {
            return _weekhigh;
        }

        double CSecurityTechnicals::get_WeekLow() const {
            return _weeklow;
        }

        double CSecurityTechnicals::get_AvgWeekChange() const {
            return _avgweekchg;
        }

        double CSecurityTechnicals::get_AvgWeekVolume() const {
            return _avgweekvol;
        }

        double CSecurityTechnicals::get_WeekVolume() const {
            return _weekvol;
        }

        double CSecurityTechnicals::get_MonthHigh() const {
            return _mhigh;
        }

        double CSecurityTechnicals::get_MonthLow() const {
            return _mlow;
        }

        double CSecurityTechnicals::get_MonthChange() const {
            return _mchg;
        }

        double CSecurityTechnicals::get_AvgMonthChange() const {
            return _avgmchg;
        }

        double CSecurityTechnicals::get_AvgMonthVolume() const {
            return _avgmvol;
        }

        double CSecurityTechnicals::get_MonthVolume() const {
            return _mvol;
        }

        double CSecurityTechnicals::get_YearHigh() const {
            return _yhigh;
        }

        double CSecurityTechnicals::get_YearLow() const {
            return _ylow;
        }

        double CSecurityTechnicals::get_YearChange() const {
            return _ychg;
        }

        double CSecurityTechnicals::get_AvgYearChange() const {
            return _avgychg;
        }

        double CSecurityTechnicals::get_AvgYearVolume() const {
            return _avgyvol;
        }

        double CSecurityTechnicals::get_YTDChange() const {
            return _ytdchg;
        }

        double CSecurityTechnicals::get_RSI14() const {
            return _rsi14;
        }

        double CSecurityTechnicals::get_STO9() const {
            return _st09;
        }

        double CSecurityTechnicals::get_WPR14() const {
            return _wpr14;
        }

        double CSecurityTechnicals::get_MTM14() const {
            return _mtm14;
        }

        double CSecurityTechnicals::get_ROC14() const {
            return _roc14;
        }

        signed long CSecurityTechnicals::get_PTC() const {
            return _ptc;
        }

        double CSecurityTechnicals::get_SAR() const {
            return _sar;
        }

        double CSecurityTechnicals::get_Volatility() const {
            return _volatility;
        }

        double CSecurityTechnicals::get_Liquidity() const {
            return _liquidity;
        }


        ///////////////////////////symbolchange////////////////////////////
        long unsigned int CSymbolChange::Init(EODSYMBOLCHANGE* chg) 
        {
            _oldsym = *chg->OldSymbol;
            _newsym = *chg->NewSymbol;
            _oldexch = *chg->ExchangeCode;
            _newexch = *chg->NewExchangeCode;
            return S_OK;
        }

        
        const char* CSymbolChange::get_OldSymbol() {
            return _oldsym.c_str();
        }

        const char* CSymbolChange::get_NewSymbol() {
            return _newsym.c_str();
        }

        const char* CSymbolChange::get_OldExchangeCode() {
            return _oldexch.c_str();
        }

        const char* CSymbolChange::get_NewExchangeCode() {
            return _newexch.c_str();
        }

        long unsigned int CSymbolChangeList::Init(EODSYMBOLCHANGELIST * const list) 
        {
            return CEnumerable<ISymbolChange,CSymbolChange,ISymbolChangeList,EODSYMBOLCHANGELIST>::Init(list);
        }
        
        
        ///////////////////////////SymbolList///////////////////////////////

        long unsigned int CSymbol::Init(EODSYMBOL* p) 
        {
            set_Symbol(p->Code->c_str());
            //set_Name((p->Name->c_str()));
            return S_OK;
        }
        
        SE_CODE CSymbolList::Init(EODSYMBOLLIST* const list)
        {
            return CEnumerable<se::securities::ISecurityDescription,CSymbol,ISymbolList,EODSYMBOLLIST>::Init(list);
        }
        
        

        
        //////////////////////////SecurityDetails//////////////////////////////////

#define SDLGR "SecurityDetails"
        
        long unsigned int CSecurityDetails::Init(IWSClientSession* psession, se::securities::ISecurityDescription* pdesc) 
        {
            _psession = psession;
            _pdesc = pdesc;
            return S_OK;
        }

        RP<se::securities::ISecurityDescription> CSecurityDetails::get_Desc() 
        {
            return _pdesc;
        }

        RP<ISecurityFundamentals> CSecurityDetails::get_Fundamentals() 
        {
            throw seexception(E_NotImpl);
        }

        RP<ISecurityTechnicals> CSecurityDetails::get_Technicals() 
        {
            throw seexception(E_NotImpl);
        }

        RP<ISecurityQuote> CSecurityDetails::get_Quote() 
        {
            return _pquote;
        }

        void CSecurityDetails::set_Quote(ISecurityQuote* pq) 
        {
            _pquote = pq;
        }


        RP<INewsList> CSecurityDetails::get_News() 
        {
            SP<INewsList> pnews;
            auto ret = _psession->GetNews(_pdesc->get_Symbol(), _pdesc->get_ExchCode(), &pnews);
            if (FAILED(ret))
            {
                BERR << "Failed to get the news instance for the symbol " << _pdesc->get_Symbol() << ":" << _pdesc->get_ExchCode();
                seexception ex(ret);
                ex << "Failed to get the news instance for the symbol " << _pdesc->get_Symbol() << ":" << _pdesc->get_ExchCode();
                throw ex;
            }
            return pnews;
        }
    }
}