/* 
 * File:   CDataset.h
 * Author: santony
 *
 * Created on January 15, 2013, 6:51 AM
 */

#ifndef CDATASET_H
#define	CDATASET_H
#include <map>
#include "CDataTable.h"
#include "../seglobal/CObjectRoot.h"

using namespace std;

namespace se
{
    namespace data
    {
        typedef map<string, CDataTable> TABLEMAP;
        typedef pair<TABLEMAP::const_iterator, TABLEMAP::const_iterator> TABLEITERATOR;

        class CDataset : public CObjectRoot
        {
        public:
            CDataset(ISession*);
            CDataset(const CDataset& orig);
            virtual ~CDataset();
            void AddTable(CSTR tableName, const CDataTable& tableInstance);
            const CDataTable* const GetTable(CSTR tableName) const;
            TABLEITERATOR EnumTables() const;
            
        private:
            TABLEMAP _tables;
            
        };

    }
}
#endif	/* CDATASET_H */

