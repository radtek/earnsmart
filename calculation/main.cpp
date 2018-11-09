/* 
 * File:   main.cpp
 * Author: arun
 *
 * Created on 21 October, 2012, 2:47 PM
 */

#include <sstream>
#include <stdexcept>
#include<stdio.h>

#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <cstring>

#include <cstdlib>
#include<string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <vector>
#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>

#include "../semysql/CSEConnections.h"
#include<mysql++/mysql++.h>
#include <mysql++/ssqls.h>
#include <sstream>


#define SEDBCONN        CSEConnections::GetMain()

using namespace std;
using namespace se;
using namespace se::data;
using namespace mysqlpp;
using namespace boost;	
using namespace std;

/*
 * 
 */

void getXmlTreeofCalculation(CSmartConnection& conn, ofstream& output, int Parent = 0){
  if(Parent == 0)
    {
      
        output<<"<xbrl>"<<endl;
       
        
    } 
    string parent;
  
    ostringstream convert;
    convert<<Parent;
    parent =convert.str();
    mysqlpp::Query query = conn->query("SELECT se_xbrl_concepts.xbrl_id, xbrl_prefix, xbrl_name from se_xbrl_concepts, \
    se_xbrl_calculation where se_xbrl_concepts.xbrl_id = se_xbrl_calculation.xbrl_id and\
    se_xbrl_calculation.xbrl_parent_id ='"+parent+"' ORDER BY xbrl_name");
   
    
    
    auto result = query.store();
    for (mysqlpp::Row& row : result)
    {
        //cout<< "<" << row[1] << ":" << row[2] << ">" <<endl;
        output<< "<" << row[1] << ":" << row[2] << ">"<<endl ;
        getXmlTreeofCalculation(conn, output, atoi(row[0]));
        output << "</" << row[1] << ":" << row[2] << ">" <<"\n";
        //cout<<"</" << row[1] << ":" << row[2] << ">" <<endl;
    }

    if(Parent == 0)
    {
        output  << "</xbrl>";
    }
}

void getXmlTree(CSmartConnection& conn, ofstream& output, int Parent = 0)
{
    //ofstream out;
    
   
    if(Parent == 0)
    {
      
        output<<"<xbrl>"<<endl;
       
        
    }
    
    
    string parent;
  
    ostringstream convert;
    convert<<Parent;
    parent =convert.str();
    mysqlpp::Query query = conn->query("SELECT se_xbrl_concepts.xbrl_id, xbrl_prefix, xbrl_name from se_xbrl_concepts, \
    se_xbrl_presentation where se_xbrl_concepts.xbrl_id = se_xbrl_presentation.xbrl_id and\
   se_xbrl_presentation.xbrl_parent_id ='"+parent+"' ORDER BY xbrl_name");
   
    
    
    auto result = query.store();
    for (mysqlpp::Row& row : result)
    {
        //cout<< "<" << row[1] << ":" << row[2] << ">" <<endl;
        output<< "<" << row[1] << ":" << row[2] << ">"<<endl ;
        getXmlTree(conn, output, atoi(row[0]));
        output << "</" << row[1] << ":" << row[2] << ">" <<"\n";
        //cout<<"</" << row[1] << ":" << row[2] << ">" <<endl;
    }

    if(Parent == 0)
    {
        output  << "</xbrl>";
    }
    
}



