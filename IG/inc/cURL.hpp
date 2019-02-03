/*
 *  cURL.hpp
 *  IG
 *
 *  Created by Sach Patel on 26/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#ifndef cURL_hpp
#define cURL_hpp

#ifdef __cplusplus
extern "C" {
#endif

/* Cannot wrap this library in a C++ namespace - causes objects from other libraries to break. */
#include "../cAPI/cURL/cURL_CB.h"
/* pthread_t and time_t structures both seem to break. */
    
#ifdef __cplusplus
}
#endif

#endif /* cURL_hpp */
