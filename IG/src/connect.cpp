/*
 *  connect.cpp
 *  Finance
 *
 *  Created by Sach Patel on 28/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#include <iostream>
#include <cstdlib>

#include "../inc/connect.hpp"

namespace IG {
    
    IGConnect :: IGConnect (const char * const fn) :
    igPtr        (init_igPtr (fn)),
    return_data  (init_memory ()),
    content_type ("application/json"),
    accept       ("application/json; charset=UTF-8"),
    curl         (nullptr),
    hd_rest      (nullptr),
    post_return  (nullptr) {
        std::string acc_type = igPtr->acc.type;
        if (acc_type == "DEMO") base_url = "https://demo-api.ig.com/gateway/deal/";
        else if (acc_type == "LIVE") base_url = "https://api.ig.com/gateway/deal/";
        else {
            std::cout << "The account type must either be 'DEMO' or 'LIVE'." << std::endl;
            return;
        }
        /* Initialise a libcurl object */
        curl_global_init (CURL_GLOBAL_ALL);
        if ((curl = curl_easy_init ()) == nullptr) return;
        /* Set LibCurl options */
        set_curl_options ();
        if (curl_easy_perform (curl) != CURLE_OK) {
            std::cout << "An error occured with data retrieval." << std::endl;
        } else process_data ();
    }
    
    void IGConnect :: process_data (void) {
        switch (cleanup_request ()) {
            case RET_CODE::FAIL_ALL: {
                std::cout << "Parsing and Reallocation of received data failed." << std::endl;
            } case RET_CODE::FAIL_RETURN: {
                std::cout << "Parsing of received data failed." << std::endl;
            } case RET_CODE::FAIL_REALLOC: {
                std::cout << "Reallocation of memory block failed." << std::endl;
            } case RET_CODE::SUCCESS: {
                std::cout << "Successfully received data." << std::endl;
                std::cout << JSON::cJSON_Print (post_return) << std::endl;
            }
        }
    }
    
    IGConnect :: ~IGConnect (void) {
        /* Cleanup for C objects */
        curl_slist_free_all (hd_rest);
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        JSON::cJSON_Delete (post_return);
        free_memory (return_data);
        free_igPtr (igPtr);
    }
    
    const char * const IGConnect :: J_body_parse (void) {
        /* Build a JSON object and parse it as a string */
        char * str = nullptr;
        JSON::cJSON * json = JSON::cJSON_CreateObject ();
        JSON::cJSON_AddStringToObject (json, "identifier", igPtr->login.username);
        JSON::cJSON_AddStringToObject (json, "password",   igPtr->login.password);
        /* Parse the object to a string and free the memory. */
        str = JSON::cJSON_Print (json);
        JSON::cJSON_Delete (json);
        return str;
    }
    
    curl_slist * const IGConnect :: set_headers (void) {
        curl_slist * hd = nullptr;
        auto add_hdrs = [&hd] (stdstr key, CC * val) -> void { hd = curl_slist_append (hd, (key+=val).c_str ()); };
        add_hdrs ("X-IG-API-KEY: ", igPtr->API_Key);
        add_hdrs ("Content-Type: ", content_type);
        add_hdrs ("Accept: ", accept);
        return hd;
    }
    
    void IGConnect :: set_curl_options (void) {
#if (DEBUG == 1)
        curl_easy_setopt (curl, CURLOPT_VERBOSE, true);
        curl_easy_setopt (curl, CURLOPT_POST, true);
#endif
        auto get_url = [url=base_url] (CC * ext="") mutable -> CC * { return (url+=ext).c_str (); };
        curl_easy_setopt (curl, CURLOPT_URL, get_url ("session"));
        
        curl_easy_setopt (curl, CURLOPT_HTTPHEADER, hd_rest = set_headers ());
        
        curl_easy_setopt (curl, CURLOPT_POSTFIELDS, J_body_parse ());
        curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, curl_callback);
        curl_easy_setopt (curl, CURLOPT_WRITEDATA, return_data);
    }
    
    RET_CODE IGConnect :: cleanup_request (void) {
        if ((post_return = JSON::cJSON_Parse (return_data->memory)) == nullptr) {
            if ((return_data->memory = (char *) std::realloc (return_data->memory, 1)) == nullptr) {
                return RET_CODE::FAIL_ALL;
            } else {
                return_data->memory = nullptr;
                return RET_CODE::FAIL_RETURN;
            }
        } else if ((return_data->memory = (char *) std::realloc (return_data->memory, 1)) == nullptr) {
            return RET_CODE::FAIL_REALLOC;
        } else {
            return_data->memory = nullptr;
            return_data->size = 0;
            return RET_CODE::SUCCESS;
        }
    }
    
}
