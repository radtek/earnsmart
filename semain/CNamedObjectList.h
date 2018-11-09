/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CNamedObjectList.h
 * Author: santony
 *
 * Created on December 29, 2015, 7:08 PM
 */

#ifndef CNAMEDOBJECTLIST_H
#define CNAMEDOBJECTLIST_H

#include <unordered_map>

typedef std::unordered_map<NamedObjectNamingType, SP<INamedObject> > NamedObjectMap;

class CNamedObjectList : public CObjectRoot, public INamedObjectList
{
private:
    NamedObjectMap _map;
public:
    CNamedObjectList(ISession*);
    virtual ~CNamedObjectList();
    
    RP<INamedObject> get(NamedObjectNamingType name) override;
    long unsigned int set(NamedObjectNamingType name, INamedObject* instance) override;
    long unsigned int remove(NamedObjectNamingType name) override;


    
private:

};

#endif /* CNAMEDOBJECTLIST_H */

