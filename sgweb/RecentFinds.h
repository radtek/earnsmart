/* 
 * File:   RecentFinds.h
 * Author: santony
 *
 * Created on September 10, 2013, 8:21 PM
 */

#ifndef RECENTFINDS_H
#define	RECENTFINDS_H

#include "ModelCommon.h"

using namespace se;
using namespace se::securities;


namespace sg
{
    namespace web
    {
        namespace models
        {
            class RecentFinds : public BaseContent
            {
            public:
                RecentFinds(cppcms::http::context& httpctxt);
                virtual void Init();
                
                void operator+=(int productid);
                void Clear();   // clear RECENTFINDS session data
                ErrorId get_RecentFinds(ISPList<ISecurityDescription>** ppOut) ;
                
                
            };
            
       }
    }
}

#endif	/* RECENTFINDS_H */

