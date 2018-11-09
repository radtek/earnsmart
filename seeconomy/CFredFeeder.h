/* 
 * File:   CFredFeeder.h
 * Author: santony
 *
 * Created on August 1, 2014, 4:10 PM
 */

#ifndef CFREDFEEDER_H
#define	CFREDFEEDER_H

#include "CAbstractFeeder.h"


namespace se
{
    
    namespace economy
    {
        class CFredFeeder : public CAbstractFeeder
        {
        public:
            CFredFeeder();
            CFredFeeder(const CFredFeeder& orig);
            virtual ~CFredFeeder();
            
        public:
            virtual long unsigned int Update(unsigned long eid, const Row& row);
            
        private:

        };

    }
    
}

#endif	/* CFREDFEEDER_H */

