

/*************************************************************************/
/*									 */
/*	Routines to handle implicitly-defined attributes		 */
/*	------------------------------------------------		 */
/*									 */
/*************************************************************************/


#include "defns.h"
#include "CCubist.h"
#include <ctype.h>



#define FailSyn(Msg)	 {DefSyntaxError(Msg); return false;}
#define FailSem(Msg)	 {DefSemanticsError(Fi, Msg, OpCode); return false;}

typedef  union  _xstack_elt
         {
            DiscrValue  _discr_val;
            ContValue   _cont_val;
            __String      _string_val;
         }
	 XStackElt;

#define	cval		_cont_val
#define	sval		_string_val
#define	dval		_discr_val

#ifdef CUBIST
#define	Unknown(c,a)	(! DVal(c,a))
#endif

namespace se
{
    namespace math
    {



        /*************************************************************************/
        /*									 */
        /*	A definition is handled in two stages:				 */
        /*	  - The definition is read (up to a line ending with a period)	 */
        /*	    replacing multiple whitespace characters with one space	 */
        /*	  - The definition is then read (using a recursive descent	 */
        /*	    parser), building up a reverse polish expression		 */
        /*	Syntax and semantics errors are flagged				 */
        /*									 */
        /*************************************************************************/


        void CCubist::ImplicitAtt(FILE *Nf)
        /*   -----------  */
        {                                

        #ifdef CUBIST
            _UNK.cval = UNKNOWN;
        #else
            _UNK.dval = UNKNOWN;
        #endif
            _NA.dval  = NA;

            /*  Get definition as a string in Buff  */

            ReadDefinition(Nf);

            PreviousError = false;
            BN = 0;

            /*  Allocate initial stack and attribute definition  */

            TStack = Alloc(TStackSize=50, EltRec);
            TSN = 0;

            AttDef[MaxAtt] = Alloc(DefSize = 100, DefElt);
            DN = 0;

            /*  Parse Buff as an expression terminated by a period  */

            Expression();
            if ( ! Find(".") ) DefSyntaxError("'.' ending definition");

            /*  Final check -- defined attribute must not be of type __String  */

            if ( ! PreviousError )
            {
            if ( DN == 1 && DefOp(AttDef[MaxAtt][0]) == OP_ATT &&
                 strcmp(AttName[MaxAtt], "case weight") )
            {
                Error(SAMEATT, AttName[ (long) DefSVal(AttDef[MaxAtt][0]) ], Nil);
            }

            if ( TStack[0].Type == 'B' )
            {
                /*  Defined attributes should never have a value N/A  */


                MaxAttVal[MaxAtt] = 3;
                AttValName[MaxAtt] = AllocZero(4, __String);
                AttValName[MaxAtt][1] = strdup("??");
                AttValName[MaxAtt][2] = strdup("t");
                AttValName[MaxAtt][3] = strdup("f");
            }
            else
            {
                MaxAttVal[MaxAtt] = 0;
            }
            }

            if ( PreviousError )
            {
            DN = 0;
            SpecialStatus[MaxAtt] = EXCLUDE;
            }

            /*  Write a terminating marker  */

            DefOp(AttDef[MaxAtt][DN]) = OP_END;

            Free(Buff);
            Free(TStack);
        }



        /*************************************************************************/
        /*									 */
        /*	Read the text of a definition.  Skip comments, collapse		 */
        /*	multiple whitespace characters.					 */
        /*									 */
        /*************************************************************************/


        void CCubist::ReadDefinition(FILE *f)
        /*   --------------  */
        {
            Boolean	LastWasPeriod=false;
            char	c;

            Buff = Alloc(BuffSize=50, char);
            BN = 0;

            while ( true )
            {
            c = InChar(f);

            if ( c == '|' ) SkipComment;

            if ( c == EOF || c == '\n' && LastWasPeriod )
            {
                /*  The definition is complete.  Add a period if it's
                not there already and terminate the string  */

                if ( ! LastWasPeriod ) Append('.');
                Append(0);

                return;
            }

            if ( Space(c) )
            {
                Append(' ');
            }
            else
            if ( c == '\\' )
            {
                /*  Escaped character -- bypass any special meaning  */

                Append(InChar(f));
            }
            else
            {
                LastWasPeriod = ( c == '.' );
                Append(c);
            }
            }
        }



