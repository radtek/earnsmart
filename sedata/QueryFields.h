/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   QueryFields.h
 * Author: santony
 *
 * Created on June 18, 2016, 10:24 AM
 */

#ifndef QUERYFIELDS_H
#define QUERYFIELDS_H

namespace se
{
    namespace data
    {
        typedef struct QueryFields
        {
            struct Country
            {
                static CSTR Id;
                static CSTR Name;
                static CSTR Code;
                static CSTR IsPayAvailable;
                static CSTR ThreeCharCode;
            };
            struct State
            {
                static CSTR Id;
                static CSTR Code;
                static CSTR Name;
            };
        } QUERYFIELDS;
    }
}

#endif /* QUERYFIELDS_H */

