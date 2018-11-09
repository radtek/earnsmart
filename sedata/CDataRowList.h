/* 
 * File:   CDataRowList.h
 * Author: santony
 *
 * Created on December 8, 2012, 3:56 PM
 */

#ifndef CDATAROWLIST_H
#define	CDATAROWLIST_H


#include <map>
#include <vector>
#include <boost/any.hpp>

#include "CDataRow.h"


using namespace std;
using namespace boost;

namespace se
{
    namespace data
    {
        typedef map<UID, SIZE> INDEXFILE;
        typedef vector<CDataRow> ROWS;
        typedef std::pair<ROWS::iterator, ROWS::iterator> ROWLISTITERATOR;
        
        class CDataRowList : public CObjectRoot
        {
            
        public:
            CDataRowList(ISession*, CDataTable* const table);
            CDataRowList(const CDataRowList& orig);
            virtual ~CDataRowList();
            
            operator bool() const;
            
            void AddRow(const CDataRow& row);
            
            bool RemoveRow(const CDataRow& row);
            
            // returns a row at the given index. if the index is out-of-range,
            // an uninitalized row is returned (check this against a false condition) (eg. if (row = list[10]) { // row is valid. } )
            CDataRow& operator[](int index);

            // returns a row at the given index. if the index is out-of-range,
            // an uninitalized row is returned (check this against a false condition) (eg. if (row = list.get_Row(10)) { // row is valid. } )
            CDataRow& get_Row(int index);
            
            ROWLISTITERATOR EnumerateRows();
            
        private:
            
            
            CDataTable* _table;
            ROWS _rows;
            INDEXFILE _defindex;
            CDataRow _falseRow;
            
        };
        
    }
}


#endif	/* CDATAROWLIST_H */

