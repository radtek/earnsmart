/* 
 * File:   CTarExtractor.h
 * Author: santony
 *
 * Created on November 2, 2012, 10:43 PM
 */

#ifndef CTAREXTRACTOR_H
#define	CTAREXTRACTOR_H

namespace se
{
    namespace utils
    {

        class CTarExtractor 
        {
        public:
            CTarExtractor();
            CTarExtractor(const CTarExtractor& orig);
            virtual ~CTarExtractor();

        public:
            static bool Extract(CSTR TarFile, CSTR ExtractDir);


        private:

        };
    }
}

#endif	/* CTAREXTRACTOR_H */

