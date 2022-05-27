#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_LINE 4096
#define LINSTENPORT 7788
#define SERVERPORT 8877
#define BUFFSIZE 4096

void sendfile(FILE *fp, int sockfd);

int main(int argc, char* argv[])
{

    if (argc != 3) 
    {
        perror("[ERRO] Verifique o README e insira os argumentos necessários.");
        exit(1);
    }


    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    if (inet_pton(AF_INET, argv[2], &serveraddr.sin_addr) < 0)
    {
        perror("[ERRO] Verifique o endereço IP");
        exit(1);
    }


    if (connect(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("[ERRO] Problema na conexão.");
        exit(1);
    }
    

    char *filename = basename(argv[1]);
    if (filename == NULL)
    {
        perror("[ERRO] Verifique o nome do arquivo.");
        exit(1);
    }
    
    char buff[BUFFSIZE] = {0};
    strncpy(buff, filename, strlen(filename));

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) 
    {
        perror("[ERRO] Não foi possível abrir o arquivo.");
        exit(1);
    }


    sendfile(fp, sockfd);
    puts("[SUCESSO] Arquivo enviado.");


    fclose(fp);
    close(sockfd);
    return 0;
}

void sendfile(FILE *fp, int sockfd) 
{
    int n;
    char sendline[MAX_LINE] = {0};
    while ((n = fread(sendline, sizeof(char), MAX_LINE, fp)) > 0) 
    {
        if (n != MAX_LINE && ferror(fp))
        {
            perror("[ERRO] Não foi possível ler o arquivo.");
            exit(1);
        }
        
    
        if (send(sockfd, sendline, n, 0) == -1)
        {
            perror("[ERRO] Não foi possível enviar o arquivo.");
            exit(1);
        }
        memset(sendline, 0, MAX_LINE);
    }
}