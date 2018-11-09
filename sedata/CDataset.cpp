/* 
 * File:   CDataset.cpp
 * Author: santony
 * 
 * Created on January 15, 2013, 6:51 AM
 */


#include "CDataset.h"

namespace se
{
    namespace data
    {

        CDataset::CDataset(ISession* ps):CObjectRoot(ps)
        {
        }

        CDataset::CDataset(const CDataset& orig) : CObjectRoot(orig)
        {
        }

        CDataset::~CDataset() 
        {
        }
        
        void CDataset::AddTable(CSTR tableName, const CDataTable& tableInstance)
        {
            _tables.insert(make_pair((string)tableName, std::move(tableInstance)));
        }
        
        const CDataTable* const CDataset::GetTable(CSTR tableName) const
        {
            auto find = _tables.find(tableName);
            if (find == _tables.end())
                return nullptr;
            return &find->second;
        }
        
        TABLEITERATOR CDataset::EnumTables() const
        {
            return TABLEITERATOR(_tables.begin(), _tables.end());
        }
        
    }
}
