/*
 *  main.cpp
 *  Finance
 *
 *  Created by Sach Patel on 24/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#include <memory>
#include "../inc/connect.hpp"

#define DEBUG 1

int main (void) {
    
    static const std::unique_ptr<IG::IGConnect> streamPtr (new IG::IGConnect ("config_demo.txt"));
    
    return 0;
}
