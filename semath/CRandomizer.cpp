/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CRandomizer.cpp
 * Author: santony
 * 
 * Created on January 1, 2016, 4:58 PM
 */

#include "CRandomizer.h"
#include <random>

using namespace se::math;

UID CRandomizer::DefaultRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<UID> dis(1000000000, 10000000000);
    return dis(gen);
}


