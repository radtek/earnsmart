/* 
 * File:   CIndicatorCategory.h
 * Author: santony
 *
 * Created on January 22, 2014, 5:39 PM
 */

#ifndef CINDICATORCATEGORY_H
#define	CINDICATORCATEGORY_H

#include <string>
#include "../seglobal/CList.h"
#include "seeconomapi.h"
using namespace std;

namespace se
{
    namespace economy
    {
        
        class CIndicatorCategory : public CObjectRoot, public IIndicatorCategory
        {
        public:
            CIndicatorCategory(ISession*);
            CIndicatorCategory(const CIndicatorCategory& orig) = delete;
            virtual ~CIndicatorCategory();
            

            virtual unsigned long get_ParentId();
            void set_ParentId(UID parentId);
            

            virtual RP<IIndicatorCategory> get_Parent();

            virtual SE_CODE get_Children(IIndicatorCategoryList**);

            
            virtual CSTR get_Description();
            void set_Description(CSTR desc);
            virtual CSTR get_Name();
            virtual void set_Name(CSTR name);
            virtual bool get_IsDisplayed();
            void set_IsDisplayed(bool v) ;
            
            virtual SE_CODE get_Indicators(bool displayTypesOnly, IEconomyIndicatorList** ppOut);
            
            virtual long unsigned int Init();;

            SE_CODE Init(const Json::Value& js);
            
            SE_CODE Init(UID id, const string& name, const string& desc, BOOL isDisplayed);

            virtual void Serialize(ISerializedData<IString>* pIn);
            

            virtual long unsigned int Store();

            virtual long unsigned int Delete();


            
        private:
            string _name, _desc;
            BOOL _isdisplayed;
            UID _parentid = 0;
        };
        
        class CIndicatorCategoryList : public CSPList<IIndicatorCategory>
        {
        public:
            CIndicatorCategoryList(ISession* ps):CSPList<IIndicatorCategory>(ps){}
        protected:
            virtual IIndicatorCategory* GetSerializedvalue(ISerializedData<IString>* pIn, CSTR propName);
            virtual void SetSerializedValue(ISerializedData<IString>* pIn, CSTR PropName, const IIndicatorCategory*& value);
        };
        
    }
}


#endif	/* CINDICATORCATEGORY_H */

