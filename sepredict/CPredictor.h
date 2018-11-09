/* 
 * File:   CPredictor.h
 * Author: santony
 *
 * Created on May 9, 2015, 11:53 AM
 */

#ifndef CPREDICTOR_H
#define	CPREDICTOR_H

#include "sepredictapi.h"


namespace se
{
    namespace predict
    {
    
        class CPredictor : public CObjectRoot, public IPredictor
        {
        public:
            CPredictor(ISession* );
            CPredictor(const CPredictor& orig) = delete;
            virtual ~CPredictor();
            
            long unsigned int Init(IProductDescription* pdesc);


            virtual const char* get_Name();

            virtual void set_Name(const char* name);


            virtual const char* get_Description();

            virtual void set_Description(const char* desc);

          
            IPredictor& SetProduct(IProductDescription* prodDesc) throw(std::exception&);


            virtual IPredictor& SetTrainingSet(ITrainingSet* tset) throw(std::exception&) ;

            
            
            virtual IPredictor& BuildEquation() throw(std::exception&);

            virtual IResult& Result() throw(std::exception&);


            virtual long unsigned int Store();
            

            virtual void Clear();
            

            virtual long unsigned int Remove();


            virtual const DATAPERIOD& get_StartPeriod();

            virtual void set_StartPeriod(const DATAPERIOD& dp);

            virtual const DATAPERIOD& get_EndPeriod();

            virtual void set_EndPeriod(const DATAPERIOD& dp);





            virtual void Serialize(ISerializedData<IString>* pIn);



        private:
            
            SE_CODE Load();
            
        private:
            std::string _sName, _sDesc;
            SP<IProductDescription> _prodDesc;
            SP<IResult> _presult ;
            SP<ITrainingSet> _tset;
            DATAPERIOD _dpStart, _dpEnd;
        };
    }
}


#endif	/* CPREDICTOR_H */

