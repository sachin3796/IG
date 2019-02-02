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

/* Turn Debugging Mode On */
#define DEBUG true
/* Templates */
template <class T1, class T2> struct _FUNC { typedef T1 (*functionPtr) (T2); };
template <typename T> using fnPtr = typename _FUNC<void, T>::functionPtr;
/* MACRO Function: Map "tmplt_(T)" --> "T = Constructor (), &Destructor(T*)" */
#define tmplt_(T) T, fnPtr<T*>

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
        const std::unique_ptr<tmplt_(const IGAuth)> igPtr;
        /* cURL API Objects */
        std::unique_ptr<tmplt_(CURL)> curl;
        /* cJSON API Objects */
        std::unique_ptr<tmplt_(JSON::cJSON)> post_return;
        /* Data required for HTTP requests */
        std::string base_url;
        const char * const content_type;
        const char * const accept;
        std::string CST;
        std::string XST;
        /* Class Functions */
        void set_curl_options (void);
        void process_data (MemoryBlock * mb);
        RET_CODE cleanup_request (MemoryBlock * mb);
        char * const J_body_parse (void);
        curl_slist * const set_headers (void);
        /* Type definitions */
        typedef const char CC;
        typedef std::string SS;
    protected:
    public:
        IGConnect (const char * const fn); // Constructor
        ~IGConnect (void); // Destructor
    };
    typedef class IGConnect IGConnect;
    
}

#endif /* connect_hpp */
