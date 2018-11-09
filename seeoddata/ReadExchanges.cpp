/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReadExchanges.cpp
 * Author: sajia
 * 
 * Created on April 22, 2018, 11:00 AM
 */

#include "seeoddataint.h"
#include "ReadExchanges.h"
#include "ReadEodFile.h"

namespace se
{
namespace eoddata
{

ReadExchanges::ReadExchanges(const string& fileName, const EODFileTraits& traits): ReadEodFile(fileName, traits)
{
    
}

ReadExchanges::~ReadExchanges()
{
    
}

}
}