        /*************************************************************************/
        /*									 */
        /*	Append a character to Buff, resizing it if necessary		 */
        /*									 */
        /*************************************************************************/


        void CCubist::Append(char c)
        /*   ------  */
        {
            if ( c == ' ' && (! BN || Buff[BN-1] == ' ' ) ) return;

            if ( BN >= BuffSize )
            {
            Realloc(Buff, BuffSize += 50, char);
            }

            Buff[BN++] = c;
        }



        /*************************************************************************/
        /*									 */
        /*	Recursive descent parser with syntax error checking.		 */
        /*	The reverse polish is built up by calls to Dump() and DumpOp(),	 */
        /*	which also check for semantic validity.				 */
        /*									 */
        /*	For possible error messages, each routine also keeps track of	 */
        /*	the beginning of the construct that it recognises (in Fi).	 */
        /*									 */
        /*************************************************************************/


        Boolean CCubist::Expression(void)
        /*      ----------  */
        {
            int		Fi=BN;

            if ( Buff[BN] == ' ' ) BN++;

            if ( ! Conjunct() ) FailSyn("expression");

            while ( Find("or") )
            {
            BN += 2;

            if ( ! Conjunct() ) FailSyn("expression");

            DumpOp(OP_OR, Fi);
            }

            return true;
        }



        Boolean CCubist::Conjunct(void)
        /*      --------  */
        {
            int		Fi=BN;

            if ( ! SExpression() ) FailSyn("expression");

            while ( Find("and") )
            {
            BN += 3;

            if ( ! SExpression() ) FailSyn("expression");

            DumpOp(OP_AND, Fi);
            }

            return true;
        }



        __String RelOps[] = {">=", "<=", "!=", "<>", ">", "<", "=", (__String) 0};

        Boolean CCubist::SExpression(void)
        /*      -----------  */
        {
            int		o, Fi=BN;

            if ( ! AExpression() ) FailSyn("expression");

            if ( (o = FindOne(RelOps)) >= 0 )
            {
            BN += strlen(RelOps[o]);

            if ( ! AExpression() ) FailSyn("expression");

            DumpOp(( o == 0 ? OP_GE :
                 o == 1 ? OP_LE :
                 o == 4 ? OP_GT :
                 o == 5 ? OP_LT :
                 o == 2 || o == 3 ?
                    ( TStack[TSN-1].Type == 'S' ? OP_SNE : OP_NE ) :
                    ( TStack[TSN-1].Type == 'S' ? OP_SEQ : OP_EQ ) ), Fi);
            }

            return true;
        }



        __String AddOps[] = {"+", "-", (__String) 0};

        Boolean CCubist::AExpression(void)
        /*      -----------  */
        {
            int		o, Fi=BN;

            if ( Buff[BN] == ' ' ) BN++;

            if ( (o = FindOne(AddOps)) >= 0 )
            {
            BN += 1;
            }

            if ( ! Term() ) FailSyn("expression");

            if ( o == 1 ) DumpOp(OP_UMINUS, Fi);

            while ( (o = FindOne(AddOps)) >= 0 )
            {
            BN += 1;

            if ( ! Term() ) FailSyn("arithmetic expression");

            DumpOp((char)(OP_PLUS + o), Fi);
            }

            return true;
        }



        __String MultOps[] = {"*", "/", "%", (__String) 0};

        Boolean CCubist::Term(void)
        /*      ----  */
        {
            int		o, Fi=BN;

            if ( ! Factor() ) FailSyn("expression");

            while ( (o = FindOne(MultOps)) >= 0 )
            {
            BN += 1;

            if ( ! Factor() ) FailSyn("arithmetic expression");

            DumpOp((char)(OP_MULT + o), Fi);
            }

            return true;
        }



