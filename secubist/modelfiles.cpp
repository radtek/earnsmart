

/*************************************************************************/
/*									 */
/*	Routines for saving and reading model files			 */
/*	-------------------------------------------			 */
/*									 */
/*************************************************************************/


#include "defns.h"
#include "CCubist.h"



#define	PROPS 31

#define IDP		 1
#define ATTP		 2
#define ELTSP		 3
#define PRECP		 4
#define GLOBALMEANP	 5
#define FLOORP		 6
#define CEILINGP	 7
#define SAMPLEP		 8
#define INITP		 9
#define MEANP		10
#define SDP		11
#define MODEP		12
#define ENTRIESP	13
#define RULESP		14
#define COVERP		15
#define LOVALP		16
#define HIVALP		17
#define EXTRAPP		18
#define INSTSP		19
#define NNP		20
#define MAXDP		21
#define ESTERRP		22
#define CONDSP		23
#define TYPEP		24
#define CUTP		25
#define RESULTP		26
#define VALP		27
#define COEFFP		28
#define	MAXP		29
#define	MINP		30
#define	REDNP		31

namespace se
{
    namespace math
    {




        /*************************************************************************/
        /*									 */
        /*	Check whether file is open.  If it is not, open it and		 */
        /*	read/write sampling information and discrete names		 */
        /*									 */
        /*************************************************************************/


        void CCubist::CheckFile(__String Extension, Boolean Write)
        /*   ---------  */
        {
            static char	*LastExt="";

            if ( ! Mf || strcmp(LastExt, Extension) )
            {
            LastExt = Extension;

            if ( Mf )
            {
                fprintf(Mf, "\n");
                fclose(Mf);					Mf = Nil;
            }

            if ( Write )
            {
                WriteFilePrefix(Extension);
            }
            else
            {
                ReadFilePrefix(Extension);
            }
            }
        }



        /*************************************************************************/
        /*									 */
        /*	Write information on system, sampling				 */
        /*									 */
        /*************************************************************************/


        void CCubist::WriteFilePrefix(__String Extension)
        /*   ---------------  */
        {
            Attribute	Att;
            time_t	clock;
            struct tm	*now;

            if ( ! (Mf = GetFile(Extension, "w")) )
            {
            Error(NOFILE, Fn, E_ForWrite);
            }

            clock = time(0);
            now = localtime(&clock);
            now->tm_mon++;
            fprintf(Mf, "id=\"Cubist %s %d-%d%d-%d%d\"\n",
                RELEASE,
                now->tm_year + 1900,
                now->tm_mon / 10, now->tm_mon % 10,
                now->tm_mday / 10, now->tm_mday % 10);

            SaveDiscreteNames();

            fprintf(Mf, "prec=\"%d\" globalmean=\"%.*g\" extrap=\"%g\" insts=\"%d\" ",
                Precision, PREC, GlobalMean, EXTRAP, USEINSTANCES);
            if ( USEINSTANCES )
            {
            fprintf(Mf, "nn=\"%d\" maxd=\"%.1f\" ", NN, MAXD);
            }
            fprintf(Mf, "ceiling=\"%.*g\" floor=\"%.*g\"\n",
                PREC, Ceiling, PREC, Floor);

            /*  Information for replacing missing values  */

            ForEach(Att, 1, MaxAtt)
            {
            if ( Exclude(Att) ) continue;

            AsciiOut("att=", AttName[Att]);
            if ( Continuous(Att) )
            {
                fprintf(Mf, " mean=\"%.*g\" sd=\"%.*g\" min=\"%g\" max=\"%g\"\n",
                    PREC, AttMean[Att], PREC, AttSD[Att],
                    AttMinVal[Att], AttMaxVal[Att]);
            }
            else
            {
                AsciiOut(" mode=", AttValName[Att][Max(1, Modal[Att])]);
                fprintf(Mf, "\n");
            }
            }

            /*  Sampling information (if needed)  */

            if ( SAMPLE > 0 )
            {
            fprintf(Mf, "sample=\"%g\" init=\"%d\"\n", SAMPLE, KRInit);
            }

            if ( MEMBERS > 1 )
            {
            fprintf(Mf, "redn=\"%.3f\" ", ErrReduction);
            }

            fprintf(Mf, "entries=\"%d\"\n", MEMBERS);
        }



        /*************************************************************************/
        /*									 */
        /*	Open model file and read header information			 */
        /*									 */
        /*************************************************************************/


