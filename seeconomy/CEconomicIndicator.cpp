/* 
 * File:   CEconomicIndicator.cpp
 * Author: santony
 * 
 * Created on January 23, 2014, 11:23 AM
 */

#include "seecconinternal.h"
#include "CEconomicIndicator.h"
#include "CEconomyAPI.h"

namespace se
{
    namespace economy
    {

        CEconomicIndicator::CEconomicIndicator(ISession* ps) : CObjectRoot(ps)
        {
        }


        CEconomicIndicator::~CEconomicIndicator() 
        {
        }
        
        SE_CODE CEconomicIndicator::Init()
        {
            return S_OK;
        }
        
        SE_CODE CEconomicIndicator::Init(
                unsigned long id, const string& name, const string& desc, const string& publishperiod, 
                const string& source, const string& reldates, unsigned long catId, bool isdisplayed,
                const string& copyright,
                const string& chartType,
                bool isseasonal,
                const string& tablename
                )
        {
            set_Id(id);
            _name = name;
            _desc = desc;
            _publishperiod = publishperiod;
            _source = source;
            _reldates = reldates;
            _catId = catId;
            _isdisplayed = isdisplayed;
            _chartType = chartType.empty() ? "Line" : chartType;
            _copyright = copyright;
            _isseasonal = isseasonal;
            _tablename = tablename;
            return S_OK;
        }

        // Used with Fred datasource.
        long unsigned int CEconomicIndicator::Init(const Json::Value& js) 
        {
            _code = js["id"].asString();
            _name = js["title"].asString();
            _desc = js["notes"].asString();
            _isseasonal = (js["seasonal_adjustment_short"].asString() != "NSA");
            _publishperiod = js["frequency"].asString();
            _reldates = js["realtime_start"].asString() + "<|>" + js["realtime_end"].asString();
            _unit = js["units"].asString();
            set_Property("frequency_short", js["frequency_short"].asCString());
            return S_OK;
        }
        
        const char* CEconomicIndicator::get_Code() 
        {
            return _code.c_str();
        }

        
        CSTR CEconomicIndicator::get_Name()
        {
            return _name.c_str();
        }
        
        void CEconomicIndicator::set_Name(const char* name)
        {
            _name = name;
        }
        
        UID CEconomicIndicator::get_CategoryId()
        {
            return _catId;
        }

        CSTR CEconomicIndicator::get_Description() 
        {
            return _name.c_str();
        }
        
        CSTR CEconomicIndicator::get_PublishPeriod()
        {
            return _publishperiod.c_str();
        }
        
        CSTR CEconomicIndicator::get_Source() 
        {
            return _source.c_str();
        }
        
        CSTR CEconomicIndicator::get_ReleaseDates() 
        {
            return _reldates.c_str();
        }
        
        SE_CODE CEconomicIndicator::get_Category(IIndicatorCategory** ppOut) 
        {
            SP<IEconomyApi> papi = (IEconomyApi*) get_Session()->get_Object(IEconomyApiName)._ptr;
            if (!papi) return E_InvalidPointer;
            return papi->get_IndicatorCategory(_catId, ppOut);
        }
        
        EIVAL CEconomicIndicator::get_LatestValue() 
        {
            SP<IEconomyApi> papi = (IEconomyApi*) get_Session()->get_Object(IEconomyApiName)._ptr;
            if (!papi) throw seexception(E_InvalidPointer);
            return papi->get_EconomicIndicatorLatestValue(get_Id());
        }
        
        bool CEconomicIndicator::get_IsDisplayed() 
        {
            return _isdisplayed;
        }

        const char* CEconomicIndicator::get_CopyRight() 
        {
            return _copyright.c_str();
        }
        
        CSTR CEconomicIndicator::get_TableName()
        {
            return _tablename.c_str();
        }

        bool CEconomicIndicator::get_IsSeasonal()
        {
            return _isseasonal;
        }

        CSTR CEconomicIndicator::get_DownloadLink()
        {
            return _downloadlink.c_str();
        }

        const char* CEconomicIndicator::get_Units() {
            return _unit.c_str();
        }


        se::training::DET CEconomicIndicator::get_Determiner() 
        {
            return _determiner;
        }
        
        void CEconomicIndicator::set_Code(const char* v) 
        {
            _code = v;
        }

        
        void CEconomicIndicator::set_Description(CSTR v)
        {
            _desc = v;
        }
        
        void CEconomicIndicator::set_Source(CSTR v)
        {
            _source = v;
        }
        
        void CEconomicIndicator::set_PublishPeriod(CSTR v) 
        {
            _publishperiod = v;
        }
        
        void CEconomicIndicator::set_ReleaseDates(CSTR v)
        {
            _reldates = v;
        }
        
        void CEconomicIndicator::set_Category(UID v)
        {
            _catId = v;
        }
        
        void CEconomicIndicator::set_IsDisplayed(bool v)
        {
            _isdisplayed = v;
        }
        
        void CEconomicIndicator::set_CopyRight(CSTR v)
        {
            _copyright = v;
        }
        
        void CEconomicIndicator::set_TableName(CSTR v)
        {
            _tablename = v;
        }
        
        void CEconomicIndicator::set_IsSeasonal(bool v)
        {
            _isseasonal = v;
        }

        void CEconomicIndicator::set_DownloadLink(CSTR v)
        {
            _downloadlink = v;
        }

        void CEconomicIndicator::set_Units(const char* v) {
            _unit = v;
        }
        

        void CEconomicIndicator::set_Determiner(se::training::DET v) 
        {
            _determiner = v;
        }

        
        void CEconomicIndicator::Serialize(ISerializedData<IString>* pIn)
        {
            pIn->Add("Id", get_Id());
            pIn->Add("Name", _name.c_str());
            pIn->Add("Description", _desc.c_str());
            pIn->Add("PublishPeriod", _publishperiod.c_str());
            pIn->Add("Source", _source.c_str());
            pIn->Add("ReleaseDates", _reldates.c_str());
            pIn->Add("IsDisplayed", _isdisplayed);
            pIn->Add("IsSeasonal", _isseasonal);
            SP<IIndicatorCategory> pcat;
            get_Category(&pcat);
            if (pcat)
                pIn->Add("Category", pcat);
            pIn->Add("ChartType", _chartType.c_str());
            pIn->Add("CopyRight", _copyright.c_str());
        }

        long unsigned int CEconomicIndicator::Store() 
        {
            return E_NotImpl;
        }

        long unsigned int CEconomicIndicator::Delete() 
        {
            return E_NotImpl;
        }


        
        SE_CODE IEconomyIndicator::Create(ISession* ps, IEconomyIndicator** ppOut)
        {
            return CObject<CEconomicIndicator>::Create(ps, ppOut);
        }

        ///////////////////////CEconomyIndicatorList///////////////////////
        
        IEconomyIndicator* CEconomyIndicatorList::GetSerializedvalue(ISerializedData<IString>* pIn, const char* propName)
        {
            return 0;
        }
        
        void CEconomyIndicatorList::SetSerializedValue(ISerializedData<IString>* pIn, const char* PropName, const IEconomyIndicator*& value)
        {
            pIn->Add(PropName, value);
        }
    }
}