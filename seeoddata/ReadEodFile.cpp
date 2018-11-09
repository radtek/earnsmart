/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReadEodFile.cpp
 * Author: sajia
 * 
 * Created on April 22, 2018, 11:00 AM
 */

#include "seeoddataint.h"
#include "ReadEodFile.h"

namespace se
{
namespace eoddata
{

ReadEodFile::ReadEodFile(const string& fileName, const EODFileTraits& traits):
_fileName(fileName), 
        _fileTraits(traits)
{
    
}

ReadEodFile::~ReadEodFile()
{
}

const string& ReadEodFile::get_FileName() const
{
    return _fileName;
}

const EODFileTraits& ReadEodFile::get_Traits() const
{
    return _fileTraits;
}


}
}