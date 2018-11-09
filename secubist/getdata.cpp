

/*************************************************************************/
/*									 */
/*	Get case descriptions from data file				 */
/*	--------------------------------------				 */
/*									 */
/*************************************************************************/


#include "defns.h"
#include "CCubist.h"


Boolean SuppressErrorMessages = false;

#define XError(a,b,c)   if (! SuppressErrorMessages) Error(a,b,c)

namespace se
{
    namespace math
    {



        /*************************************************************************/
        /*									 */
        /*	Read raw cases from file with given extension.			 */
        /*									 */
        /*	On completion, cases are stored in array Case in the form	 */
        /*	of vectors of attribute values, and MaxCase is set to the	 */
        /*	number of data cases.						 */
        /*									 */
        /*************************************************************************/


        void CCubist::GetData(FILE *Df, Boolean Train, Boolean AllowUnknownTarget)
        /*   -------  */
        {
            CaseNo	CaseSpace, i;
            DataRec	DVec;
            Boolean	AnyUnknown=false, FirstIgnore=true, *AttMsg;
            Attribute	Att;
            ContValue	Val, Range;
            char	CVS[20];
            CaseCount	*Freq;			/* discrete value frequencies */
            CaseNo	Count, WantTrain, LeftTrain, WantTest, LeftTest;
            Boolean	SelectTrain;
            double	Sum, SumSq;
            DiscrValue	v;

            LineNo = 0;
            SuppressErrorMessages = SAMPLE && ! Train;

            if ( Train || ! Case )
            {
            MaxCase = MaxLabel = CaseSpace = 0;
            Case = Alloc(1, DataRec);	/* for error reporting */
            }
            else
            {
            CaseSpace = MaxCase + 1;
            MaxCase++;
            }

            if ( SAMPLE )
            {
            if ( Train )
            {
                SampleFrom = CountData(Df);
                ResetKR(KRInit);		/* initialise KRandom() */
            }
            else
            {
                ResetKR(KRInit);		/* restore  KRandom() */
            }

            WantTrain = SampleFrom * SAMPLE + 0.5;
            LeftTrain = SampleFrom;

            WantTest  = ( SAMPLE < 0.5 ? WantTrain : SampleFrom - WantTrain );
            LeftTest  = SampleFrom - WantTrain;
            }

            while ( (DVec = GetDataRec(Df, Train)) )
            {
            /*  Check whether to include if we are sampling */

            if ( SAMPLE )
            {                          
                SelectTrain = KRandom() < WantTrain / (float) LeftTrain--;

                /*  Include if 
                 * Select and this is the training set
                 * ! Select and this is the test set and sub-select
                NB: Must use different random number generator for
                sub-selection since cannot disturb random number sequence  */

                if ( SelectTrain )                                             
                {                                                              
                WantTrain--;                                               
                }                                                              

                if ( SelectTrain != Train ||                                   
                 ( ! Train && AltRandom >= WantTest / (float) LeftTest-- ) )
                {                                                              
                FreeLastCase(DVec);
                continue;
                }                                                   

                if ( ! Train )
                {                                                         
                WantTest--;
                }                                 
            }

            /*  Make sure there is room for another item  */

            if ( MaxCase >= CaseSpace )
            {
                CaseSpace += Inc;
                Realloc(Case, CaseSpace+1, DataRec);
            }

            /*  Ignore cases with N/A target value (!) but possibly allow
                cases with unknown target value  */

            if ( ! NotApplic(DVec, ClassAtt) &&
                 ( AllowUnknownTarget || CVal(DVec, ClassAtt) != UNKNOWN ) )
            {
                Case[MaxCase] = DVec;

                MaxCase++;
            }
            else
            {
                if ( FirstIgnore && Of )
                {
                fprintf(Of, ( AllowUnknownTarget ? T_IgnoreNATarget :
                                   T_IgnoreBadTarget ));
                FirstIgnore = false;
                }

                FreeLastCase(DVec);
            }
            }

            fclose(Df);
            MaxCase--;

            if ( Of && MaxCase < 0 )
            {
            fprintf(Of, T_NoCases);
            Goodbye(1);
            }


            /*  Replace unknown values by means or modals  */

            if ( AttMean == Nil )
            {
            /*  Find means, limits, modal values and precision  */

            AttMean   = AllocZero(MaxAtt+1, ContValue);
            AttSD     = AllocZero(MaxAtt+1, ContValue);
            AttMaxVal = AllocZero(MaxAtt+1, ContValue);
            AttMinVal = AllocZero(MaxAtt+1, ContValue);
            AttPref   = AllocZero(MaxAtt+1, ContValue);
            AttUnit   = AllocZero(MaxAtt+1, double);
            AttPrec   = AllocZero(MaxAtt+1, int);
            Modal     = Alloc(MaxAtt+1, DiscrValue);
            Freq      = Alloc(MaxDiscrVal+1, CaseCount);

            ForEach(Att, 1, MaxAtt)
            {
                if ( Exclude(Att) ) continue;

                if ( Continuous(Att) )
                {
                Sum = SumSq = Count = 0;

                ForEach(i, 0, MaxCase)
                {
                    Val = CVal(Case[i], Att);
                    if ( Val != UNKNOWN && DVal(Case[i], Att) != NA )
                    {
                    Sum   += Val;
                    SumSq += Val * Val;
                    Count++;
                    }
                }

                if ( Count > 1 )
                {
                    AttPrec[Att] = 10 * FracBase(Att);
                    AttMean[Att] = floor((Sum / Count) * AttPrec[Att] + 0.1) /
                           AttPrec[Att];
                    AttSD[Att]   = sqrt((SumSq - Sum * Sum / Count) / (Count-1))
                           + 1E-6;
                }
                else
                {
                    AttPrec[Att] = 0;
                    AttMean[Att] = 0;
                    AttSD[Att]   = 1;
                    SpecialStatus[Att] |= SKIP;
                }

                FindLimits(Att, &AttMinVal[Att], &AttMaxVal[Att]);
                }
                else
                {
                ForEach(v, 0, MaxAttVal[Att])
                {
                    Freq[v] = 0;
                }

                ForEach(i, 0, MaxCase)
                {
                    if ( (v = DVal(Case[i], Att)) ) Freq[v]++;
                }

                Modal[Att] = 0;		/* cannot be N/A */
                ForEach(v, 2, MaxAttVal[Att])
                {
                    if ( Freq[v] > Freq[Modal[Att]] ) Modal[Att] = v;
                }

                if ( ! Modal[Att] )
                {
                    SpecialStatus[Att] |= SKIP;
                }
                }
            }
            Free(Freq);					Freq = Nil;

            Precision = log(AttPrec[ClassAtt] / 10.0) / log(10.0) + 0.5;
            }

            /*  Now, the actual replacement  */

            AttMsg = AllocZero(MaxAtt+1, Boolean);

            ForEach(i, 0, MaxCase)
            {
            AnyUnknown |= ReplaceUnknowns(Case[i], AttMsg);
            }

            if ( Of && Train && AnyUnknown )
            {
            fprintf(Of, T_ReplaceUnknowns);

            ForEach(Att, 1, MaxAtt)
            {
                if ( AttMsg[Att] )
                {
                if ( Skip(Att) )
                {
                    fprintf(Of, "\t`%s' -- " T_NoAppVals "\n", AttName[Att]);
                    continue;
                }

                fprintf(Of, "\t`%s' " T_By " ", AttName[Att]);

                if ( Discrete(Att) )
                {
                    fprintf(Of, "`%s'\n", AttValName[Att][Modal[Att]]);
                }
                else
                {
                    CValToStr(AttMean[Att], Att, CVS);
                    fprintf(Of, "%s\n", CVS);
                }
                }
            }
            }

            if ( Train )
            {
            /*  Find global range and set lowest and highest predicted value  */

            Range = AttMaxVal[ClassAtt] - AttMinVal[ClassAtt];

            Ceiling = AttMaxVal[ClassAtt] + EXTRAP * Range;
            if ( AttMaxVal[ClassAtt] <= 0 && Ceiling > 0 ) Ceiling = 0;

            Floor = AttMinVal[ClassAtt] - EXTRAP * Range;
            if ( AttMinVal[ClassAtt] >= 0 && Floor < 0 ) Floor = 0;
            }

            Free(AttMsg);					AttMsg = Nil;
        }



