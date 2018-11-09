/* 
 * File:   CDataRowList.cpp
 * Author: santony
 * 
 * Created on December 8, 2012, 3:56 PM
 */

#include "sedataheaders.h"
#include "CDataRowList.h"

namespace se
{
    namespace data
    {

        CDataRowList::CDataRowList(ISession* ps, CDataTable* const table) : CObjectRoot(ps), _falseRow (table->get_NewFalseRow() )
        {
            _table = table;
            _table->Acquire();
        }

        CDataRowList::CDataRowList(const CDataRowList& orig) :CObjectRoot(orig),  _falseRow(orig._falseRow)
        {
            this->_defindex = orig._defindex;
            this->_rows = orig._rows;
            this->_table = orig._table;
        }

        CDataRowList::~CDataRowList() 
        {
            if (_table)
            {
                _table->Release();
            }
        }

        CDataRowList::operator  bool() const
        {
            return !!_rows.size();
        }

        void CDataRowList::AddRow(const CDataRow& row)
        {
            auto size = _rows.size();
            _rows.push_back(row);
            _defindex.insert(make_pair(row.get_RowId(), size));
        }

        bool CDataRowList::RemoveRow(const CDataRow& row)
        {
            auto find = _defindex.find(row.get_RowId());
            if (find != _defindex.end())
            {
                auto index = find->second;
                _rows.erase(_rows.begin() + index);
                _defindex.erase(find);
            }
        }
        
        CDataRow& CDataRowList::operator[](int index)
        {
            return get_Row(index);
        }

        CDataRow& CDataRowList::get_Row(int index)
        {
            if (index < 0 || index >= _rows.size())
                return _falseRow;
            return _rows[index];
        }
        
        ROWLISTITERATOR CDataRowList::EnumerateRows()
        {
            return ROWLISTITERATOR(_rows.begin(), _rows.end());
        }
        
    }
}