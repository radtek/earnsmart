/* 
 * File:   CDataFieldList.h
 * Author: santony
 *
 * Created on December 4, 2012, 7:16 AM
 */

#ifndef CDATAFIELDLIST_H
#define	CDATAFIELDLIST_H

#include <map>
#include <tuple>
#include <string>
#include "../seglobal/CObjectRoot.h"
#include "../seglobaltypedefs.h"

using namespace std;

namespace se
{
    namespace data
    {
        
        class CDataTable;
       
        typedef std::tuple<string, DATATYPES> FIELDINFO;
        
        class CDataFieldList : public CObjectRoot
        {
        public:
            CDataFieldList(ISession*);   // only datatable can create this instance.
            CDataFieldList(CDataFieldList&&);
        public:
            CDataFieldList(const CDataFieldList& orig);
            virtual ~CDataFieldList();
            
            U32 AddField(string fieldName, DATATYPES dataType);
            void RemoveField(const string& fieldName);
            void RemoveField(U32 fieldId);
            DATATYPES get_Type(const string& fieldName);
            
            CDataFieldList& operator=(CDataFieldList&&);
            
            CDataFieldList& operator=(const CDataFieldList&);
            
            const string& get_FieldName(U32 ) const;
            U32 get_FieldId(const string&) const;
            
            bool TryGetFieldId(const string&, U32* fieldId);
            
            bool get_HasField(const string&) const;

            SIZE get_Size() const { return _fields.size() ; }
            
        private:
            friend class CDataTable;

            typedef string FIELDNAME;
            typedef map<U32, FIELDINFO> FIELDSINFO;
            typedef map<string, U32> FIELDINDEXNAMEMAP;
            
            FIELDSINFO _fields;
            FIELDINDEXNAMEMAP _fieldMap;
            
            string _empty;
        };
    }
}
#endif	/* CDATAFIELDLIST_H */

