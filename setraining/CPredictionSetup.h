/* 
 * File:   CPredictionSetup.h
 * Author: santony
 *
 * Created on May 12, 2014, 9:17 AM
 */

#ifndef CPREDICTIONSETUP_H
#define	CPREDICTIONSETUP_H

#include "setrainingapi.h"
#include "../seglobal/CList.h"

namespace se
{
    namespace training
    {
        class CPredictionSetup : public CObjectRoot, public IPredictionSetup
        {
        public:
            CPredictionSetup(ISession*);
            virtual ~CPredictionSetup();
            virtual void Serialize(ISerializedData<IString>* pIn);

            virtual const char* get_Name();

            virtual const char* get_Desc();

            virtual const char* get_Algo();
            

            virtual void set_Name(const char* v);

            virtual void set_Desc(const char* v);

            virtual void set_Algo(const char* v);

            virtual const TRAINING_RESULT& get_ResultInfo();
            
            void set_ResultInfo(const TRAINING_RESULT& v);
            
            virtual long unsigned int get_TrainingData(DATAPERIOD periodFrom, DATAPERIOD periodTo, IExTrainingTable** ppOut);


        private:
            string _name, _desc, _algo;
            SP<IExTrainingTable> _pExTable;
            TRAINING_RESULT _resultinfo;
        };
        
        
        /////////////////////////////CPredictionSetupList////////////////////
        class CPredictionSetupList : public CSPList<IPredictionSetup, IPredictionSetupList>
        {
        public:
            CPredictionSetupList(ISession*);
            
            virtual ~CPredictionSetupList();
            
            virtual SE_CODE FindByResult(const TRAINING_RESULT& result, IPredictionSetup** ppOut);
            
            virtual void Add(IPredictionSetup* item);
            
        private:
            typedef std::map<TRAINING_RESULT, SP<IPredictionSetup> > Map;
            Map _map;

        };
        
        
    }
}


#endif	/* CPREDICTIONSETUP_H */

