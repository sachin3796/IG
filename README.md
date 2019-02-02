# IG

## *-- About --*

'IG' is an interface to connect to the IG-labs API, from the trading platform 'IG.com'.

At this current time, it is only able to connect to the RESTful API to collect data on accounts and watchlists.

In order to use this software, an account with IG.com (either 'LIVE' or 'DEMO') must be configured.

## *-- Installation --*

There is currently no support for Windows.
For Mac OS X or Linux, a link to the libcurl library and it is as easy as compiling the source files and running the executable 
(for now - no test have been built yet) - the interface is written in C, and wrapped in C++ for type-safety.

The C is all written in ANSI C (C89), and the C++ is written to the C++11 standard.

## *-- Dependencies --*

There are two external C libraries used to link this project.

1. libcurl: A library used to communicate with the IG API.
2. cJSON: A library used to create and convert JSON (JavaScript Object Notation) to and from C-style strings

cJSON does not require linkage to a library as the single .h and .c file has been included already in the project.

## *-- Usage --*

To use the project, set the text files to their appropriate values as shown in the config_example.txt file,
and read it into the `unique_ptr` in main.cpp.

## *-- Communication --*

The IG Labs API requires some information to set the POST request:
-> Login Credentials (Username and Password)
-> API Key (40 character string)
-> An Account type (Either "LIVE" or "DEMO"

A structure has been defined to store this information:
```
struct IGAuth {
    const char * API_Key; /* The API Key for IG labs */
    struct {
        const char * username;
        const char * password;
    } login; /* Login credentials for my IG account */
    struct {
        const char * type;
        const char * number;
    } acc; /* Account Information */
};
typedef struct IGAuth IGAuth;
```

Using the cURL and cJSON libraries, an interface for this data can communicate directly to the API, which then returns a
`cJSON *` object containing the server's response.