        void CCubist::ReadFilePrefix(__String Extension)
        /*   --------------  */
        {
            int		Head;

            if ( ! (Mf = GetFile(Extension, "r")) ) Error(NOFILE, Fn, "");

            ReadHeader();
        }



        /*************************************************************************/
        /*									 */
        /*	Save attribute values read with "discrete N"			 */
        /*									 */
        /*************************************************************************/


        void CCubist::SaveDiscreteNames(void)
        /*   -----------------  */
        {
            Attribute	Att;
            DiscrValue	v;

            ForEach(Att, 1, MaxAtt)
            {
            if ( ! StatBit(Att, DISCRETE) || MaxAttVal[Att] < 2 ) continue;

            AsciiOut("att=", AttName[Att]);
            AsciiOut(" elts=", AttValName[Att][2]);         /* skip N/A */

            ForEach(v, 3, MaxAttVal[Att])
            {
                AsciiOut(",", AttValName[Att][v]);
            }
            fprintf(Mf, "\n");
            }
        }



        /*************************************************************************/
        /*								  	 */
        /*	Save the current rulesets in model file				 */
        /*								  	 */
        /*************************************************************************/


        void CCubist::SaveCommittee(RRuleSet *Cttee, __String modelText)
        /*   ---------  */
        {
            int		m;

            //CheckFile(Extension, true);

            ForEach(m, 0, MEMBERS-1)
            {
            SaveRules(Cttee[m], modelText);
            }
        }



        void CCubist::SaveRules(RRuleSet RS, __String modelText)
        /*   ---------  */
        {
            int		ri, d;
            CRule	R;
            Condition	C;
            DiscrValue	v;
            Attribute	Att;
            Boolean	First;

            char buffer[4096];
            memset(buffer, 0, 0x400);
            sprintf(buffer, "rules=\"%d\" ", RS->SNRules);
            strcat(modelText, buffer);

            ForEach(ri, 1, RS->SNRules)
            {
            R = RS->SRule[ri];
                memset(buffer, 0, 0x400);
            sprintf(buffer, " [ conds=\"%d\" cover=\"%d\" mean=\"%.*f\" "
                    "loval=\"%g\" hival=\"%g\" esterr=\"%.*f\"",
                     R->Size, R->Cover,
                     Precision+1, R->Mean,
                     R->LoVal, R->HiVal,
                     Precision+1, R->EstErr);
                strcat(modelText, buffer);

            ForEach(d, 1, R->Size)
            {
                C = R->Lhs[d];
                    memset(buffer, 0, 0x400);
                sprintf(buffer, " type=\"%d\"", C->NodeType);
                    strcat(modelText, buffer);
                AsciiOut1(" att=", AttName[C->Tested], modelText);
                switch ( C->NodeType )
                {
                case BrDiscr:
                    AsciiOut1(" val=", AttValName[C->Tested][C->TestValue], modelText);
                    break;

                case BrThresh:
                            memset(buffer, 0, 0x400);
                    if ( C->TestValue == 1 )
                    {
                    sprintf(buffer, " val=\"N/A\"");
                    }
                    else
                    {
                    sprintf(buffer, " cut=\"%.*g\" result=\"%s\"",
                            PREC+1, C->Cut,
                            ( C->TestValue == 2 ? "<=" : ">" ));
                    }
                            strcat(modelText, buffer);
                    break;

                case BrSubset:
                    First=true;
                    ForEach(v, 1, MaxAttVal[C->Tested])
                    {
                    if ( In(v, C->Subset) )
                    {
                        if ( First )
                        {
                        AsciiOut1(" elts=", AttValName[C->Tested][v], modelText);
                        First = false;
                        }
                        else
                        {
                        AsciiOut1(",", AttValName[C->Tested][v], modelText);
                        }
                    }
                    }
                    break;
                }
                    memset(buffer, 0, 0x400);
                sprintf(buffer, "\n");
                    strcat(modelText, buffer);
            }
                memset(buffer, 0, 0x400);
            sprintf(buffer, " coeff=\"%.14g\"", R->Rhs[0]);
                strcat(modelText, buffer);
            ForEach(Att, 1, MaxAtt)
            {
                if ( fabs(R->Rhs[Att]) > 0 )
                {
                AsciiOut1(" att=", AttName[Att], modelText);
                        memset(buffer, 0, 0x400);
                sprintf(buffer, " coeff=\"%.14g\"", R->Rhs[Att]);
                        strcat(modelText, buffer);
                }
            }
                memset(buffer, 0, 0x400);
            sprintf(buffer, " ] \n");
                strcat(modelText, buffer);
            }
        }



