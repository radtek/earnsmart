/* 
 * File:   CXXXAlgoProcessBase.h
 * Author: santony
 *
 * Created on May 27, 2015, 9:22 PM
 */

#ifndef CXXXALGOPROCESSBASE_H
#define	CXXXALGOPROCESSBASE_H

#include "sepredictapi.h"


namespace se
{
    namespace predict
    {
        /*
         * Base class for different algo training set processing
         * Different derived classes will have its implementation 
         * of algo on the training set.
         */

        class CXXXAlgoProcessBase : public CObjectRoot, public IObject
        {
        public:
            CXXXAlgoProcessBase(ISession*, IPredictor* predictor, ITrainingSet*);
            CXXXAlgoProcessBase(const CXXXAlgoProcessBase& orig) = delete;
            virtual ~CXXXAlgoProcessBase();
            
            virtual IResult* Run();
            
            static CXXXAlgoProcessBase* Construct(IPredictor*, ITrainingSet*) throw(const std::exception&);
            
        protected:
            
            SP<IPredictor> _predictor;
            SP<IResult> _result;
            SP<ITrainingSet> _tset;
        };
    }
    
}
#endif	/* CXXXALGOPROCESSBASE_H */

