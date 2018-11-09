/* 
 * File:   Subscriptions.h
 * Author: santony
 *
 * Created on September 10, 2013, 8:23 PM
 */

#ifndef SUBSCRIPTIONS_H
#define	SUBSCRIPTIONS_H

#include "ModelCommon.h"
#include "Master.h"

using namespace se;
using namespace se::securities;


namespace sg
{
    namespace web
    {
        namespace models
        {
            class Subscriptions : public Master
            {
            public:
                Subscriptions(cppcms::http::context& httpctxt);
                virtual void Init();
                
            };
       }
    }
}

#endif	/* SUBSCRIPTIONS_H */

