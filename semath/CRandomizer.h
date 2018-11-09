/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CRandomizer.h
 * Author: santony
 *
 * Created on January 1, 2016, 4:58 PM
 */

#ifndef CRANDOMIZER_H
#define CRANDOMIZER_H

#include "../seglobaltypedefs.h"

namespace se
{
    namespace math
    {
        class CRandomizer 
        {
        public:
            // generate a random number between start and end;
            static UID DefaultRandomNumber();
        };
    }
}


#endif /* CRANDOMIZER_H */

