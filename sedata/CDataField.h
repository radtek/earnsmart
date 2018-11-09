/* 
 * File:   CDataField.h
 * Author: santony
 *
 * Created on December 4, 2012, 7:15 AM
 */

#ifndef CDATAFIELD_H
#define	CDATAFIELD_H

namespace se
{
    namespace data
    {
        /* The field definition in a Table. */
        class CDataField 
        {
        public:
            CDataField();
            CDataField(const CDataField& orig);
            virtual ~CDataField();
        private:

        };
        
    }
}


#endif	/* CDATAFIELD_H */