        /*************************************************************************/
        /*									 */
        /*	Write ASCII string with prefix, escaping any quotes		 */
        /*									 */
        /*************************************************************************/


        void CCubist::AsciiOut(__String Pre, __String S)
        /*   --------  */
        {
            fprintf(Mf, "%s\"", Pre);
            while ( *S )
            {
            if ( *S == '"' || *S == '\\' ) fputc('\\', Mf);
            fputc(*S++, Mf);
            }
            fputc('"', Mf);
        }

        void CCubist::AsciiOut1(__String Pre, __String S, __String modelText)
        /*   --------  */
        {
            char buffer[0x400];
            memset(buffer, 0, 0x400);
            sprintf(buffer, "%s\"", Pre);
            int pos = strlen(Pre);
            buffer[pos++] = '"';
            while ( *S )
            {
                buffer[pos++] = *S++;
            }
            buffer[pos++] = '"';
            strcat(modelText, buffer);
        }



        /*************************************************************************/
        /*								  	 */
        /*	Read the header information (id, saved names, models)		 */
        /*								  	 */
        /*************************************************************************/


        void CCubist::ReadHeader(void)
        /*   ---------  */
        {
            Attribute	Att;
            DiscrValue	v;
            char	*p, Dummy;
            double	Xd;
            int		Year, Month, Day;

            /*  First allocate storage for various globals  */

            AttMean   = Alloc(MaxAtt+1, ContValue);
            AttSD     = Alloc(MaxAtt+1, ContValue);
            AttMaxVal = Alloc(MaxAtt+1, ContValue);
            AttMinVal = Alloc(MaxAtt+1, ContValue);
            Modal     = Alloc(MaxAtt+1, DiscrValue);

            while ( true )
            {
            switch ( ReadProp(&Dummy) )
            {
                case IDP:
                /*  Recover year run and set base date for timestamps  */

                if ( sscanf(PropVal + strlen(PropVal) - 11,
                        "%d-%d-%d\"", &Year, &Month, &Day) == 3 )
                {
                    SetTSBase(Year);
                }
                break;

                case ATTP:
                Unquoted = RemoveQuotes(PropVal);
                Att = Which(Unquoted, AttName, 1, MaxAtt);
                if ( ! Att || Exclude(Att) )
                {
                    Error(MODELFILE, E_MFATT, Unquoted);
                }
                break;

                case ELTSP:
                /*  First element ("N/A") already added by GetNames()  */

                MaxAttVal[Att] = 1;

                for ( p = PropVal ; *p ; )
                {
                    p = RemoveQuotes(p);
                    v = ++MaxAttVal[Att];
                    AttValName[Att][v] = Alloc(strlen(p)+1, char);
                    strcpy(AttValName[Att][v], p);

                    for ( p += strlen(p) ; *p != '"' ; p++ )
                    ;
                    p++;
                    if ( *p == ',' ) p++;
                }
                AttValName[Att][MaxAttVal[Att]+1] = "<other>";
                MaxDiscrVal = Max(MaxDiscrVal, MaxAttVal[Att]+1);
                break;

                case PRECP:
                sscanf(PropVal, "\"%d\"", &Precision);
                break;

                case GLOBALMEANP:
                sscanf(PropVal, "\"%f\"", &GlobalMean);
                break;

                case EXTRAPP:
                sscanf(PropVal, "\"%f\"", &EXTRAP);
                break;

                case INSTSP:
                USEINSTANCES = PropVal[1] - '0';
                if ( USEINSTANCES )
                {
                    /*  Set legacy values  */

                    NN   = 5;
                    MAXD = 50;
                }
                break;

                case NNP:
                sscanf(PropVal, "\"%d\"", &NN);
                break;

                case MAXDP:
                sscanf(PropVal, "\"%f\"", &MAXD);
                break;

                case CEILINGP:
                sscanf(PropVal, "\"%lf\"", &Xd);	Ceiling = Xd;
                break;

                case FLOORP:
                sscanf(PropVal, "\"%lf\"", &Xd);	Floor = Xd;
                break;

                case MEANP:
                sscanf(PropVal, "\"%lf\"", &Xd);	AttMean[Att] = Xd;
                break;

                case SDP:
                sscanf(PropVal, "\"%lf\"", &Xd);	AttSD[Att] = Xd;
                break;

                case MAXP:
                sscanf(PropVal, "\"%lf\"", &Xd);	AttMaxVal[Att] = Xd;
                break;

                case MINP:
                sscanf(PropVal, "\"%lf\"", &Xd);	AttMinVal[Att] = Xd;
                break;

                case MODEP:
                Unquoted = RemoveQuotes(PropVal);
                Modal[Att] = Which(Unquoted,
                           AttValName[Att], 1, MaxAttVal[Att]);

                if ( ! Modal[Att] )
                {
                    /*  An unknown modal value is an error!  */

                    Error(MODELFILE, E_MFATTVAL, Unquoted);
                }
                else
                if ( Modal[Att] == 1 )
                {
                    /*  This means that all training cases had value N/A.
                    For consistency with instance distances, this
                    attribute is ignored  */

                    SpecialStatus[Att] |= SKIP;
                }
                break;

                case SAMPLEP:
                sscanf(PropVal, "\"%f\"", &SAMPLE);
                break;

                case INITP:
                sscanf(PropVal, "\"%d\"", &KRInit);
                break;

                case REDNP:
                sscanf(PropVal, "\"%f\"", &ErrReduction);
                break;

                case ENTRIESP:
                sscanf(PropVal, "\"%d\"", &MEMBERS);
                Entry = 0;
                return;
            }
            }
        }



