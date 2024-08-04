#include <stdio.h>
#include <string.h>

typedef enum FileType
{
    JPEG,
    PNG,
    GIF,
    BMP,
    UNKNOWN
} Ftype;

int main()
{
    FILE *file;
    char buffer[4]; // Read the first 4 bytes of the file
    const char *magicNumbers[][2] = {
        {"\xFF\xD8\xFF", "JPEG"},
        {"\x89\x50\x4E\x47", "PNG"},
        {"\x47\x49\x46\x38", "GIF"},
        {"\x42\x4D", "BMP"}};

    file = fopen("fff.jpg", "rb"); // Open the file in binary mode
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    fread(buffer, 1, 4, file); // Read the first 4 bytes of the file
    printf("buffer: %s\n",buffer);
    Ftype fileType = UNKNOWN;

    for (int i = 0; i < sizeof(magicNumbers) / sizeof(magicNumbers[0]); i++)
    {
        if (memcmp(buffer, magicNumbers[i][0], 4) == 0)
        {
            fileType = i;
            break;
        }
    }

    switch (fileType)
    {
    case JPEG:
        printf("File type: JPEG\n");
        break;
    case PNG:
        printf("File type: PNG\n");
        break;
    case GIF:
        printf("File type: GIF\n");
        break;
    case BMP:
        printf("File type: BMP\n");
        break;
    default:
        printf("Unknown file type\n");
    }

    fclose(file); // Close the file

    return 0;
}
