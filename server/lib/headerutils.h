#ifndef HEADER_UTILS_H
#define HEADER_UTILS_H
#include <stdio.h>
typedef struct
{
    int status_code;
    char *content_type;
    long content_length;
    char content_encoding[8];
    char server[16];
    char date[128];
} resp_header;
extern void create_response(char *responsebuffer, resp_header, char *filebuffer);
extern char *get_content_type(FILE *, char *filename);
extern char *get_file_extension(char *filename);
#endif
