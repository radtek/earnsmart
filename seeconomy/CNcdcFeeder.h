/* 
 * File:   CNcdcFeeder.h
 * Author: santony
 *
 * Created on August 1, 2014, 4:38 PM
 */

#ifndef CNCDCFEEDER_H
#define	CNCDCFEEDER_H

#include "CAbstractFeeder.h"


namespace se
{
    
    namespace economy
    {

        class CNcdcFeeder : public CAbstractFeeder
        {
        public:
            CNcdcFeeder();
            CNcdcFeeder(const CNcdcFeeder& orig);
            virtual ~CNcdcFeeder();
            

            virtual long unsigned int Update(unsigned long eid, const Row& row);

            
        private:

        };

    }
    
}
        
#endif	/* CNCDCFEEDER_H */

