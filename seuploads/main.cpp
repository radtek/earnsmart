/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on October 19, 2012, 9:02 PM
 */

#include <cstdlib>
#include<string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <vector>


#include "../semysql/CSEConnections.h"
#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

#define SEDBCONN        CSEConnections::GetMain()

using namespace std;
using namespace se;
using namespace se::data;
using namespace mysqlpp;
using namespace boost;	


void se_eco_AverageHourlyEarnings()
{
    CSmartConnection conn(SEDBCONN);
    Query query=conn->query();
    
    query.exec("drop table se_eco_AverageHourlyEarnings");
    query.exec("create table IF NOT EXISTS se_eco_AverageHourlyEarnings (year SMALLINT,month SMALLINT,Nominal DOUBLE )");
    
    
    string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/AverageHourlyEarnings-2006-2012.csv");
    ifstream in(data.c_str());
    string line,year;
    //if (!in.is_open()) return 1;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    cout<<"se_eco_AverageHourlyEarnings"<<endl;
    while(getline(in,line))
    {
        
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        year = vec[0];
        
        
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','1','"+vec[1]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','2','"+vec[2]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','3','"+vec[3]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','4','"+vec[4]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','5','"+vec[5]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','6','"+vec[6]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','7','"+vec[7]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','8','"+vec[8]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','9','"+vec[9]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','10','"+vec[10]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','11','"+vec[11]+"')");
        query.exec();
        query = conn->query("insert into se_eco_AverageHourlyEarnings values ('"+year+"','12','"+vec[12]+"')");
        query.exec();
        
    }
}

void se_eco_CPI()
{
    CSmartConnection conn(SEDBCONN);
    Query query=conn->query();
    
    query.exec("drop table se_eco_CPI");
    query.exec("create table IF NOT EXISTS se_eco_CPI (year SMALLINT,month SMALLINT,Nominal DOUBLE )");
    string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/CPI-1947-2012.csv");
    ifstream in(data.c_str());
    string line,year;
    cout<<"se_eco_CPI"<<endl;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    while(getline(in,line))
    {
        
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        year = vec[0];
        
        
        query = conn->query("insert into se_eco_CPI values ('"+year+"','1','"+vec[1]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','2','"+vec[2]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','3','"+vec[3]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','4','"+vec[4]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','5','"+vec[5]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','6','"+vec[6]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','7','"+vec[7]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','8','"+vec[8]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','9','"+vec[9]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','10','"+vec[10]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','11','"+vec[11]+"')");
        query.exec();
        query = conn->query("insert into se_eco_CPI values ('"+year+"','12','"+vec[12]+"')");
        query.exec();
        
    }
}