        Boolean CCubist::Factor(void)
        /*      ----  */
        {
            int		Fi=BN;

            if ( ! Primary() ) FailSyn("value");

            while ( Find("^") )
            {
            BN += 1;

            if ( ! Primary() ) FailSyn("exponent");

            DumpOp(OP_POW, Fi);
            }

            return true;
        }



        Boolean CCubist::Primary(void)
        /*      -------  */
        {
            if ( Atom() )
            {
            return true;
            }
            else
            if ( Find("(") )
            {
            BN++;
            if ( ! Expression() ) FailSyn("expression in parentheses");
            if ( ! Find(")") ) FailSyn("')'");
            BN++;
            return true;
            }
            else
            {
            FailSyn("attribute, value, or '('");
            }
        }



        __String Funcs[] = {"sin", "cos", "tan", "log", "exp", "int", (__String) 0};

        Boolean CCubist::Atom(void)
        /*      ----  */
        {
            char	*EndPtr, *Str, Date[11], Time[9];
            int		o, FirstBN, Fi=BN;
            ContValue	F;
            Attribute	Att;

            if ( Buff[BN] == ' ' ) BN++;

            if ( Buff[BN] == '"' )
            {
            FirstBN = ++BN;
            while ( Buff[BN] != '"' )
            {
                if ( ! Buff[BN] ) FailSyn("closing '\"'");
                BN++;
            }

            /*  Make a copy of the string without double quotes  */

            Buff[BN] = '\00';
            Str = strdup(Buff + FirstBN);

            Buff[BN++] = '"';
            Dump(OP_STR, 0, Str, Fi);
            }
            else
            if ( (Att = FindAttName()) )
            {
            BN += strlen(AttName[Att]);

            Dump(OP_ATT, 0, (__String) (long) Att, Fi);
            }
            else
            if ( isdigit(Buff[BN]) )
            {
            /*  Check for date or time first  */

            if ( ( Buff[BN+4] == '/' && Buff[BN+7] == '/' ||
                   Buff[BN+4] == '-' && Buff[BN+7] == '-' )&&
                 isdigit(Buff[BN+1]) && isdigit(Buff[BN+2]) &&
                isdigit(Buff[BN+3]) &&
                 isdigit(Buff[BN+5]) && isdigit(Buff[BN+6]) &&
                 isdigit(Buff[BN+8]) && isdigit(Buff[BN+9]) )
            {
                memcpy(Date, Buff+BN, 10);
                Date[10] = '\00';
                if ( (F = DateToDay(Date)) == 0 )
                {
                Error(BADDEF1, Date, "date");
                }

                BN += 10;
            }
            else
            if ( Buff[BN+2] == ':' && Buff[BN+5] == ':' &&
                 isdigit(Buff[BN+1]) &&
                 isdigit(Buff[BN+3]) && isdigit(Buff[BN+4]) &&
                 isdigit(Buff[BN+6]) && isdigit(Buff[BN+7]) )
            {
                memcpy(Time, Buff+BN, 8);
                Time[8] = '\00';
                if ( (F = TimeToSecs(Time)) == 0 )
                {
                Error(BADDEF1, Time, "time");
                }

                BN += 8;
            }
            else
            {
                F = strtod(Buff+BN, &EndPtr);

                /*  Check for period after integer  */

                if ( EndPtr > Buff+BN+1 && *(EndPtr-1) == '.' )
                {
                EndPtr--;
                }

                BN = EndPtr - Buff;
            }

            Dump(OP_NUM, F, Nil, Fi);
            }
            else
            if ( (o = FindOne(Funcs)) >= 0 )
            {
            BN += 3;

            if ( ! Find("(") ) FailSyn("'(' after function name");
            BN++;

            if ( ! Expression() ) FailSyn("expression");

            if ( ! Find(")") ) FailSyn("')' after function argument");
            BN++;

            DumpOp((char)(OP_SIN + o), Fi);
            }
            else
            if ( Buff[BN] == '?' )
            {
            BN++;
            if ( TStack[TSN-1].Type == 'N' )
            {
                Dump(OP_NUM, _UNK.cval, Nil, Fi);
            }
            else
            {
                Dump(OP_STR, 0, Nil, Fi);
            }
            }
            else
            if ( ! memcmp(Buff+BN, "N/A", 3) )
            {
            BN += 3;
            if ( TStack[TSN-1].Type == 'N' )
            {
                Dump(OP_NUM, _NA.cval, Nil, Fi);
            }
            else
            {
                Dump(OP_STR, 0, strdup("N/A"), Fi);
            }
            }
            else
            {
            return false;
            }

            return true;
        }



