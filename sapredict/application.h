/* 
 * File:   application.h
 * Author: santony
 *
 * Created on March 12, 2015, 9:33 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

class application {
public:
    application();
    application(const application& orig) = delete;
    application(application&& other) = delete;
    virtual ~application();
    
    
private:

};

#endif	/* APPLICATION_H */

