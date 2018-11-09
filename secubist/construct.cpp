
/*************************************************************************/
/*									 */
/*	Routines for constructing rulesets and committees		 */
/*	-------------------------------------------------		 */
/*									 */
/*************************************************************************/


#include "defns.h"
#include <time.h>
#include "CCubist.h"

namespace se
{
    namespace math
    {

        /*************************************************************************/
        /*                                                                       */
        /*      Construct, save and evaluate a single model			 */
        /*                                                                       */
        /*************************************************************************/

        void CCubist::SingleCttee(void)
        /*   -----------  */
        {
            ConstructCttee(0);

            //EvaluateCttee(Cttee, false);

            FreeCttee(Cttee);					

            Cttee = Nil;
        }



        /*************************************************************************/
        /*                                                                       */
        /*      Main construction routines					 */
        /*                                                                       */
        /*************************************************************************/


        Boolean CCubist::ConstructCttee(__String modelText)
        /*   --------------  */
        {
            int		m;
            Boolean	SaveUSEINSTANCES;
            CaseNo	i;
            double	Cases, SumErr=0, Err, FinalErr=0;

            /*  Preserve original item order  */

            SaveCase = Alloc(MaxCase+1, DataRec);
            memcpy(SaveCase, Case, (MaxCase+1) * sizeof(DataRec));

            FindGlobalProperties();

            if ( CHOOSEMODE ) USEINSTANCES = true;

            /*  Set minimum target coverage for a rule as 1% of cases (to a
            maximum of 20).  However, it must be at least MINSPLIT and
            must not exceed cases / MAXRULES  */

            Cases = MaxCase+1;

            MINITEMS = Min(20, rint(Cases / 100));

            if ( MINITEMS > Cases / MAXRULES )
            {
                MINITEMS = Cases / MAXRULES;
            }

            if ( MINITEMS < MINSPLIT )
            {
                MINITEMS = MINSPLIT;
            }

            Cttee = AllocZero(MEMBERS, RRuleSet);

            ForEach(m, 0, MEMBERS-1)
            {

                Cttee[m] = ConstructRuleSet(m);

                memcpy(Case, SaveCase, (MaxCase+1) * sizeof(DataRec)); /* restore */

                if ( m < MEMBERS-1 )
                {
                    /*  Adjust target value for next regression tree  */

                    Err = 0;

                    ForEach(i, 0, MaxCase)
                    {
                PredVal(Case[i]) = RuleSetPrediction(Cttee[m], Case[i]);

                        Err += fabs(Class(Case[i]) - PredVal(Case[i]));

                        Class(Case[i]) = 2 * CVal(Case[i], ClassAtt) - PredVal(Case[i]);
                    }

                    SumErr += Err;
                }
            }

            FreeUnlessNil(SaveCase);				

            SaveCase = Nil;

            if ( ! XVAL && MEMBERS > 1 )
            {
            /*  Calculate the error reduction achieved by committee model  */

            SaveUSEINSTANCES = USEINSTANCES;
            USEINSTANCES = false;

            FindPredictedValues(Cttee, 0, MaxCase);

            USEINSTANCES = SaveUSEINSTANCES;

            ForEach(i, 0, MaxCase)
            {
                FinalErr += fabs(Class(Case[i]) - PredVal(Case[i]));
            }

            ErrReduction = FinalErr / (SumErr / (MEMBERS - 1));
            }

            /*  See whether to use rulesets or rulesets with instances  */

            if ( USEINSTANCES )
            {
            MAXD = -1;	/* causes InitialiseInstances to set it */

                if (!InitialiseInstances(Cttee))
                    return false;
            }

            if ( CHOOSEMODE )
            {
            CheckForms(Cttee);
            }

            if ( ! XVAL && modelText) 
            {
                SaveCommittee(Cttee, modelText);
            }
            return true;
        }

