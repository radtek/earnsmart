/* 
 * File:   CWebControl.cpp
 * Author: santony
 * 
 * Created on August 5, 2015, 5:32 AM
 */

#include <cppcms/http_response.h>
#include <cppcms/http_request.h>

#include "CWebControl.h"

CWebControl::CWebControl(cppcms::application& application, const std::string& name) :
app(application)
{
    if (name.empty())
        throw std::runtime_error("Control must have a name.");
    this->name = name;
}

CWebControl::~CWebControl() {
}

void CWebControl::Render() 
{
    auto& strm = app.response().out();
    RenderControl(strm);
}

void CWebControl::get_validationLabel(std::ostream& out) 
{
    if (validate and app.request().request_method()=="POST")
    {
        if (validationMessage.empty())
        {
            if (isrequired)
                validationMessage = "Required.";
            else
                validationMessage = "Not in correct format.";
        }
        out << "| <label style='color:orange'>";
        out << validationMessage;
        out << "</label>";
    }
    // else nothing is outputted.
}


////////////////////////////Inputbox//////////////////////////////

CInputControl::CInputControl(cppcms::application&, const std::string& name) :
CWebControl(app, name)
{

}

CInputControl::~CInputControl()
{
    
}

void CInputControl::RenderControl(std::ostream& out) 
{
    out << "<input ";
    if (!id.empty())
        out << "id='" << id << "' ";
    out << "name='" << name << "' ";
    out << "type='";
    switch (type)
    {
    case InputType::Text:
        out << "text";
        break;
    case InputType::Hidden:
        out << "hidden";
        break;
    case InputType::Checkbox:
        out << "checkbox";
        break;
    case InputType::Password:
        out << "password";
        break;
    case InputType::Radio:
        out << "radio";
        break;
    }
    out << "' ";
    out << "value='" << value << "' ";
    if (!className.empty())
        out << "class='" << className << "'>";
    if (isrequired)
        out << "| <label style='color:red'>" << requiredstr << "</label>";
    get_validationLabel(out);
}



/////////////////////////////////
CSelectControl::CSelectControl(cppcms::application& app, const std::string& name) : 
CWebControl(app, name)
{
}

CSelectControl::~CSelectControl() 
{
    
}

void CSelectControl::set_OptionsCb(std::function<void(const Options&) > cb) 
{
    this->cb = cb;
}


void CSelectControl::RenderControl(std::ostream& out) 
{
 
   out << "<select ";
    if (!id.empty())
        out << "id='" << id << "' ";
    out << "name='" << name << "' ";
    out << "value='" << value << "' ";
    if (!className.empty())
        out << "class='" << className << "' ";
    if (isrequired)
        out << "required ";
    if (size)
        out << "size='" << size << "' ";
    if (multiple)
        out << "multiple ";
    out << ">";
    if (cb)
    {
        Options options;
        cb(options);
        if (options.size())
        {
            for (auto p : options)
            {
                out << "<option value='" << p.first << "' ";
                if (p.first == value)
                    out << " selected ";
                out << ">";
                out << p.second;
                out << "</option>";
            }
        }
    }
    out << "</select>";
    if (isrequired)
        out << "| <label style='color:red'>" << requiredstr << "</label>";
    get_validationLabel(out);
    
}

std::ostream& operator<<(std::ostream& out, CWebControl& ctrl) 
{
    ctrl.RenderControl(out);
    return out;
}
