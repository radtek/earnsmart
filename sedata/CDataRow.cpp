/* 
 * File:   CDataRow.cpp
 * Author: santony
 * 
 * Created on December 4, 2012, 7:17 AM
 */

#include "sedataheaders.h"
#include "CDataRow.h"
#include <algorithm>
#include <errno.h>

namespace se
{
    namespace data
    {


        CDataRow::CDataRow(ISession* ps, CDataTable* const datatable , UID rowId, bool init) : CObjectRoot(ps), _bInit(init), _pDataTable(datatable)
        {
            if (!datatable)
                throw std::bad_alloc();
            _pDataTable->Acquire();
            // initialize fieldlist array.
            this->set_Id(rowId);
            auto& fieldList = _pDataTable->get_Fields();
            if (_bInit)
            {
                _fieldSize = fieldList.get_Size();
                _fieldList = new FIELDLIST[_fieldSize];
            }
            else
            {
                _fieldSize = 0;
                _fieldList = nullptr;
            }
        }
        
        CDataRow::CDataRow(CDataRow&& other):CObjectRoot(other)
        {
            _fieldList = other._fieldList;
            other._fieldList = nullptr;
            _pDataTable = other._pDataTable;
            other._pDataTable = nullptr;
            _fieldSize = other._fieldSize;
            other._fieldSize = 0;
            set_Id(other.get_Id());
        }

        CDataRow::CDataRow(const CDataRow& orig):CObjectRoot(orig) 
        {
            _pDataTable = orig._pDataTable;
            _pDataTable->Acquire();
            _fieldSize = orig._fieldSize;
            _fieldList = new FIELDLIST[_fieldSize];
            for (int i=0; i!=_fieldSize; ++i)
            {
                // alocate memory
                _fieldList[i] = orig._fieldList[i];
            }
            CObjectRoot::_set_Id(orig._get_Id());
        }
        
        CDataRow& CDataRow::operator =(const CDataRow& orig)
        {
            if (this != &orig)
            {
                if (_pDataTable)
                    _pDataTable->Release();
                _pDataTable = orig._pDataTable;
                _pDataTable->Acquire();
                _fieldSize = orig._fieldSize;
                _fieldList = new FIELDLIST[_fieldSize];
                for (int i=0; i!=_fieldSize; ++i)
                {
                    _fieldList[i] = orig._fieldList[i];
                }
                _set_Id(orig._get_Id());
            }
            return *this;
        }
        
        CDataRow& CDataRow::operator =(CDataRow&& other)
        {
            if (this != &other)
            {
                _fieldList = other._fieldList;
                other._fieldList = nullptr;
                _pDataTable = other._pDataTable;
                other._pDataTable = nullptr;
                _fieldSize = other._fieldSize;
                other._fieldSize = 0;
                set_Id(other.get_Id());
            }
            return *this;
        }
        

        CDataRow::~CDataRow() 
        {
            delete [] _fieldList;
            if (_pDataTable)
                _pDataTable->Release();
        }

        CDataRow::operator  bool() const
        {
            return _bInit;
        }
        
        UID CDataRow::get_RowId() const
        {
            return _get_Id();
        }
        
    }
}