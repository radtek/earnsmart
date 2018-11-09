/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CMathApi.h
 * Author: santony
 *
 * Created on December 18, 2016, 10:43 AM
 */

#ifndef CMATHAPI_H
#define CMATHAPI_H

#include "semathapi.h"


namespace se
{
    namespace math
    {
        class CMathApi : public CObjectRoot, public IMathApi
        {
        public:
            CMathApi(se::ISession*);
            CMathApi(const CMathApi& orig) = delete;
            virtual ~CMathApi();
            
            SE_CODE get_InterpolationMethods(InterPolationTypes type, SIZE dimension, IInterpolations** ppOut) override;
            
        private:

        };
    }
}


#endif /* CMATHAPI_H */