        /*************************************************************************/
        /*									 */
        /*	Skip spaces and check for specific string			 */
        /*									 */
        /*************************************************************************/


        Boolean CCubist::Find(__String S)
        /*      ----  */
        {
            if ( Buff[BN] == ' ' ) BN++;

            return ( ! Buff[BN] ? false : ! memcmp(Buff+BN, S, strlen(S)) );
        }



        /*************************************************************************/
        /*									 */
        /*	Find one of a zero-terminated list of alternatives		 */
        /*									 */
        /*************************************************************************/


        int CCubist::FindOne(__String *Alt)
        /*  -------  */
        {
            int	a;

            for ( a = 0 ; Alt[a] ; a++ )
            {
            if ( Find(Alt[a]) ) return a;
            }

            return -1;
        }



        /*************************************************************************/
        /*									 */
        /*	Find an attribute name						 */
        /*									 */
        /*************************************************************************/


        Attribute CCubist::FindAttName(void)
        /*        -----------  */
        {
            Attribute	Att, LongestAtt=0;

            ForEach(Att, 1, MaxAtt-1)
            {
            if ( ! Exclude(Att) && Find(AttName[Att]) )
            {
                if ( ! LongestAtt ||
                 strlen(AttName[Att]) > strlen(AttName[LongestAtt]) )
                {
                LongestAtt = Att;
                }
            }
            }

            return LongestAtt;
        }



        /*************************************************************************/
        /*									 */
        /*	Error message routines.  Syntax errors come from the		 */
        /*	recursive descent parser, semantics errors from the routines	 */
        /*	that build up the equivalent polish				 */
        /*									 */
        /*************************************************************************/


        void CCubist::DefSyntaxError(__String Msg)
        /*   --------------  */
        {
            __String	RestOfText;
            int		i=10;

            if ( ! PreviousError )
            {
            RestOfText = Buff + BN;

            /*  Abbreviate text if longer than 12 characters  */

            if ( CharWidth(RestOfText) > 12 )
            {
        #ifdef UTF8
                /*  Find beginning of UTF-8 character  */

                for ( ; (RestOfText[i] & 0x80) ; i++)
                ;
        #endif
                RestOfText[i] = RestOfText[i+1] = '.';
            }

            Error(BADDEF1, RestOfText, Msg);
            PreviousError = true;
            }
        }



        void CCubist::DefSemanticsError(int Fi, __String Msg, int OpCode)
        /*   -----------------  */
        {
            char	Exp[1000], XMsg[1000], Op[1000];

            if ( ! PreviousError )
            {
            /*  Abbreviate the input if necessary  */

            if ( BN - Fi > 23 )
            {
                sprintf(Exp, "%.10s...%.10s", Buff+Fi, Buff+BN-10);
            }
            else
            {
                sprintf(Exp, "%.*s", BN - Fi, Buff+Fi);
            }

            switch ( OpCode )
            {
                case OP_AND:	sprintf(Op, "%s", "and"); break;
                case OP_OR:		sprintf(Op, "%s", "or"); break;
                case OP_SEQ:
                case OP_EQ:		sprintf(Op, "%s", "="); break;
                case OP_SNE:
                case OP_NE:		sprintf(Op, "%s", "<>"); break;
                case OP_GT:		sprintf(Op, "%s", ">"); break;
                case OP_GE:		sprintf(Op, "%s", ">="); break;
                case OP_LT:		sprintf(Op, "%s", "<"); break;
                case OP_LE:		sprintf(Op, "%s", "<="); break;
                case OP_PLUS:	sprintf(Op, "%s", "+"); break;
                case OP_MINUS:	sprintf(Op, "%s", "-"); break;
                case OP_UMINUS:	sprintf(Op, "%s", "unary -"); break;
                case OP_MULT:	sprintf(Op, "%s", "*"); break;
                case OP_DIV:	sprintf(Op, "%s", "/"); break;
                case OP_MOD:	sprintf(Op, "%s", "%"); break;
                case OP_POW:	sprintf(Op, "%s", "^"); break;
                case OP_SIN:	sprintf(Op, "%s", "sin"); break;
                case OP_COS:	sprintf(Op, "%s", "cos"); break;
                case OP_TAN:	sprintf(Op, "%s", "tan"); break;
                case OP_LOG:	sprintf(Op, "%s", "log"); break;
                case OP_EXP:	sprintf(Op, "%s", "exp"); break;
                case OP_INT:	sprintf(Op, "%s", "int");
            }

            sprintf(XMsg, "%s with '%s'", Msg, Op);
            Error(BADDEF2, Exp, XMsg);
            PreviousError = true;
            }
        }



