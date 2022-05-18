#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Função designada para chat entre cliente e servidor
void connectionChat(int sockfd){
    char buffer[MAX];
    int n;
    // loop infinito para o chat
    do{
        bzero(buffer, MAX);

        // le a mensagem do cliente e copia para o buffer
        recv(sockfd, buffer, sizeof(buffer), 0);

        

        if(strncmp("exit", buffer, 4) != 0){
            // printa o buffer que contém o conteúdo do cliente
            printf("\nMensagem do cliente: %s\t Resposta para o cliente : ", buffer);
            
            bzero(buffer, MAX);
            scanf("%[^\n]%*c", buffer);
            strcat(buffer, "\n");

            // e envia o buffer para o cliente
            send(sockfd, buffer, sizeof(buffer), 0);
        } else {
            send(sockfd, buffer, sizeof(buffer), 0);
        }
        printf("\n...Aguardando mensagem do cliente...\n\n");

    }  while(strncmp("exit", buffer, 4) != 0);


    printf("Cliente solicitou fechar conexão...\n");
    
}

// Driver connectionChattion
int main(){
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // cria o socket e verificação
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Falha na criação do socket...\n");
        exit(0);
    }
    else
        printf("Socket criado com sucesso...\n");
    bzero(&servaddr, sizeof(servaddr));

    // instancia IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // conecta o socket recem criado a um IP e verificação
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Ligação com o socket falhou...\n");
        exit(0);
    }
    else
        printf("Ligação com socket criada com sucesso...\n");

    // agora o servidor está pronto para ouvir e verificar
    if ((listen(sockfd, 5)) != 0) {
        printf("Falha na escuta...\n");
        exit(0);
    }
    else
        printf("Servidor ouvindo..\n");
    len = sizeof(cli);

    // aceita os pacotes de dados do cliente e verifica 
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Falha na aceitação do servidor...\n");
        exit(0);
    }
    else
        printf("Servidor aceitou o cliente...\n");

    // função para conversar entre cliente e servidor
    connectionChat(connfd);

    // depois de conversar fecha o socket
    close(sockfd);

    return 0;
}