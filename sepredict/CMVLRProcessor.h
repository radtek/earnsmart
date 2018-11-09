/* 
 * File:   CMVLRProcessor.h
 * Author: santony
 *
 * Created on May 28, 2015, 7:28 AM
 */

#ifndef CMVLRPROCESSOR_H
#define	CMVLRPROCESSOR_H

#include "CXXXAlgoProcessBase.h"


namespace se
{
    namespace predict
    {

        class CMVLRProcessor : CXXXAlgoProcessBase
        {
        public:
            CMVLRProcessor(ISession*, IPredictor* predictor, ITrainingSet* tset);
            CMVLRProcessor(const CMVLRProcessor& orig) = delete;
            virtual ~CMVLRProcessor();

            virtual IResult* Run();

            
            
        private:

        };

    }
    
}
#endif	/* CMVLRPROCESSOR_H */

