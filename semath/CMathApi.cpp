/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CMathApi.cpp
 * Author: santony
 * 
 * Created on December 18, 2016, 10:43 AM
 */

#include "internal.h"
#include "CMathApi.h"
#include "CInterpolations.h"

using namespace se::math;

CMathApi::CMathApi(se::ISession* ps) : CObjectRoot(ps)
{
}

CMathApi::~CMathApi()
{
}

SE_CODE CMathApi::get_InterpolationMethods(InterPolationTypes type, SIZE dimension, IInterpolations** ppOut)
{
    switch(type)
    {
        case InterPolationTypes::Linear:
            return CObject<CInterpolations>::Create(get_Session(), dimension, ppOut);
        default:
            return E_NotSupported;
    }
}

