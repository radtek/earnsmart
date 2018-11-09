/* 
 * File:   CSecurityExchange.h
 * Author: santony
 *
 * Created on March 28, 2014, 10:23 PM
 */

#ifndef CSECURITYEXCHANGE_H
#define	CSECURITYEXCHANGE_H

#include "../seglobal/CIdName.h"

namespace se
{
    namespace securities
    {
        class CSecurityExchange : public CIdName<IExchange>
        {
        public:
            CSecurityExchange(ISession*);
            CSecurityExchange(const CSecurityExchange& orig) = delete;
            virtual ~CSecurityExchange();
            

            virtual const char* get_Code();
            
            virtual void set_Code(CSTR code);
            

            virtual const char* get_Country();

            virtual void set_Country(CSTR country);
            

            virtual const char* get_TimeZone();
            
            virtual void set_TimeZone(CSTR zone);


            
        private:
            string _code, _country, _timezone;

        };
    }
}


#endif	/* CSECURITYEXCHANGE_H */

