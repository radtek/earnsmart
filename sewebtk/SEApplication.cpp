/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SEApplication.cpp
 * Author: santony
 * 
 * Created on November 24, 2017, 11:31 AM
 */

#include "SEApplication.h"
#include <memory>

/*
 * https://redmine.webtoolkit.eu/boards/2/topics/6831
 * https://redmine.webtoolkit.eu/projects/wt/wiki/Frequently_Asked_Questions#Q-How-do-I-deal-with-look-and-layout-Does-Wt-support-CSS
 * https://redmine.webtoolkit.eu/projects/wt/wiki/Using_CSS
 * https://www.webtoolkit.eu/wt/doc/reference/html/overview.html#layout
 * https://redmine.webtoolkit.eu/boards/2/topics/223
 */

/*
 * Authentication sample
 * https://www.webtoolkit.eu/wt/doc/tutorial/auth.html
 * 
 */

SEApplication::SEApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env)
{
   auto layout = root()->setLayout(make_unique<WHBoxLayout>());
    layout->setPreferredImplementation(Wt::LayoutImplementation::JavaScript);
    layout->setContentsMargins(2,2,2,2);
    
    setTitle("Earn-smart");
 

}

SEApplication::~SEApplication()
{
}



