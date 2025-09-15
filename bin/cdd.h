/* cdd.h */
#ifndef CDD_H
#define CDD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>    
    #include <shellapi.h>
    #define CLEAR_CMD "rmdir /s /q \"%s\""
#else
    #include <unistd.h>
    #define CLEAR_CMD "rm -rf \"%s/*\""
#endif

int make_up(int argc, char** argv);

#endif