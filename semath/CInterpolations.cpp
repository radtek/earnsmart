/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CInterpolations.cpp
 * Author: santony
 * 
 * Created on December 18, 2016, 10:57 AM
 */

#include "internal.h"
#include "CInterpolations.h"
#include <gsl/gsl_errno.h>


using namespace se::math;

CInterpolations::CInterpolations(se::ISession* ps) : CObjectRoot(ps)
{
    if (currentGslErrHandler != nullptr)
    {
        currentGslErrHandler = Gsl_ErrorHandler;
        oldErrHandler = gsl_set_error_handler(currentGslErrHandler);
    }
}

CInterpolations::~CInterpolations()
{
    gsl_interp_accel_free(pinterpacc);
    gsl_interp_free(pinterp);
    gsl_set_error_handler(oldErrHandler);
    currentGslErrHandler = nullptr;
}

long unsigned int CInterpolations::Init(SIZE dimension)
{
    dim = dimension;
    pinterp = gsl_interp_alloc(gsl_interp_linear, dimension);
    pinterpacc = gsl_interp_accel_alloc();
    return S_OK;
}

double CInterpolations::get_y(double y1, double y2, double x1, double x2, double x)
{
    R8 xdata[2] = {x1, x2};
    R8 ydata[2] = {y1, y2};
    gsl_interp_init(pinterp, xdata, ydata, dim);
    auto y = gsl_interp_eval(pinterp, xdata, ydata, x, pinterpacc);
    return y;
}
