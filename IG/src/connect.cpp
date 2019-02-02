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
    igPtr        (init_igPtr (fn), &free_igPtr),
    content_type ("application/json"),
    accept       ("application/json; charset=UTF-8"),
    curl         (create_curl (), &destroy_curl),
    post_return  (nullptr, &JSON::cJSON_Delete)
    {
        std::string acc_type = igPtr->acc.type;
        if (acc_type == "DEMO") base_url = "https://demo-api.ig.com/gateway/deal/";
        else if (acc_type == "LIVE") base_url = "https://api.ig.com/gateway/deal/";
        else {
            std::cout << "The account type must either be 'DEMO' or 'LIVE'." << std::endl;
            return;
        }
        /* Initialise a libcurl object */
        curl.reset (curl_easy_init ());
        /* Set LibCurl options */
        if (curl.get () != nullptr) set_curl_options ();
    }
    
    IGConnect :: ~IGConnect (void) { }
    
    void IGConnect :: set_curl_options (void) {
        std::unique_ptr<tmplt_(curl_slist)> hd_rest (set_headers (), &curl_slist_free_all);
        std::unique_ptr<tmplt_(void)> json_postfields (J_body_parse (), &JSON::cJSON_free);
        std::unique_ptr<tmplt_(MemoryBlock)> return_data (init_memory (), &free_memory);
        
#if (DEBUG == 1)
        curl_easy_setopt (curl.get (), CURLOPT_VERBOSE, true);
        curl_easy_setopt (curl.get (), CURLOPT_POST, true);
#endif
        curl_easy_setopt (curl.get (), CURLOPT_URL, [url=base_url] (CC * ext) mutable -> CC * { return (url+=ext).c_str (); } ("session"));
        curl_easy_setopt (curl.get (), CURLOPT_HTTPHEADER, hd_rest.get ());
        curl_easy_setopt (curl.get (), CURLOPT_POSTFIELDS, json_postfields.get ());
        curl_easy_setopt (curl.get (), CURLOPT_WRITEFUNCTION, curl_callback);
        curl_easy_setopt (curl.get (), CURLOPT_WRITEDATA, return_data.get ());
        
        if (curl_easy_perform (curl.get ()) != CURLE_OK) std::cout << "An error occured with data retrieval." << std::endl;
        else process_data (return_data.get ());
    }
    
    void IGConnect :: process_data (MemoryBlock * mb) {
        switch (cleanup_request (mb)) {
            case RET_CODE::FAIL_ALL: { std::cout << "Parsing and Reallocation of received data failed." << std::endl; }
            case RET_CODE::FAIL_RETURN: { std::cout << "Parsing of received data failed." << std::endl; }
            case RET_CODE::FAIL_REALLOC: { std::cout << "Reallocation of memory block failed." << std::endl; }
            case RET_CODE::SUCCESS: {
                std::cout << "Successfully received data." << std::endl;
                std::cout << JSON::cJSON_Print (post_return.get ()) << std::endl;
            }
        }
    }
    
    RET_CODE IGConnect :: cleanup_request (MemoryBlock * mb) {
        post_return.reset (JSON::cJSON_Parse (mb->memory));
        if (post_return.get () == nullptr) {
            if ((mb->memory = (char *) std::realloc (mb->memory, 1)) == nullptr) return RET_CODE::FAIL_ALL;
            else {
                mb->memory = nullptr;
                return RET_CODE::FAIL_RETURN;
            }
        } else if ((mb->memory = (char *) std::realloc (mb->memory, 1)) == nullptr) {
            return RET_CODE::FAIL_REALLOC;
        } else {
            mb->memory = nullptr;
            mb->size = 0;
            return RET_CODE::SUCCESS;
        }
    }
    
    char * const IGConnect :: J_body_parse (void) {
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
        auto add_hdrs = [&hd] (SS key, CC * val) -> void { hd = curl_slist_append (hd, (key+=val).c_str ()); };
        add_hdrs ("X-IG-API-KEY: ", igPtr->API_Key);
        add_hdrs ("Content-Type: ", content_type);
        add_hdrs ("Accept: ", accept);
        return hd;
    }
    
}
