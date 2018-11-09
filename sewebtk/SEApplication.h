/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SEApplication.h
 * Author: santony
 *
 * Created on November 24, 2017, 11:31 AM
 */
#ifndef SEAPPLICATION_H
#define SEAPPLICATION_H


#include "Common.h"

using namespace std;
using namespace Wt;

class SEApplication : public Wt::WApplication
{
    
private:

public:
    SEApplication(const Wt::WEnvironment& env);
    virtual ~SEApplication();

private:
    
};

#endif /* SEAPPLICATION_H */

