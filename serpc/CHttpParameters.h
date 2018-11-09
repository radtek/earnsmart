/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHttpParameters.h
 * Author: santony
 *
 * Created on March 5, 2016, 12:14 PM
 */

#ifndef CHTTPPARAMETERS_H
#define CHTTPPARAMETERS_H

#include "CMapIterator.h"


class CHttpParameters : public CThinMap 
{
public:
    CHttpParameters(nxweb_http_parameter* parameters);
    CHttpParameters(const CHttpParameters& orig) = delete;
    virtual ~CHttpParameters();
private:

};

#endif /* CHTTPPARAMETERS_H */

