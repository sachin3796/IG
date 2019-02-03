/*
 *  IG.h
 *  IG
 *
 *  Created by Sach Patel on 24/01/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#ifndef IG_h
#define IG_h

#ifdef __cplusplus
extern "C" {
#endif
    
enum FLAGS {
    FLAG_0 = 0x01,
    FLAG_1 = 0x02,
    FLAG_2 = 0x04,
    FLAG_3 = 0x08,
    FLAG_4 = 0x10,
    FLAG_5 = 0x20
};
typedef enum FLAGS FLAGS;

struct IGAuth {
    const char API_Key[41]; /* The API Key for IG labs */
    struct {
        const char * username;
        const char * password;
    } login; /* Login credentials for my IG account */
    struct {
        const char type[5];
        const char * number;
    } acc; /* Account Information */
};
typedef struct IGAuth IGAuth;
    
extern struct IGAuth * const init_igPtr (const char * fn);
struct IGAuth * file2ig (struct IGAuth * ig_p, const char * fn);
extern void free_igPtr (struct IGAuth * ig_p);

#ifdef __cplusplus
}
#endif
    
#endif /* IG_h */
