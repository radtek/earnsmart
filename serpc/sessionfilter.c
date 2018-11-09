/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "sessionfilter.h"

nxweb_filter SessionFilter={
    .init=SessionInit, 
    .finalize=SessionFinalize,
    .translate_cache_key=SessionTranslateCacheKey, 
    .do_filter=SessionDoFilter
};


//NXWEB_DEFINE_FILTER(SessionFilter, SessionFilter);


