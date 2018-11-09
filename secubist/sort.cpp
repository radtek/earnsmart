

/*************************************************************************/
/*									 */
/*	Sorting utilities						 */
/*	-----------------						 */
/*									 */
/*************************************************************************/


#include "defns.h"
#include "CCubist.h"


#define SwapSRec(a,b)	{Xab=SRec[a]; SRec[a]=SRec[b]; SRec[b]=Xab;}

namespace se
{
    namespace math
    {



        /*************************************************************************/
        /*									 */
        /*	Sort elements Fp to Lp of SRec					 */
        /*									 */
        /*************************************************************************/


        void CCubist::Cachesort(CaseNo Fp, CaseNo Lp)
        /*   ---------  */
        {
            CaseNo	i, Middle, High;
            ContValue	Thresh, Val;
            SortRec	Xab;

            while ( Fp < Lp )
            {
            Thresh = SRec[(Fp+Lp) / 2].V;

            /*  Divide elements into three groups:
                Fp .. Middle-1: values < Thresh
                Middle .. High: values = Thresh
                High+1 .. Lp:   values > Thresh  */

            for ( Middle = Fp ; SRec[Middle].V < Thresh ; Middle++ )
                ;

            for ( High = Lp ; SRec[High].V > Thresh ; High-- )
                ;

            for ( i = Middle ; i <= High ; )
            {
                if ( (Val = SRec[i].V) < Thresh )
                {
                SwapSRec(Middle, i);
                Middle++;
                i++;
                }
                else
                if ( Val > Thresh )
                {
                SwapSRec(High, i);
                High--;
                }
                else
                {
                i++;
                }
            }

            /*  Sort the first group  */

            Cachesort(Fp, Middle-1);

            /*  Continue with the last group  */

            Fp = High+1;
            }
        }

    }
    
}