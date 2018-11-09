/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "sedataheaders.h"
#include "QueryFields.h"

using namespace se::data;

CSTR QueryFields::Country::Id = "country_id";
CSTR QueryFields::Country::Name = "country_name";
CSTR QueryFields::Country::Code = "country_code";
CSTR QueryFields::Country::IsPayAvailable = "country_ispaypalavailable";
CSTR QueryFields::Country::ThreeCharCode = "country_code1";

CSTR QueryFields::State::Id = "state_id";
CSTR QueryFields::State::Code = "state_code";
CSTR QueryFields::State::Name = "state_name";