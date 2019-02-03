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
    content_type ("application/json"),
    accept       ("application/json; charset=UTF-8") {
        std::string acc_type = igPtr->acc.type;
        try { if (acc_type != "DEMO" && acc_type != "LIVE") throw Except::StringValue ("Account type must either be 'DEMO' or 'LIVE'."); }
        catch (const Except::StringValue &e) { return; }
        base_url = [&t=acc_type] (SS s) -> SS { return (s += (t == "DEMO") ? "demo-" : "") += "api.ig.com/gateway/deal/"; } ("https://");
        /* Initialise a libcurl object */
        curl.reset (curl_easy_init ());
        /* Set LibCurl options */
        if (~curl != nullptr) set_curl_options ();
    }
    
    IGConnect :: ~IGConnect (void) { }
    
    void IGConnect :: set_curl_options (void) {
        uniquePtr<curl_slist> hd_rest (set_headers (), &curl_slist_free_all);
        uniquePtr<char> json_post (J_body_parse (), [] (char * s) { std::free (s); });
        uniquePtr<MemoryBlock> sent_data (init_memory (), &free_memory);
        uniquePtr<MemoryBlock> return_data (init_memory (), &free_memory);
        uniquePtr<cJSON> post_sent (nullptr, &cJSON_Delete);
        uniquePtr<cJSON> post_return (nullptr, &cJSON_Delete);
        
        curl_easy_setopt (~curl, CURLOPT_VERBOSE, VERBOSE_MODE);
        curl_easy_setopt (~curl, CURLOPT_POST, POST_MODE);
        curl_easy_setopt (~curl, CURLOPT_URL, [u=base_url] (CC * x) mutable -> CC * { return (u += x).c_str (); } ("session"));
        curl_easy_setopt (~curl, CURLOPT_HTTPHEADER, ~hd_rest);
        curl_easy_setopt (~curl, CURLOPT_POSTFIELDS, ~json_post);
        curl_easy_setopt (~curl, CURLOPT_READFUNCTION, &curl_callback);
        curl_easy_setopt (~curl, CURLOPT_READDATA, ~sent_data);
        curl_easy_setopt (~curl, CURLOPT_WRITEFUNCTION, &curl_callback);
        curl_easy_setopt (~curl, CURLOPT_WRITEDATA, ~return_data);
        
        if (curl_easy_perform (~curl) != CURLE_OK) std::cout << "An error occured with data retrieval." << std::endl;
        else {
            process_data (~sent_data, ~post_sent); /* Expect no data */
            process_data (~return_data, ~post_return); /* Expect lots of data */
        }
    }
    
    void IGConnect :: process_data (MemoryBlock * mb, cJSON * js) {
        char * tmp = nullptr;
        js = cJSON_Parse (mb->memory);
        switch ([&] (void) -> RET_CODE { if ((tmp = (char *) std::realloc (mb->memory, 1)) != nullptr) { *mb = { .memory = tmp, .size = 0 }; return (js != nullptr) ? SUCCESS : FAIL_POST; } else return (js != nullptr) ? FAIL_REALLOC : FAIL_ALL; } ()) {
            case FAIL_ALL:
                std::cout << "Parsing and Reallocation of data failed." << std::endl;
                break;
            case FAIL_POST:
                std::cout << "Parsing of data failed, or no data existed." << std::endl;
                break;
            case FAIL_REALLOC:
                std::cout << "Reallocation of memory block failed." << std::endl;
                break;
            case SUCCESS:
                std::cout << "Successfully parsed the data." << std::endl;
                std::cout << cJSON_Print (js) << std::endl;
                break;
        }
    }
    
    char * IGConnect :: J_body_parse (void) {
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
        auto add_hdrs = [&hd] (SS k, CC * v) -> void { hd = curl_slist_append (hd, (k += v).c_str ()); };
        add_hdrs ("X-IG-API-KEY: ", igPtr->API_Key);
        add_hdrs ("Content-Type: ", content_type);
        add_hdrs ("Accept: ", accept);
        return hd;
    }
    
}
