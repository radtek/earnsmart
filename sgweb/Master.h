/* 
 * File:   Master.h
 * Author: santony
 *
 * Created on September 10, 2013, 8:22 PM
 */

#ifndef MASTER_H
#define	MASTER_H

#include "CompSearch.h"
#include "RecentFinds.h"

using namespace se;
using namespace se::securities;


namespace sg
{
    namespace web
    {
        namespace models
        {
            class Master : public BaseContent
            {
            public:
                
                string Title;
                string CName;
                Footer footer;
                Header header;
                
                PID pid;
                CompSearch compSearch;  // search may be used from any page, so moving to master model.
                RecentFinds recentFinds;
                
            public:
                Master(cppcms::http::context& httpctxt);
                virtual void Init();
            };
            
       }
    }
}

#endif	/* MASTER_H */

