/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on November 14, 2012, 7:20 AM
 */

#include <cstdlib>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <iostream>

//#include "xbrlcalparsingtest.c"
#include "sexbrlinternal.h"
//#include "CXbrlCalculation.h";
#include <boost/filesystem.hpp>
#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

//#include "CXbrlCalculation.h"
#include "CXbrlPresentation.h"

#include "../sedata/sedataheaders.h"

using namespace std;
using namespace se;
using namespace se::xbrl;
using namespace se::data;


void OnParseCompleted(ErrorId status);
void DatatableTests();
void DataTableToJsonConversionTest(CDataTable* datatable);

/*
 * 
 */
int main(int argc, char** argv) 
{
    
    std::string log4config = "../log4cxx.conf";
    bool fileCheck = boost::filesystem::exists(log4config);
    if (!fileCheck)
        log4config = "./log4cxx.conf";
    log4cxx::PropertyConfigurator::configure(log4config);
    
    DatatableTests();
    
    
    //ParseCalXbrlFile("/home/santony/Documents/xbrl/adbe-20071130_cal.xml");
  
    //CSTR location = "/home/santony/Documents/xbrl/schema/";
    
//    CXbrlCalculation calculate;
//    calculate.Parse("/home/aashok/Documents/xbrl/schema/");
    
    CXbrlPresentation Presentation;
    Presentation.Parse("/home/aashok/Documents/xbrl/schema/");
    return 0;
}


void DatatableTests()
{
    // create a datatable
    CDataTable table("IncomeStatement");
    table.Acquire();
    // enable fields
    CDataFieldList& fields = table.get_Fields();
    // fields are added in place.
    fields.AddField("AccountName", se::DT_STR); // this is index 0
    fields.AddField("Children", se::DT_POINTER);    // this is index 1 and so on  below
    fields.AddField("YEAR1", se::DT_R8);
    fields.AddField("YEAR2", se::DT_R8);
    fields.AddField("YEAR3", se::DT_R8);
    
    // adding some rows. 
    // since fields are added to the datable, we create a new datarow from CDAtaTable
    CDataRow row = table.get_NewRow();
    if (row)
    {
        row.set("AccountName", (string)"Assets");
        row.set(1, (CDataTable*) nullptr);
        row.set(2, 123.00);
        row.set(3, 113.00);
        row.set("Year3", 0.00);
        table.get_Rows().AddRow(row);
        
        // get values
        
        string accName;
        if (row.get("AccountName", accName))
        {
            // got the value.
        }
        
        R8 year0 ;
        if (row.get(2, year0))
        {
            // we got field value.
        }
        
    }
    
    // enumerat rows.
    auto enumerator = table.get_Rows().EnumerateRows();
    for (auto start = enumerator.first; start != enumerator.second; ++start )
    {
        CDataRow& row = *start;
        string accName;
        if (row.get("AccountName", accName))
        {
            cout << accName << endl;
        }
    }
    
    DataTableToJsonConversionTest(&table);
}

void DataTableToJsonConversionTest(CDataTable* datatable)
{
    SP<IDataFormattedOutput> jsonoutput;
    ErrorId ret = ConvertDataTableToJson(datatable, &jsonoutput);
    if (ret == S_Success)
    {
        cout << "Successfully converted to json." << endl;
        SP<IString> str;
        jsonoutput->ToString(&str);
        cout << str->get_Buffer() << endl;
    }
}



