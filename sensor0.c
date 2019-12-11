/*
    Copyright mavismg - Mateus Jorge
    Materia: Sistemas de Computação - UFOP, Dezembro 2019
    Curso: Engenharia de Controle e Automação
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#define backlog 5
#define MAXHOSTNAME 256

#define true 1

struct limite{ int limite; };
struct data{ char buff[512]; };

int flag;

void* server(void* arg)
{   
    int port = 6000;
    int sockopt__ = true; int state__ = true;
    int sock, connect, bytes_received;
    char send_data[1024], recv_data[1024];

    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;

    /*struct rec_data *recv__ = (struct rec_data*)arg;
    struct limite *max = (struct limite*)arg;*/

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("socket\n");
        exit(state__);
    }

    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&sockopt__,sizeof(int)) == -1) {
        perror("setsockopt\n");
        exit(state__);
    } 

    server_addr.sin_family = AF_INET;
    server_addr.sin_port    = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_addr.sin_zero), 8); 

    if(bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Unable to bind\n");
        exit(state__);
    }

    if(listen(sock, backlog) == -1)
    {
        perror("Listen\n");
        exit(state__);
    }

    printf("\n\nTCPServer esperando pela conexao na porta %d\n", port);
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
    
    pthread_exit(0);
}

void* interface(void* arg)
{   
    struct data *buff = (struct data*)arg;
    struct limite *max = (struct limite*)arg;

    unsigned char *m_data = malloc(sizeof(char)*512);

    printf("Mensagem: ");

    for(int i = 0; i <= (max->limite); i++) 
    {
        scanf("%s" , &m_data[0]);
        buff->buff[i] = m_data[i];
    }

    pthread_exit(0);
}

void* cliente(void* arg)
{
    struct sockaddr_in remoteSocketInfo;
    struct hostent *hPtr;
    struct data socket_message;
    //struct rec_data state;
    //struct limite *max = (struct limite*)arg;

    /*  DOCUMENTATION
        struct hostent *
        gethostname (char *host)
    */

    int socketHandle;
    char *remoteHost = "127.0.0.1";
    int portNumber = 5000;

    bzero(&remoteSocketInfo, sizeof(struct sockaddr_in));

    hPtr = gethostbyname(remoteHost);

    if(hPtr == NULL)
    {
        printf("\nErro de DNS\n");
        return 0;
    }

    if((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        close(socketHandle);
        printf("Erro de criacao de socket...\n");
        return 0;
    }

    /*  DOCUMENTATION
        h_addr passou a ser h_addr_list
        V1.0.1 Erro de Segmentacao -> Status: Corrigido
    */

    memcpy((char *)&remoteSocketInfo.sin_addr, hPtr->h_addr_list[0], hPtr->h_length);
    remoteSocketInfo.sin_family = AF_INET;
    remoteSocketInfo.sin_port = htons((__u_short)portNumber);

    if(connect(socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(struct sockaddr_in)) < 0)
    {
        close(socketHandle);
        printf("Erro de conexao\n");
        return 0;
    }

    do
    {
        /*
        if(flag == 1) 
        {
            for(int i = 0; i <= (max->limite); i++) socket_message.buff[i] = state.recv__[i];
            strcpy(socket_message.buff, "Message to send ");
            send(socketHandle, socket_message.buff, strlen(socket_message.buff)+1, 0);
        }*/

        /*
            Se a flag for diferente de 1 ele ira mandar apenas a mensagem atual.
        */

        strcpy(socket_message.buff, "Message to send: ");
        send(socketHandle, socket_message.buff, strlen(socket_message.buff)+1, 0);

    } while (!strcpy(socket_message.buff, "fim"));

    pthread_exit(0);
}

//SYNOPSIS
       //#include <pthread.h>

       //int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
       //                   void *(*start_routine) (void *), void *arg);

       //Compile and link with -pthread.

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Utilizacao: %s iniciar\n" ,argv[0]);
        exit(-1);
    }
    
    int arg_num = argc-1;

    pthread_t THREAD_ID_1[arg_num];
    pthread_t THREAD_ID_2[arg_num];
    pthread_t THREAD_ID_3[arg_num];

    struct limite limite[arg_num];

    for(int i = 0; i < arg_num; i++)
    {
        limite[i].limite = atoll(argv[1+i]);

        pthread_attr_t THREAD_ATRIBUTE;
        pthread_attr_init(&THREAD_ATRIBUTE);

        pthread_create(&THREAD_ID_1[i], &THREAD_ATRIBUTE, server, &limite[i]);
        pthread_create(&THREAD_ID_2[i], &THREAD_ATRIBUTE, interface, &limite[i]);
        pthread_create(&THREAD_ID_3[i], &THREAD_ATRIBUTE, cliente, &limite[i]);
    }

    void *exit_status;

    /*OUTROS PROCESSOS*/

    /*JOIN*/

    for(int i = 0; i < arg_num; i++)
    {
        pthread_join(THREAD_ID_1[i], &exit_status);
        pthread_join(THREAD_ID_1[i], &exit_status);
        pthread_join(THREAD_ID_1[i], &exit_status);
    }
}