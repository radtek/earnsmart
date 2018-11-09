

/*************************************************************************/
/*                                                              	 */
/*	Predict the value of a case from a ruleset			 */
/*	------------------------------------------			 */
/*                                                              	 */
/*************************************************************************/


#include "defns.h"
#include "CCubist.h"

namespace se
{
    namespace math
    {



        /*************************************************************************/
        /*                                                              	 */
        /*	Find rules that apply to case and form average			 */
        /*                                                              	 */
        /*************************************************************************/


        float CCubist::PredictValue(RRuleSet *Cttee, DataRec CaseDesc)
        /*    ------------  */
        {
            double	PredSum=0;
            int		m;

            ForEach(m, 0, MEMBERS-1)
            {
                RRuleSet r = Cttee[m];
            PredSum += RuleSetPrediction(r, CaseDesc);
            }


            return PredSum / MEMBERS;
        }



        float CCubist::RuleSetPrediction(RRuleSet RS, DataRec CaseDesc)
        /*    -----------------  */
        {
            double	Sum=0, Weight=0, Val;
            int		r;
            CRule	R;
            Attribute	Att;

            ForEach(r, 1, RS->SNRules)
            {
            R = RS->SRule[r];

            if ( Matches(R, CaseDesc) )
            {
                /*  Evaluate RHS.  Cannot use RawLinModel() because
                have not run FindModelAtts()  */

                Val = R->Rhs[0];
                ForEach(Att, 1, MaxAtt)
                {
                Val += CVal(CaseDesc, Att) * R->Rhs[Att];
                }

                Sum += ( Val < R->LoLim ? R->LoLim :
                     Val > R->HiLim ? R->HiLim : Val );
                Weight += 1.0;
            }
            }

            if ( Weight )
            {
            return Sum / Weight;
            }
            else
            {
            return GlobalMean;
            }
        }


        /*************************************************************************/
        /*								         */
        /*	Determine whether a case satisfies all conditions of a rule	 */
        /*								         */
        /*************************************************************************/


        Boolean CCubist::Matches(CRule R, DataRec Case)
        /*      -------  */
        {
            int d;

            ForEach(d, 1, R->Size)
            {
            if ( ! Satisfies(Case, R->Lhs[d]) )
            {
                return false;
            }
            }

            return true;
        }



        /*************************************************************************/
        /*								         */
        /*	Evaluate a linear model on a case				 */
        /*								         */
        /*************************************************************************/


        float CCubist::RawLinModel(double *Model, DataRec Case)
        /*    -----------  */
        {
            double	Sum;
            Attribute	Att, a;

            Sum = Model[0];
            ForEach(a, 1, GEnv.NModelAtt)
            {
            Att = GEnv.ModelAtt[a];
            Sum += Model[Att] * CVal(Case, Att);
            }

            return Sum;
        }



        float CCubist::LinModel(double *Model, DataRec Case)
        /*    --------  */
        {
            float	Raw;

            Raw = RawLinModel(Model, Case);

            return ( Raw < Floor ? Floor :
                 Raw > Ceiling ? Ceiling : Raw );
        }



        /*************************************************************************/
        /*                                                                       */
        /*	Find values predicted by a model for cases Fp to Lp.		 */
        /*                                                                       */
        /*************************************************************************/


        void CCubist::FindPredictedValues(RRuleSet *Cttee, CaseNo Fp, CaseNo Lp)
        /*   -------------------  */
        {
            CaseNo	i;

            ForEach(i, Fp, Lp)
            {
            PredVal(Case[i]) = ( USEINSTANCES ? NNEstimate(Cttee, Case[i]) :
                                PredictValue(Cttee, Case[i]) );
            }
        }

    }
    
}