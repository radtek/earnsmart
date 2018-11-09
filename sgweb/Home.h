/* 
 * File:   Home.h
 * Author: santony
 *
 * Created on September 10, 2013, 8:24 PM
 */

#ifndef HOME_H
#define	HOME_H

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
            class Home : public Master
            {
            public:
                
            public:
                
                Home(cppcms::http::context& httpctxt);
                virtual void Init();
            };
            
       }
    }
}
        

#endif	/* HOME_H */