        RRuleSet CCubist::ConstructRuleSet(int ModelNo)
        /*       ----------------  */
        {
            RRuleSet	RS;
            char	Msg[20];
            CaseNo	i;
            RuleNo	r;
            float	TempMTSize;

            //NotifyStage(GROUPDATA);
            //Progress( -(MaxCase+1.0) );

            FormTree(0, MaxCase, 0, &TempMT, Nil);

            //NotifyStage(ADDMODELS);
            //Progress( (TempMTSize = (float) -TreeSize(TempMT)) );

            AddModels(0, MaxCase, TempMT, Nil);

            //NotifyStage(SIMPLIFYGROUPS);
            //Progress( TempMTSize );

            Prune(TempMT);
            AdjustAllThresholds(TempMT);
            //Verbosity(1, PrintTree(TempMT, "Model tree"))

            //NotifyStage(FORMRULES);
            //Progress( -(MaxCase+1.0) );

            /*  Restore original target values  */

            ForEach(i, 0, MaxCase)
            {
            Class(Case[i]) = CVal(Case[i], ClassAtt);
            }

            RS = FormRules(TempMT);
            ForEach(r, 1, RS->SNRules)
            {
            RS->SRule[r]->MNo = ModelNo;
            }

            //if ( MEMBERS > 1 )
            //{
        //	sprintf(Msg, "Model %d:", ModelNo+1);
        //    }
        //    else
        //    {
        //              sprintf(Msg, "Model:");
        //    }

        //    PrintRules(RS, Msg);

            FreeTree(TempMT);				TempMT = Nil;

            return RS;
        }



        /*************************************************************************/
        /*                                                                       */
        /*      Check performance of model                                       */
        /*                                                                       */
        /*************************************************************************/


        void CCubist::EvaluateCttee(RRuleSet *Cttee, Boolean Details)
        /*   -------------  */
        {
            CaseNo	i, MaxTest;
            double	Real, CorCoeff=0, Tests,
                    AbsErr=0, BaseAbsErr=0, ProdVar,
                    SumX=0, SumY=0, SumXY=0, SumXX=0, SumYY=0;

            if ( MaxCase < 0 ) return;

            MaxTest = MaxCase;

            if ( USEINSTANCES )
            {
            if ( Details )
            {
                /*  Copy instances to improve cache performance  */

                CopyInstances();
            }
            else
            if ( MaxCase >= 2 * EVALSAMPLE )
            {
                /*  For composite models, use only a sample for evaluating large
                training sets  */

                SampleTrainingCases();
                MaxTest = EVALSAMPLE-1;
            }
            }

            /*  Find the cases' predicted values if they are not already known  */

            if ( Details || USEINSTANCES || MEMBERS == 1 )
            {
            FindPredictedValues(Cttee, 0, MaxTest);
            }

            if ( Details )
            {
        /*
            if ( ! (Pf = GetFile(".pred", "w")) ) Error(NOFILE, Fn, E_ForWrite);

            fprintf(Pf, T_Default, Precision+1, GlobalMean);
            fprintf(Pf, F_Actual "  " F_Predicted "    "    F_Case "\n"
                    F_Value  "  " F_Value     "\n"
                    " --------  ---------    " F_UCase "\n");
        */
            }

            /*  Evaluate performance of model  */

            ForEach(i, 0, MaxTest)
            {
            Real = Class(Case[i]);

            AbsErr     += fabs(Real - PredVal(Case[i]));
            BaseAbsErr += fabs(Real - GlobalMean);

            SumX  += Real;
            SumXX += Real * Real;
            SumY  += PredVal(Case[i]);
            SumYY += PredVal(Case[i]) * PredVal(Case[i]);
            SumXY += Real * PredVal(Case[i]);

            if ( Details )
            {
        /*
                fprintf(Pf, "%9.*f  %9.*f    %s\n",
                    Precision, Real, Precision+1, PredVal(Case[i]),
                    CaseLabel(i));
        */
            }

            Progress(1.0);
            }

            if ( Details )
            {
            fclose(Pf);
            Pf = 0;
            }

            Tests = MaxTest+1;

            ProdVar = (SumXX - SumX * SumX / Tests) * (SumYY - SumY * SumY / Tests);
            if ( ProdVar > 0 )
            {
            CorCoeff = (SumXY - SumX * SumY / Tests) / sqrt(ProdVar);
            }

        /*
            fprintf(Of,"\n");
            fprintf(Of, F_AvErr "%10.*f\n", Precision+1, AbsErr / Tests);
            fprintf(Of, F_RelErr "%10.2f\n",
                ( BaseAbsErr ? AbsErr / BaseAbsErr : 1.0 ));
            fprintf(Of, F_CorrCoeff "%10.2f\n", Max(0, CorCoeff));
        */

            /*  Summarise attribute usage on training data  */

            if ( ! Details )
            {
            AttributeUsage();
            }
        }



