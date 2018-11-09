/* 
 * File:   CDataRow.h
 * Author: santony
 *
 * Created on December 4, 2012, 7:17 AM
 */

#ifndef CDATAROW_H
#define	CDATAROW_H

#include <boost/any.hpp>
#include <map>

#include "CDataTable.h"

using boost::any_cast;
using namespace std;

namespace se
{
    namespace data
    {
        
        class CDataRow : public CObjectRoot 
        {
            friend class CDataTable;
            friend class CDataRowList;
            
        private:
            CDataRow(ISession*, CDataTable* const, UID rowId, bool init);
            
        public:
            CDataRow(const CDataRow& );
            CDataRow(CDataRow&& );
            virtual ~CDataRow();
            
            CDataRow& operator=(const CDataRow&);
            CDataRow& operator=(CDataRow&&);
            
            template<typename R>
            bool get(U32 fieldIndex, R& out)
            {
                if (fieldIndex < 0 || fieldIndex >= _fieldSize)
                    return false;
                out = boost::any_cast<R>(_fieldList[fieldIndex]);
                return true;
            }
            
            template<typename T>
            bool set(U32 fieldIndex, T value)
            {
                if (fieldIndex < 0 || fieldIndex >= _fieldSize)
                    return false;
                _fieldList[fieldIndex] = value;
            }
            
            template<typename R>
            bool get(CSTR fieldName, R& out)
            {
                U32 fieldindex;
                if (_pDataTable->get_Fields().TryGetFieldId(fieldName, &fieldindex))
                {
                    out = boost::any_cast<R>(_fieldList[fieldindex]);
                    return true;
                }
                return false;
            }
            
            template<typename T>
            bool set(CSTR fieldName, T value)
            {
                U32 fieldindex;
                if (_pDataTable->get_Fields().TryGetFieldId(fieldName, &fieldindex))
                {
                    _fieldList[fieldindex] = value;
                    return true;
                }
                return false;
            }
           
            UID get_Id() 
            {
                return CObjectRoot::_get_Id();
            }

            void set_Id(UID id) 
            {
                CObjectRoot::_set_Id(id);
            }            
            
            operator bool() const ;
            
            SIZE get_FieldSize() { return _fieldSize; }

            UID get_RowId() const ;
            
        private:
            typedef boost::any FIELDLIST;  // an array of fields.
            FIELDLIST* _fieldList;
            CDataTable* _pDataTable;
            bool _bInit;
            int _fieldSize;
            
        };
    }
}
#endif	/* CDATAROW_H */

