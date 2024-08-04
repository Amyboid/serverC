#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headerutils.h"

void create_response(char *resp, resp_header rh, char *f)
{
    sprintf(resp, "HTTP/2 %d\ncontent-type: %s\ncontent-length: %ld\nserver: %s\ndate: %s\n\n", rh.status_code, rh.content_type, rh.content_length, rh.server, rh.date);
}

char *get_file_extension(char *filename)
{
    printf("filename from headreutils: %s\n", filename);
    char *file_extension;
    char *extension_position = strchr(filename, '.');
    if (extension_position != NULL)
    {
        file_extension = (char *)malloc(strlen(extension_position) + 1);
        strcpy(file_extension, extension_position);
        printf("\nextension: %s\n", file_extension);
        return file_extension;
    }
    else
    {
        return NULL;
    }
}

char *get_content_type(FILE *f, char *filename)
{

    char *file_extension = get_file_extension(filename);
    char *content_type = NULL;
    if (strcmp(filename, "/"))
    {
        if (file_extension != NULL && !strcmp(file_extension, ".html"))
        {
            content_type = (char *)malloc(strlen("text/html") + 1);
            strcpy(content_type, "text/html");
        }
        if (file_extension != NULL && !strcmp(file_extension, ".jpg"))
        {
            content_type = (char *)malloc(strlen("image/jpeg") + 1);
            strcpy(content_type, "image/jpeg");
        }
        if (file_extension != NULL && !strcmp(file_extension, ".png"))
        {
            content_type = (char *)malloc(strlen("image/png") + 1);
            strcpy(content_type, "image/png");
        }
        if (file_extension != NULL && !strcmp(file_extension, ".js"))
        {
            content_type = (char *)malloc(strlen("text/javascript") + 1);
            strcpy(content_type, "text/javascript");
        }
        if (file_extension != NULL && !strcmp(file_extension, ".txt"))
        {
            content_type = (char *)malloc(strlen("text/plain") + 1);
            strcpy(content_type, "text/plain");
        }
        if (file_extension != NULL && !strcmp(file_extension, ".gif"))
        {
            content_type = (char *)malloc(strlen("image/gif") + 1);
            strcpy(content_type, "image/gif");
        }
        if (file_extension != NULL && !strcmp(file_extension, ".pdf"))
        {
            content_type = (char *)malloc(strlen("application/pdf") + 1);
            strcpy(content_type, "application/pdf");
        }
        if (file_extension != NULL && !strcmp(file_extension, ".css"))
        {
            content_type = (char *)malloc(strlen("text/css") + 1);
            strcpy(content_type, "text/css");
        }
        if (file_extension != NULL && !strcmp(file_extension, ".json"))
        {
            content_type = (char *)malloc(strlen("application/json") + 1);
            strcpy(content_type, "application/json");
        }
        // if (file_extension != NULL && !strcmp(file_extension, ".mp3"))
        // {
        //     content_type = (char *)malloc(strlen("audio/mpeg") + 1);
        //     strcpy(content_type, "application/pdf");
        // }

        free(file_extension);
        printf("\nfrom get_content_type content_type: %s\n", content_type);
        return content_type;
    }
    else
    {
        return "text/html";
    }
}
