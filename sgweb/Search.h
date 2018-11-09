/* 
 * File:   Search.h
 * Author: santony
 *
 * Created on September 10, 2013, 8:35 PM
 */

#ifndef SEARCH_H
#define	SEARCH_H

#include "models.h"

namespace sg
{
    namespace web
    {
        namespace models
        {

            class Search : public Master 
            {
            public:
                Search(cppcms::http::context& httpctxt);
                virtual ~Search();
                
                virtual void Init() ;
                
            private:

            };

        }
    }
#endif	/* SEARCH_H */

