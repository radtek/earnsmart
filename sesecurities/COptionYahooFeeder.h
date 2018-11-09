/* 
 * File:   COptionYahooFeeder.h
 * Author: santony
 *
 * Created on August 11, 2014, 11:48 PM
 */

#ifndef COPTIONYAHOOFEEDER_H
#define	COPTIONYAHOOFEEDER_H

#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include "COptionFeeder.h"

namespace se
{
    namespace securities
    {

        class COptionYahooFeeder : public COptionFeeder
        {
        public:
            COptionYahooFeeder(ISession*);
            COptionYahooFeeder(const COptionYahooFeeder& orig)=delete;
            virtual ~COptionYahooFeeder();
        private:

        };
        
    }
    
}

#endif	/* COPTIONYAHOOFEEDER_H */

