/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   seappmodule.h
 * Author: santony
 *
 * Created on December 28, 2015, 8:54 AM
 */

#ifndef SEAPPMODULE_H
#define SEAPPMODULE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <nxweb.h>
    
int on_module_startup() ;
void on_module_shutdown() ;
void on_module_config(const nx_json* js);



#ifdef __cplusplus
}
#endif

#endif /* SEAPPMODULE_H */

