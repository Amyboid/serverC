#include <winsock2.h>
#include <ws2tcpip.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdio.h>

#define PORT 4443

int main()
{
    WSADATA wsaData;
    SOCKET listenSocket, clientSocket;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    SSL_CTX *ctx;
    SSL *ssl;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    // Create a listening socket
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        printf("socket() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Bind the socket to the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    if (bind(listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("bind() failed: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen() failed: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    printf("HTTPS server listening on port %d\n", PORT);

    // Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    // Create an SSL context
    ctx = SSL_CTX_new(TLSv1_server_method());
    if (ctx == NULL)
    {
        printf("SSL_CTX_new failed: %s\n", ERR_error_string(ERR_get_error(), NULL));
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Load the server certificate and private key
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0)
    {
        printf("SSL_CTX_use_certificate_file or SSL_CTX_use_PrivateKey_file failed: %s\n", ERR_error_string(ERR_get_error(), NULL));
        SSL_CTX_free(ctx);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Accept incoming connections
    while (1)
    {
        clientSocket = accept(listenSocket, (SOCKADDR *)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET)
        {
            printf("accept() failed: %d\n", WSAGetLastError());
            SSL_CTX_free(ctx);
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        // Create an SSL object and associate it with the client socket
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, clientSocket);

        // Perform the SSL/TLS handshake
        if (SSL_accept(ssl) <= 0)
        {
            printf("SSL_accept failed: %s\n", ERR_error_string(ERR_get_error(), NULL));
            SSL_free(ssl);
            closesocket(clientSocket);
            continue;
        }

        printf("HTTPS connection established with client\n");

        // Handle the client request
        char buffer[1024];
        int bytesRead = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            printf("Received request: %s\n", buffer);

            // Send a response
            const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, HTTPS client!\r\n";
            SSL_write(ssl, response, strlen(response));
        }
        else
        {
            printf("SSL_read failed: %s\n", ERR_error_string(ERR_get_error(), NULL));
        }

        // Clean up
        SSL_shutdown(ssl);
        SSL_free(ssl);
        closesocket(clientSocket);
    }

    // Clean up
    SSL_CTX_free(ctx);
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}