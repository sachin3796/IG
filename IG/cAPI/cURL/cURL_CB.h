/*
 *  cURL_CB.h
 *  IG
 *
 *  Created by Sach Patel on 31/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#ifndef cURL_CB_h
#define cURL_CB_h

#include <curl/curl.h>

typedef struct curl_slist curl_slist;

struct MemoryBlock {
    char * memory;
    size_t size;
};
typedef struct MemoryBlock MemoryBlock;

extern struct MemoryBlock * init_memory (void);
extern void free_memory (struct MemoryBlock * memblk);
extern size_t curl_callback (void * content, size_t size, size_t nmemb, struct MemoryBlock * memblk);

extern CURL * create_curl (void);
extern void destroy_curl (CURL * c);

#endif /* cURL_CB_h */
