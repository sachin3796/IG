/*
 *  connect.cpp
 *  IG
 *
 *  Created by Sach Patel on 28/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#ifndef connect_hpp
#define connect_hpp

#include "IG.hpp"
#include "cURL.hpp"
#include "cJSON.hpp"
#include "functionPtr.hpp"

#include <string>

namespace IG {
    
    /* Custom Enumerations */
    enum CURL_MODE {
        VERBOSE_MODE = false,
        POST_MODE    = true
    };
    typedef enum CURL_MODE CURL_MODE;
    enum RET_CODE {
        SUCCESS,
        FAIL_POST,
        FAIL_REALLOC,
        FAIL_ALL
    };
    typedef enum RET_CODE RET_CODE;
    
    class IGConnect {
    private:
        /* IG Data Structure */
        uniquePtr<IGAuth> igPtr;
        /* cURL API Objects */
        uniquePtr<CURL> curl;
        /* cJSON API Objects */
        /* Data required for HTTP requests */
        std::string base_url;
        const char * const content_type;
        const char * const accept;
        std::string CST;
        std::string XST;
        /* Class Functions */
        void set_curl_options (void);
        void process_data (MemoryBlock * mb, cJSON * js);
        char * J_body_parse (void);
        curl_slist * const set_headers (void);
        /* Type definitions */
        typedef const char CC;
        typedef std::string SS;
        typedef cJSON cJ;
        typedef CURL CU;
        typedef curl_slist CLL;
    protected:
    public:
        IGConnect (const char * const fn); // Constructor
        ~IGConnect (void); // Destructor
    };
    typedef class IGConnect IGConnect;
    
}

#endif /* connect_hpp */
