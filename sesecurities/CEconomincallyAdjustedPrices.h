/* 
 * File:   CEconomincallyAdjustedPrices.h
 * Author: santony
 *
 * Created on November 18, 2013, 11:20 AM
 */

#ifndef CECONOMINCALLYADJUSTEDPRICES_H
#define	CECONOMINCALLYADJUSTEDPRICES_H

#include "sesecuritiesapi.h"


namespace se
{
    namespace securities
    {
        class CEconomincallyAdjustedPrices : public CObjectRoot, public IEconomicallyAdjustedPrices
        {
            R8* _v;

        public:
            CEconomincallyAdjustedPrices(ISession*);
            CEconomincallyAdjustedPrices(const CEconomincallyAdjustedPrices& orig)=delete;
            virtual ~CEconomincallyAdjustedPrices();

            virtual R8 get_Value(EAM economicAdjustor) ;
            void set_Value(EAM economicAdjustor, R8 value);
            
        private:

        };
    }
}


#endif	/* CECONOMINCALLYADJUSTEDPRICES_H */

