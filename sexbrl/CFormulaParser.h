/* 
 * File:   calculator.h
 * Author: santony
 *
 * Created on February 15, 2014, 8:40 PM
 */

#ifndef CALCULATOR_H
#define	CALCULATOR_H

#include <pegtl.hh>
#include "../seglobaltypedefs.h"

namespace se
{
    namespace xbrl
    {
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
            void* _session;

         public:
             statefulvector() : _state(nullptr), _session(nullptr)
             {
                 
             }
            statefulvector(void* state, void* session) : _state(state), _session(session)
            {
                
            }
            
            void* getsession()
            {
                return _session;
            }
            
            void* getstate()
            {
                return _state;
            }
        };

        // Canonical use of an evaluation stack, here implemented with a std::vector.

        typedef double value_type;
        //typedef std::vector< value_type > stack_type;
        typedef statefulvector<value_type> stack_type;

        // Helper function that is a "value returning pop() operation" that is not
        // in general exception safe, but fine here since the elements are a POD.

        value_type pull( stack_type & s );

        // The actions.

        // This action converts the matched sub-string to an integer and pushes it on
        // the stack, which must be its only additional state argument.

        // Deriving from action_base<> is necessary since version 0.26; the base class
        // takes care of the pretty-printing for diagnostic messages; this is necessary
        // for all action classes (that do not derive from a rule class).
        
        typedef std::vector<std::string> Xbrlidlist;

        struct push_action
              : action_base< push_action >
        {
           static void apply( const std::string & m, stack_type & s );
        };
        
        struct push_xbrlid_action
              : action_base< push_xbrlid_action >
        {
           static void apply( const std::string & m, stack_type & s );
           
           static value_type getxbrlvalue(UID xbrlid, UID xbrlsegid, stack_type& s );
           
           static void tokenizexbrlids(std::string m, Xbrlidlist& tokenfill);
           
        };
        
        struct push_matchaccount_action
              : action_base< push_matchaccount_action >
        {
           static void apply( const std::string & m, stack_type & s );
           
        };        
        

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
              
              if (rhs != ER8 && lhs != ER8)
              {
                  s.push_back( Operation()( lhs, rhs ) );
              }
              else
                  PEGTL_THROW("pegtl: invalid number.");
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
              value_type rhs = pull( s );
              if (!rhs) 
                 PEGTL_THROW( "pegtl: division by zero" );
              if (rhs == ER8)
                  rhs = 1.0;
              const value_type lhs = pull( s );
              s.push_back( std::divides< value_type >()( lhs, rhs ) );
           }
        };

        // if operation involves multiplication and rhs is an ER8, then make it 1
        template<>
        struct op_action< std::multiplies< value_type > >
              : action_base< op_action< std::multiplies< value_type > > >
        {
           template< typename State >
           static void apply( const std::string &, State & s )
           {
              value_type rhs = pull( s );
              if ( rhs == ER8) 
                 PEGTL_THROW( "pegtl: Cannot multiply with rhs which is a ER8" );

              value_type lhs = pull( s );
              if ( lhs == ER8) 
                 PEGTL_THROW( "pegtl: Cannot multiply with lhs which is a ER8" );
              
              s.push_back( std::multiplies< value_type >()( lhs, rhs ) );
           }
        };

        // when a + operation is involved and one of the operand is an ER8, make it 0 to make it unaffecting
        template<>
        struct op_action< std::plus< value_type > > 
              : action_base< op_action< std::plus< value_type > > >
        {
           template< typename State >
           static void apply( const std::string &, State & s )
           {
              value_type rhs = pull( s );
              if ( rhs == ER8) 
                 PEGTL_THROW( "pegtl: Cannot add with rhs which is a ER8" );
              
              value_type lhs = pull( s );
              if ( lhs == ER8) 
                 PEGTL_THROW( "pegtl: Cannot add with lhs which is a ER8" );
              
              s.push_back( std::plus< value_type >()( lhs, rhs ) );
           }
        };

        // when a - operation is involved and one of the operand is an ER8, make it 0 to make it unaffecting
        template<>
        struct op_action< std::minus< value_type > >
              : action_base< op_action< std::minus< value_type > > >
        {
           template< typename State >
           static void apply( const std::string &, State & s )
           {
              value_type rhs = pull( s );
              if ( rhs == ER8) 
                 PEGTL_THROW( "pegtl: Cannot substract with rhs which is a ER8" );
              
              value_type lhs = pull( s );
              if ( lhs == ER8) 
                 PEGTL_THROW( "pegtl: Cannot substract with lhs which is a ER8" );
              
              s.push_back( std::minus< value_type >()( lhs, rhs ) );
           }
        };
        

        struct xbrldigits
              : seq< plus<one<'x'>>, plus<digit>, star< one<'|'> >, star< one<'&'> >  >
        {
           template< typename Print >
           static void prepare( Print & st )
           {
              st.template update< alpha >( "xbrldigits", true );
           }
        };
        
        struct maccountids
              : seq< plus< one<'m'> >, plus<digit> >
        {
           template< typename Print >
           static void prepare( Print & st )
           {
              st.template update< alpha >( "maccountids", true );
           }
        };
        

        struct ddigit
              : sor< digit, one<'.'> >
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
              : seq< opt< one< '+', '-' > >, sor<plus<ddigit> > > {};

        struct read_xbrlid
              : seq< plus< xbrldigits > > {};

        struct read_matchaccounts
              : seq< plus< maccountids > > {};
              
        // This rule uses the rule read_number to match a number in the input
        // and, on success, applies the push_action to the matched sub-string
        // and the state. Here in calculator.cc the state is an instance of
        // stack_type, the evaluation stack on which the number is pushed.

        struct push_rule
              : pad< sor< 
                    ifapply< read_number, push_action >,
                    ifapply< read_matchaccounts, push_matchaccount_action >,
                    ifapply< read_xbrlid, push_xbrlid_action >
                >, space > {};

        template< int C >
        struct calc_pad
              : pad_one< C, space > {};

        struct read_open
              : calc_pad< '(' > {};

        struct read_close
              : calc_pad< ')' > {};

        struct read_expr;

        struct read_atom
              : sor< push_rule, seq< read_open, read_expr, read_close > > {};

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

        struct read_expr
              : seq< read_prod, star< sor< read_add, read_sub > > > {};

        struct read_calc
              : seq< read_expr, space_until_eof > {};
    }
} // calculator

#endif	/* CALCULATOR_H */

