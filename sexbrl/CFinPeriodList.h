/* 
 * File:   CFinPeriodList.h
 * Author: santony
 *
 * Created on June 16, 2014, 8:22 PM
 */

#ifndef CFINPERIODLIST_H
#define	CFINPERIODLIST_H

#include "../seglobal/CList.h"
#include "sexbrlapi.h"

namespace se
{
    namespace xbrl
    {
        class CFinPeriodList : public CList<FINPRDDATA, IFinPeriodList>
        {
        public:
            CFinPeriodList(ISession*);
            CFinPeriodList(const CFinPeriodList& orig) = delete;
            virtual ~CFinPeriodList();

            virtual SE_CODE get(DATAPERIOD period, OmniType* pOut);
            
            virtual void Add(FINPRDDATA item);

        protected:

            virtual void SetSerializedValue(ISerializedData<IString>* pIn, const char* PropName, const FINPRDDATA& value){}

            virtual FINPRDDATA GetSerializedvalue(ISerializedData<IString>* pIn, const char* propName){ return FINPRDDATA();}
            
        private:
            typedef std::map<DATAPERIOD, SIZE> POS;
            POS _pos;

        };
        
    }
}


#endif	/* CFINPERIODLIST_H */

