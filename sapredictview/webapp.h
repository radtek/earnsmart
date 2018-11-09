/* 
 * File:   webapp.h
 * Author: santony
 *
 * Created on March 12, 2015, 9:49 PM
 */

#ifndef WEBAPP_H
#define	WEBAPP_H

#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include <cppcms/application.h>

using namespace cppcms;

class webapp : public application
{
public:
    webapp(cppcms::service& srv);
    webapp(const webapp& orig) = delete;
    webapp(webapp&& orig) = delete;
    virtual ~webapp();
private:

};

#endif	/* WEBAPP_H */

