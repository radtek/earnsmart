/* 
 * File:   CEODDataWrappers.h
 * Author: santony
 *
 * Created on February 4, 2015, 6:31 PM
 */

#ifndef CEODDATAWRAPPERS_H
#define	CEODDATAWRAPPERS_H

#include "seeoddata.h"
#include "nsDataSoapProxy.h"
#include "../seglobal/SecurityDescription.h"

using namespace ns;

namespace se
{
    namespace eoddata
    {
     
        typedef ns1__EXCHANGE EODEXCHANGE;
        typedef std::vector<ns1__EXCHANGE * > EODEXCHANGELIST;
        typedef ns1__NEWS EODNEWS;
        typedef std::vector<class ns1__NEWS * > EODNEWSLIST;
        typedef std::vector<class ns1__FUNDAMENTAL * > EODFUNDAMENTALLIST;
        typedef ns1__FUNDAMENTAL EODFUNDAMENTALS;
        typedef std::vector<class ns1__TECHNICAL * > EODTECHNICALLIST;
        typedef class ns1__TECHNICAL EODTECHNICAL;
        typedef ns1__QUOTE EODQUOTE;
        typedef std::vector<class ns1__SYMBOLCHANGE * > EODSYMBOLCHANGELIST;
        typedef ns1__SYMBOLCHANGE EODSYMBOLCHANGE;
        typedef std::vector<class ns1__QUOTE* > EODSECURITYQUOTELIST;
        typedef ns1__SYMBOL EODSYMBOL;
        typedef std::vector<class ns1__SYMBOL * > EODSYMBOLLIST;
        
        template <typename TItem, typename TCItem, typename TList, typename TOrigList>
        class CEnumerator : public CObjectRoot, public IEnumerator1<TItem>
        {
        public:
            SP<TList> _pl;
            TOrigList* _pIntList;
            typedef typename TOrigList::iterator Iter;
            Iter _start;
            SP<TItem> _item;
        public:
            CEnumerator(ISession* ps):CObjectRoot(ps)
            {
                
            }
            

            virtual long unsigned int Init(TList* pl)
            {
                _pl = pl;
                _pIntList = _pl->getInternalList();
                Reset();
            }


            virtual TItem* get_Current()
            {
                return _item._ptr;
            }
            

            virtual bool MoveNext()
            {
                if (_start == _pIntList->end())
                    return false;
                CObject<TCItem>::Create(get_Session(), *_start, &_item);
                ++_start;
                return true;
            }


            virtual void Reset()
            {
                _start = _pIntList->begin();
            }
        };
        
        template <typename TItem, typename TCItem, typename TIList, typename TOrigList>
        class CEnumerable : public CObjectRoot, public TIList
        {
            friend class CEnumerator
                <
                    TItem, 
                    TCItem, 
                    CEnumerable<TItem, TCItem, TIList, TOrigList>, 
                    TOrigList
                >;
            
        private:
            
            TOrigList* _intlist;
            
        protected:
            TOrigList* getInternalList()
            {
                return _intlist;
            }
            
        public:
            
            CEnumerable(ISession* ps):CObjectRoot(ps){}
            
            virtual long unsigned int Init(TOrigList* const origList)
            {
                _intlist = origList;
                return S_OK;
            }

            virtual SE_CODE get_Enumerator(IEnumerator1<TItem>** outVal)
            {
                return CObject< 
                        CEnumerator< 
                            TItem,
                            TCItem,
                            CEnumerable<TItem, TCItem, TIList, TOrigList> ,
                        TOrigList
                        > 
                    >::Create(get_Session(), this, outVal);
            }
        };
        
        ////////////////////////////////Exchange////////////////////////////////////////
        class CExchange;
        class CNews;
        
        class CExchangeList : public CEnumerable<IExchange, CExchange, IExchangeList, EODEXCHANGELIST >
        {
        public:
            CExchangeList(ISession*);
            virtual SE_CODE Init(EODEXCHANGELIST* const eodData);
            virtual SE_CODE Find( std::function< bool(const IExchange&) > func, IExchange* pOut);
            

            virtual long unsigned int get_Size();

        };
        
        
        class CExchange : public CObjectRoot, public IExchange
        {
            
        public:
            
            CExchange(ISession* ps):CObjectRoot(ps){}
            
            SE_CODE Init(EODEXCHANGE* const );
            virtual CSTR get_Name();
            virtual CSTR get_Code() ;
            virtual CSTR get_Country();
            virtual CSTR get_TimeZone();
            
            virtual void set_Name(CSTR){}
            
            virtual void Serialize(ISerializedData<IString>* pIn){}
            

        private:
            string _sName;
            string _sCode;
            string _sCountry;
            string _sTZone;
        };
        
        
        ///////////////////////////News///////////////////////////
        class CNews : public CObjectRoot, public INews
        {
        public:
            
