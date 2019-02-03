/*
 *  CustomExceptions.cpp
 *  IG
 *
 *  Created by Sach Patel on 03/02/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#include "CustomExceptions.hpp"

#include <iostream>

StringValueException :: StringValueException (const char * thr) : message (thr) {
    std::cout << what () << std::endl;
}

StringValueException :: ~StringValueException (void) {}

const char * StringValueException :: what (void) const throw () {
    return message;
}
