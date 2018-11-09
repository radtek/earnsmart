/* 
 * File:   CDataFieldList.cpp
 * Author: santony
 * 
 * Created on December 4, 2012, 7:16 AM
 */

#include "CDataTable.h"
#include "CDataFieldList.h"
#include <linux/futex.h>
#include <dirent.h>
#include <algorithm>

namespace se
{
    namespace data
    {


        CDataFieldList::CDataFieldList(ISession* ps):CObjectRoot(ps) 
        {
        }

        CDataFieldList::CDataFieldList(const CDataFieldList& orig) : CObjectRoot(orig)
        {
            _fieldMap = orig._fieldMap;
            _fields = orig._fields;
        }

        CDataFieldList::~CDataFieldList() 
        {
            _fieldMap.clear();
            _fields.clear();
        }
        
        CDataFieldList::CDataFieldList(CDataFieldList&& other):CObjectRoot(other)
        {
            _fields = move(other._fields);
            _fieldMap = move(other._fieldMap);
        }
        
        CDataFieldList& CDataFieldList::operator =(CDataFieldList&& other)
        {
            if (this != &other)
            {
                _fields = move(other._fields);
                _fieldMap = move(other._fieldMap);
            }
            return *this;
        }

        CDataFieldList& CDataFieldList::operator =(const CDataFieldList& other)
        {
            if (this != &other)
            {
                _fields = other._fields;
                _fieldMap = other._fieldMap;
            }
            return *this;
        }
        
        U32 CDataFieldList::AddField(string fieldName, DATATYPES dataType)
        {
            // atomic lock in future
            auto find = _fieldMap.find(fieldName);
            if (find == _fieldMap.end())
            {
                int pos = _fieldMap.size() ;
                _fields.insert(std::make_pair(pos, FIELDINFO(fieldName, dataType)));
                _fieldMap.insert(std::make_pair(fieldName, pos));
            }
        }
        
        void CDataFieldList::RemoveField(const string& fieldName)
        {
            auto find = _fieldMap.find(fieldName);
            if (find != _fieldMap.end())
            {
                U32 pos = find->second;
                _fields.erase(pos);
                _fieldMap.erase(find);
            }
        }
        
        void CDataFieldList::RemoveField(unsigned int fieldIndex)
        {
            auto find = _fields.find(fieldIndex);
            if (find != _fields.end())
            {
                string& name = get<0>(find->second);
                _fieldMap.erase(name);
                _fields.erase(find);
            }
        }

        DATATYPES CDataFieldList::get_Type(const string& fieldName)
        {
            auto find = _fieldMap.find(fieldName);
            if (find != _fieldMap.end())
            {
                return get<1>(_fields[find->second]);
            }
            return DT_ERROR;
        }
        
        const string& CDataFieldList::get_FieldName(U32 id) const
        {
            auto find = _fields.find(id);
            if (find != _fields.end())
            {
                return get<0>(find->second);
            }
            return _empty;
        }

        U32 CDataFieldList::get_FieldId(const string& name) const
        {
            auto find = _fieldMap.find(name);
            if (find != _fieldMap.end())
                return find->second;
            return 0; 
        }
        
        bool CDataFieldList::get_HasField(const string& fieldName) const
        {
            auto find = _fieldMap.find(fieldName);
            return (find != _fieldMap.end());
        }
        
        bool CDataFieldList::TryGetFieldId(const string& name, U32* fieldId)
        {
            auto find = _fieldMap.find(name);
            if (find != _fieldMap.end())
            {
                *fieldId = find->second;
                return true;
            }
            return false;
        }
    }
    
}