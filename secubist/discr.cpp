

/*************************************************************************/
/*									 */
/*	Evaluation of a test on a discrete valued attribute		 */
/*      ---------------------------------------------------		 */
/*									 */
/*************************************************************************/

#include "defns.h"
#include "CCubist.h"


#define	 Reasonable ((GEnv.BrFreq[1]>=MINSPLIT) + (GEnv.BrFreq[2]>=MINSPLIT) +\
		     (GEnv.BrFreq[3]>=MINSPLIT) >= 2)

namespace se
{
    namespace math
    {



        /*************************************************************************/
        /*									 */
        /*	Set Gain[] for discrete partition of items Fp to Lp		 */
        /*									 */
        /*************************************************************************/


        void CCubist::EvalDiscreteAtt(Tree Node, Attribute Att, CaseNo Fp, CaseNo Lp)
        /*   ---------------  */ 
        { 
            if ( MaxAttVal[Att] == 3 )
            {
            EvalBinarySplit(Node, Att, Fp, Lp);
            }
            else
            {
            EvalSubsetSplit(Node, Att, Fp, Lp);
            }

            Verbosity(2,
            if ( GEnv.Gain[Att] >= 0 )
            {
                fprintf(Of, "Att %s: gain %.3f\n", AttName[Att], GEnv.Gain[Att]);
            }
            else
            {
                fprintf(Of, "Att %s: no gain\n", AttName[Att]);
            })
        } 



        /*************************************************************************/
        /*									 */
        /*	Special case of binary split					 */
        /*									 */
        /*************************************************************************/


        void CCubist::EvalBinarySplit(Tree Node, Attribute Att, CaseNo Fp, CaseNo Lp)
        /*   ---------------  */ 
        { 
            CaseNo	i;
            DiscrValue	v;
            ContValue	Cv;
            double	Wt;

            ForEach(v, 1, 3)
            {
            GEnv.BrFreq[v] = GEnv.BrSum[v] = GEnv.BrSumSq[v] = 0;
            }

            ForEach(i, Fp, Lp)
            {
            Wt = CWeight(Case[i]);
            v  = DVal(Case[i], Att);

            GEnv.BrFreq[v]  += Wt;
            GEnv.BrSum[v]   += Wt * (Cv = Resid(Case[i]));
            GEnv.BrSumSq[v] += Wt * Cv * Cv;
            }

            GEnv.Gain[Att] = ( Reasonable ? ComputeGain(Node) : None );
        }



        /*************************************************************************/
        /*									 */
        /*	Divide attribute values into two subsets			 */
        /*									 */
        /*************************************************************************/


        void CCubist::EvalSubsetSplit(Tree Node, Attribute Att, CaseNo Fp, CaseNo Lp)
        /*   ---------------  */ 
        { 
            CaseNo	i;
            DiscrValue	v, sv, Cycle;
            ContValue	Cv;
            int		Bytes;
            double	Wt, ThisGain;

            ForEach(v, 1, MaxAttVal[Att])
            {
            GEnv.ValFreq[v] = GEnv.ValSum[v] = GEnv.ValSumSq[v] = 0;
            }

            ForEach(i, Fp, Lp)
            {
            Wt = CWeight(Case[i]);

            v = DVal(Case[i], Att);
            GEnv.ValFreq[v]  += Wt;
            GEnv.ValSum[v]   += Wt * (Cv = Resid(Case[i]));
            GEnv.ValSumSq[v] += Wt * Cv * Cv;
            }

            /*  Initialise branch counts and sums  */

            GEnv.BrFreq[1]  = GEnv.ValFreq[1];
            GEnv.BrSum[1]   = GEnv.ValSum[1];
            GEnv.BrSumSq[1] = GEnv.ValSumSq[1];

            Bytes = (MaxAttVal[Att]>>3) + 1;
            ClearBits(Bytes, GEnv.Subset[Att][1]);
            SetBit(1, GEnv.Subset[Att][1]);

            ForEach(v, 2, 3)
            {
            GEnv.BrFreq[v] = GEnv.BrSum[v] = GEnv.BrSumSq[v] = 0;
            }

            /*  Examine subsets, starting with all values in the left branch.
            At each iteration, move the value with the highest mean from
            the left branch to the right branch and check the gain.
            (In the case of ordered attributes, the value moved is the
            rightmost value in the left branch.) Save the best gain so far
            in GEnv.Subset[Att]. */

            ForEach(v, 2, MaxAttVal[Att])
            {
            GEnv.BrFreq[2]  += GEnv.ValFreq[v];
            GEnv.BrSum[2]   += GEnv.ValSum[v];
            GEnv.BrSumSq[2] += GEnv.ValSumSq[v];
            }

            ForEach(v, 2, MaxAttVal[Att])
            {
            GEnv.Left[v] = ( GEnv.ValFreq[v] > 0 );
            }

            ForEach(Cycle, 2, MaxAttVal[Att])
            {
            if ( Ordered(Att) )
            {
                for ( sv = MaxAttVal[Att] ; sv > 1 && ! GEnv.Left[sv] ; sv-- )
                ;
            }
            else
            {
                sv = 0;

                ForEach(v, 2, MaxAttVal[Att])
                {
                if ( GEnv.Left[v] &&
                     ( ! sv ||
                       GEnv.ValSum[v] / GEnv.ValFreq[v] >
                       GEnv.ValSum[sv] / GEnv.ValFreq[sv] ) )
                {
                    sv = v ;
                }
                }
            }

            if ( sv < 2 ) break;

            GEnv.Left[sv] = false;

            GEnv.BrFreq[2]  -= GEnv.ValFreq[sv];
            GEnv.BrSum[2]   -= GEnv.ValSum[sv];
            GEnv.BrSumSq[2] -= GEnv.ValSumSq[sv];
            GEnv.BrFreq[3]  += GEnv.ValFreq[sv];
            GEnv.BrSum[3]   += GEnv.ValSum[sv];
            GEnv.BrSumSq[3] += GEnv.ValSumSq[sv];

            ThisGain = ( Reasonable ? ComputeGain(Node) : None );
            if ( ThisGain > GEnv.Gain[Att] )
            {
                GEnv.Gain[Att] = ThisGain;

                /*  Record in GEnv.Subset[Att]  */

                ClearBits(Bytes, GEnv.Subset[Att][2]);
                ClearBits(Bytes, GEnv.Subset[Att][3]);

                ForEach(v, 2, MaxAttVal[Att])
                {
                if ( GEnv.ValFreq[v] > 0 )
                {
                    SetBit(v, GEnv.Subset[Att][ ( GEnv.Left[v] ? 2 : 3 ) ]);
                }
                }
            }
            }
        }



        /*************************************************************************/
        /*									 */
        /*	Construct and return a node for a test on a discrete attribute	 */
        /*									 */
        /*************************************************************************/


        void CCubist::DiscreteTest(Tree Node, Attribute Att, __Set *Subset)
        /*   ------------  */
        {
            int	S, Bytes;

            Sprout(Node, 3);

            Node->Tested = Att;

            if ( MaxAttVal[Att] == 3 )
            {
            Node->NodeType	= BrDiscr;
            }
            else
            {
            Node->NodeType	= BrSubset;

            Bytes = (MaxAttVal[Att]>>3) + 1;
            Node->Subset = AllocZero(4, __Set);
            ForEach(S, 1, 3)
            {
                Node->Subset[S] = Alloc(Bytes, unsigned char);
                CopyBits(Bytes, Subset[S], Node->Subset[S]);
            }
            }
        } 
    }
}
