#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include "../lib/headerutils.h"
#include <openssl/ssl.h>
#include <openssl/err.h> 
SSL_CTX *create_context()
{
    // SSL_library_init();
    // SSL_load_error_strings();
    printf("c1.0\n");
    SSL_CTX *ctx;
    const SSL_METHOD *method = TLSv1_server_method();
    ctx = SSL_CTX_new(method);
    printf("check: %d\n", ctx);
    printf("c1.1\n");
    if (!ctx)
    {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    printf("c1.3\n");
    return ctx;
}
void configure_context(SSL_CTX *ctx)
{
    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void print_server_url(int portnumber)
{
    printf("\x1b[38;5;11mServer is running at \x1b[0m");
    printf("\x1b[38;5;46mhttp://localhost:%d/\n\x1b[0m", portnumber);
}
int main()
{

    // OpenSSL_add_all_algorithms();

    // SSL_CTX *ctx = SSL_CTX_new(SSLv23_server_method());
    // SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM);
    // SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM);
    printf("c1\n");
    SSL_CTX *ctx;
    ctx = create_context();
    printf("c2\n");
    configure_context(ctx);
    printf("c3\n");
    char foldername[20] = {0};
    int portnumber = 5776;
    printf("Enter a folder name (present in root directory) where all files are present to serve by the server\n");
    scanf("%s", &foldername);
    printf("foldername: %s\n", foldername);

    WSADATA wsadata;
    // WSAStartup is used for initializing windows socket
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    // creating a server socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(portnumber);

    // bind the socket to the address
    if (bind(s, (const struct sockaddr *)&addr, sizeof(addr)))
    {
        printf("Error For bind: %d\n", WSAGetLastError());
    }
    // https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen
    if (listen(s, 12))
    {
        printf("Error For listen: %d\n", WSAGetLastError());
    }

    while (1)
    {
        SSL *ssl;
        print_server_url(portnumber);
        char request[1024] = {0};
        char method[10] = {0};
        char path[100] = {0};
        char filepath[100] = {0};
        // listening socket to accept a connection request from a client, creating a new socket for communication with that client
        SOCKET client = accept(s, 0, 0);
        printf("c4\n");
        ssl = SSL_new(ctx);
        printf("c5\n");
        SSL_set_fd(ssl, client);
        printf("c6\n");
        // recv(client, request, 1024, 0);
        // printf("\nPrinting value of request buffer:\n%s", request);
        // SSL *ssl = SSL_new(ctx);
        // SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0)
        {
            ERR_print_errors_fp(stderr);
            closesocket(client);
            continue;
        }

        SSL_read(ssl, request, sizeof(request));
        // Handle HTTPS connection using SSL_read() and SSL_write()

        char *filename, *temppath;
        sscanf(request, "%s %s ", &method, &path);
        // printf("\nmethods %s and  path: %s\n", method, path);
        filename = malloc(strlen(path) + 10);
        temppath = malloc(strlen(path) + 1);
        if (!strcmp(path, "/"))
        {
            sprintf(filename, "index.html", path);
            sprintf(filepath, "../%s/%s", foldername, filename);
        }

        else
        {
            strcpy(temppath, path);
            if (strchr(temppath, '.') == NULL)
            {
                sprintf(filepath, "../%s%s.html", foldername, path);
                sprintf(filename, "%s.html", path);
            }
            else
            {
                sprintf(filepath, "../%s%s", foldername, path);
                sprintf(filename, "%s", path);
            }
            free(temppath); // freeing memory for temppath
        }

        printf("\nfilepath: %s and filename %s\n", filepath, filename);
        FILE *f = fopen(filepath, "rb");
        if (f != NULL)
        {
            // calculating size of the file
            fseek(f, 0, SEEK_END);
            long filelength = ftell(f);
            fseek(f, 0, SEEK_SET);
            // printf("length: %ld\n", filelength);
            // creating response buffer and and another buffer to store content of requested file
            char responsebuffer[200 * 1024] = {0};
            char *buffer = (char *)malloc(filelength);
            fread(buffer, filelength, 1, f);

            // if client request for any file without extension it adds extension as .html

            char *content_type = get_content_type(f, filename);
            free(filename); // freeing memory for filename
            // printf("\ncontent-type = %s\n", content_type);

            if (content_type != NULL)
            {
                // initializing response header structure (present in headerutils.h)
                resp_header rs = {
                    .status_code = 200,
                    .content_length = filelength};
                rs.content_type = malloc(strlen(content_type) + 1);
                strcpy(rs.content_type, content_type);
                strcpy(rs.content_encoding, "gzip");
                strcpy(rs.server, "sSsh");
                strcpy(rs.date, "2022-01-01");

                // adding the header buffer to the begining of the response buffer and then adding the requested file buffer
                create_response(responsebuffer, rs, buffer);
                // sending response to the client
                // send(client, responsebuffer, strlen(responsebuffer), 0);
                // send(client, buffer, filelength, 0);
                SSL_write(ssl, responsebuffer, strlen(responsebuffer));
                SSL_write(ssl, buffer, filelength);
                free(rs.content_type); // freeing memory for rs.content_type
            }
            else
            {
                send(client, "415 Unsupported Media Type", 26, 0);
            }
            fclose(f);
            free(buffer); // freeing memory for buffer
        }
        else
        {
            printf("File does not exist\n");
            send(client, "404 not found", 13, 0);
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        closesocket(client);
    }
    SSL_CTX_free(ctx);
    WSACleanup();
    return 0;
}