            CNews(ISession* ps):CObjectRoot(ps){}
            
            SE_CODE Init(EODNEWS*);
            
            virtual CSTR get_Symbol() ;
            virtual CSTR get_Exchange() ;
            virtual CSTR get_Source() ;
            virtual DATETIME& get_When() ;
            virtual CSTR get_Title() ;
            virtual CSTR get_Intro() ;
            virtual CSTR get_Story() ;
            virtual CSTR get_Caption() ;
            virtual I32 get_Views() ;
             
        private:
            string _sSym;
            string _sExch;
            string _sSource;
            DATETIME _dtWhen;
            string _sTitle;
            string _sIntro;
            string _sCaption;
            string _sStory;
            I32 _iViews = 0;
            
        };
        
        class CNewsList : public CEnumerable
            <
                INews,
                CNews,
                INewsList,
                EODNEWSLIST
            >
        {
        public:
            CNewsList(ISession* ps):CEnumerable(ps){}
            SE_CODE Init(EODNEWSLIST *const);
            
            
            
        };
        
        //////////////////////////// Fundamentals////////////////////////////
        class CSecurityFundamentals : public CObjectRoot, public ISecurityFundamentals
        {
        public:
            CSecurityFundamentals(ISession* ps):CObjectRoot(ps){}
            SE_CODE Init(EODFUNDAMENTALS*);
            

            virtual DATETIME& get_DateTime();

            virtual const char* get_Industry() const;

            virtual const char* get_Sector() const;

            virtual unsigned long get_NumOfShares() const;

            virtual unsigned long get_MarketCap() const;

            virtual double get_PricePerEarnings() const;

            virtual double get_EPS() const;

            virtual double get_NTA() const;

            virtual double get_DivYield() const;

            virtual double get_Dividend() const;

            virtual DATETIME& get_DividendDate() ;

            virtual double get_DividendPerShare() const;

            virtual double get_ImputationCredits() const;

            virtual double get_EBITDA() const;

            virtual double get_PEG() const;

            virtual double get_PtS() const;
            

            virtual double get_PtB() const;


            virtual double get_Yield() const;

        private:
            DATETIME _dt, _dtDivDate;
            string _industry, _sector;
            U64 _numOfShares, _marketCap;
            R8  _pe, 
                _eps,
                _nta,
                _divYld,
                _div,
                _divPShare,
                _impCredits,
                _ebitda,
                _peg,
                _pts,
                _ptb,
                _yld;
        };

        //////////////////////////////quote///////////////////////////////////
        class CSecurityQuote : public CObjectRoot, public ISecurityQuote
        {
        public:
            CSecurityQuote(ISession* ps):CObjectRoot(ps){}
            SE_CODE Init(EODQUOTE*);
            

            virtual DATETIME& get_DateTime();

            virtual double get_Open() const;

            virtual double get_High() const;

            virtual double get_Low() const;

            virtual double get_Close() const;

            virtual unsigned long get_Volume() const;

            virtual signed long get_Openinterest() const;

            virtual double get_Prev() const;

            virtual double get_Change() const;

            virtual double get_Bid() const;

            virtual double get_Ask() const;

            virtual double get_PreviousClose() const;

            virtual double get_NextOpen() const;

            virtual DATETIME& get_Modified();

        private:
            DATETIME _dt, _dtmod;
            R8  _open,
                _high,
                _low,
                _close,
                _prev,
                _change,
                _bid,
                _ask,
                _prevClose,
                _nextOpen;
            U64 _vol;
            I64 _openint;

        };
        
        /////////////////////// Symbol Quote List//////////////////////
        class CSecurityQuoteList : public CEnumerable<ISecurityQuote, CSecurityQuote, ISecurityQuoteList, EODSECURITYQUOTELIST>
        {
        public:
            CSecurityQuoteList(ISession* ps):CEnumerable<ISecurityQuote, CSecurityQuote, ISecurityQuoteList, EODSECURITYQUOTELIST>(ps){}
            SE_CODE Init(EODSECURITYQUOTELIST* const);
        };
        
        //////////////////////////Technicals/////////////////////////
        
        class CSecurityTechnicals : public CObjectRoot, public ISecurityTechnicals
        {
        public:
            CSecurityTechnicals(ISession* ps):CObjectRoot(ps){}
            SE_CODE Init(EODTECHNICAL*);
            

            virtual DATETIME& get_DateTime();

            virtual double get_Previous() const;

            virtual double get_Change() const;

            virtual signed int get_MA1() const;

            virtual signed int get_MA2() const;

            virtual signed int get_MA5() const;

            virtual signed int get_MA20() const;

            virtual signed int get_MA100() const;

            virtual signed int get_MA200() const;

            virtual double get_MAPercent() const;

            virtual double get_MAReturn() const;

            virtual signed long get_VolumeChange() const;