        /*************************************************************************/
        /*									 */
        /*	Replace any unknown values in a case.  If AttMsg is not Nil,	 */
        /*	note the attributes involved.					 */
        /*									 */
        /*	Note: From release 1.13, N/A values are not replaced		 */
        /*									 */
        /*************************************************************************/


        Boolean CCubist::ReplaceUnknowns(DataRec Case, Boolean *AttMsg)
        /*      ---------------  */
        {
            Attribute	Att;
            Boolean	Replaced=false;

            ForEach(Att, 1, MaxAtt)
            {
            if ( Skip(Att) || Att == ClassAtt ) continue;

            if ( Discrete(Att) && ! DVal(Case, Att) )
            {
                DVal(Case, Att) = Modal[Att];
                if ( AttMsg ) AttMsg[Att] = Replaced = true;
            }
            else
            if ( Continuous(Att) && CVal(Case, Att) == UNKNOWN )
            {
                CVal(Case, Att) = AttMean[Att];
                if ( AttMsg ) AttMsg[Att] = Replaced = true;
            }
            }

            Class(Case) = CVal(Case, ClassAtt);

            return Replaced;
        }



        /*************************************************************************/
        /*									 */
        /*	Read a raw case from file Df.					 */
        /*									 */
        /*	For each attribute, read the attribute value from the file.	 */
        /*	If it is a discrete valued attribute, find the associated no.	 */
        /*	of this attribute value (if the value is unknown this is 0).	 */
        /*									 */
        /*	Returns the DataRec of the case (i.e. the array of attribute	 */
        /*	values).							 */
        /*									 */
        /*************************************************************************/


