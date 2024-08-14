#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const* argv[]) {
    int server_fd, new_socket, tot, b;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Criando o socket do servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Erro ao configurar o socket");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // faz o bind e escuta
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("Erro no listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("Erro no accept");
            exit(EXIT_FAILURE);
        }

        // envia menu para o cliente
        char menu[] = "Escolha uma opção:\n1. Enviar arquivo\n2. Receber lista de arquivos\n";
        send(new_socket, menu, strlen(menu), 0);

        // recebendo a escolha do cliente
        int choice;
        recv(new_socket, &choice, sizeof(choice), 0);

        if (choice == 1) {
            // recebe file
	    int t = 0;
	    char recv_buf[1024] = {0};
            FILE* received_file = fopen("received_file", "wb");
            if (received_file == NULL) {
                perror("Erro ao criar received_file");
            }

            while ((b = recv(new_socket, recv_buf, sizeof(recv_buf), 0)) > 0) {
		printf("%s\n", recv_buf);
		fwrite(recv_buf, 1, b, received_file);
            }
            if (b < 0) perror("Erro ao receber");

            fclose(received_file);
            printf("Dados recebidos salvos em received.txt\n");
        } else if (choice == 2) {
            // enviando a lista de arquivos
	    system("ls > list");
            FILE* list_file = fopen("list", "rb");
            if (list_file != NULL) {
                while ((b = fread(buffer, 1, sizeof(buffer), list_file)) > 0) {
                    send(new_socket, buffer, b, 0);
                }
                fclose(list_file);
            } else {
                perror("Erro ao abrir o arquivo list");
            }
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }

        close(new_socket);
    }

    close(server_fd);
    return 0;
}
