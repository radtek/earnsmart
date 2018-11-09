/* 
 * File:   CGeoIPProvider.h
 * Author: santony
 *
 * Created on August 16, 2015, 3:15 PM
 */

#ifndef CGEOIPPROVIDER_H
#define	CGEOIPPROVIDER_H


namespace se
{
    namespace net
    {

        class CGeoIPProvider 
        {
        public:
            CGeoIPProvider();
            CGeoIPProvider(const CGeoIPProvider& orig);
            virtual ~CGeoIPProvider();
            
            SE_CODE get_GeoIpCountry(CSTR ipAddress, IString** ppOut);

        private:

        };

    }
    
}

#endif	/* CGEOIPPROVIDER_H */

