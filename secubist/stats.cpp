


#include "defns.h"
#include "CCubist.h"

namespace se
{
    namespace math
    {



        double CCubist::AverageDev(float Mean, CaseNo Fp, CaseNo Lp)
        /*     ----------  */
        {
            double	Wt, SumWt=0, Sum=0;
            CaseNo	i;

            ForEach(i, Fp, Lp)
            {
            Wt = CWeight(Case[i]);

            SumWt += Wt;
            Sum   += Wt * fabs(Mean - Class(Case[i]));
            }

            return Sum / SumWt;
        }



        double CCubist::SD(double N, double Sum, double SumSq)
        /*     --  */
        {
            return ( N < 2 ? GlobalSD :
                 sqrt( (SumSq - Sum * Sum / N + 1E-3) / (N - 1) ) );
        }



        double CCubist::AverageErr(DataRec *D, CaseNo Fp, CaseNo Lp, double *Model)
        /*     ----------  */
        {
            CaseNo	i;
            double	Wt, Sum=0, SumWt=0;

            FindModelAtts(Model);

            ForEach(i, Fp, Lp)
            {
            Wt = CWeight(Case[i]);

            Sum   += Wt * fabs(Class(D[i]) - LinModel(Model, D[i]));
            SumWt += Wt;
            }

            return Sum / SumWt;
        }



        double CCubist::ComputeGain(Tree Node)
        /*     -----------  */
        {
            double	Resid=0, Cases=0;
            DiscrValue	v;

            ForEach(v, 1, 3)
            {
            Cases += GEnv.BrFreq[v];
            Resid += GEnv.BrFreq[v] *
                 SD(GEnv.BrFreq[v], GEnv.BrSum[v], GEnv.BrSumSq[v]);
            }

            return Node->SD - Resid / Cases;
        }



        double CCubist::EstimateErr(double Val, double NData, float NParam)
        /*     -----------  */
        {
            if ( NParam >= NData ) NParam = NData - 1;

            return Val * (NData + NParam) / (NData - NParam);
        }

    }
    
}