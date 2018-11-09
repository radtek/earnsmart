/* 
 * File:   CAppMain.h
 * Author: santony
 *
 * Created on January 24, 2015, 1:00 PM
 */

#ifndef CAPPMAIN_H
#define	CAPPMAIN_H

#include "CAppBase.h"
#include <string>

using namespace std;

/*
 * The main cppcms service application class. 
 */

class CAppMain : public CAppBase 
{
private:
    
public:
    CAppMain(cppcms::service& srv);
    
    virtual ~CAppMain();


    virtual void init();


    virtual void main(std::string url);


    
protected:
    
private:

    /////////////////////// USER MANAGEMENT RENDER METHODS //////////////////////////
    void Pwd(string email);
    void RegisterStep1_BasicOrLicensed(string guid = "");
    void RegisterStep2_EmailConfirm(string guid);
    void RegisterStep3_PayCompleted(string guid);
    void Signon();
    void Account();
    
    
    
};


#endif	/* CAPPMAIN_H */