        /*************************************************************************/
        /*                                                                       */
        /*      Sample EVALSAMPLE cases from training set			 */
        /*                                                                       */
        /*************************************************************************/


        void CCubist::SampleTrainingCases(void)
        /*   -------------------  */
        {
            CaseNo	i, j;
            double	Step;
            DataRec	xab;

            Step    = (MaxCase+1) / (double) EVALSAMPLE;
            ForEach(i, 1, EVALSAMPLE-1)
            {
            j = i * Step;
            Swap(i, j);
            }
        }



        /*************************************************************************/
        /*                                                                       */
        /*      Report attribute usage in conditions and models			 */
        /*                                                                       */
        /*************************************************************************/




        void CCubist::AttributeUsage(void)
        /*   --------------  */
        {
            RRuleSet	RS;
            int		m;
            RuleNo	r;
            Attribute	Att, BestAtt;
            char	U1[5], U2[5];

            /*  Initialise counts  */

            SumCases = 0;
            SumCond  = AllocZero(MaxAtt+1, CaseCount);
            SumModel = AllocZero(MaxAtt+1, CaseCount);
            AttUsed  = Alloc(MaxAtt+1, Boolean);

            /*  Scan rules in committee  */

            ForEach(m, 0, MEMBERS-1)
            {
            RS = Cttee[m];

            ForEach(r, 1, RS->SNRules)
            {
                UpdateUsage(RS->SRule[r]);
            }
            }

            /*  Show attributes in order of combined usage  */

            //fprintf(Of, T_AttUsage);

            while ( true )
            {
            BestAtt = 0;

            ForEach(Att, 1, MaxAtt)
            {
                if ( Max(SumCond[Att], SumModel[Att]) >= 0.01 * SumCases &&
                 ( ! BestAtt ||
                     SumCond[Att] > SumCond[BestAtt] ||
                     SumCond[Att] >= SumCond[BestAtt] &&
                     SumModel[Att] > SumModel[BestAtt] ) )
                {
                BestAtt = Att;
                }
            }

            if ( ! BestAtt ) break;

        /*
            sprintf(U1, "%3g%%", rint((100.0 * SumCond[BestAtt]) / SumCases));
            sprintf(U2, "%3g%%", rint((100.0 * SumModel[BestAtt]) / SumCases));

            fprintf(Of, "\t  %4s   %4s    %s\n",
                    ( SumCond[BestAtt] >= 0.01 * SumCases ? U1 : " " ),
                    ( SumModel[BestAtt] >= 0.01 * SumCases ? U2 : " " ),
                    AttName[BestAtt]);
        */

            SumCond[BestAtt] = SumModel[BestAtt] = 0;
            }

            Free(SumCond);
            Free(SumModel);
            Free(AttUsed);
        }



        void CCubist::UpdateUsage(CRule R)
        /*   -----------  */
        {
            Attribute	Att;
            int		d;

            SumCases += R->Cover;

            /*  Attributes used in conditions.  Must assemble in table in case
            same attribute appears more than once  */

            memset(AttUsed, false, MaxAtt+1);

            ForEach(d, 1, R->Size)
            {
            NoteUsed(R->Lhs[d]->Tested);
            }

            ForEach(Att, 1, MaxAtt)
            {
            if ( AttUsed[Att] ) SumCond[Att] += R->Cover;
            }

            /*  Attributes used in model  */

            memset(AttUsed, false, MaxAtt+1);

            ForEach(Att, 1, MaxAtt)
            {
            if ( R->Rhs[Att] ) NoteUsed(Att);
            }

            ForEach(Att, 1, MaxAtt)
            {
            if ( AttUsed[Att] ) SumModel[Att] += R->Cover;
            }
        }



        void CCubist::NoteUsed(Attribute Att)
        /*   --------  */
        {
            int	i;

            if ( AttUsed[Att] ) return;

            AttUsed[Att] = true;

            if ( AttDef[Att] )
            {
            /*  Include attributes that appear in definition  */

            ForEach(i, 1, AttDefUses[Att][0])
            {
                NoteUsed(AttDefUses[Att][i]);
            }
            }
        }
    }
}
