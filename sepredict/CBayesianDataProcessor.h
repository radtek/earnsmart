/* 
 * File:   CBayesianDataProcessor.h
 * Author: santony
 *
 * Created on August 24, 2015, 1:54 PM
 */

#ifndef CBAYESIANDATAPROCESSOR_H
#define	CBAYESIANDATAPROCESSOR_H

#include "CXXXAlgoProcessBase.h"


namespace se
{
    namespace predict
    {
        class CBayesianDataProcessor : public CXXXAlgoProcessBase
        {
        public:
            CBayesianDataProcessor(ISession*, IPredictor* predictor, ITrainingSet* tset);
            CBayesianDataProcessor(const CBayesianDataProcessor& orig) = delete;
            virtual ~CBayesianDataProcessor();
            

            virtual IResult* Run();

            
        private:

        };
    }
}

#endif	/* CBAYESIANDATAPROCESSOR_H */