        /*************************************************************************/
        /*									 */
        /*	Reverse polish routines.  These use a model of the stack	 */
        /*	during expression evaluation to detect type conflicts etc	 */
        /*									 */
        /*************************************************************************/



        void CCubist::Dump(char OpCode, ContValue F, __String S, int Fi)
        /*   ----  */
        {
            if ( Buff[Fi] == ' ' ) Fi++;

            if ( ! UpdateTStack(OpCode, F, S, Fi) ) return;

            /*  Make sure enough room for this element  */

            if ( DN >= DefSize-1 )
            {
            Realloc(AttDef[MaxAtt], DefSize += 100, DefElt);
            }

            DefOp(AttDef[MaxAtt][DN]) = OpCode;
            if ( OpCode == OP_ATT || OpCode == OP_STR )
            {
            DefSVal(AttDef[MaxAtt][DN]) = S;
            }
            else
            {
            DefNVal(AttDef[MaxAtt][DN]) = F;
            }

            DN++;
        }



        void CCubist::DumpOp(char OpCode, int Fi)
        /*   ------  */
        {
            Dump(OpCode, 0, Nil, Fi);
        }



        Boolean CCubist::UpdateTStack(char OpCode, ContValue F, __String S, int Fi)
        /*      ------------  */
        {
            if ( TSN >= TStackSize )
            {
            Realloc(TStack, TStackSize += 50, EltRec);
            }

            switch ( OpCode )
            {
            case OP_ATT:
                TStack[TSN].Type = ( Continuous((long) S) ? 'N' : 'S' );
                break;

            case OP_NUM:
                TStack[TSN].Type = 'N';
                break;

            case OP_STR:
                TStack[TSN].Type = 'S';
                break;

            case OP_AND:
            case OP_OR:
                if ( TStack[TSN-2].Type != 'B' || TStack[TSN-1].Type != 'B' )
                {
                    FailSem("non-logical value");
                }
                TSN -= 2;
                break;

            case OP_EQ:
            case OP_NE:
                if ( TStack[TSN-2].Type != TStack[TSN-1].Type )
                {
                    FailSem("incompatible values");
                }
                TSN -= 2;
                TStack[TSN].Type = 'B';
                break;

            case OP_GT:
            case OP_GE:
            case OP_LT:
            case OP_LE:
                if ( TStack[TSN-2].Type != 'N' || TStack[TSN-1].Type != 'N' )
                {
                    FailSem("non-arithmetic value");
                }
                TSN -= 2;
                TStack[TSN].Type = 'B';
                break;

            case OP_SEQ:
            case OP_SNE:
                if ( TStack[TSN-2].Type != 'S' || TStack[TSN-1].Type != 'S' )
                {
                    FailSem("incompatible values");
                }
                TSN -= 2;
                TStack[TSN].Type = 'B';
                break;

            case OP_PLUS:
            case OP_MINUS:
            case OP_MULT:
            case OP_DIV:
            case OP_MOD:
            case OP_POW:
                if ( TStack[TSN-2].Type != 'N' || TStack[TSN-1].Type != 'N' )
                {
                    FailSem("non-arithmetic value");
                }
                TSN -= 2;
                break;

            case OP_UMINUS:
                if ( TStack[TSN-1].Type != 'N' )
                {
                    FailSem("non-arithmetic value");
                }
                TSN--;
                break;

            case OP_SIN:
            case OP_COS:
            case OP_TAN:
            case OP_LOG:
            case OP_EXP:
            case OP_INT:
                if ( TStack[TSN-1].Type != 'N' )
                {
                    FailSem("non-arithmetic argument");
                }
                TSN--;
            }

            TStack[TSN].Fi = Fi;
            TStack[TSN].Li = BN-1;
            TSN++;

            return true;
        }



