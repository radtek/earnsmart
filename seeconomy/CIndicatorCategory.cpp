/* 
 * File:   CIndicatorCategory.cpp
 * Author: santony
 * 
 * Created on January 22, 2014, 5:39 PM
 */

#include "seecconinternal.h"
#include "CIndicatorCategory.h"
#include <string>


namespace se
{
    namespace economy
    {

        //////////////////////CIndicatorCategory///////////////////////
        
        CIndicatorCategory::CIndicatorCategory(ISession* ps) : CObjectRoot(ps)
        {

        }


        CIndicatorCategory::~CIndicatorCategory() 
        {
        }
        
        SE_CODE IIndicatorCategory::Create(ISession* ps, IIndicatorCategory** ppOut)
        {
            return CObject<CIndicatorCategory>::Create(ps, ppOut);
        }

        long unsigned int CIndicatorCategory::Init() 
        {
            return S_OK;
        }

        long unsigned int CIndicatorCategory::Init(const Json::Value& js) 
        {
            // NOTE THIS IS ONLY USED WITH FRED DATASOURCE (NOT APPROPRIATE, OK FOR NOW)
            set_Id(js["id"].asInt64());
            set_Name(js["name"].asCString());
            return S_OK;
        }


        SE_CODE CIndicatorCategory::Init(UID id, const string& name, const string& desc, bool isDisplayed)
        {
            set_Id(id);
            _name = name;
            _desc = desc;
            _isdisplayed = isDisplayed;
            return S_OK;
        }

        unsigned long CIndicatorCategory::get_ParentId() 
        {
            return _parentid;
        }

        void CIndicatorCategory::set_ParentId(unsigned long parentId) 
        {
            _parentid = parentId;
        }

        RP<IIndicatorCategory> CIndicatorCategory::get_Parent() 
        {
            SP<IEconomyApi> papi;
            SP<IIndicatorCategory> pcat;
            LOGIFFAILEDANDTHROW( EconOpen(get_Session(), &papi), "Failed to open econopen api");
            LOGIFFAILEDANDTHROW(papi->get_IndicatorCategory(_parentid, &pcat), "IEconomyAPI::get_IndicatorCategory");
            return pcat;
        }


        
        CSTR CIndicatorCategory::get_Name()
        {
            return _name.c_str();
        }
        
        void CIndicatorCategory::set_Name(const char* name)
        {
            _name = name;
        }
        
        CSTR CIndicatorCategory::get_Description()
        {
            return _desc.c_str();
        }
        
        void CIndicatorCategory::set_Description(const char* desc)
        {
            _desc = desc;
        }
        
        BOOL CIndicatorCategory::get_IsDisplayed()
        {
            return _isdisplayed;
        }
        
        void CIndicatorCategory::set_IsDisplayed(bool v)
        {
            _isdisplayed = v;
        }

        long unsigned int CIndicatorCategory::get_Children(IIndicatorCategoryList**) 
        {
            return E_NotImpl;
        }


        long unsigned int CIndicatorCategory::get_Indicators(bool displayTypesOnly, IEconomyIndicatorList** ppOut) 
        {
            SP<IEconomyApi> pApi;
            RETURNIFFAILED(EconOpen(get_Session(), &pApi));
            return pApi->get_EconomicIndicators(
                    ppOut,
                    [this, displayTypesOnly](IEconomyIndicator::QUERYFLDS flds, LGX& lgx, BOOL& include)
                    {
                        if (flds == IEconomyIndicator::Displayed)
                        {
                            include = true;
                            lgx = LGX_And;
                            return "1";
                        }
                        else if (flds == IEconomyIndicator::CategoryId)
                        {
                            lgx = LGX_And;
                            include = true;
                            std::stringstream str;
                            str << get_Id();
                            return str.str().c_str();
                        }
                        return "";
                    }
            );
            return S_OK;
        }

        long unsigned int CIndicatorCategory::Store() {

        }

        long unsigned int CIndicatorCategory::Delete() {

        }

        
        void CIndicatorCategory::Serialize(ISerializedData<IString>* pIn)
        {
            pIn->Add("Id", get_Id());
            pIn->Add("Name", _name.c_str());
            pIn->Add("Description", _desc.c_str());
            pIn->Add("IsDisplayed", _isdisplayed);
        }

        ///////////////////////CIndicatorCategoryList///////////////////////
        
        IIndicatorCategory* CIndicatorCategoryList::GetSerializedvalue(ISerializedData<IString>* pIn, const char* propName)
        {
            return 0;
        }
        
        void CIndicatorCategoryList::SetSerializedValue(ISerializedData<IString>* pIn, const char* PropName, const IIndicatorCategory*& value)
        {
            pIn->Add(PropName, value);
        }
    }
    
}

