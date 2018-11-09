/* 
 * File:   CDataTable.cpp
 * Author: santony
 * 
 * Created on December 4, 2012, 7:15 AM
 */

#include "CDataRowList.h"
#include "CDataRow.h"
#include "CDataTable.h"
#include <algorithm>

namespace se
{
    namespace data
    {
        
        CDataTable::CDataTable(ISession* ps, string tableName, bool init) : CObjectRoot(ps), _fields(ps), _tableName(tableName), _bInit(init), _rows(NULL)
        {
        }

        CDataTable::CDataTable(CDataTable&& other) : 
            CObjectRoot(other), 
                _fields(other._fields),
                _bInit(other._bInit)
        {
            _rows = other._rows; other._rows = NULL;
            _tableName = std::move(other._tableName);
        }

        CDataTable::CDataTable(const CDataTable& orig) : 
                CObjectRoot(orig),
                _fields(orig._fields),
                _bInit(orig._bInit)
        {
            _tableName = orig._tableName;
            orig._rows->Acquire();
            _rows = orig._rows;
        }

        CDataTable::~CDataTable() 
        {
            if (_rows)
            {
                _rows->Release();
                _rows = NULL;
            }
        }
        
        // TODO: move and copy assignment operators.
        
        CDataTable::operator  bool() const
        {
            return _bInit;
        }
        
        CDataFieldList& CDataTable::get_Fields() 
        {
            return _fields;
        }
        
        CSTR CDataTable::get_Name() const
        {
            return _tableName.c_str();
        }
        
        CDataRowList& CDataTable::get_Rows()
        {
            if (!_rows)
            {
                _rows = new CDataRowList(get_Session(), this);
                _rows->Acquire();
            }
            return *_rows;
        }
        
        CDataRow CDataTable::get_NewRow()
        {
            CDataRow row(get_Session(), this, ++_lastrowId, true);
            return row; // return r-value optimization is expected.
        }
        
        
        CDataRow CDataTable::get_NewFalseRow()
        {
            CDataRow row(get_Session(), this, 0, false);
            return row; // return r-value optimization is expected.
        }
    }
}