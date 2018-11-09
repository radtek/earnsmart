// Copyright (c) 2008 Dr. Colin Hirsch
// Please see license.txt for license.

//#include <log4cxx/propertyconfigurator.h>

#include "CFormulaParser.h"
#include "CXbrlCalculator.h"
#include "sexbrlinternal.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "CXbrlAccountMatcher.h"
#include <boost/algorithm/string.hpp>

namespace se
{
    namespace xbrl
    {
        


        boost::char_separator<char> sep1("|");
        boost::char_separator<char> sep2("&");
        typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
        
        void push_action::apply(const std::string& m, stack_type& s)
        {
            s.push_back(strtod(m.c_str(), nullptr));
        }
        
        value_type pull(stack_type& s)
        {
           assert( ! s.empty() );
           value_type nrv( s.back() );
           s.pop_back();
           return nrv;
        }
        
        void push_xbrlid_action::apply(const std::string& m, stack_type& s)
        {
           //s.push_back( string_to_signed< value_type >( m ) );
            Xbrlidlist xbrlids;
            tokenizexbrlids(m, xbrlids);
            // call method for each xbrl id until you can get a value.
            R8 val = ER8;
            for (std::string xbrlid : xbrlids)
            {
                Tokenizer tok(xbrlid, sep2);
                std::vector<std::string> xbrlids;
                xbrlids.assign(tok.begin(), tok.end());
                if (xbrlids.size()>1)
                {
                    replace_all(xbrlids[0], "x", "");
                    replace_all(xbrlids[1], "x", "");
                    val = getxbrlvalue(strtol(xbrlids[0].c_str(), nullptr, 10), strtol(xbrlids[1].c_str(), nullptr, 10), s);
                }
                else
                {
                    replace_all(xbrlids[0], "x", "");
                    val = getxbrlvalue(strtol(xbrlids[0].c_str(), nullptr, 10), 0, s);
                }
                if (val != ER8)
                    break;
            }
            s.push_back( val );
        }
        
        

        void push_xbrlid_action::tokenizexbrlids(std::string m, Xbrlidlist& tokenfill) 
        {
            //long v = strtol( m.c_str(), 0, 10);
            Tokenizer tok(m, sep1);
            for (const std::string& member : tok)
            {
                tokenfill.push_back(member);
            }
        }

        
        value_type push_xbrlid_action::getxbrlvalue(UID xbrlid, UID xbrlsegid, stack_type& s)
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.formulaparser.getxbrlvalue");
            
            IFinancialsConsolidated* pcon = 
                    static_cast<IFinancialsConsolidated*>(s.getstate());
            
            I32 year = pcon->get_Property("Year");
            I32 quarter = pcon->get_Property("Quarter");
            
            SP<IXbrlRecord> pRec;
            SE_CODE ret = pcon->get_Record(xbrlid, xbrlsegid, &pRec);
            R8 dVal = ER8;
            if (FAILED(ret))
            {
               // LOGE(_logger, "Failed to get financial record from consolidated finance instance for xbrl_id = " << xbrlid << " and cik = " << pcon->get_Cik());
            }
            else
            {
                SP<IXbrlField> pfld;
                ret = pRec->get_Financials( year , &pfld);
                if (FAILED(ret))
                {
                   // LOGE(_logger, "Failed to get financial record field from consolidated finance instance for xbrl_id = " << xbrlid << " and cik = " << pcon->get_Cik());
                }
                else
                {
                    OmniType r;
                    if (!quarter)
                    {
                        r = pfld->get_FY();
                    }
                    else
                    {
                        switch (quarter)
                        {
                            case 1:
                                r = pfld->get_Q1();
                                break;
                            case 2:
                                r = pfld->get_Q2();
                                break;
                            case 3:
                                r = pfld->get_Q3();
                                break;
                            case 4:
                                r = pfld->get_Q4();
                                break;
                        }
                    }
                    if (r.dt == DT_I64)
                        dVal = (R8)(I64)r;
                    else if (r.dt == DT_R8)
                        dVal = r;
                }
            }
            return dVal;
        }
        
        ///////////////// matchaccountaction/////////////////////////////

        void push_matchaccount_action::apply(const std::string& m, stack_type& s) 
        {
            if (m.empty())
            {
                PEGTL_THROW("pegl: Match account number is empty/missing");
            }
            
            UID mid = strtol(m.substr(1).c_str(), nullptr, 10);
            
            // delegate to account matching utility.
            IFinancialsConsolidated* pcon = 
                    static_cast<IFinancialsConsolidated*>(s.getstate());
            I32 year = pcon->get_Property("Year");
            I32 quarter = pcon->get_Property("Quarter");            
            
#ifdef __DEBUG__
            if (year == 2013)
            {
                std::string b = "B";
            }
#endif            
            se::xbrl::CXbrlAccountMatcher matcher((ISession*)s.getsession(),  pcon, mid );
            OmniType accountVal = matcher.GetAccountValue(year, quarter);
            // we need the value to converted  to R8
            R8 pushValue;
            if (accountVal.IsEmpty())
            {
                //PEGTL_THROW("pegl: Match account value returned for the match is invalid.");
                pushValue = ER8;
            }
            else
            {
                if (accountVal.dt == DT_R8)
                    pushValue = accountVal;
                else if (accountVal.dt == DT_I64)
                    pushValue = (R8)(I64)accountVal;
                else if (accountVal.dt == DT_CSTR)
                {
                    CSTR str = accountVal;
                    pushValue = strtod(str, nullptr);
                }
            }            
            s.push_back( pushValue );
        }
        
        double Calculate(ISession* ps, IFinancialsConsolidated* psrc, const char* mathstring)
        {
            ////LoggerPtr _logger = Logger::getLogger("se.xbrl.formulaparser.calculate");
            try
            {
                
                // tokenize formula
                std::string mstr = mathstring;
                Tokenizer tok(mstr, sep1);
                for (std::string formula : tok)
                {
                    boost::trim(formula);
                   // LOGI(_logger, "Parsing formula " << formula);
#ifdef __DEBUG__
                
                    if (!strcmp(formula.c_str(), "m1"))
                    {
                        std::string b = "B";
                    }
#endif
                    
                    try
                    {
                        stack_type stack(psrc, ps);
#ifdef __DEBUG__
                        //pegtl::trace_parse_string< read_calc >(true, formula , stack);
                        pegtl::basic_parse_string< read_calc >( formula , stack);
#else
                        pegtl::basic_parse_string< read_calc >( formula , stack);
#endif
                        R8 result = stack.front();
                        if (result == ER8)
                            continue;
                        else
                            return result;
                    }
                    catch(std::exception & ex)
                    {
                       // LOGEP(ex.what());
                        continue;
                    }
                }
                return ER8;
            }
            catch(std::exception & ex)
            {
               // LOGEP(ex.what());
                return ER8;
            }
        }
    }
}