        /*************************************************************************/
        /*									 */
        /*	Retrieve ruleset with extension Extension			 */
        /*	(Separate functions for ruleset, single rule, single condition)	 */
        /*									 */
        /*************************************************************************/


        RRuleSet* CCubist::GetCommittee(__String Extension)
        /*	 -------------  */
        {
            RRuleSet	*Cttee;
            int		m;

            ErrMsgs = 0;

            CheckFile(Extension, false);
            if ( ErrMsgs )
            {
            if ( Mf )
            {
                fclose(Mf);					Mf = Nil;
            }
            return Nil;
            }

            Cttee = Alloc(MEMBERS, RRuleSet);

            ForEach(m, 0, MEMBERS-1)
            {
            Cttee[m] = InRules();
            }

            fclose(Mf);						Mf = Nil;

            return ( ErrMsgs ? Nil : Cttee );
        }



        RRuleSet CCubist::InRules(void)
        /*	 -------  */
        {
            RRuleSet	RS;
            RuleNo	r;
            char	Delim;

            RS = Alloc(1, RuleSetRec);

            do
            {
            switch ( ReadProp(&Delim) )
            {
                case RULESP:
                sscanf(PropVal, "\"%d\"", &RS->SNRules);
                break;
            }
            }
            while ( Delim == ' ' );

            /*  Read each rule  */

            RS->SRule = Alloc(RS->SNRules+1, CRule);
            ForEach(r, 1, RS->SNRules)
            {
            RS->SRule[r] = InRule();
            RS->SRule[r]->RNo = r;
            RS->SRule[r]->MNo = Entry;
            }

            Entry++;

            return RS;
        }



        CRule CCubist::InRule(void)
        /*    ------  */
        {
            CRule	R;
            int		d;
            char	Delim;
            Attribute	Att=0;
            float	V, Range;

            R = Alloc(1, RuleRec);

            /*  General rule information  */

            do
            {
            switch ( ReadProp(&Delim) )
            {
                case CONDSP:
                sscanf(PropVal, "\"%d\"", &R->Size);
                break;

                case COVERP:
                sscanf(PropVal, "\"%d\"", &R->Cover);
                break;

                case MEANP:
                sscanf(PropVal, "\"%f\"", &R->Mean);
                break;

                case LOVALP:
                sscanf(PropVal, "\"%f\"", &R->LoVal);
                break;

                case HIVALP:
                sscanf(PropVal, "\"%f\"", &R->HiVal);
                break;

                case ESTERRP:
                sscanf(PropVal, "\"%f\"", &R->EstErr);
                break;
            }
            }
            while ( Delim == ' ' );

            Range    = R->HiVal - R->LoVal;
            R->LoLim = ( (V = R->LoVal - EXTRAP * Range) < 0 && R->LoVal >= 0 ? 0 : V );
            R->HiLim = ( (V = R->HiVal + EXTRAP * Range) > 0 && R->HiVal <= 0 ? 0 : V );

            /*  Conditions making up rule's left-hand side  */

            R->Lhs = Alloc(R->Size+1, Condition);
            ForEach(d, 1, R->Size)
            {
            R->Lhs[d] = InCondition();
            }

            /*  Linear model associated with rule  */

            R->Rhs = AllocZero(MaxAtt+1, double);
            do
            {
            switch ( ReadProp(&Delim) )
            {
                case ATTP:
                Unquoted = RemoveQuotes(PropVal);
                Att = Which(Unquoted, AttName, 1, MaxAtt);
                if ( ! Att || Exclude(Att) )
                {
                    Error(MODELFILE, E_MFATT, Unquoted);
                }
                break;

                case COEFFP:
                sscanf(PropVal, "\"%lf\"", &R->Rhs[Att]);
                break;
            }
            }
            while ( Delim == ' ' );

            return R;
        }



