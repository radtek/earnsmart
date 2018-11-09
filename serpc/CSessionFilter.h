/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CAuthenticationFilter.h
 * Author: santony
 *
 * Created on December 27, 2015, 8:50 PM
 */

#ifndef CAUTHENTICATIONFILTER_H
#define CAUTHENTICATIONFILTER_H

class CSessionFilter
{
public:
    CSessionFilter();
    CSessionFilter(const CSessionFilter& orig) = delete;
    virtual ~CSessionFilter();
    
private:

};

#endif /* CAUTHENTICATIONFILTER_H */