        DataRec CCubist::GetDataRec(FILE *Df, Boolean Train)
        /*      ----------  */
        {
            Attribute	Att;
            char	Name[1000], *EndVal;
            int		Dv, Chars;
            ContValue	Cv;
            DataRec	DVec;
            Boolean	FirstValue=true;


            if ( ReadName(Df, Name, 1000, '\00') )
            {
            Case[MaxCase] = DVec = NewCase();
            ForEach(Att, 1, MaxAtt)
            {
                if ( AttDef[Att] )
                {
                DVec[Att] = EvaluateDef(AttDef[Att], DVec);
                if ( Continuous(Att) )
                {
                    CheckValue(DVec, Att);
                }
                continue;
                }

                /*  Get the attribute value if don't already have it  */

                if ( ! FirstValue && ! ReadName(Df, Name, 1000, '\00') )
                {
                XError(EOFINATT, AttName[Att], "");
                FreeLastCase(DVec);
                return Nil;
                }
                FirstValue = false;

                if ( Exclude(Att) )
                {
                if ( Att == LabelAtt )
                {
                    /*  Record the value as a string  */

                    SVal(DVec,Att) = StoreIVal(Name);
                }
                }
                else
                if ( ! strcmp(Name, "?") )
                {
                /*  Unknown value  */

                if ( Continuous(Att) )
                {
                    CVal(DVec, Att) = UNKNOWN;
                }
                else
                {
                    DVal(DVec, Att) = 0;
                }
                }
                else
                if ( ! strcmp(Name, "N/A") )
                {
                /*  Non-applicable value  */

                DVal(DVec, Att) = NA;
                }
                else
                if ( Discrete(Att) )
                {
                Dv = Which(Name, AttValName[Att], 1, MaxAttVal[Att]);
                if ( ! Dv )
                {
                    if ( StatBit(Att, DISCRETE) )
                    {
                    if ( Train || XVAL )
                    {
                        /*  Add value to list  */

                        if ( MaxAttVal[Att] >= (long) AttValName[Att][0] )
                        {
                        XError(TOOMANYVALS, AttName[Att],
                             (char *) AttValName[Att][0] - 1);
                        Dv = MaxAttVal[Att];
                        }
                        else
                        {
                        Dv = ++MaxAttVal[Att];
                        AttValName[Att][Dv]   = strdup(Name);
                        AttValName[Att][Dv+1] = "<other>"; /* no free */
                        }
                        if ( Dv > MaxDiscrVal )	       
                        {	       
                           MaxDiscrVal = Dv;
                        }			     
                    }
                    else
                    {
                        /*  Set value to "<other>"  */

                        Dv = MaxAttVal[Att] + 1;
                    }
                    }
                    else
                    {
                    XError(BADATTVAL, AttName[Att], Name);
                    }
                }
                DVal(DVec, Att) = Dv;
                }
                else
                {
                /*  Continuous value  */

                if ( TStampVal(Att) )
                {
                    Cv = TStampToMins(Name);
                    if ( Cv >= 1E9 )	/* long time in future */
                    {
                    XError(BADTSTMP, AttName[Att], Name);
                    Cv = UNKNOWN;
                    }
                }
                else
                if ( DateVal(Att) )
                {
                    Cv = DateToDay(Name);
                    if ( Cv < 1 )
                    {
                    XError(BADDATE, AttName[Att], Name);
                    Cv = UNKNOWN;
                    }
                }
                else
                if ( TimeVal(Att) )
                {
                    Cv = TimeToSecs(Name);
                    if ( Cv < 0 )
                    {
                    XError(BADTIME, AttName[Att], Name);
                    Cv = UNKNOWN;
                    }
                }
                else
                {
                    Cv = strtod(Name, &EndVal);
                    if ( EndVal == Name || *EndVal != '\0' )
                    {
                    XError(BADATTVAL, AttName[Att], Name);
                    Cv = UNKNOWN;
                    }
                }

                CVal(DVec, Att) = Cv;

                CheckValue(DVec, Att);
                }
            }

            Class(DVec) = CVal(DVec, ClassAtt);

            if ( LabelAtt &&
                 (Chars = strlen(IgnoredVals + SVal(DVec, LabelAtt))) > MaxLabel )
            {
                MaxLabel = Chars;
            }

            return DVec;
            }
            else
            {
            return Nil;
            }
        }



