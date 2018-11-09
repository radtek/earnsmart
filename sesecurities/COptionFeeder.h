/* 
 * File:   COptionFeeder.h
 * Author: santony
 *
 * Created on August 11, 2014, 11:48 PM
 */

#ifndef COPTIONFEEDER_H
#define	COPTIONFEEDER_H

namespace se
{
    namespace securities
    {

        class COptionFeeder : public CObjectRoot, public IOptionFeeder
        {
        public:
            COptionFeeder(ISession*);
            COptionFeeder(const COptionFeeder& orig)=delete;
            virtual ~COptionFeeder();
        private:

        };

    }
    
}

#endif	/* COPTIONFEEDER_H */

