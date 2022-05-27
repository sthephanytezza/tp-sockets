#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_LINE 4096
#define LINSTENPORT 7788
#define SERVERPORT 8877
#define BUFFSIZE 4096

void writefile(int sockfd, FILE *fp);

int main(int argc, char *argv[]) 
{

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientaddr, serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);


    if (bind(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1) 
    {
        perror("Bind Error");
        exit(1);
    }


    if (listen(sockfd, LINSTENPORT) == -1) 
    {
        perror("[ERRO] Não foi possível ouvir o client");
        exit(1);
    }


    socklen_t addrlen = sizeof(clientaddr);
    int connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &addrlen);
    if (connfd == -1) 
    {
        perror("[ERRO] Não foi possível estabelecer a conexão");
        exit(1);
    }
    close(sockfd);


    char filename[BUFFSIZE] = {0};
    if (recv(connfd, filename, BUFFSIZE, 0) == -1) 
    {
        perror("[ERRO] Não foi possível receber o nome do arquivo.");
        exit(1);
    }


    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) 
    {
        perror("[ERRO] Não foi possível abrir o arquivo.");
        exit(1);
    }
    

    char addr[INET_ADDRSTRLEN];
    printf("Iniciando recebimento do arquivo: %s de %s\n", filename, inet_ntop(AF_INET, &clientaddr.sin_addr, addr, INET_ADDRSTRLEN));
    writefile(connfd, fp);
    puts("[SUCESSO] Arquivo recebido com sucesso!");


    fclose(fp);
    close(connfd);
    return 0;
}

void writefile(int sockfd, FILE *fp)
{
    ssize_t n;
    char buff[MAX_LINE] = {0};
    while ((n = recv(sockfd, buff, MAX_LINE, 0)) > 0) 
    {
        if (n == -1)
        {
            perror("[ERRO] Não foi possível receber o arquivo");
            exit(1);
        }
        
    
        if (fwrite(buff, sizeof(char), n, fp) != n)
        {
            perror("[ERRO] Erro ao executar fwrite()");
            exit(1);
        }
        memset(buff, 0, MAX_LINE);
    }
}