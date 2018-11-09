/* 
 * File:   CGICList.h
 * Author: santony
 *
 * Created on March 25, 2014, 11:45 AM
 */

#ifndef CGICLIST_H
#define	CGICLIST_H

#include "../seglobal/CROList.h"

namespace se
{
    namespace securities
    {
        class CGICList : public CROSPList<IGICInfo, IGICList>
        {
        public:
            CGICList(ISession*);
            CGICList(const CGICList& orig)=delete;
            virtual ~CGICList();

        private:

        };
    }
}


#endif	/* CGICLIST_H */

