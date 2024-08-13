#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
	int server_fd, new_socket, tot, b;
	ssize_t valread;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);
	char buffer[1024] = {0};

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
        if (address.sin_addr.s_addr = inet_addr("192.168.100.221") == INADDR_NONE) {
                perror("Inet_addr");
                return -1;
        }
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("Bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("Listen");
		exit(EXIT_FAILURE);
	}

	while (1) {
		if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
			perror("Accept");
			exit(EXIT_FAILURE);
		}

		FILE* fp = fopen("example_pcb.png", "wb");

		tot = 0;
		if (fp != NULL) {
			while ( (b = recv(new_socket, buffer, 1024-1, 0)) > 0) {
				tot += b;
				fwrite(buffer, 1, b, fp);
			}

			printf("Received byte: %d\n", tot);
			if (b < 0) perror("Receiving");

			fclose(fp);
		} else {
			perror("File");
		}
		close(new_socket);
	}
	//valread = read(new_socket, buffer, 1024 - 1);

	printf("%s\n", buffer);
	printf("Hello message sent\n");

	close(new_socket);
	close(server_fd);
	return 0;
}
