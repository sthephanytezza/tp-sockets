#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void connectionChat(int welcomeSocket){
    char buffer[MAX];
    int n;

    do{
        //limpando buffer
        bzero(buffer, sizeof(buffer));

        printf("\nDigite uma mensagem: ");
        scanf("%[^\n]%*c", buffer);
        strcat(buffer, "\n");

        send(welcomeSocket, buffer, sizeof(buffer), 0);

        printf("\n...Aguardando resposta do servidor...\n\n");

        
        bzero(buffer, sizeof(buffer));
        recv(welcomeSocket, buffer, sizeof(buffer), 0);

        if(strncmp("exit", buffer, 4) != 0)
            printf("\tServidor responde: : %s", buffer);
            
    } while(strncmp(buffer, "exit", 4) != 0);

    printf("Encerrando conexão com servidor...\n");
}

int main(){
    int welcomeSocket, connfd;
    struct sockaddr_in serverAddr, cli;

    // cria o socket com três argumentos
    // 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case)
    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSocket == -1) {
        printf("Falha na criação do socket...\n");
        exit(0);
    }
    else
        printf("Socket criado com sucesso..\n");

    bzero(&serverAddr, sizeof(serverAddr));

    // instancia IP, PORT
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    // conecta o cliente socket ao servidor
    if (connect(welcomeSocket, (SA*)&serverAddr, sizeof(serverAddr)) != 0) {
        printf("Falha na conexão com o servidor...\n");
        exit(0);
    }
    else
        printf("Conectado com o servidor..\n");

    // função para o chat
    connectionChat(welcomeSocket);

    // fecha o socket
    close(welcomeSocket);
}