int main(int argc, char** argv) 
{
    ofstream xmlfile;
    CSEConnections::InitConfig("../dbconn.conf");
    CSmartConnection conn(SEDBCONN);
    xmlfile.open("/home/santony/Downloads/presentation.xml",ios::app);
    getXmlTree(conn, xmlfile);
    xmlfile.close();
   
    ofstream xmlcalculation;
    xmlcalculation.open("/home/santony/Downloads/calculation.xml",ios::app);
    getXmlTreeofCalculation(conn,xmlcalculation);
    xmlcalculation.close();
    
    return 0;
     
}/* try
    {
        
        CSEConnections::InitConfig("../dbconn.conf");
        CSmartConnection conn(SEDBCONN);
        Query query=conn->query();
        query.exec("truncate table se_xbrl_calculation");
        string data("/home/santony/Documents/smartearn/Design Documents/xbrl/UGT-2012-Calculations.csv");
        ifstream in(data.c_str());
        char_separator<char> sep("`", "", keep_empty_tokens);
        typedef tokenizer< char_separator<char> > Tokenizer;
        typedef vector<string> TokenVector;
        
        TokenVector vec;
        string line,test,test2;
        string parent="",word;
        string 
                priority,
                order,
                depth,
                weight;

        string  parent_id,
                xbrl_id,
                sPfx,
                sName;

        int flag = 0;

        while(getline(in,line))
        {
      
            //prefix`name`label`depth`order`priority`weight`parent`arcrole`systemid

                trim(line);
                if (line.empty())
                    continue;
                
                Tokenizer tok(line, sep);
                vec.assign(tok.begin(), tok.end());

                sPfx=vec[0];
                
                trim(sPfx);
                sPfx.erase(remove(sPfx.begin(),sPfx.end(),'\''),sPfx.end());
                       
                if(sPfx=="LinkRole"||sPfx=="prefix"||sPfx=="")
                	continue;
                
                sName=vec[1];
                trim(sName);
                sName.erase(remove(sName.begin(),sName.end(),'\''),sName.end());
                
                //cout<<sPfx<<"\t"<<sName<<endl;
                mysqlpp::Query query = conn->query("SELECT xbrl_id FROM se_xbrl_concepts where xbrl_name='"+sName+"' and xbrl_prefix='"+sPfx+"'");
                mysqlpp::StoreQueryResult res = query.store();
                if(res.empty())
                     xbrl_id="0";
                else
                     xbrl_id=string(res[0][0]);

                //cout<<vec[7]<<endl;
                parent=vec[7];
                trim(parent);
                
                if(parent=="")
                    parent_id="0";
                else
                {
                    stringstream stream(parent);
                     while( getline(stream, word, ':') ){
                        if(flag==0){
                                test= word;
                                flag=1;
                        }
                        else if(flag==1){
                                test2=word;
                                flag=0;
                               
                        }
                     }
                    //cout<<test<<":"<<test2<<endl;
                    mysqlpp::Query query1 = conn->query("SELECT xbrl_id FROM se_xbrl_concepts where xbrl_name='"+test2+"' and xbrl_prefix='"+test+"'");
                    mysqlpp::StoreQueryResult res1 = query1.store();
                    if(res1.empty())
                        cout<<"notfound"<<endl;
                    else{
                        parent_id=string(res1[0][0]); 
                    }
                    
                    
                }
                //cout<<xbrl_id<<"\t"<<parent_id<<endl;    
            //prefix`name`label`depth`order`priority`weight`parent`arcrole`systemid
               
                priority = string(vec[5]);
                trim(priority);
                priority = priority.empty() ? "0" : priority;
                
                order = string(vec[4]);
                trim(order);
                order = order.empty() ? "0" : order;
                
                depth = string(vec[3]);
                trim(depth);
                depth = depth.empty() ? "0" : depth;

                weight = string(vec[6]);
                trim(weight);
                weight = weight.empty() ? "0" : weight;
                
                mysqlpp::Query query2 = conn->query("insert  into se_xbrl_calculation values ("+xbrl_id+", "+parent_id+", "+priority+", "+order+", "+depth+", " + weight + ")");
                query2.exec();
                
        }
    }
      catch (const mysqlpp::BadQuery& er) 
    {
       
        return E_DBQueryException;
    }
    catch (const mysqlpp::BadConversion& er) 
    {
     
        return E_DBQueryException;
    }
    catch (const mysqlpp::Exception& er) 
    {
       
        return E_DBQueryException;
    }
    catch (const std::exception& er)
    {
      
        return E_DBQueryException;
    }
    
}  

*/