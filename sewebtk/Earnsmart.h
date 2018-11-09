/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Earnsmart.h
 * Author: santony
 *
 * Created on November 26, 2017, 10:02 PM
 */

#ifndef EARNSMART_H
#define EARNSMART_H

#include "Common.h"

class Earnsmart : public WContainerWidget 
{
private:
    WNavigationBar *navigation;
    WStackedWidget *contentstack;
    
public:
    Earnsmart();
    virtual ~Earnsmart();
private:
    //WMenuItem* AddToMenu(WMenu* menu, const WString& name, unique_ptr<TopicWid)

};

#endif /* EARNSMART_H */

