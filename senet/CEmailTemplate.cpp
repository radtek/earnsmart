/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEmailTemplate.cpp
 * Author: santony
 * 
 * Created on March 26, 2016, 5:25 PM
 */

#include "senetinternal.h"
#include "CEmailTemplate.h"
#include <regex>
#include "../seglobal/CFile.h"
#include <streambuf>
#include "../semain/semain.h"
#include "../seglobal/exceptions.h"
#include "../seglobal/CDirectory.h"

using namespace std;
using namespace se::net;

CEmailTemplate::CEmailTemplate(ISession* pSession, const string& templateFile, const TemplateVaribles& variables) :
    _vars(variables)
{
    
    // get app 
    SP<IApp> papp;
    auto retCode = se::SEOpenApp(&papp);
    if (FAILED(retCode))
    {
        throw seexception(retCode);
    }
    
    // expected in emailtemplates folder in the working directory.
    const auto& cfgRoot = papp->get_Configurator()->get_ConfigRoot();
    string path = "/" + CDirectory::GetCurDir() + "/" + cfgRoot["mail"]["templatelocation"].asString();
    _filePath = path + "/" + templateFile;
    
    
    
    
    _pSession = pSession;
}

CEmailTemplate::~CEmailTemplate() 
{
    Close();
}

string CEmailTemplate::Apply() 
{
    string replacedFile = _filePath + "." + _pSession->get_UniqueId();
    
    ifstream ifs;
    ofstream ofs;
    ifs.open(_filePath);
    ofs.open(replacedFile);
    bool isgood = ofs.good();
    if (!isgood)
        throw std::runtime_error("File " + _filePath + " is invalid.");
    
    std::regex rgpattern(R"((\<%=)([a-zA-Z0-9]+)(%\>))", regex_constants::icase);
    string str;
    while (ifs >> str)
    {
        std::smatch matches;
        if (std::regex_search(str, matches, rgpattern))
        {
            auto itr = _vars.find(matches[2].str());
            if (itr != _vars.end())
            {
                str = itr->second;
            }
        }
        str = str + (str.find('>', str.length() - 1) | str.find('.', str.length() - 1) != string::npos ? " " : " ");
        ofs << str;
    }
    ofs.close();
    ifs.close();

    fstream f(replacedFile);
    stringstream ostrm ;
    ostrm << f.rdbuf();
    f.close();
    string output = ostrm.str();
    
    return output;
}

void CEmailTemplate::Close() 
{
    string replacedFile = _filePath + "." + _pSession->get_UniqueId();
    CFile::Delete(replacedFile.c_str());
}


