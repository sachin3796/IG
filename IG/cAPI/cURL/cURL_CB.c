/*
 *  cURL_CB.c
 *  Finance
 *
 *  Created by Sach Patel on 31/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cURL_CB.h"

size_t curl_callback (void * content, size_t sz, size_t nmemb, struct MemoryBlock * memblk) {
    size_t bufsize = sz * nmemb;
    char * tmp = NULL;
    if ((tmp = realloc (memblk->memory,  memblk->size + bufsize + 1)) == NULL) {
        printf ("Reallocation of memory failed.\n");
        return 0;
    }
    memblk->memory = tmp;
    memcpy (&(memblk->memory[memblk->size]), content, bufsize);
    memblk->size += bufsize;
    memblk->memory[memblk->size] = '\0'; /* Null-Terminate the string */
    return bufsize;
}

struct MemoryBlock * init_memory (void) {
    struct MemoryBlock * memblk = NULL;
    if ((memblk = malloc (sizeof (struct MemoryBlock))) == NULL) {
        printf ("Allocation of a memory block failed.\n");
        return (struct MemoryBlock *) NULL;
    }
    if ((memblk->memory = malloc (1)) == NULL) {
        printf ("Allocation of a memory block failed.\n");
        free (memblk);
        return (struct MemoryBlock *) NULL;
    }
    memblk->size = 0;
    return memblk;
}

void free_memory (struct MemoryBlock * memblk) {
    
    free ((char *) memblk->memory);
    free ((struct MemoryBlock *) memblk);
}

CURL * create_curl (void) {
    curl_global_init (CURL_GLOBAL_ALL);
    return (CURL *) NULL;
}

void destroy_curl (CURL * c) {
    curl_easy_cleanup (c);
    curl_global_cleanup ();
};
