#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const* argv[]) {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer_menu[1024] = {0}, buffer_data[1024] = {0};

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "192.168.100.229", &server_address.sin_addr) <= 0) {
        perror("Endereço inválido");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Erro na conexão");
        exit(EXIT_FAILURE);
    }

    // Recebendo o menu do servidor
    recv(client_socket, buffer_menu, sizeof(buffer_menu), 0);
    printf("%s", buffer_menu);

    int choice;
    scanf("%d", &choice);
    send(client_socket, &choice, sizeof(choice), 0);

    if (choice == 1) {
        FILE* file_to_send = fopen("client7.c", "rb");
        if (file_to_send == NULL) {
            perror("Erro ao abrir o arquivo file.txt");
        }

        while ((fread(buffer_data, 1, sizeof(buffer_data), file_to_send)) > 0) {
            send(client_socket, buffer_data, sizeof(buffer_data), 0);
        }

        fclose(file_to_send);
        printf("Arquivo enviado com sucesso!\n");
    } else if (choice == 2) {
        // Recebendo a lista de arquivos do servidor
        FILE* received_list = fopen("received_list.txt", "wb");
        if (received_list == NULL) {
            perror("Erro ao criar received_list.txt");
        }

        while (1) {
            ssize_t b = recv(client_socket, buffer_data, sizeof(buffer_data), 0);
            if (b <= 0) break;
            fwrite(buffer_data, 1, b, received_list);
        }

        fclose(received_list);
        printf("Lista de arquivos recebida e salva em received_list.txt\n");
    } else {
        printf("Opção inválida.\n");
    }

    close(client_socket);
    return 0;
}
("client7.c", "rb");
        if (file_to_send == NULL) {
            perror("Erro ao abrir o arquivo file.txt");
        }

        while ((fread(buffer_data, 1, sizeof(buffer_data), file_to_send)) > 0) {
            send(client_socket, buffer_data, sizeof(buffer_data), 0);
        }

        fclose(file_to_send);
        printf("Arquivo enviado com sucesso!\n");
    } else if (choice == 2) {
        // Recebendo a lista de arquivos do servidor
        FILE* received_list = fopen("received_list.txt", "wb");
        if (received_list == NULL) {
            perror("Erro ao criar received_list.txt");
        }

        while (1) {
            ssize_t b = recv(client_socket, buffer_data, sizeof(buffer_data), 0);
            if (b <= 0) break;
            fwrite(buffer_data, 1, b, received_list);
        }

        fclose(received_list);
        printf("Lista de arquivos recebida e salva em received_list.txt\n");
	
    } else {
	printf("Opção inválida.\n");
    }

    close(client_socket);
    return 0;
}