        /*************************************************************************/
        /*									 */
        /*	Evaluate an implicit attribute for a case			 */
        /*									 */
        /*************************************************************************/

        #define	CUnknownVal(AV)		(AV.cval==_UNK.cval)
        #define	DUnknownVal(AV)		(! AV.dval)
        #define DUNA(a)	(DUnknownVal(XStack[a]) || NotApplicVal(XStack[a]))
        #define CUNA(a)	(CUnknownVal(XStack[a]) || NotApplicVal(XStack[a]))
        #define	C1(x)	(CUNA(XSN-1) ? _UNK.cval : (x))
        #define	C2(x)	(CUNA(XSN-1) || CUNA(XSN-2) ? _UNK.cval : (x))
        #define	CD2(x)	(CUNA(XSN-1) || CUNA(XSN-2) ? 0 : (x))
        #define	D2(x)	(DUNA(XSN-1) || DUNA(XSN-2) ? 0 : (x))


        AttValue CCubist::EvaluateDef(Definition D, DataRec Case)
        /*       -----------  */
        {
            XStackElt	XStack[100];			/* allows 100-level nesting  */
            int		XSN=0, DN, bv1, bv2, Mult;
            double	cv1, cv2;
            __String	sv1, sv2;
            Attribute	Att;
            DefElt	DElt;
            AttValue	ReturnVal;

            for ( DN = 0 ; ; DN++)
            {
            switch ( DefOp((DElt = D[DN])) )
            {
                case OP_ATT:
                    Att = (long) DefSVal(DElt);

                    if ( Continuous(Att) )
                    {
                    XStack[XSN++].cval = CVal(Case, Att);
                    }
                    else
                    {
                    XStack[XSN++].sval =
                        ( Unknown(Case, Att) && ! NotApplic(Case, Att) ? 0 :
                          AttValName[Att][XDVal(Case, Att)] );
                    }
                    break;

                case OP_NUM:
                    XStack[XSN++].cval = DefNVal(DElt);
                    break;

                case OP_STR:
                    XStack[XSN++].sval = DefSVal(DElt);
                    break;

                case OP_AND:
                    bv1 = XStack[XSN-2].dval;
                    bv2 = XStack[XSN-1].dval;
                    XStack[XSN-2].dval = ( bv1 == 3 || bv2 == 3 ? 3 :
                               D2(bv1 == 2 && bv2 == 2 ? 2 : 3) );
                    XSN--;
                    break;

                case OP_OR:
                    bv1 = XStack[XSN-2].dval;
                    bv2 = XStack[XSN-1].dval;
                    XStack[XSN-2].dval = ( bv1 == 2 || bv2 == 2 ? 2 :
                               D2(bv1 == 2 || bv2 == 2 ? 2 : 3) );
                    XSN--;
                    break;

                case OP_EQ:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].dval = ( cv1 == cv2 ? 2 : 3 );
                    XSN--;
                    break;

                case OP_NE:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].dval = ( cv1 != cv2 ? 2 : 3 );
                    XSN--;
                    break;

                case OP_GT:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].dval = CD2(cv1 > cv2 ? 2 : 3);
                    XSN--;
                    break;

