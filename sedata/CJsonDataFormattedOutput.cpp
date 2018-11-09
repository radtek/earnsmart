/* 
 * File:   CJsonDataFormattedOutput.cpp
 * Author: santony
 * 
 * Created on January 19, 2013, 9:33 PM
 */

#include "CJsonDataFormattedOutput.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>


#include "../seglobal/CString.h"
#include "CDataRowList.h"

//using namespace Json;

namespace se
{
    namespace data
    {
        CJsonDataFormattedOutput::CJsonDataFormattedOutput(ISession* ps):CObjectRoot(ps)
        {
            
        }

        CJsonDataFormattedOutput::CJsonDataFormattedOutput(const CJsonDataFormattedOutput& orig) : CObjectRoot(orig)
        {
            
        }

        CJsonDataFormattedOutput::~CJsonDataFormattedOutput() 
        {
            
        }

        ErrorId CJsonDataFormattedOutput::Init(CDataTable* table)
        {
            //LOG4CXX_INFO(_logger, "Initializing the json writer for datatable.");
   
            
            //string jsonwriteoutput;
            Json::Value root;
            Json::Value vec(Json::arrayValue);
            Json::FastWriter writer;
            
            
            root["Statement"] =table->get_Name();
            
            
            auto& rootLvl = root;
            buildJson(table,rootLvl);
     
            cout<<root<<endl;
             jsonwriteoutput = writer.write(root);
                
       
            
            //jsonwriteoutput = writer....
            
            
            return S_Success;
          
            
        }

        ErrorId CJsonDataFormattedOutput::ToString(IString** outVal)
        {
            //LOG4CXX_INFO(_logger, "Returing a json representation of datatable in string form.");
           // SP<IString> strjson;
           // ErrorId ret = CObject<CString>::Create(jsonwriteoutput.c_str(), outVal);
            //return ret;
        }

        void CJsonDataFormattedOutput::buildJson(CDataTable* table,Json::Value& json)
        {
         //cout<<json<<endl;
            Json::Value child;
            Json::Value vec(Json::arrayValue);
            
            auto enumerator = table->get_Rows().EnumerateRows();
            for(auto start=enumerator.first;start != enumerator.second; ++start)
            {
                CDataRow& row = *start;
                string accName;
                CDataTable* ptr;
                R8 value;
                row.get("AccountName", accName);
                row.get(1, ptr);
                row.get(2,value);
                child["AccountName"] = accName;
                child["Amount"] = value;
                vec.append(Json::Value(child));
                               
                              
                auto& rootLvl = json;
                if(ptr!=NULL)
                        buildJson(ptr,rootLvl);
                json["Accounts"]=vec;
               
            }
        }
        
    }
    
}

        