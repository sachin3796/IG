/*
 *  IG.c
 *  IG
 *
 *  Created by Sach Patel on 24/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#include "IG.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const struct IGAuth * const init_igPtr (const char * fn) {
    struct IGAuth * IG = NULL;
    if ((IG = file2ig (IG, fn)) == NULL) {
        printf ("Memory could not be allocated.\n");
        exit (EXIT_FAILURE);
    }
    return IG;
}

struct IGAuth * file2ig (struct IGAuth * ig_ptr, const char * fn) {
    unsigned int i;
    FILE * f;
    char buff [5][64];
    /* Read data from text file */
    if ((f = fopen (fn, "r")) == NULL) {
        printf ("Configuration file could not be read.\n");
        exit (EXIT_FAILURE);
    }
    for (i = 0; i < 5; ++i) {
        fscanf (f, "%s", buff[i]);
        fscanf (f, "%s", buff[i]);
    }
    fclose (f);
    /* Assert that fixed length strings are the correct size. */
    assert (buff[2][39] != '\0' && buff[2][40] == '\0');
    assert (buff[3][3]  != '\0' && buff[3][4]  == '\0');
    /* Allocate memory */
    ig_ptr                 = (struct IGAuth *) malloc (sizeof (struct IGAuth));
    ig_ptr->login.username = (const char *)    malloc (strlen ( buff[0] ) + 1);
    ig_ptr->login.password = (const char *)    malloc (strlen ( buff[1] ) + 1);
    ig_ptr->acc.number     = (const char *)    malloc (strlen ( buff[4] ) + 1);
    i = 0; /* Use as a bit-flag checker */
    if (ig_ptr                 == NULL) i |= FLAG_0;
    if (ig_ptr->login.username == NULL) i |= FLAG_1;
    if (ig_ptr->login.password == NULL) i |= FLAG_2;
    if (ig_ptr->acc.number     == NULL) i |= FLAG_5;
    /* If struct was not allocated properly free memory. */
    if (i != 0) {
        i = ~i; /* Invert bits */
        if (i & FLAG_5) free ((char *) ig_ptr->acc.number);
        if (i & FLAG_2) free ((char *) ig_ptr->login.password);
        if (i & FLAG_1) free ((char *) ig_ptr->login.username);
        if (i & FLAG_0) free ((struct IGAuth *) ig_ptr);
        return (struct IGAuth *) NULL;
    }
    /* Copy the strings to the struct */
    strcpy ((char *) ig_ptr->login.username, buff[0]);
    strcpy ((char *) ig_ptr->login.password, buff[1]);
    strcpy ((char *) ig_ptr->API_Key,        buff[2]);
    strcpy ((char *) ig_ptr->acc.type,       buff[3]);
    strcpy ((char *) ig_ptr->acc.number,     buff[4]);
    return ig_ptr;
}

void free_igPtr (const struct IGAuth * ig_ptr) {
    /* Deallocate memory. */
    free ((char *) ig_ptr->acc.number);
    free ((char *) ig_ptr->login.password);
    free ((char *) ig_ptr->login.username);
    free ((struct IGAuth *) ig_ptr);
}
