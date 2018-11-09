/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReadEodFile.h
 * Author: sajia
 *
 * Created on April 22, 2018, 11:00 AM
 */

#ifndef READEODFILE_H
#define READEODFILE_H

#include "seeoddata.h"

namespace se
{
namespace eoddata
{

class ReadEodFile
{
    string _fileName;
    EODFileTraits _fileTraits;
    
public:
    ReadEodFile(const string& fileName, const EODFileTraits& traits);
    ReadEodFile(const ReadEodFile& orig) = delete;
    virtual ~ReadEodFile();

    const string& get_FileName() const;
    const EODFileTraits& get_Traits() const;
    
protected:
    virtual void ReadStringLine(const string& line) = 0;
    
private:

};

}
}
#endif /* READEODFILE_H */

