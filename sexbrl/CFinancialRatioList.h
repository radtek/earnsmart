/* 
 * File:   CFinancialRatioList.h
 * Author: santony
 *
 * Created on February 14, 2014, 5:01 PM
 */

#ifndef CFINANCIALRATIOLIST_H
#define	CFINANCIALRATIOLIST_H

#include "sexbrlapi.h"
#include "../seglobal/CROList.h"
#include "CFinancialRatio.h"

namespace se
{
    namespace xbrl
    {


        class CFinancialRatioList : public CROSPList<IFinancialRatio, IFinancialRatioList>
        {
        public:
            CFinancialRatioList(ISession*);
            CFinancialRatioList(const CFinancialRatioList& orig)=delete;
            virtual ~CFinancialRatioList();
            virtual long unsigned int get_Ratio(FNR ratio, IFinancialRatio** ppOut);
            virtual void Add(IFinancialRatio* item);
            virtual void AddRange(IFinancialRatioList* pIn);
            
        private:
            typedef std::map<UID, IFinancialRatio*> Map;
            Map _m;

        };

        typedef std::map<UID, SP<CFinancialRatioGroup> > GROUPMAP;
        
        class CFinancialRatioGroupList : public CROSPList<IFinancialRatioGroup, IFinancialRatioGroupList>
        {
        public:
            CFinancialRatioGroupList(ISession*);
            CFinancialRatioGroupList(const CFinancialRatioGroupList& orig)=delete;
            virtual ~CFinancialRatioGroupList();

            virtual long unsigned int get_Ratios(unsigned long groupId, IFinancialRatioList** ppOut);
            // returns a single list from all groups.
            virtual SE_CODE get_JoinedList(IFinancialRatioList**);
            
        private:
            GROUPMAP _gmap;
            
        };        

    }
}

#endif	/* CFINANCIALRATIOLIST_H */

