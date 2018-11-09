/* 
 * File:   CXbrlElement.h
 * Author: santony
 *
 * Created on December 10, 2012, 10:53 PM
 */

#ifndef CXBRLELEMENT_H
#define	CXBRLELEMENT_H

#include "../seglobaltypedefs.h"
#include "sexbrlinternal.h"
#include <string>
#include <unordered_map>
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/parsers.hpp>
#include <tuple>
#include "../seglobal/CGlobalStringCache.h"

using namespace std;
using namespace boost;
using namespace boost::gregorian;

namespace se
{
    namespace xbrl
    {
    
        typedef enum XBRL_BALANCE
        {
            XBRL_BAL_None,
            XBRL_BAL_Debit,
            XBRL_BAL_Credit
        } BALANCE;

        /* Various known xbrl element types. One of the following official items.
            ('dei:centralIndexKeyItemType'),
            ('dei:countryItemType'),
            ('dei:currencyItemType'),
            ('dei:filerCategoryItemType'),
            ('dei:fiscalPeriodItemType'),
            ('dei:nineDigitItemType'),
            ('dei:submissionTypeItemType'),
            ('dei:yesNoItemType'),
            ('invest:dateStringItemType'),
            ('invest:foreignCurrencyContractTransactionItemType'),
            ('nonnum:domainItemType'),
            ('nonnum:textBlockItemType'),
            ('num:areaItemType'),
            ('num:energyItemType'),
            ('num:massItemType'),
            ('num:percentItemType'),
            ('num:perShareItemType'),
            ('num:volumeItemType'),
            ('us-types:boeItemType'),
            ('us-types:dateStringItemType'),
            ('us-types:durationStringItemType'),
            ('us-types:FederalHomeLoanBankAdvancesGeneralDebtObligationsDisclosuresInterestRateTypeItemType'),
            ('us-types:FederalHomeLoanBankAdvancesStateFHLBBank-InterestRateTypeItemType'),
            ('us-types:fundedStatusItemType'),
            ('us-types:fundingImprovementAndRehabilitationPlanItemType'),
            ('us-types:investmentOnLoanForShortSalesItemType'),
            ('us-types:investmentPledgedItemType'),
            ('us-types:MalpracticeInsurance-OccurrenceOrClaims-madeItemType'),
            ('us-types:MalpracticeInsurance-RetrospectivelyRatedItemType'),
            ('us-types:perUnitItemType');
         */

        typedef enum XBRL_ELEMENT_TYPE
        {
            XBRL_ELM_None,
            XBRL_ELM_MonetaryItem,
            XBRL_ELM_SharesItem

        } ELEMENTTYPE;

        class CXbrlContext
        {
            boost::gregorian::date _dtStart ;
            boost::gregorian::date _dtEnd;

        public:
            CXbrlContext()
            {

            }

            CXbrlContext(CSTR startDate, CSTR endDate)
            {
                _dtStart = from_simple_string(startDate);
                _dtEnd = from_simple_string(endDate);
            }

            CXbrlContext(const CXbrlContext& other)
            {
                _dtStart = other._dtStart;
                _dtEnd = other._dtEnd;
            }    

            CXbrlContext(CXbrlContext&& other)
            {
                _dtStart = std::move(other._dtStart);
                _dtEnd = std::move(other._dtEnd);
            }    

            bool operator==(const CXbrlContext& other)
            {
                return _dtStart == other._dtStart && 
                        _dtEnd == other._dtEnd;
            }

            bool operator!=(const CXbrlContext& other)
            {
                return !operator==(other);
            }

            bool operator > (const CXbrlContext& other)
            {
                return _dtEnd >  other._dtEnd;
            }

            bool operator < (const CXbrlContext& other)
            {
                return _dtEnd < other._dtEnd;
            }


            CXbrlContext& operator=(const CXbrlContext& right)
            {
                if (this != &right)
                {
                    _dtStart = right._dtStart;
                    _dtEnd = right._dtEnd;
                }
                return *this;
            }

            CXbrlContext& operator=(CXbrlContext&& right)
            {
                if (this != &right)
                {
                    _dtStart = std::move(right._dtStart);
                    _dtEnd = std::move(right._dtEnd);
                }
                return *this;
            }

            CSTR get_StartDate() const
            {
                return to_simple_string(_dtStart).c_str();
            }

            void set_StartDate(CSTR startDate)
            {
                _dtStart = from_simple_string(startDate);
            }

            CSTR get_EndDate()  const
            {
                return to_simple_string(_dtEnd).c_str();
            }

            void set_EndDate(CSTR endDate)
            {
                _dtEnd = from_simple_string(endDate);
            }

        };

        inline bool operator< (const CXbrlContext & s1, const CXbrlContext & s2)
        {
            return  s1.get_EndDate() < s2.get_EndDate();
        }

        class CXbrlElementBase
        {
        protected:
            string _sPfx;
            string _sName;
            string _sLabel;
            BALANCE _balance;
            ELEMENTTYPE _type;
            bool _bIsAbstract;
            string _sRoleName;

