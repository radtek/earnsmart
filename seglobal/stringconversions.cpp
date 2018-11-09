
#include "stringconversions.h"
//#include <boost/lexical_cast.hpp>
//#include <boost/locale.hpp>
#include <string>
#include <locale>

using namespace std;
//using namespace boost::locale::conv;

//CWSTR STRTOWSTR(CSTR in)
//{
//    string s(in);
//    wstring wstr = to_utf<WCHR>(s , "Latin1");
//    return wstr.c_str();
//}
//
//CSTR WSTRTOSTR(CWSTR in)
//{
//    wstring s(in);
//    string str = utf_to_utf<CHR>(s);
//    return str.c_str();
//}

void stringconvert(std::string& from, std::wstring& to)
{
    std::wstring strto(from.begin(), from.end());
    to = strto;
}

void stringconvert(std::wstring& from, std::string& to)
{
    std::string strto(from.begin(), from.end());
    to = strto;
}