                case OP_GE:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].dval = CD2(cv1 >= cv2 ? 2 : 3);
                    XSN--;
                    break;

                case OP_LT:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].dval = CD2(cv1 < cv2 ? 2 : 3);
                    XSN--;
                    break;

                case OP_LE:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].dval = CD2(cv1 <= cv2 ? 2 : 3);
                    XSN--;
                    break;

                case OP_SEQ:
                    sv1 = XStack[XSN-2].sval;
                    sv2 = XStack[XSN-1].sval;
                    XStack[XSN-2].dval =
                    ( ! sv1 && ! sv2 ? 2 :
                      ! sv1 || ! sv2 ? 3 :
                      ! strcmp(sv1, sv2) ? 2 : 3 );
                    XSN--;
                    break;

                case OP_SNE:
                    sv1 = XStack[XSN-2].sval;
                    sv2 = XStack[XSN-1].sval;
                    XStack[XSN-2].dval =
                    ( ! sv1 && ! sv2 ? 3 :
                      ! sv1 || ! sv2 ? 2 :
                      strcmp(sv1, sv2) ? 2 : 3 );
                    XSN--;
                    break;

                case OP_PLUS:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].cval = C2(cv1 + cv2);
                    XSN--;
                    break;

                case OP_MINUS:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].cval = C2(cv1 - cv2);
                    XSN--;
                    break;

                case OP_MULT:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].cval = C2(cv1 * cv2);
                    XSN--;
                    break;

                case OP_DIV:
                    /*  Note: have to set precision of result  */

                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    if ( ! cv2 ||
                     CUnknownVal(XStack[XSN-2]) ||
                     CUnknownVal(XStack[XSN-1]) ||
                     NotApplicVal(XStack[XSN-2]) ||
                     NotApplicVal(XStack[XSN-1]) )
                    {
                    XStack[XSN-2].cval = _UNK.cval;
                    }
                    else
                    {
                    Mult = Denominator(cv1);
                    cv1 = cv1 / cv2;
                    while ( fabs(cv2) > 1 )
                    {
                        Mult *= 10;
                        cv2 /= 10;
                    }
                    XStack[XSN-2].cval = rint(cv1 * Mult) / Mult;
                    }
                    XSN--;
                    break;

                case OP_MOD:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].cval = C2(fmod(cv1, cv2));
                    XSN--;
                    break;

                case OP_POW:
                    cv1 = XStack[XSN-2].cval;
                    cv2 = XStack[XSN-1].cval;
                    XStack[XSN-2].cval =
                    ( CUNA(XSN-1) || CUNA(XSN-2) ||
                      ( cv1 < 0 && ceil(cv2) != cv2 ) ? _UNK.cval :
                      pow(cv1, cv2) );
                    XSN--;
                    break;

                case OP_UMINUS:
                    cv1 = XStack[XSN-1].cval;
                    XStack[XSN-1].cval = C1(-cv1);
                    break;

                case OP_SIN:
                    cv1 = XStack[XSN-1].cval;
                    XStack[XSN-1].cval = C1(sin(cv1));
                    break;

                case OP_COS:
                    cv1 = XStack[XSN-1].cval;
                    XStack[XSN-1].cval = C1(cos(cv1));
                    break;

                case OP_TAN:
                    cv1 = XStack[XSN-1].cval;
                    XStack[XSN-1].cval = C1(tan(cv1));
                    break;

                case OP_LOG:
                    cv1 = XStack[XSN-1].cval;
                    XStack[XSN-1].cval =
                    ( CUNA(XSN-1) || cv1 <= 0 ? _UNK.cval : log(cv1) );
                    break;

                case OP_EXP:
                    cv1 = XStack[XSN-1].cval;
                    XStack[XSN-1].cval = C1(exp(cv1));
                    break;

                case OP_INT:
                    cv1 = XStack[XSN-1].cval;
                    XStack[XSN-1].cval = C1(rint(cv1));
                    break;

                case OP_END:
                    ReturnVal.cval = XStack[0].cval;	/* cval >= dval bytes */
                    return ReturnVal;
            }
            }
        }

    }
    
}