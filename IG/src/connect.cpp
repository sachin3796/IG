/*
 *  connect.cpp
 *  IG
 *
 *  Created by Sach Patel on 28/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#include "../inc/connect.hpp"
#include "../inc/CustomExceptions.hpp"

#include <iostream>
#include <cstdlib>

namespace IG {
    
    IGConnect :: IGConnect (const char * const fn) :
    igPtr        (init_igPtr (fn), &free_igPtr),
    curl         (create_curl (), &destroy_curl),
    post_return  (nullptr, &cJSON_Delete),
    content_type ("application/json"),
    accept       ("application/json; charset=UTF-8")
    {
        std::string acc_type = igPtr->acc.type;
        try { if (acc_type != "DEMO" && acc_type != "LIVE") throw StringValueException ("Account type must either be 'DEMO' or 'LIVE'."); }
        catch (const StringValueException &e) { return; }
        base_url = [at=acc_type] (SS s) -> SS { return (s += (at == "DEMO") ? "demo-" : "") += "api.ig.com/gateway/deal/"; } ("https://");
        /* Initialise a libcurl object */
        curl.reset (curl_easy_init ());
        /* Set LibCurl options */
        if (curl.get () != nullptr) set_curl_options ();
    }
    
    IGConnect :: ~IGConnect (void) { }
    
    void IGConnect :: set_curl_options (void) {
        std::unique_ptr<tmplt_(curl_slist)> hd_rest (set_headers (), &curl_slist_free_all);
        std::unique_ptr<tmplt_(void)> json_postfields (J_body_parse (), &cJSON_free);
        std::unique_ptr<tmplt_(MemoryBlock)> return_data (init_memory (), &free_memory);
        
        curl_easy_setopt (curl.get (), CURLOPT_VERBOSE, VERBOSE_MODE);
        curl_easy_setopt (curl.get (), CURLOPT_POST, POST_MODE);
        
        curl_easy_setopt (curl.get (), CURLOPT_URL, [url=base_url] (CC * ext) mutable -> CC * { return (url+=ext).c_str (); } ("session"));
        curl_easy_setopt (curl.get (), CURLOPT_HTTPHEADER, hd_rest.get ());
        curl_easy_setopt (curl.get (), CURLOPT_POSTFIELDS, json_postfields.get ());
        curl_easy_setopt (curl.get (), CURLOPT_WRITEFUNCTION, &curl_callback);
        curl_easy_setopt (curl.get (), CURLOPT_WRITEDATA, return_data.get ());
        
        if (curl_easy_perform (curl.get ()) != CURLE_OK) std::cout << "An error occured with data retrieval." << std::endl;
        else process_data (return_data.get ());
    }
    
    void IGConnect :: process_data (MemoryBlock * mb) {
        switch (cleanup_request (mb)) {
            case RET_CODE::FAIL_ALL:
                std::cout << "Parsing and Reallocation of received data failed." << std::endl;
                break;
            case RET_CODE::FAIL_RETURN:
                std::cout << "Parsing of received data failed." << std::endl;
                break;
            case RET_CODE::FAIL_REALLOC:
                std::cout << "Reallocation of memory block failed." << std::endl;
                break;
            case RET_CODE::SUCCESS:
                std::cout << "Successfully received data." << std::endl;
                std::cout << cJSON_Print (post_return.get ()) << std::endl;
                break;
        }
    }
    
    RET_CODE IGConnect :: cleanup_request (MemoryBlock * mb) {
        char * tmp = nullptr;
        post_return.reset (cJSON_Parse (mb->memory));
        tmp = (char *) std::realloc (mb->memory, 1);
        if (post_return.get () == nullptr) {
            if (tmp == nullptr) {
                return RET_CODE::FAIL_ALL;
            } else {
                mb->memory = tmp;
                mb->size = 0;
                return RET_CODE::FAIL_RETURN;
            }
        } else if (tmp == nullptr) {
            return RET_CODE::FAIL_REALLOC;
        } else {
            mb->memory = tmp;
            mb->size = 0;
            return RET_CODE::SUCCESS;
        }
    }
    
    char * const IGConnect :: J_body_parse (void) {
        /* Build a JSON object and parse it as a string */
        char * str = nullptr;
        cJSON * json = cJSON_CreateObject ();
        cJSON_AddStringToObject (json, "identifier", igPtr->login.username);
        cJSON_AddStringToObject (json, "password",   igPtr->login.password);
        /* Parse the object to a string and free the memory. */
        str = cJSON_Print (json);
        cJSON_Delete (json);
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
