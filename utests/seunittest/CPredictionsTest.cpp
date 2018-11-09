/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPredictionsTest.cpp
 * Author: santony
 * 
 * Created on September 3, 2016, 11:22 AM
 */

#include "CPredictionsTest.h"
#include "../../sesecurities/sesecuritiesapi.h"
#include "../../seglobal/CDirectory.h"
#include <libfccp/csv.h>
#include <boost/unordered/unordered_map.hpp>
#include "../../seglobal/cpplinq.hpp"
#include <unordered_map>
#include "../../semath/semathapi.h"
#include "../../semath/CCommon.h"

using namespace se::securities;
using namespace io;
using namespace se::math;


CPredictionsTest::CPredictionsTest() 
{
}

CPredictionsTest::CPredictionsTest(const CPredictionsTest& orig) 
{
}

CPredictionsTest::~CPredictionsTest() 
{
}

void CPredictionsTest::testPredictUsingMVLR() 
{
    this->NonInteractiveUserLogin("admin", "963051");
    
    RP<ISecurityAPI> psapi = pSession->get_SecurityApi();
    CPPUNIT_ASSERT(psapi);
    Json::Value args;
    args["Exch"] = "AMEX";
    args["Sym"] = "TIS";
    Json::Value output;
    SE_CODE ret = psapi->get_Security(args, output);
    CPPUNIT_ASSERT(ret == S_OK);
    BINF << output["Name"];
    
}

void CPredictionsTest::testConvertGDPData()
{
    using namespace cpplinq;
    
    this->NonInteractiveUserLogin("admin", "963051");
    
    SP<IMathApi> pMath;
    CPPUNIT_ASSERT(MathOpen(pSession, &pMath)==S_OK);
    
    R8 y1, y2, x1, x2, y, x;
    y1 = 0.;
    y2 = 100.;
    x1 = 10000.;
    x2 = 50000.;
    x =  20512.;
    SP<IInterpolations> plin;
    CPPUNIT_ASSERT(pMath->get_InterpolationMethods(InterPolationTypes::Linear,
            2, &plin )==S_OK);
    y = plin->get_y(y1, y2, x1, x2, x);
    BINF << "Interpolated value of y is " <<  y ;
    BINF << " for (y1, y2) = " << "(" << y1 << ", " << y2 << ") and ";
    BINF << " (x1, x2) = " << "(" << x1 << ", " << x2 << ")";
    
    se::math::CCommon mops;
    
    typedef vector<string> Files, Parts;
    auto homePath = CDirectory::GetHome();
    string dataLoc = CDirectory::BuildPath({homePath.c_str(), "Documents", "smartgaze", "gdpprojection", "gdpcubistmodel", "data"});
    auto func = [](CSTR path, bool isdir, POINTER state)->bool
    {
        Files* f = static_cast<Files*>(state);
        if (!isdir && strstr(path, ".csv"))
        {
            f->push_back(path);
        }
        return true;
    };
    // get files
    Files files;
    bool success = CDirectory::EnumDir(dataLoc.c_str(), func, (POINTER)&files);
    CPPUNIT_ASSERT(success);
    Json::Value metas ;
    for(const auto& str : files)
    {
        Json::Value meta;
        meta["path"] = str;
        string cols[2];
        string filename = cpplinq::from(CDirectory::BuildParts(str.c_str())) >> last_or_default();
        Json::Value& cn = meta["Cols"];
        cn.append("DATE");
        if (filename == "ConsumerSpendingAdjusted.csv")
        {
            meta["title"] = "ConsumerSpending";
            cn.append("PCEC");
            meta["order"] = 0;
        }
        else if (filename == "GDP.csv")
        {
            meta["title"] = "GDP";
            cn.append("GDP");
            meta["order"] = 4;
        }
        else if (filename == "GovtTotalExpendituresAdjusted.csv")
        {
            meta["title"] = "GovtSpending";
            cn.append("W068RCQ027SBEA");
            meta["order"] = 1;
        }
        else if (filename == "GrossPrivateInvestmentAdjusted.csv")
        {
            meta["title"] = "FirmSpending";
            cn.append("GPDI");
            meta["order"] = 2;
        }
        else if (filename == "NETEXP.csv")
        {
            meta["title"] = "NetExpImp";
            cn.append("NETEXP");
            meta["order"] = 3;
        }
        meta["Stats"]["Min"] = 0.;
        meta["Stats"]["Max"] = 0.;
        metas.append(meta);
    }
    auto result = from(metas) >> orderby([](Json::Value const& v){ return v["order"].asInt();}) >> to_vector();
    Json::Value data;
    Json::Value& metalist = data["Meta"];
    Json::Value& matrix = data["matrix"];
    Json::Value& matrixdata = matrix["data"];
    typedef std::vector<R8> Values;
    typedef std::unordered_map<string, Values> Dict;
    Json::Value header;
    header.append("Date");
    Dict dict;
    int index = 0;
    for (auto& meta : result)
    {
        header.append(meta["title"].asString());
        CSVReader<2> in(meta["path"].asString());
        Json::Value& cols = meta["Cols"];
        Json::Value& stat = meta["Stats"];
        string c1 = cols[1].asString();
        in.read_header(ignore_extra_column, "DATE", c1);
        string date;
        R8 value = 0.;
        stat["Min"] = numeric_limits<R8>::max();
        stat["Max"] = numeric_limits<R8>::min();
        while (in.read_row(date, value))
        {
            auto dt = CDate::Parse(date.c_str());
            if (dt.get_Year() < 2000)
            {
                continue;
            }
            auto ifound = dict.find(date);
            if (ifound != dict.end())
            {
                ifound->second[index] = value;
            }
            else
            {
                Values vals(result.size());
                vals[index] = value;
                dict.insert(make_pair(date, vals));
            }
            stat["Min"] =  mops.RoundNear(std::min(stat["Min"].asDouble(), value), 4);
            stat["Max"] = mops.RoundNear(std::max(stat["Max"].asDouble(), value), 4);
        }
        metalist.append(meta);
        index++;
    }
    matrixdata.append(header);
    for(auto& k : dict)
    {
        auto& v = k.second;
        Json::Value row;
        row.append(k.first);
        Json::Value& metas = data["Meta"];
        int metapos = 0;
        for(R8 r : v)
        {
            // linear interpolation calculation goes here...
            Json::Value& meta = metas[metapos];
            Json::Value rdet;
            R8 Min = meta["Stats"]["Min"].asDouble(), Max = meta["Stats"]["Max"].asDouble();
            R8 scalePos = plin->get_y(0., 100., Min, Max, !r ? Min : mops.RoundNear(r, 4));
            rdet.append(scalePos);
            rdet.append(r);
            row.append(rdet);
            metapos++;
        }
        matrixdata.append(row);
    }
    BINF << data;
    ofstream ofs;
    ofs.open (dataLoc + "/gdp.json");
    ofs << data;
    ofs.close(); 
    return;
}

