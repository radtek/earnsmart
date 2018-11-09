/* 
 * File:   CPublicCompanyInfoList.cpp
 * Author: santony
 * 
 * Created on March 25, 2014, 1:30 PM
 */

#include "internal.h"
#include "CPublicCompanyInfoList.h"

namespace se
{
    namespace securities
    {

        CPublicCompanyInfoList::CPublicCompanyInfoList(ISession* ps):CROSPList<IPublicCompanyInfo, IPublicCompanyInfoList>(ps) 
        {
        }

        CPublicCompanyInfoList::~CPublicCompanyInfoList() 
        {
        }

    }
}