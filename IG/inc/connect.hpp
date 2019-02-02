/*
 *  connect.cpp
 *  Finance
 *
 *  Created by Sach Patel on 28/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#ifndef connect_hpp
#define connect_hpp

#include <string>

#include "IG.hpp"
#include "cURL.hpp"
#include "cJSON.hpp"

namespace IG {
    
    enum class RET_CODE {
        SUCCESS,
        FAIL_RETURN,
        FAIL_REALLOC,
        FAIL_ALL
    };
    typedef enum RET_CODE RET_CODE;
    
    class IGConnect {
    private:
        /* IG Data Structure */
        const IGAuth * const igPtr;
        /* cURL API Objects */
        CURL * curl;
        MemoryBlock * return_data;
        curl_slist * hd_rest;
        /* Data required for HTTP requests */
        std::string base_url;
        const char * const content_type;
        const char * const accept;
        JSON::cJSON * post_return;
        std::string CST;
        std::string XST;
        /* Class Functions */
        void process_data (void);
        /* Set-up Functions */
        const char * const J_body_parse (void);
        curl_slist * const set_headers (void);
        void set_curl_options (void);
        RET_CODE cleanup_request (void);
        typedef const char CC;
        typedef std::string stdstr;
    protected:
    public:
        IGConnect (const char * const fn); // Constructor
        ~IGConnect (void); // Destructor
    };
    typedef class IGConnect IGConnect;
    
}

#endif /* connect_hpp */
