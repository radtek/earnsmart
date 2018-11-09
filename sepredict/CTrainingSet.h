/* 
 * File:   CTrainingSet.h
 * Author: santony
 *
 * Created on May 14, 2015, 6:55 PM
 */

#ifndef CTRAININGSET_H
#define	CTRAININGSET_H

#include "sepredictapi.h"
#include "../seglobal/CList.h"
#include "CAttributeDescription.h"


namespace se
{
    namespace predict
    {
        class CTrainingSetList;
        
        class CTrainingSet : public CObjectRoot, public ITrainingSet
        {
        public:
            CTrainingSet(ISession* );
            CTrainingSet(const CTrainingSet& orig) = delete;
            virtual ~CTrainingSet();
            

            virtual long unsigned int Init();

            SE_CODE Init(I32 tsetId);
            

            virtual const char* get_Name();

            virtual void set_Name(const char*);

            virtual const char* get_Desc();

            virtual void set_Desc(const char*);



            virtual IResultAttribDescription* get_ResultContext();

            virtual void set_ResultContext(IResultAttribDescription*);


            virtual void set_Algo(PredictionAlgos algo);

            virtual PredictionAlgos get_Algo();

            

            virtual void add_Attribute(IAttributeDescription*);

            virtual void rem_Attribute(unsigned long attribId);


            virtual IAttributeDescriptionList* get_Attributes();

            virtual void set_CaseWeightAttribute(IAttributeDescription*);
            virtual IAttributeDescription* get_CaseWeightAttribute();

            virtual void AddDependent(ITrainingSet* tsetChild);

            virtual void RemoveDependent(signed int tsetId);

            virtual ITrainingSetList* get_Dependents() throw(std::exception&);
            

            virtual se::training::IExTrainingTable* get_Table(bool refresh = false);


            virtual const DATAPERIOD& get_StartPeriod();

            virtual void set_StartPeriod(const DATAPERIOD& dp);

            virtual const DATAPERIOD& get_EndPeriod();

            virtual void set_EndPeriod(const DATAPERIOD& dp);

            virtual void Serialize(ISerializedData<IString>* pIn);

            virtual long unsigned int Store();

            virtual long unsigned int Delete();

            static SE_CODE _CreateorOpen(ISession*, se::data::CSmartConnection& conn, CSTR name, ITrainingSet** ppOut);
            static SE_CODE _Get(ISession*, se::data::CSmartConnection& conn, I32 tsetid, ITrainingSet**) throw(std::exception&);
            static SE_CODE Get(ISession*, se::data::CSmartConnection& conn, UID predictorId, I32 tsetid, ITrainingSet**) throw(std::exception&);

            SE_CODE _Store(se::data::CSmartConnection& conn) throw(const std::exception&);
            SE_CODE _Delete(se::data::CSmartConnection& conn) throw(const std::exception&);

        private:
            string _sName, _sDesc;
            SP<CTrainingSetList> _dependents;
            SP<CAttributeDescription> _pCaseWeight;
            SP<CResultAttributeDescription> _resultContext ;
            PredictionAlgos _algo = PredictionAlgos::MultiVarLinearRegression;
            SP<CAttributeDescriptionList> _pAttribs;
            DATAPERIOD _dpStart, _dpEnd;
        };
        
        class CTrainingSetList : public CSPList<ITrainingSet,ITrainingSetList> 
        {
        public:
            CTrainingSetList(ISession* ps) : CSPList<ITrainingSet,ITrainingSetList>(ps)
            {
                
            }
            
        };
    }
}


#endif	/* CTRAININGSET_H */

