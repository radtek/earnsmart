/* 
 * File:   CDataTable.h
 * Author: santony
 *
 * Created on December 4, 2012, 7:15 AM
 */

#ifndef CDATATABLE_H
#define	CDATATABLE_H

#include "IDataProvider.h"
#include "CDataFieldList.h"

using namespace std;

namespace se
{
    namespace data
    {
        class CDataRowList;
        class CDataRow;

        class CDataTable : public CObjectRoot, public IDataProvider
        {
            friend class CDataRowList;
            
        public:
            CDataTable(ISession*, string tableName, bool init=true);
            CDataTable(CDataTable&&);
            CDataTable(const CDataTable& orig);
            virtual ~CDataTable();
            
            virtual CSTR ProviderName()
            {
                return "CDataTable";
            }
            
            operator bool() const;
            
            CDataFieldList& get_Fields() ;
            
            CSTR get_Name() const;
            
            CDataRowList& get_Rows() ;
            
            CDataRow get_NewRow();
            
            
            /* TODO ITEMS */
            //Commits all the changes made to this table since the last time AcceptChanges was called.
            //void AcceptChanges();
            //Clears the DataTable of all data.
            //void Clear()
            /* END OF TODO ITEMS */
            
        private:
            CDataRow get_NewFalseRow();
            
        private:
            bool _bInit;
            CDataFieldList _fields;
            string _tableName;
            CDataRowList* _rows;
            UID _lastrowId;
            
        };

    }
}
#endif	/* CDATATABLE_H */

