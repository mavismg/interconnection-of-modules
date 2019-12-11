//copyright mavismg - Mateus Jorge

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define MAXHOSTNAME 256

struct limite{ int limite; };
struct data{ char buff[512]; };

void* interface(void* arg)
{   
    struct data *buff = (struct data*)arg;
    struct limite *max = (struct limite*)arg;

    char *m_data = malloc(sizeof(char)*512);

    printf("Mensagem: ");

    for(int i = 0; i <= (max->limite); i++) 
    {
        scanf("%s" , m_data);
        buff->buff[i] = m_data[i];
    }

    pthread_exit(0);
}

void* cliente(void* arg)
{
    struct sockaddr_in remoteSocketInfo;
    struct hostent *hPtr;
    struct data socket_message;

    /*struct hostent *
gethostname (char *host)*/

    int socketHandle;
    char *remoteHost = "127.0.0.1"; //Ou localhost
    int portNumber = 6000;

    bzero(&remoteSocketInfo, sizeof(struct sockaddr_in));

    hPtr = gethostbyname(remoteHost);

    if(hPtr == NULL)
    {
        printf("Erro de DNS\n");
        return 0;
    }

    if((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        close(socketHandle);
        printf("Erro de criacao de socket...\n");
        return 0;
    }

    memcpy((char *)&remoteSocketInfo.sin_addr, hPtr->h_addr_list[1], hPtr->h_length); //h_addr é o primeiro elemento de h_addr_list
    remoteSocketInfo.sin_family = AF_INET;
    remoteSocketInfo.sin_port = htons((__u_short)portNumber);

    if(connect(socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(struct sockaddr_in)) < 0)
    {
        close(socketHandle);
        printf("Erro de conexao");
        return 0;
    }

    do
    {
        strcpy(socket_message.buff, "Message to send");
        send(socketHandle, socket_message.buff, strlen(socket_message.buff)+1, 0);
    } while (!strcpy(socket_message.buff, "fim"));

    pthread_exit(0);
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Utilizacao: %s iniciar\n" ,argv[0]);
        exit(-1);
    }

    int arg_num = argc-1;

    //SYNOPSIS
       //#include <pthread.h>

       //int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
       //                   void *(*start_routine) (void *), void *arg);

       //Compile and link with -pthread.

    pthread_t THREAD_ID_1[arg_num];
    pthread_t THREAD_ID_2[arg_num];

    struct limite limite[arg_num];

    for(int i = 0; i < arg_num; i++)
    {
        limite[i].limite = atoll(argv[1+i]);

        pthread_attr_t THREAD_ATRIBUTE;
        pthread_attr_init(&THREAD_ATRIBUTE);

        pthread_create(&THREAD_ID_1[i], &THREAD_ATRIBUTE, interface, &limite[i]);
        pthread_create(&THREAD_ID_2[i], &THREAD_ATRIBUTE, cliente, &limite[i]);
    }

    void *exit_status;

    //PROCESSOS

    for(int i = 0; i < arg_num; i++)
    {
        pthread_join(THREAD_ID_1[i], &exit_status);
        pthread_join(THREAD_ID_2[i], &exit_status);
    }
}