void se_eco_EmploymentCostIndex()
{
    CSmartConnection conn(SEDBCONN);
    Query query=conn->query();
    
    query.exec("drop table se_eco_EmploymentCostIndex");
    query.exec("create table IF NOT EXISTS se_eco_EmploymentCostIndex (year SMALLINT,Quarter SMALLINT,Nominal DOUBLE )");
    
    string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/EmploymentCostIndex-2001-2012.csv");
    
    ifstream in(data.c_str());
    string line,year;
    cout<<"se_eco_EmploymentCostIndex"<<endl;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    while(getline(in,line))
    {
        
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        year = vec[0];
        
        
        query = conn->query("insert into se_eco_EmploymentCostIndex values ('"+year+"','1','"+vec[1]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentCostIndex values ('"+year+"','2','"+vec[2]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentCostIndex values ('"+year+"','3','"+vec[3]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentCostIndex values ('"+year+"','4','"+vec[4]+"')");
        query.exec();
    }
}
void se_eco_EmploymentHoursEarnings()
{
    CSmartConnection conn(SEDBCONN);
    Query query=conn->query();
    
    query.exec("drop table se_eco_EmploymentHoursEarnings");
    query.exec("create table IF NOT EXISTS se_eco_EmploymentHoursEarnings (year SMALLINT,month SMALLINT,Nominal DOUBLE )");
    string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/EmploymentHoursEarnings-1947-2012.csv");
    ifstream in(data.c_str());
    string line,year;
    cout<<"se_eco_EmploymentHoursEarnings"<<endl;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    while(getline(in,line))
    {
        
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        year = vec[0];
        
        
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','1','"+vec[1]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','2','"+vec[2]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','3','"+vec[3]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','4','"+vec[4]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','5','"+vec[5]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','6','"+vec[6]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','7','"+vec[7]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','8','"+vec[8]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','9','"+vec[9]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','10','"+vec[10]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','11','"+vec[11]+"')");
        query.exec();
        query = conn->query("insert into se_eco_EmploymentHoursEarnings values ('"+year+"','12','"+vec[12]+"')");
        query.exec();
        
    }
}
void se_eco_Import_ExportPriceIndex()
{
    CSmartConnection conn(SEDBCONN);
    Query query=conn->query();
    
    query.exec("drop table se_eco_Import_ExportPriceIndex");
    query.exec("create table IF NOT EXISTS se_eco_Import_ExportPriceIndex (year SMALLINT,month SMALLINT,Nominal DOUBLE )");
    string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/Import-ExportPriceIndex-1982-2012.csv");
    ifstream in(data.c_str());
    string line,year;
    cout<<"se_eco_Import_ExportPriceIndex"<<endl;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    while(getline(in,line))
    {
        
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        year = vec[0];
        
        
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','1','"+vec[1]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','2','"+vec[2]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','3','"+vec[3]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','4','"+vec[4]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','5','"+vec[5]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','6','"+vec[6]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','7','"+vec[7]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','8','"+vec[8]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','9','"+vec[9]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','10','"+vec[10]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','11','"+vec[11]+"')");
        query.exec();
        query = conn->query("insert into se_eco_Import_ExportPriceIndex values ('"+year+"','12','"+vec[12]+"')");
        query.exec();
        
    }
    
}
void se_eco_ImportExportPriceIndex1983()
{
    CSmartConnection conn(SEDBCONN);
    Query query=conn->query();
    
    query.exec("drop table se_eco_ImportExportPriceIndex1983");
    query.exec("create table IF NOT EXISTS se_eco_ImportExportPriceIndex1983 (year SMALLINT,month SMALLINT,Nominal DOUBLE )");
    string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/ImportExportPriceIndex-1983-2012.csv");
    ifstream in(data.c_str());
    string line,year;
    cout<<"se_eco_ImportExportPriceIndex1983"<<endl;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    while(getline(in,line))
    {
        
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        year = vec[0];
        
        
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','1','"+vec[1]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','2','"+vec[2]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','3','"+vec[3]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','4','"+vec[4]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','5','"+vec[5]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','6','"+vec[6]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','7','"+vec[7]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','8','"+vec[8]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','9','"+vec[9]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','10','"+vec[10]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','11','"+vec[11]+"')");
        query.exec();
        query = conn->query("insert into se_eco_ImportExportPriceIndex1983 values ('"+year+"','12','"+vec[12]+"')");
        query.exec();
        
    }  
 }


void se_eco_MajorSectorProductivityCosts()
{
    
    CSmartConnection conn(SEDBCONN);
    Query query=conn->query();
    
    query.exec("drop table se_eco_MajorSectorProductivityCosts");
    query.exec("create table IF NOT EXISTS se_eco_MajorSectorProductivityCosts (year SMALLINT,Quarter SMALLINT,Nominal DOUBLE )");
    
    string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/MajorSectorProductivityCosts-1947-2012.csv");
    
    ifstream in(data.c_str());
    string line,year;
    cout<<"se_eco_MajorSectorProductivityCosts"<<endl;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    while(getline(in,line))
    {
        
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        year = vec[0];
        
        
        query = conn->query("insert into se_eco_MajorSectorProductivityCosts values ('"+year+"','1','"+vec[1]+"')");
        query.exec();
        query = conn->query("insert into se_eco_MajorSectorProductivityCosts values ('"+year+"','2','"+vec[2]+"')");
        query.exec();
        query = conn->query("insert into se_eco_MajorSectorProductivityCosts values ('"+year+"','3','"+vec[3]+"')");
        query.exec();
        query = conn->query("insert into se_eco_MajorSectorProductivityCosts values ('"+year+"','4','"+vec[4]+"')");
        query.exec();
    }
}

void se_eco_PPI_Commodities()
{
    
    CSmartConnection conn(SEDBCONN);
    Query query=conn->query();
    
    query.exec("drop table se_eco_PPI_Commodities");
    query.exec("create table IF NOT EXISTS se_eco_PPI_Commodities (year SMALLINT,month SMALLINT,Nominal DOUBLE )");
    string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/PPI-Commodities-1947-2012.csv");
    ifstream in(data.c_str());
    string line,year;
    cout<<"se_eco_PPI_Commodities"<<endl;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    while(getline(in,line))
    {
        
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        year = vec[0];
        
        
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','1','"+vec[1]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','2','"+vec[2]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','3','"+vec[3]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','4','"+vec[4]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','5','"+vec[5]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','6','"+vec[6]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','7','"+vec[7]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','8','"+vec[8]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','9','"+vec[9]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','10','"+vec[10]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','11','"+vec[11]+"')");
        query.exec();
        query = conn->query("insert into se_eco_PPI_Commodities values ('"+year+"','12','"+vec[12]+"')");
        query.exec();
        
    }  
}

void se_eco_UnemploymentRate()
{
    
    CSmartConnection conn(SEDBCONN);
    Query query=conn->query();
    
    query.exec("drop table se_eco_UnemploymentRate");
    query.exec("create table IF NOT EXISTS se_eco_UnemploymentRate (year SMALLINT,month SMALLINT,Nominal DOUBLE )");
    string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/UnemploymentRate-1947-2012.csv");
    ifstream in(data.c_str());
    string line,year;
    cout<<"se_eco_UnemploymentRate"<<endl;
    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    while(getline(in,line))
    {
        
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());
        year = vec[0];
        
        
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','1','"+vec[1]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','2','"+vec[2]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','3','"+vec[3]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','4','"+vec[4]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','5','"+vec[5]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','6','"+vec[6]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','7','"+vec[7]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','8','"+vec[8]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','9','"+vec[9]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','10','"+vec[10]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','11','"+vec[11]+"')");
        query.exec();
        query = conn->query("insert into se_eco_UnemploymentRate values ('"+year+"','12','"+vec[12]+"')");
        query.exec();
        
    }  
    
}

