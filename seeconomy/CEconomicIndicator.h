/* 
 * File:   CEconomicIndicator.h
 * Author: santony
 *
 * Created on January 23, 2014, 11:23 AM
 */

#ifndef CECONOMICINDICATOR_H
#define	CECONOMICINDICATOR_H

#include <string>
#include "../seglobal/CList.h"
#include "seeconomapi.h"
using namespace std;

namespace se
{
    
    namespace economy
    {

        class CEconomicIndicator : public CObjectRoot, public IEconomyIndicator
        {
        public:
            CEconomicIndicator(ISession*);
            virtual ~CEconomicIndicator();
            
            virtual CSTR get_Code();
            virtual CSTR get_Description() ;
            virtual CSTR get_PublishPeriod();
            virtual CSTR get_Source() ;
            virtual CSTR get_ReleaseDates() ;
            virtual SE_CODE get_Category(IIndicatorCategory**) ;
            virtual UID get_CategoryId();
            virtual EIVAL get_LatestValue() ;
            virtual bool get_IsDisplayed() ;
            virtual CSTR get_Name();
            virtual void set_Name(CSTR name);
            virtual CSTR get_CopyRight();
            virtual CSTR get_TableName();
            virtual bool get_IsSeasonal();
            virtual CSTR get_DownloadLink();

            virtual const char* get_Units();

            virtual se::training::DET get_Determiner();
        
            void set_Code(CSTR);
            virtual void set_Description(CSTR);
            virtual void set_Source(CSTR);
            virtual void set_PublishPeriod(CSTR);
            virtual void set_ReleaseDates(CSTR);
            virtual void set_Category(UID catId);
            virtual void set_IsDisplayed(bool);
            virtual void set_CopyRight(CSTR);
            virtual void set_TableName(CSTR);
            virtual void set_IsSeasonal(bool);
            virtual void set_DownloadLink(CSTR);

            virtual void set_Units(const char* );

            virtual void set_Determiner(se::training::DET);
            
            virtual void Serialize(ISerializedData<IString>* pIn);
            
            virtual SE_CODE Init();
            
            SE_CODE Init(
                UID id,
                const string& name,
                const string& desc,
                const string& publishperiod,
                const string& source,
                const string& reldates,
                UID catId,
                BOOL isdisplayed,
                const string& copyright,
                const string& Type,
                bool isseasonal,
                const string& tablename
                );
            
            SE_CODE Init(const Json::Value& js);

            virtual long unsigned int Store();

            virtual long unsigned int Delete();

            
        private:
            string 
                    _code,
                    _unit,
                _name, _desc, _source, _reldates, 
                _publishperiod, _chartType, _copyright, 
                _tablename, _downloadlink;
            
            UID _catId;
            BOOL _isdisplayed, _isseasonal;
            se::training::DET _determiner;

        };
        
        class CEconomyIndicatorList : public CSPList<IEconomyIndicator>
        {
        public:
            CEconomyIndicatorList(ISession* ps): CSPList<IEconomyIndicator>(ps)
            {
                
            }
            
        protected:
            virtual IEconomyIndicator* GetSerializedvalue(ISerializedData<IString>* pIn, CSTR propName);
            virtual void SetSerializedValue(ISerializedData<IString>* pIn, CSTR PropName, const IEconomyIndicator*& value);
        };
        

    }

}
#endif	/* CECONOMICINDICATOR_H */

