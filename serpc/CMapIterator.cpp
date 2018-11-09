/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CMapIterator.cpp
 * Author: santony
 * 
 * Created on February 7, 2016, 8:38 AM
 */

#include "CMapIterator.h"

CThinMapIterator::CThinMapIterator(nx_simple_map_entry* start) : _head(start)
{
}

CThinMapIterator::CThinMapIterator(const CThinMapIterator& other) : _head(other._head)
{
    
}

CThinMapIterator::~CThinMapIterator()
{
}

CThinMapIterator& CThinMapIterator::operator++() 
{
    this->_head = nx_simple_map_itr_next(this->_head);
    return *this;
}

CThinMapIterator& CThinMapIterator::operator=(nx_simple_map_entry* start) 
{
    this->_head = start;
    return *this;
}

bool CThinMapIterator::operator==(const CThinMapIterator& other) 
{
    return this->_head == other._head;
}

bool CThinMapIterator::operator!=(const CThinMapIterator& other) 
{
    return this->_head != other._head;
}


/////////////////// CThinMap/////////////////

const char* CThinMap::findvalue(const char* name) const
{
    auto entry = nx_simple_map_find(this->_head, name);
    if (entry != nullptr)
        return entry->value;
    return "";
}

const char* CThinMap::getvalue(const char* key) const
{
    return nx_simple_map_get_nocase(_head, key);
}