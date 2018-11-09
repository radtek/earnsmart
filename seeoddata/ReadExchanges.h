/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReadExchanges.h
 * Author: sajia
 *
 * Created on April 22, 2018, 11:00 AM
 */

#ifndef READEXCHANGES_H
#define READEXCHANGES_H

#include "ReadEodFile.h"

namespace se
{

namespace eoddata
{

class ReadExchanges : public ReadEodFile
{
public:
    ReadExchanges(const string& fileName, const EODFileTraits& traits);
    ReadExchanges(const ReadExchanges& orig) = delete;
    virtual ~ReadExchanges();

protected:

private:

};

}
}
#endif /* READEXCHANGES_H */

