//server
//copyright mavis - Mateus Jorge

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define port 5000
#define backlog 5

int main(int argc, char **argv)
{
    bool sockopt__ = true; bool state__ = true;
    int sock, connect, bytes_received;
    char send_data[1024], recv_data[1024];

    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&sockopt__,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    } 

    server_addr.sin_family = AF_INET;
    server_addr.sin_port    = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_addr.sin_zero), 8); 

    if(bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Unable to bind");
        exit(state__);
    }

    if(listen(sock, backlog) == -1)
    {
        perror("Listen");
        exit(state__);
    }

    printf("\n\nTCPServer esperando pela conexao na porta %d", port);
    fflush(stdout);

    while(state__)
    {
        sin_size = sizeof(struct sockaddr_in);
        connect = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
        printf("\n Conexao recebida de (%s, %d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        while(state__)
        {
            send(connect, send_data, strlen(send_data), 0);
            bytes_received = recv(connect, recv_data, 1024,0);
            recv_data[bytes_received] = '\0';

            printf("\n\n Dados recebidos %s", recv_data);
            fflush(stdout);
        }
    }
    close(sock);
    exit(0);
}