void se_eco_inflation()
{
        CSmartConnection conn(SEDBCONN);
        Query query=conn->query(); 
        query.exec("drop table se_eco_inflation");
        query.exec("create table IF NOT EXISTS se_eco_inflation (country_id SMALLINT,state_id SMALLINT,year SMALLINT,month SMALLINT,Nominal DOUBLE)");
        string data("/home/arun/Documents/smartearn/ebooks/Economics/Historical Data/FeedData/inflation.csv");
        ifstream in(data.c_str());
        string line,year,jan,feb,mar,apr,may,jun,jul,aug,sep,oct,nov,dec;
        cout<<"se_eco_inflation"<<endl;
        typedef tokenizer< escaped_list_separator<char> > Tokenizer;
        vector< string > vec;
        while(getline(in,line))
        {
        
                Tokenizer tok(line);
                vec.assign(tok.begin(),tok.end());
                year = vec[0];
                jan = vec[1];
                jan.erase(remove(jan.begin(), jan.end(), '%'), jan.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','1','"+jan+"')");
                query.exec();
                feb = vec[2];
                feb.erase(remove(feb.begin(), feb.end(), '%'), feb.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','2','"+feb+"')");
                query.exec();
                mar = vec[3];
                mar.erase(remove(mar.begin(), mar.end(), '%'), mar.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','3','"+mar+"')");
                query.exec();
                apr = vec[4];
                apr.erase(remove(apr.begin(), apr.end(), '%'), apr.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','4','"+apr+"')");
                query.exec();
                may = vec[5];
                may.erase(remove(may.begin(), may.end(), '%'), may.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','5','"+may+"')");
                query.exec();
                jun = vec[6];
                jun.erase(remove(jun.begin(), jun.end(), '%'), jun.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','6','"+jun+"')");
                query.exec();
                jul = vec[7];
                jul.erase(remove(jul.begin(), jul.end(), '%'), jul.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','7','"+jul+"')");
                query.exec();
                aug = vec[8];
                aug.erase(remove(aug.begin(), aug.end(), '%'), aug.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','8','"+aug+"')");
                query.exec();
                sep = vec[9];
                sep.erase(remove(sep.begin(), sep.end(), '%'), sep.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','9','"+sep+"')");
                query.exec();
                oct = vec[10];
                oct.erase(remove(oct.begin(), oct.end(), '%'), oct.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','10','"+oct+"')");
                query.exec();
                nov = vec[11];
                nov.erase(remove(nov.begin(), nov.end(), '%'), nov.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','11','"+nov+"')");
                query.exec();
                dec = vec[12];
                dec.erase(remove(dec.begin(), dec.end(), '%'), dec.end());
                query = conn->query("insert into se_eco_inflation values ('1','0','"+year+"','12','"+dec+"')");
                query.exec();
        }
}
int main(int argc, char** argv) 
{
    
    CSEConnections::InitConfig("../dbconn.conf");
    
    se_eco_inflation();
    se_eco_AverageHourlyEarnings();
    se_eco_CPI();
    se_eco_EmploymentCostIndex();
    se_eco_EmploymentHoursEarnings();
    se_eco_Import_ExportPriceIndex();
    se_eco_ImportExportPriceIndex1983();
    se_eco_MajorSectorProductivityCosts();
    se_eco_PPI_Commodities();
    se_eco_UnemploymentRate();
    
    return 0;
}