        Condition CCubist::InCondition(void)
        /*        -----------  */
        {
            Condition	C;
            char	Delim;
            int		X;
            double	XD;

            C = Alloc(1, CondRec);

            do
            {
            switch ( ReadProp(&Delim) )
            {
                case TYPEP:
                sscanf(PropVal, "\"%d\"", &X); C->NodeType = X;
                break;

                case ATTP:
                Unquoted = RemoveQuotes(PropVal);
                C->Tested = Which(Unquoted, AttName, 1, MaxAtt);
                if ( ! C->Tested || Exclude(C->Tested) )
                {
                    Error(MODELFILE, E_MFATT, Unquoted);
                }
                break;

                case CUTP:
                sscanf(PropVal, "\"%lf\"", &XD);	C->Cut = XD;
                break;

                case RESULTP:
                C->TestValue = ( PropVal[1] == '<' ? 2 : 3 );
                break;

                case VALP:
                if ( Continuous(C->Tested) )
                {
                    C->TestValue = 1;
                }
                else
                {
                    Unquoted = RemoveQuotes(PropVal);
                    C->TestValue = Which(Unquoted,
                             AttValName[C->Tested],
                             1, MaxAttVal[C->Tested]);
                    if ( ! C->TestValue )
                    {
                    Error(MODELFILE, E_MFATTVAL, Unquoted);
                    }
                }
                break;

                case ELTSP:
                C->Subset = MakeSubset(C->Tested);
                C->TestValue = 1;
                break;
            }
            }
            while ( Delim == ' ' );

            return C;
        }



        /*************************************************************************/
        /*									 */
        /*	ASCII reading utilities						 */
        /*									 */
        /*************************************************************************/


        int CCubist::ReadProp(char *Delim)
        /*  --------  */
        {
            int		c, i;
            char	*p;
            Boolean	Quote=false;

            for ( p = PropName ; (c = fgetc(Mf)) != '=' ;  )
            {
            if ( p - PropName >= 19 || c == EOF )
            {
                Error(MODELFILE, E_MFEOF, "");
                PropName[0] = PropVal[0] = *Delim = '\00';
                return 0;
            }
            *p++ = c;
            }
            *p = '\00';

            for ( p = PropVal ; ((c = fgetc(Mf)) != ' ' && c != '\n') || Quote ; )
            {
            if ( c == EOF )
            {
                Error(MODELFILE, E_MFEOF, "");
                PropName[0] = PropVal[0] = '\00';
                return 0;
            }

            if ( (i = p - PropVal) >= PropValSize )
            {
                Realloc(PropVal, (PropValSize += 10000) + 3, char);
                p = PropVal + i;
            }
            *p++ = c;
            if ( c == '\\' )
            {
                *p++ = fgetc(Mf);
            }
            else
            if ( c == '"' )
            {
                Quote = ! Quote;
            }
            }
            *p = '\00';
            *Delim = c;

            return Which(PropName, Prop, 1, PROPS);
        }


        __String CCubist::RemoveQuotes(__String S)
        /*     ------------  */
        {
            char	*p, *Start;

            p = Start = S;

            for ( S++ ; *S != '"' ; S++ )
            {
            if ( *S == '\\' ) S++;
            *p++ = *S;
            *S = '-';
            }
            *p = '\00';

            return Start;
        }



        __Set CCubist::MakeSubset(Attribute Att)
        /*  ----------  */
        {
            int		Bytes, b;
            char	*p;
            __Set		S;

            Bytes = (MaxAttVal[Att]>>3) + 1;
            S = AllocZero(Bytes, unsigned char);

            for ( p = PropVal ; *p ; )
            {
            p = RemoveQuotes(p);
            b = Which(p, AttValName[Att], 1, MaxAttVal[Att]);
            if ( ! b ) Error(MODELFILE, E_MFATTVAL, p);
            SetBit(b, S);

            for ( p += strlen(p) ; *p != '"' ; p++ )
                ;
            p++;
            if ( *p == ',' ) p++;
            }

            return S;
        }

    }
    
}