            I8 _Weight ;
            R4 _fOrder ;
            string _sUse;
            I8 _priority;
            string& _roleName;  // this stored in global string cache



        protected:
            CXbrlElementBase(CSTR pfx, CSTR name, CSTR label, BALANCE balance, bool IsAbstract) : 
                _Weight(0), _fOrder(0.0), _sUse(""), _priority(0), _type(XBRL_ELM_None), _roleName(const_cast<string&>(CGlobalStringCache::Get()[""]))
            {
                _sPfx = pfx;
                _sName = name;
                _sLabel = label;
                _balance = balance;
                _bIsAbstract = IsAbstract;
            }

            CXbrlElementBase(const CXbrlElementBase& orig) :_roleName(const_cast<string&>(CGlobalStringCache::Get()[""]))
            {
                assign(orig);
            }

            // move constructor
            CXbrlElementBase(CXbrlElementBase&& right):_roleName(const_cast<string&>(CGlobalStringCache::Get()[""]))
            {
                move(right);
            }

            virtual CXbrlElementBase& operator=(const CXbrlElementBase& other)
            {
                if (this != &other)
                {
                    assign(other);
                }
                return *this;
            }

            virtual CXbrlElementBase& operator=(CXbrlElementBase&& other)
            {
                if (this != &other)
                    move(other);            
                return *this;
            }

        private:
            
            void assign(const CXbrlElementBase& other)
            {
                _sPfx = other._sPfx;
                _sName = other._sName;
                _sLabel = other._sLabel;
                _balance = other._balance;
                _type = other._type;
                _bIsAbstract = other._bIsAbstract;
                _Weight = other._Weight;
                _fOrder = other._fOrder;
                _sUse = other._sUse;
                _priority = other._priority;
                _roleName = other._roleName;
            }
            
            void move(CXbrlElementBase& other)
            {
                _sPfx = std::move(other._sPfx);
                _sName = std::move(other._sName);
                _sLabel = std::move(other._sLabel);
                _balance = other._balance;
                _type = other._type;
                _bIsAbstract = other._bIsAbstract;
                _Weight = other._Weight;
                _fOrder = other._fOrder;
                _sUse = std::move(other._sUse);
                _priority = other._priority;
                _roleName = other._roleName;
            }

        public:            

            virtual ~CXbrlElementBase()
            {

            }


            CSTR get_Prefix()
            {
                return _sPfx.c_str();
            }

            CSTR get_Name()
            {
                return _sName.c_str();
            }

            CSTR get_Label()
            {
                return _sLabel.c_str();
            }

            BALANCE get_Balance()
            {
                return _balance;
            }

            ELEMENTTYPE get_Type()
            {
                return _type;
            }

            bool get_IsAbstract()
            {
                return _bIsAbstract;
            }

            I8 get_Weight()
            {
                return _Weight;
            }

            R4 get_Order()
            {
                return _fOrder;
            }

            CSTR get_Use()
            {
                return _sUse.c_str();
            }

            I8 get_Priority()
            {
                return _priority;
            }

            void set_Weight(I8 value)
            {
                _Weight = value;
            }

            void set_Order(R4 value)
            {
                _fOrder = value;
            }

            void set_User(CSTR value)
            {
                _sUse = value;
            }

            void set_Priority(I8 value)
            {
                _priority = value;
            }
            
            CSTR get_RoleName() 
            {
                return _roleName.c_str();
            }
            
            void set_RoleName(CSTR rname)
            {
                _roleName = CGlobalStringCache::Get()[rname];
            }
        };

        template<typename TValue>
        class CXbrlElement : public CXbrlElementBase
        {
            typedef map<CXbrlContext, TValue> ContextValue;

        protected:
            ContextValue _value;

        protected:
            CXbrlElement(CSTR pfx, CSTR name, CSTR label, BALANCE balance, bool IsAbstract) :
                CXbrlElementBase(pfx, name, label, balance, IsAbstract)
            {
            }

            CXbrlElement(const CXbrlElement& orig) : CXbrlElementBase(orig)
            {
                _value = orig._value;
            }

            CXbrlElement(CXbrlElement&& orig) : CXbrlElementBase(orig)
            {
                _value = orig._value;
            }

        public:

            CXbrlElement& operator=(const CXbrlElement& other)
            {
                if (this != &other)
                {
                    _value = other._value;
                    CXbrlElementBase::operator =(other);
                }
                return *this;
            }

            CXbrlElement& operator=(CXbrlElement&& other)
            {
                if (this != &other)
                {
                    *this = move(other);
                    _value = other._value;
                }
                return *this;
            }    

            virtual ~CXbrlElement()
            {

            }


            TValue get_Value(const CXbrlContext& context)
            {
                auto find = _value.find(context);
                if (find != _value.end())
                {
                    return find.second;
                }
                return TValue();
            }

            void set_Value(CXbrlContext context, TValue value)
            {
                auto find = _value.find(context);
                if (find != _value.end())
                {
                    _value[context] = value;
                    return;
                }
                _value.insert(make_pair(context, value));
            }

