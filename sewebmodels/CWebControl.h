/* 
 * File:   CWebControl.h
 * Author: santony
 *
 * Created on August 5, 2015, 5:32 AM
 */

#include <cppcms/application.h>
#include <string>
#include <unordered_map>

#ifndef CWEBCONTROL_H
#define	CWEBCONTROL_H


class CWebControl  
{
public:
    cppcms::application& app;
    bool isrequired = false;
    bool validate = false;
    std::string id; // client side id.
    std::string validationText; // validation regex text
    std::string validationMessage;  // message displayed when validation failed.
    std::string className;      // Css class name
    std::string name;           // control name;
    std::string value;          // control value;
    std::string requiredstr = "*"; // displayed to right when isrequired is true.
    
public:
    CWebControl(cppcms::application&, const std::string& name);
    CWebControl(const CWebControl& orig) = delete;
    CWebControl& operator=(const CWebControl& orig) = delete;
    virtual ~CWebControl();
    void Render();
    virtual void RenderControl(std::ostream& out) = 0;
    
protected:
    virtual void get_validationLabel(std::ostream& out);
    
};

////////////////////inputbox////////////////////////
enum class InputType
{
    Text,
    Password,
    Radio,
    Checkbox,
    Hidden
};

class CInputControl : public CWebControl
{
public:
    InputType type = InputType::Text;
    
public:
    CInputControl(cppcms::application&, const std::string& name);
    CInputControl(const CInputControl& orig) = delete;
    CInputControl& operator=(const CInputControl& orig) = delete;
    virtual ~CInputControl();
    
    virtual void RenderControl(std::ostream& out);

};

////////////////////////////
typedef std::unordered_map<std::string, std::string> Options;

class CSelectControl : public CWebControl
{
private:
    std::function<void(const Options&)> cb = nullptr;
    
public:
    int size = 0;
    bool multiple = false;
    
public:
    CSelectControl(cppcms::application&, const std::string& name);
    CSelectControl(const CSelectControl& orig) = delete;
    CSelectControl& operator=(const CSelectControl& orig) = delete;
    virtual ~CSelectControl();
    
    void set_OptionsCb(std::function<void(const Options&)> cb);

    virtual void RenderControl(std::ostream& out);

};

std::ostream &operator<<(std::ostream &out, CWebControl &ctrl);



#endif	/* CWEBCONTROL_H */