            virtual double get_ThreeMonthChange() const;

            virtual double get_SixMonthChange() const;

            virtual double get_WeekHigh() const;

            virtual double get_WeekLow() const;
            
            virtual double get_AvgWeekChange() const;

            virtual double get_AvgWeekVolume() const;

            virtual double get_WeekVolume() const;

            virtual double get_MonthHigh() const;

            virtual double get_MonthLow() const;

            virtual double get_MonthChange() const;

            virtual double get_AvgMonthChange() const;

            virtual double get_AvgMonthVolume() const;

            virtual double get_MonthVolume() const;

            virtual double get_YearHigh() const;

            virtual double get_YearLow() const;

            virtual double get_YearChange() const;

            virtual double get_AvgYearChange() const;

            virtual double get_AvgYearVolume() const;

            virtual double get_YTDChange() const;

            virtual double get_RSI14() const;

            virtual double get_STO9() const;

            virtual double get_WPR14() const;

            virtual double get_MTM14() const;

            virtual double get_ROC14() const;

            virtual I64 get_PTC() const;

            virtual double get_SAR() const;

            virtual double get_Volatility() const;

            virtual double get_Liquidity() const;

        private:
            DATETIME _dt;
            I32     _ma1, 
                    _ma2, 
                    _ma5, 
                    _ma20, 
                    _ma100, 
                    _ma200,
                    _volchg,
                    _ptc;
            
            R8      _prev,
                    _change,
                    _mapct,
                    _maret,
                    _3mchg,
                    _6mchg,
                    _weekhigh,
                    _weeklow,
                    _avgweekchg,
                    _avgweekvol,
                    _weekvol,
                    _mhigh,
                    _mlow,
                    _mchg,
                    _avgmchg,
                    _avgmvol,
                    _mvol,
                    _yhigh,
                    _ylow,
                    _ychg,
                    _avgychg,
                    _avgyvol,
                    _ytdchg,
                    _rsi14,
                    _st09,
                    _wpr14,
                    _mtm14,
                    _roc14,
                    _sar,
                    _volatility,
                    _liquidity;
            
        };

        
        /////////////////////////////Symbolchanges////////////////////////////////////
        
        class CSymbolChange : public CObjectRoot, public ISymbolChange
        {
        public:
            CSymbolChange(ISession* ps):CObjectRoot(ps){}
            SE_CODE Init(EODSYMBOLCHANGE*);
            

            virtual const char* get_OldSymbol();

            virtual const char* get_NewSymbol();

            virtual const char* get_OldExchangeCode();

            virtual const char* get_NewExchangeCode();
            
        private:
            string  _oldsym,
                    _newsym;
            string  _oldexch,
                    _newexch;

        };


        class CSymbolChangeList : public CEnumerable<
            ISymbolChange, 
                CSymbolChange, 
                ISymbolChangeList, 
                EODSYMBOLCHANGELIST >
        {
        public:
            CSymbolChangeList(ISession* ps):CEnumerable<
            ISymbolChange, 
                CSymbolChange, 
                ISymbolChangeList, 
                EODSYMBOLCHANGELIST >(ps){}
            virtual SE_CODE Init(EODSYMBOLCHANGELIST* const list);

        };
        
        class CSymbol : public CSecurityDescription
        {
        public:
            CSymbol(ISession* ps):CSecurityDescription(ps)
            {
            }
            virtual SE_CODE Init(EODSYMBOL*);
        };

        class CSymbolList : public CEnumerable<
            se::securities::ISecurityDescription, 
                CSymbol, 
                ISymbolList, 
                EODSYMBOLLIST >
        {
        public:
            CSymbolList(ISession* ps):CEnumerable<
            se::securities::ISecurityDescription, 
                CSymbol, 
                ISymbolList, 
                EODSYMBOLLIST >(ps){}
            virtual SE_CODE Init(EODSYMBOLLIST* const list);
        };

        //////////////////////////SecurityDetails////////////////////////////////
        class CSecurityDetails : public CObjectRoot, public ISecurityDetails
        {
        public:
            CSecurityDetails(ISession* ps):CObjectRoot(ps){}
            SE_CODE Init(IWSClientSession*, se::securities::ISecurityDescription*);

            virtual RP<se::securities::ISecurityDescription> get_Desc();

            virtual RP<ISecurityFundamentals> get_Fundamentals();

            virtual RP<ISecurityTechnicals> get_Technicals();

            virtual RP<ISecurityQuote> get_Quote();

            void set_Quote(ISecurityQuote*);
            
            virtual RP<INewsList> get_News();

        private:
            SP<IWSClientSession> _psession;
            SP<se::securities::ISecurityDescription> _pdesc;
            SP<ISecurityQuote> _pquote;
        };
    }
}

#endif	/* CEODDATAWRAPPERS_H */

