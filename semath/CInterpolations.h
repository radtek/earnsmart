/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CInterpolations.h
 * Author: santony
 *
 * Created on December 18, 2016, 10:57 AM
 */

#ifndef CINTERPOLATIONS_H
#define CINTERPOLATIONS_H

#include "semathapi.h"
#include <gsl/gsl_interp.h>

namespace se
{
    namespace math
    {
        class CInterpolations : public CObjectRoot, public IInterpolations
        {
        public:
            CInterpolations(se::ISession*);
            CInterpolations(const CInterpolations& orig) = delete;
            virtual ~CInterpolations();
            
            double get_y(double y1, double y2, double x1, double x2, double x) override;
            
            long unsigned int Init(SIZE dimension);
            
        private:
            GSLERRHANDLER oldErrHandler;
            gsl_interp* pinterp;
            gsl_interp_accel* pinterpacc;
            SIZE dim;
        };
    }
}


#endif /* CINTERPOLATIONS_H */