        /*************************************************************************/
        /*                                                                       */
        /*      Count cases in data file                                         */
        /*                                                                       */
        /*************************************************************************/


        CaseNo CCubist::CountData(FILE *Df)
        /*     ---------  */
        {
            char        Last=',';
            int         Count=0, Next;

            while ( true )
            {
            if ( (Next = getc(Df)) == EOF )
            {
                if ( Last != ',' ) Count++;
                rewind(Df);
                return Count;
            }

            if ( Next == '|' )
            {
                while ( (Next = getc(Df)) != '\n' )
                ;
            }

            if ( Next == '\n' )
            {
                if ( Last != ',' ) Count++;
                Last = ',';
            }
            else
            if ( Next == '\\' )
            {
                /*  Skip escaped character  */

                getc(Df);
            }
            else
            if ( Next != '\t' && Next != ' ' )
            {
                Last = Next;
            }
            }
        }



        /*************************************************************************/
        /*									 */
        /*	Store a label or ignored value in IValStore			 */
        /*									 */
        /*************************************************************************/


        int CCubist::StoreIVal(__String S)
        /*  ---------  */
        {
            int		StartIx, Length;

            if ( (Length=strlen(S) + 1) + IValsOffset > IValsSize )
            {
            if ( IgnoredVals )
            {
                Realloc(IgnoredVals, IValsSize += 32768, char);
            }
            else
            {
                IValsSize   = 32768;
                IValsOffset = 0;
                IgnoredVals = Alloc(IValsSize, char);
            }
            }

            StartIx = IValsOffset;
            strcpy(IgnoredVals + StartIx, S);
            IValsOffset += Length;

            return StartIx;
        }



        /*************************************************************************/
        /*									 */
        /*	Free case description space					 */
        /*									 */
        /*************************************************************************/


        void CCubist::FreeData(DataRec *Case)
        /*   --------  */
        {
            /*  Do not free entire data block when freeing instances.
            Otherwise cannot free excluded attributes in real data  */

            if ( Case != Instance )
            {
            FreeCases();
            }

            FreeUnlessNil(IgnoredVals);				IgnoredVals = Nil;
                                    IValsSize = 0;

            Free(Case);
        }



        /*************************************************************************/
        /*									 */
        /*	Check for bad continuous value					 */
        /*									 */
        /*************************************************************************/


        void CCubist::CheckValue(DataRec DVec, Attribute Att)
        /*   ----------  */
        {
            ContValue	Cv;

            Cv = CVal(DVec, Att);
            if ( ! finite(Cv) )
            {
            Error(BADNUMBER, AttName[Att], "");

            CVal(DVec, Att) = UNKNOWN;
            DVal(DVec, Att) = 0;
            }
        }



        /*************************************************************************/
        /*									 */
        /*	Find maximum and minimum values for a continuous attribute	 */
        /*									 */
        /*************************************************************************/


        void CCubist::FindLimits(Attribute Att, ContValue *Min, ContValue *Max)
        /*   ----------  */
        {
            ContValue	Val;
            CaseNo	i;

            *Max = -1E38;
            *Min =  1E38;

            ForEach(i, 0, MaxCase)
            {
            if ( (Val = CVal(Case[i], Att) ) == UNKNOWN ) continue;

            if ( Val > *Max ) *Max = Val;
            if ( Val < *Min ) *Min = Val;
            }
        }

    }

}

