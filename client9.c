#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000

int main(int argc, char const* argv[]) {
    int client_socket, b;
    struct sockaddr_in server_address;
    char buffer_menu[1024] = {0}, buffer_data[1024] = {0};

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // configura endereço do servidor
    if (inet_pton(AF_INET, "192.168.2.61", &server_address.sin_addr) <= 0) {
        perror("Endereço inválido");
        exit(EXIT_FAILURE);
    }

    // tenta conectar ao servidor
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Erro na conexão");
        exit(EXIT_FAILURE);
    }

    // recebe o menu do servidor
    recv(client_socket, buffer_menu, sizeof(buffer_menu), 0);
    printf("%s", buffer_menu);

    // coleta e envia opção para o servidor
    int choice;
    scanf("%d", &choice);
    send(client_socket, &choice, sizeof(choice), 0);

    if (choice == 1) {
        char filename[256];
        printf("Digite o nome do arquivo a ser enviado: ");
        scanf("%s", filename);

        // enviando o nome do arquivo para o servidor
        send(client_socket, filename, strlen(filename) + 1, 0);

        FILE* file_to_send = fopen(filename, "rb");
        if (file_to_send == NULL) {
            perror("Erro ao abrir o arquivo");
            exit(EXIT_FAILURE);
        }

        while ((b = fread(buffer_data, 1, sizeof(buffer_data), file_to_send)) > 0) {
            send(client_socket, buffer_data, b, 0);
        }

        fclose(file_to_send);
        printf("Arquivo enviado com sucesso!\n");
    } else if (choice == 2) {
        // recebendo a lista de arquivos do servidor
        FILE* received_list = fopen("list", "wb");
        if (received_list == NULL) {
            perror("Erro ao criar list");
        }

        while ((b = recv(client_socket, buffer_data, sizeof(buffer_data), 0)) > 0) {
            fwrite(buffer_data, 1, b, received_list);
        }
	if (b < 0) perror("Erro ao receber lista");

        fclose(received_list);
        printf("Lista recebida e salva no arquivo list\n");
    } else {
        printf("Opção inválida.\n");
    }

    close(client_socket);
    return 0;
}
