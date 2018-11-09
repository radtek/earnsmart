/* 
 * File:   CProductDescription.h
 * Author: santony
 *
 * Created on May 9, 2015, 10:35 PM
 */

#ifndef CPRODUCTDESCRIPTION_H
#define	CPRODUCTDESCRIPTION_H

#include "sepredictapi.h"


namespace se
{
    namespace predict
    {
        class CProductDescription : public CObjectRoot, public IProductDescription
        {
        public:
            CProductDescription(ISession* );
            virtual ~CProductDescription();
            

            long unsigned int Init(UID Id, ProductDescriptionType type, TrainingDataContext ctxt);


            virtual ProductDescriptionType get_Type();

            virtual void set_Type(ProductDescriptionType type);
            

            virtual TrainingDataContext get_Context();

            virtual void set_Context(TrainingDataContext);




            
        private:
            TrainingDataContext _ctxt = TrainingDataContext::EconomicIndicator;
            ProductDescriptionType _type;
        };
        
    }
}

#endif	/* CPRODUCTDESCRIPTION_H */

