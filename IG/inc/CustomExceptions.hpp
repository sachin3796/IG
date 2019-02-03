/*
 *  CustomExceptions.hpp
 *  IG
 *
 *  Created by Sach Patel on 03/02/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#ifndef CustomExceptions_hpp
#define CustomExceptions_hpp

#include <exception>

namespace Except {
    
    class StringValue : public std::exception {
    private:
        const char * message;
        const char * what (void) const throw ();
    public:
        StringValue (const char * thr);
        ~StringValue (void);
    };
    
}

#endif /* CustomExceptions_hpp */
