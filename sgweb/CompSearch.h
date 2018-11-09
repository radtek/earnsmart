/* 
 * File:   CompSearch.h
 * Author: santony
 *
 * Created on September 10, 2013, 8:20 PM
 */

#ifndef COMPSEARCH_H
#define	COMPSEARCH_H

#include "ModelCommon.h"

using namespace se;
using namespace se::securities;


namespace sg
{
    namespace web
    {
        namespace models
        {
            class CompSearch : public BaseContent
            {
            public:
                std::string name;
               
            public:
                CompSearch(cppcms::http::context& httpctxt);
                
                virtual void Init();
            };
            
        }
    }
}


#endif	/* COMPSEARCH_H */

