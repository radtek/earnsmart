/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPositions.h
 * Author: santony
 *
 * Created on September 28, 2016, 8:40 PM
 */

#ifndef CPOSITIONS_H
#define CPOSITIONS_H

#include "../seglobal/CJsonObject.h"

class CPositions : public CJsonObject<se::admin::IPositions>
{
public:
    CPositions();
    CPositions(ISession* ps);
    CPositions(const CPositions& orig) = delete;
    virtual ~CPositions();
    
    long unsigned int Init(Json::Value& rootData) override;

    
private:

};

#endif /* CPOSITIONS_H */

