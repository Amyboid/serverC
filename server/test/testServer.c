#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/headerutils.h"

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

char *get_content_types(FILE *f, char *filename, long filelength)
{
    char *file_extension = get_file_extension(filename);
    char *content_type = NULL;
    long file_length = filelength;
    int bytes_to_read[] = {3, 4, 1};
    char *file_buffer[12];
    char *magic_numbers_of_extentions[][2] = {
        {"\xFF\xD8\xFF", ".jpg"},
        {"\x89\x50\x4E\x47", ".png"},
        {"\x7B", ".json"},
    };

    char *content_type_list[] = {"image/jpeg", "image/png", "application/json"};
    printf("******: %d", sizeof(magic_numbers_of_extentions[0]));
    for (int i = 0; i < sizeof(magic_numbers_of_extentions) / sizeof(magic_numbers_of_extentions[0]); i++)
    {
        if (!strcmp(file_extension, magic_numbers_of_extentions[i][1]))
        {
            fread(file_buffer, 1, bytes_to_read[i], f);
            if (memcmp(file_buffer, magic_numbers_of_extentions[i][0], 4) == 0)
            {
                content_type = content_type_list[i];
                break;
            }
        }
    }
    printf("\nbuffer is: %s\n", file_buffer);
    free(file_extension);
    printf("\nfrom get_content_type content_type: %s\n", content_type);
    return content_type;
}
