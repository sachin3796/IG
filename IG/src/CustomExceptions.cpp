/*
 *  CustomExceptions.cpp
 *  IG
 *
 *  Created by Sach Patel on 03/02/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#include "CustomExceptions.hpp"

#include <iostream>

namespace Except {
    
    StringValue :: StringValue (const char * thr) : message (thr) {
        std::cout << what () << std::endl;
    }
    
    StringValue :: ~StringValue (void) {}
    
    const char * StringValue :: what (void) const throw () {
        return message;
    }
    
}
