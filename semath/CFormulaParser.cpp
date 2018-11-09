// Copyright (c) 2008 Dr. Colin Hirsch
// Please see license.txt for license.

#include "semathapi.h"
#include "CFormulaParser.h"
#include <boost/tokenizer.hpp>
#include <pegtl.hh>
#include <cmath>
#include <complex>

namespace se
{
    namespace math
    {

        //boost::char_separator<char> sep("|");
        //typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
        
        // The first program used during development and debugging
        // of the library that uses actions. It evaluates each command
        // line argument as arithmetic expression consisting of
        // - integers with optional sign,
        // - the four basic arithmetic operations,
        // - grouping brackets.
        // For example input "3 * ( -7 + 9)" yields result 6.

        using namespace pegtl;

        // The state.
        template<typename T>
        class statefulvector : public std::vector<T>
        {
         private:
            void* _state;

         public:
             statefulvector() : _state(nullptr)
             {

             }
            statefulvector(void* state) : _state(state)
            {
            }
            void* getstate(){return _state;}
        };

        // Canonical use of an evaluation stack, here implemented with a std::vector.

        typedef double value_type;
        typedef statefulvector<value_type> stack_type;
        
        
        template<typename _Tp>
        struct loge //: public std::unary_function<_Tp, _Tp>
        {
          _Tp operator()(const _Tp& x) const
          { 
              return std::log(x);
          }
        };

        template<typename _Tp>
        struct abs //: public std::unary_function<_Tp, _Tp>
        {
          _Tp operator()(const _Tp& x) const
          { 
              return std::abs(x);
          }
        };
        

        // Helper function that is a "value returning pop() operation" that is not
        // in general exception safe, but fine here since the elements are a POD.

        value_type pull( stack_type & s )
        {
           assert( ! s.empty() );
           value_type nrv( s.back() );
           s.pop_back();
           return nrv;
        }

        // The actions.

        // This action converts the matched sub-string to an integer and pushes it on
        // the stack, which must be its only additional state argument.

        // Deriving from action_base<> is necessary since version 0.26; the base class
        // takes care of the pretty-printing for diagnostic messages; this is necessary
        // for all action classes (that do not derive from a rule class).
        
        typedef std::vector<UID> Xbrlidlist;

        struct push_action
              : action_base< push_action >
        {
           static void apply( const std::string & m, stack_type & s )
           {
                //s.push_back( string_to_signed< value_type >( m ) );
               s.push_back(strtod(m.c_str(), nullptr));
           }
        };
        
        /*
        struct push_function_action
              : action_base< push_function_action >
        {
           static void apply( const std::string & m, stack_type & s )
           {
                //s.push_back( string_to_signed< value_type >( m ) );
               //s.push_back(strtod(m.c_str(), nullptr));
               
               s.push_back(1.0);
           }
        };
         */
        
        // Class op_action performs an operation on the two top-most elements of
        // the evaluation stack. This should always be possible in the sense that
        // the grammar must make sure to only apply this action when sufficiently
        // many operands are on the stack.

        template< typename Operation >
        struct op_action
              : action_base< op_action< Operation > >
        {
           static void apply( const std::string &, stack_type & s )
           {
              const value_type rhs = pull( s );
              const value_type lhs = pull( s );
              s.push_back( Operation()( lhs, rhs ) );
           }
        };

        template< typename Operation >
        struct func_action
              : action_base< func_action< Operation > >
        {
           static void apply( const std::string & d, stack_type & s )
           {
              value_type dd = strtod(d.c_str(), nullptr);
              s.push_back( Operation()( dd ) );
           }
        };
        
        // Specialisation for division that checks for division by zero.

        template<>
        struct op_action< std::divides< value_type > >
              : action_base< op_action< std::divides< value_type > > >
        {
           template< typename State >
           static void apply( const std::string &, State & s )
           {
              const value_type rhs = pull( s );
              if ( ! rhs ) {
                 PEGTL_THROW( "pegtl: division by zero" );
              }
              const value_type lhs = pull( s );
              s.push_back( std::divides< value_type >()( lhs, rhs ) );
           }
        };


//        struct xbrldigits
//              : sor< digit, one<'|'> >
//        {
//           template< typename Print >
//           static void prepare( Print & st )
//           {
//              st.template update< alpha >( "xbrldigits", true );
//           }
//        };

        struct ddigit
              : seq< sor< one<'.'>, digit>, opt< one<'.'> > >
        {
           template< typename Print >
           static void prepare( Print & st )
           {
              st.template update< alpha >( "ddigit", true );
           }
        };


        // Apart for the actions this grammar is identical to the one in parsetree.cc.
        // There, a syntax or expression tree is generated, here the expressions are
        // evaluated on the fly.

        struct read_number
              : seq< opt< one< '+', '-' > >, plus< ddigit > > {};

        /*
        struct read_function
              : seq< plus<alnum>, one<'('>, plus<ddigit>, one<')'> > {};
        struct push_function_rule
              : pad< ifapply< read_function, push_function_action >, space > {};
        */

        struct read_expr;
        
        struct read_loge 
              : seq< 
                    string<'l','n'>, 
                    one<'('>, 
                    ifapply< plus<ddigit>, func_action< loge<value_type> > >, 
                    one<')'> 
                > {};

        struct read_abs 
              : seq< 
                    string<'a','b','s'>, 
                    one<'('>, 
                    ifapply< plus<ddigit>, func_action< abs<value_type> > >, 
                    one<')'>
                > {};
              
        // This rule uses the rule read_number to match a number in the input
        // and, on success, applies the push_action to the matched sub-string
        // and the state. Here in calculator.cc the state is an instance of
        // stack_type, the evaluation stack on which the number is pushed.

        struct push_rule
              : pad< ifapply< read_number, push_action >, space > {};

        template< int C >
        struct calc_pad
              : pad_one< C, space > {};

        struct read_open
              : calc_pad< '(' > {};

        struct read_close
              : calc_pad< ')' > {};

        struct math_functions : sor<read_loge, read_abs>{};

        struct read_atom
              : sor< math_functions, push_rule, seq< read_open, read_expr, read_close > > 
        {
        };
        
        
        // The operation rules first read two sub-expressions, and then combine their
        // respective results with an arithmetic operation, replacing the two top-most
        // stack elements with the result -- just as every simple stack machine does.

        template< int P, typename O, typename A >
        struct read_op
              : ifapply< ifmust< calc_pad< P >, O >, op_action< A > > {};

        struct read_mul
              : read_op< '*', read_atom, std::multiplies< value_type > > {};

        struct read_div
              : read_op< '/', read_atom, std::divides< value_type > > {};

        struct read_prod
              : seq< read_atom, star< sor< read_mul, read_div > > > {};

        struct read_add
              : read_op< '+', read_prod, std::plus< value_type > > {};

        struct read_sub
              : read_op< '-', read_prod, std::minus< value_type > > {};

        struct read_expr_including_math
              : sor< math_functions, seq< read_prod, star< sor< read_add, read_sub > > > >{};

        struct read_expr
              : seq< read_prod, star< sor< read_add, read_sub > > > {};

        struct read_calc
              : seq< read_expr, space_until_eof > {};
              
        //////////////////////CFormulaParser//////////////////////////////////

        double CFormulaParser::Run(CSTR formula) 
        {
            stack_type stack;
            pegtl::basic_parse_string< read_calc >( formula , stack);
            //pegtl::trace_parse_string<read_calc>(true, formula, stack);
            return stack.front();
        }
    }
}