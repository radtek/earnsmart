/* 
 * File:   CFieldVariant.h
 * Author: santony
 *
 * Created on December 5, 2012, 8:43 AM
 */

#ifndef CFIELDVARIANT_H
#define	CFIELDVARIANT_H

#include <boost/variant.hpp>
#include <string>

using namespace boost;

namespace se
{
    namespace data
    {

        class CFieldVariantVisitor  : public static_visitor<>
        {
        public:
            CFieldVariantVisitor();
            CFieldVariantVisitor(const CFieldVariantVisitor& orig);
            virtual ~CFieldVariantVisitor();

            int operator++(int i);
            
        private:

        };

    }
}

#endif	/* CFIELDVARIANT_H */

