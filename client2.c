#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8080

int main(int argc, char const* argv[]) {
        int status, valread, client_fd, b, new_socket;
        struct sockaddr_in serv_addr;
        char *hello = "Hello from client";
        char buffer[1024] = { 0 };
	char sendbuffer[100];
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	//Criacao do descritor de arquivo do socket para o cliente
        if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                printf("\n Socket creation error \n");
                return -1;
        }

        serv_addr.sin_family = AF_INET;//ipv4
        serv_addr.sin_port = htons(PORT); //porta

	//Traducao do endereco para reajuste do parametro na struct (endereco)
	if (inet_pton(AF_INET, "192.168.3.110", &serv_addr.sin_addr) <= 0) {
		printf("\n Invalid address or address not supported \n");
		return -1;
	}

	//Estabelece a conexao
        if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
                printf("\nConnection failed\n");
                return -1;
        }

	//Aplicacao
	while (1) {
		if((new_socket = accept(client_fd, (struct sockaddr*)&serv_addr, &addrlen)) < 0){
			perror("Accept");
			exit(EXIT_FAILURE);
		}

		FILE *flist = fopen("list", "wb");
		int tot = 0, b = 0;
                if (flist != NULL) {
                        while ( (b = read(client_fd, buffer, 1024-1)) > 0) {
                                tot += b;
                                fwrite(buffer, 1, b, flist);
                        }

                        printf("Received byte: %d\n", tot);
                        if (b < 0) perror("Receiving");

                        fclose(flist);
                } else {
                        perror("File");
                }


		FILE* fp = fopen("123.txt", "rb");

		if (fp != NULL) {
			while ( (b = fread(sendbuffer, 1, sizeof(sendbuffer), fp)) > 0) {
				send(client_fd, sendbuffer, b, 0);
			}

			fclose(fp);

		} else {
			perror("File");
		}

		close(client_fd);
	}

        return 0;
}
