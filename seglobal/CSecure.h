/* 
 * File:   CSecure.h
 * Author: santony
 *
 * Created on August 10, 2015, 3:56 PM
 */

#ifndef CSECURE_H
#define	CSECURE_H

class CSecure {
public:
    CSecure();
    CSecure(const CSecure& orig);
    virtual ~CSecure();
    
    // Generate a random password by filling positions in freeBuffer parameter.
    // The freeBuffer must be pre-allocated.
    static void GenerateRandomPassword(char* freeBuffer, size_t len = 10);
    
private:

};

#endif	/* CSECURE_H */

