/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "seappmodule.h"


NXWEB_MODULE(
        SEAppModule, 
        on_server_startup:on_module_startup, 
        on_server_shutdown:on_module_shutdown, 
        on_config:on_module_config);


