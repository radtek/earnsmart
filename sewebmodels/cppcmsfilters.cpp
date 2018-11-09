/* 
 * File:   cppcmsfilters.cpp
 * Author: santony
 * 
 * Created on July 31, 2015, 10:24 AM
 */

#include "cppcmsfilters.h"
#include <regex>

using namespace cppcms::filters;

validationwidget::validationwidget()  
{
}

validationwidget::~validationwidget() 
{
}

validationwidget::validationwidget(validationwidget const &other) : 
    _controlValue(other._controlValue),
        _rgPattern(other._rgPattern),
        _controlName(other._controlName) ,
        _warningtext(other._warningtext)
{
}

validationwidget const &validationwidget::operator=(validationwidget const &other) 
{ 
    _controlValue=other._controlValue; 
    _rgPattern = other._rgPattern; 
    _controlName = other._controlName; 
    _warningtext = other._warningtext;
    return *this; 
}

validationwidget::validationwidget(streamable const &controlvalue, std::string const& controlname, std::string const& regexpattern, std::string const& warningtext) 
: _controlValue(controlvalue), _controlName(controlname), _rgPattern(regexpattern), _warningtext(warningtext)
{
}

void validationwidget::operator()(std::ostream &out) const
{
    // validate
    std::ostringstream ss;
    ss << _controlValue.get<string>();
    regex rg(_rgPattern);
    bool b = regex_match(ss.str(), rg);
    ss.clear();
    if (b)
    {
        ss << "";
    }
    else
    {
        ss << "<label class='validationerror'>" + _warningtext + "</label>";
    }
    out << ss.str();
}
