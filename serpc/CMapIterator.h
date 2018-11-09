/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CMapIterator.h
 * Author: santony
 *
 * Created on February 7, 2016, 8:38 AM
 */

#ifndef CMAPITERATOR_H
#define CMAPITERATOR_H

#include "internal.h"
#include <map>

using namespace std;

class CThinMapIterator
{
private:
    nx_simple_map_entry* _head;
    
public:
    explicit CThinMapIterator(nx_simple_map_entry* start);
    
    CThinMapIterator(const CThinMapIterator& other);

    virtual ~CThinMapIterator();

    CThinMapIterator& operator++();
    
    CThinMapIterator& operator=(nx_simple_map_entry* start);

    bool operator==(const CThinMapIterator& other);

    bool operator!=(const CThinMapIterator& other);
    
    explicit operator nx_simple_map_entry*() const
    {
        return this->_head;
    }
    
private:

};

class CThinMap
{
public:
    typedef CThinMapIterator iterator;
    
protected:
    nx_simple_map_entry* _head;
    
public:
    
    string RpcData() const
    {
        string result;
        for (auto d = _head; d != nullptr; d = _head->next)
        {
            result += d->name ;
            result += d->value;
        }
        return result;
    }
    
    CThinMap(nx_simple_map_entry* head) : _head(head)
    {
        
    }
    
    CThinMap(const CThinMap& other) : _head(other._head)
    {
        
    }
    
    CThinMap& operator=(nx_simple_map_entry* start)
    {
        this->_head = start;
        return *this;
    }

    CSTR findvalue(CSTR name) const;
    
    CSTR getvalue(CSTR key) const;
    
    
    iterator begin()
    {
        return (CThinMapIterator) _head;
    }
    
    
    iterator end()
    {
        return (CThinMapIterator)nullptr;
    }
};


#endif /* CMAPITERATOR_H */

