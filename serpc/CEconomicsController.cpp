/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEconomicsController.cpp
 * Author: santony
 * 
 * Created on December 17, 2016, 7:02 AM
 */

#include "CEconomicsController.h"

CEconomicsController::CEconomicsController() 
{
    AddAction("getforcastmatrix", Function<const Json::Value&>(bind(&CEconomicsController::GetForcastMatrix, this, _1, _2)));
}

CEconomicsController::~CEconomicsController() 
{
    
}

NXR CEconomicsController::GetForcastMatrix(Json::Value& rsp, const Json::Value& req) 
{
    /*
     * Request json may include params like the model
     * {
     *      "name" : "GDP",
     *      "model" : "MVLR", 
     *      "Period" : { "From" : "12/12/2002", "To" : "12/12/2017" }
     * }
     */
    
    // input has no purpose now.
    // hardcoded path
    ifstream ifs ;
    ifs.open ("/home/santony/Documents/smartgaze/gdpprojection/gdpcubistmodel/data/gdp.json", std::ifstream::in);

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(ifs, root, false))
    {
        InvalidAppOperationResponse(rsp, "Failed to read gdp.json file.");
    }
    else
    {
        OkResponse(rsp, root);
    }
    ifs.close();
    return NXWEB_OK;
}


