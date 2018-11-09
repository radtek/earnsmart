/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CJsonObject.h
 * Author: santony
 *
 * Created on September 28, 2016, 8:26 PM
 */

#ifndef CJSONOBJECT_H
#define CJSONOBJECT_H

#include "IObject.h"
#include "CObjectRoot.h"

template<typename TIf = se::IObject>
class CJsonObject : public CObjectRoot, public TIf
{
public:
    CJsonObject()
    {
    }
    
    CJsonObject(se::ISession* ps) : CObjectRoot(ps)
    {
        
    }
    
    CJsonObject(const CJsonObject& orig) = delete;
    
    virtual ~CJsonObject()
    {
        
    }
    
    virtual SE_CODE Init(Json::Value& rootData)
    {
        this->_root = rootData;
        return S_OK;
    }
    
    Json::Value& get_Root() override
    {
        return this->_root;
    }
    
private:
    Json::Value _root;
};

#endif /* CJSONOBJECT_H */

