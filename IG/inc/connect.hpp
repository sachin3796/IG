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
#include <memory>

#include "IG.hpp"
#include "cURL.hpp"
#include "cJSON.hpp"

#define tmplt(T) T, void(*)(T *)

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
        std::unique_ptr<tmplt(const IGAuth)> igPtr;
        /* cURL API Objects */
        std::unique_ptr<tmplt(CURL)> curl;
        /* cJSON API Objects */
        std::unique_ptr<tmplt(JSON::cJSON)> post_return;
        /* Data required for HTTP requests */
        std::string base_url;
        const char * const content_type;
        const char * const accept;
        std::string CST;
        std::string XST;
        /* Class Functions */
        void process_data (MemoryBlock * &mb);
        /* Set-up Functions */
        char * const J_body_parse (void);
        curl_slist * const set_headers (void);
        void set_curl_options (void);
        RET_CODE cleanup_request (MemoryBlock * &mb);
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
