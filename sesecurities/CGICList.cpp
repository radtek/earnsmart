/* 
 * File:   CGICList.cpp
 * Author: santony
 * 
 * Created on March 25, 2014, 11:45 AM
 */

#include "internal.h"
#include "CGICList.h"

namespace se
{
    namespace securities
    {
        CGICList::CGICList(ISession* ps) : CROSPList<IGICInfo, IGICList>(ps) 
        {
        }

        
        CGICList::~CGICList() 
        {
        }

    }
}