/* 
 * File:   CInvestmentProducts.cpp
 * Author: santony
 * 
 * Created on October 20, 2015, 10:42 PM
 */

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <string>
#include "CInvestmentProducts.h"
#include "internal.h"
#include "../sewebmodels/CSecurityCharts.h"



using namespace std;

extern string _currentSkin;
extern string _emailtempls;


CInvestmentProducts::CInvestmentProducts(cppcms::service& srv) : cppcms::application(srv)
{
    dispatcher().assign(R"(/([a-zA-Z]+))", [this](string producttype)
    {
        BDBG << producttype;
        if (producttype == "stocks")
        {
            Stocks();
        }
        else if (producttype == "options")
        {
            Options();
        }
    }, 1);
    mapper().assign("ipcharts", R"(/{1}/charts)");

    dispatcher().assign(R"(/([a-zA-Z]+\/)?charts)", [this](string producttype)
    {
        BDBG << producttype;
        if (producttype.empty() or producttype == "stocks/")
        {
            StockCharts(nullptr);
        }
        else if (producttype == "options/")
        {
            
        }
        
    }, 1);
    mapper().assign("ipcharts", R"(/{1}/charts)");
    
    
    dispatcher().assign("", [this]()
    {
        response().out() << "Investment products.";
    });
    mapper().assign("");
    
    
    
}

CInvestmentProducts::~CInvestmentProducts()
{
    
}

void CInvestmentProducts::Stocks()
{

}

void CInvestmentProducts::Options()
{

}


void CInvestmentProducts::StockCharts(ISecurityDescription* pDesc)
{
    CSecurityCharts charts;
    render(_currentSkin, "ChartsView", charts);
}
