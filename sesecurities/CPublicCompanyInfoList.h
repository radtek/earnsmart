/* 
 * File:   CPublicCompanyInfoList.h
 * Author: santony
 *
 * Created on March 25, 2014, 1:30 PM
 */

#ifndef CPUBLICCOMPANYINFOLIST_H
#define	CPUBLICCOMPANYINFOLIST_H

#include "../seglobal/CROList.h"
#include "sesecuritiesapi.h"

namespace se
{
    namespace securities
    {
        class CPublicCompanyInfoList : public CROSPList<IPublicCompanyInfo, IPublicCompanyInfoList>
        {
        public:
            CPublicCompanyInfoList(ISession*);
            CPublicCompanyInfoList(const CPublicCompanyInfoList& orig)=delete;
            virtual ~CPublicCompanyInfoList();
        private:

        };
        
    }
}


#endif	/* CPUBLICCOMPANYINFOLIST_H */