            SIZE get_ContextCount() const
            {
                return _value.size();
            }

            typedef std::tuple<typename ContextValue::const_iterator, typename ContextValue::const_iterator> ContextTie;

            ContextTie get_Contexts()
            {
                return ContextTie(_value.begin(), _value.end());
            }


        private:

        };

        class CXbrlRootElement : public CXbrlElement<CSTR>
        {
        private:
            STATEMENTTYPE _statementType;
            
        public:
              CXbrlRootElement(CSTR rootName) : 
                CXbrlElement("root", rootName, rootName, XBRL_BAL_None, false)
              {
                  _type = XBRL_ELM_None;
              }
              
              CXbrlRootElement(CSTR rootName, STATEMENTTYPE statementType) : 
                CXbrlElement("root", rootName, rootName, XBRL_BAL_None, false)
              {
                  _type = XBRL_ELM_None;
                  _statementType = statementType;
              }

                
              CXbrlRootElement(const CXbrlRootElement& orig) : CXbrlElement(orig)
              {
              }

              CXbrlRootElement(CXbrlRootElement&& orig) : CXbrlElement(orig)
              {
              }

              CXbrlRootElement& operator=(const CXbrlRootElement& other)
              {
                  if (this != &other)
                  {
                      _value = other._value;
                      CXbrlElement<CSTR>::operator =(other);
                  }
                  return *this;
              }

              CXbrlRootElement operator=(CXbrlRootElement&& other)
              {
                  return std::move(other);
              }

              virtual CXbrlElementBase& operator=(const CXbrlElementBase& other)
              {
                  if (this != &other)
                  {
                      *this = dynamic_cast<const CXbrlRootElement&>(other);
                  }
                  return *this;
              }

              virtual CXbrlElementBase& operator=(CXbrlElementBase&& other)
              {
                  if (this != &other)
                  {
                        *this = std::move(dynamic_cast<CXbrlRootElement&>(other));
                  }
                  return *this;
              }
              
              // Returns the statement type (cashflow, income, etc)
              STATEMENTTYPE get_StatementType() const
              {
                  return _statementType;
              }
              
              void set_StatementType(STATEMENTTYPE st)
              {
                  _statementType = st;
              }
             

        };

        class CXbrlMonetaryElement : public CXbrlElement<R8>
        {
        public:
              CXbrlMonetaryElement(CSTR pfx, CSTR name, CSTR label, BALANCE balance, bool IsAbstract) : 
                CXbrlElement(pfx, name, label, balance, IsAbstract)
              {
                  _type = XBRL_ELM_MonetaryItem;
              }

              CXbrlMonetaryElement(const CXbrlMonetaryElement& orig) : CXbrlElement(orig)
              {
              }

              CXbrlMonetaryElement(CXbrlMonetaryElement&& orig) : CXbrlElement(orig)
              {
              }

              CXbrlMonetaryElement& operator=(const CXbrlMonetaryElement& other)
              {
                  if (this != &other)
                  {
                      _value = other._value;
                      CXbrlElement<R8>::operator =(other);
                  }
                  return *this;
              }

              CXbrlMonetaryElement operator=(CXbrlMonetaryElement&& other)
              {
                  return std::move(other);
              }

              virtual CXbrlElementBase& operator=(const CXbrlElementBase& other)
              {
                  if (this != &other)
                  {
                      *this = dynamic_cast<const CXbrlMonetaryElement&>(other);
                  }
                  return *this;
              }

              virtual CXbrlElementBase& operator=(CXbrlElementBase&& other)
              {
                  if (this != &other)
                  {
                        *this = std::move(dynamic_cast<CXbrlMonetaryElement&>(other));
                  }
                  return *this;
              }

        };

        class CXbrlUnknownElement : public CXbrlElement<CSTR>
        {
        public:
              CXbrlUnknownElement(CSTR pfx, CSTR name, CSTR label, BALANCE balance, bool IsAbstract) : 
                CXbrlElement(pfx, name, label, balance, IsAbstract)
              {
                  _type = XBRL_ELM_None;
              }

              CXbrlUnknownElement(const CXbrlUnknownElement& orig) : CXbrlElement(orig)
              {
              }

              CXbrlUnknownElement(CXbrlUnknownElement&& orig) : CXbrlElement(orig)
              {
              }
        };
        
        
        /* Utility functions */
        
        extern "C" STATEMENTTYPE ParseStatementType(const string& statementDesc);

        // intermediate storage of arc data.
        struct ArcNodeData
        {
            string name;
            I8 weight;
            R4 order;
            I8 priority;
            string use;
        };
        
        // comparer
        static bool operator<(const ArcNodeData& left, const ArcNodeData& right)
        {
            return left.name < right.name;
        }
        // comparer
        static bool operator>(const ArcNodeData& left, const ArcNodeData& right)
        {
            return left.name > right.name;
        }
        
    }
}



#endif	/* CXBRLELEMENT_H */

