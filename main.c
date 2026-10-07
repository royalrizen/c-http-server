#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

int main(void)
{
	// socket() creates a TCP socket and returns its file descriptor.
	int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sockfd == -1)
	{
		perror("socket");
		return EXIT_FAILURE;
	}

	printf("My Socket is: %d\n", sockfd);

	// sockaddr_in stores an IPv4 address and port.
	// htons() converts the port to network byte order.
	// INADDR_ANY lets the server accept connections on any local IP.
	struct sockaddr_in sa = {
		.sin_family = AF_INET,
		.sin_port = htons(8080),
		.sin_addr.s_addr = htonl(INADDR_ANY)
	};

	// bind() attaches our socket to port 8080.
	// (struct sockaddr *)&sa converts the sockaddr_in pointer to the generic sockaddr pointer expected by bind(). its called typecasting
	if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
	{
		perror("bind");
		close(sockfd);
		return EXIT_FAILURE;
	}

	printf("Socket bound to port 8080\n");

	// listen() puts the socket into listening mode.
	// 5 is the backlog: how many pending connections can wait.
	if (listen(sockfd, 5) == -1)
	{
		perror("listen");
		close(sockfd);
		return EXIT_FAILURE;
	}

	printf("Listening on port 8080...\n");

	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	// accept() waits for a client and gives us a new socket specifically for communicating with that client.
	int clientfd = accept(
		sockfd,
		(struct sockaddr *)&client_addr,
		&client_len
	);

	if (clientfd == -1)
	{
		perror("accept");
		close(sockfd);
		return EXIT_FAILURE;
	}

	printf("Client connected!\n");
	printf("Client IP: %s\n", inet_ntoa(client_addr.sin_addr));
	printf("Client port: %d\n", ntohs(client_addr.sin_port));

	// An array of 1024 chars gives recv() memory where it can store the incoming bytes.
	char buffer[1024];

	// recv() receives bytes from the client and puts them into buffer.
	// It can receive AT MOST 1023 bytes here, leaving one byte for '\0'.
	int bytes = recv(
		clientfd,
		buffer,
		sizeof(buffer) - 1,
		0
	);

	if (bytes == -1)
	{
		perror("recv");
		close(clientfd);
		close(sockfd);
		return EXIT_FAILURE;
	}

	// C strings end with '\0'. recv() doesn't add it for us.
	buffer[bytes] = '\0';

	printf("Received:\n---------------\n%s\n", buffer);

	// This is an HTTP response stored as a C string.
	// \r\n marks the end of each HTTP line.
	// The empty \r\n separates the headers from the body.
	char response[] =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 14\r\n"
		"\r\n"
		"Hello from C!\n";

	// send() sends our HTTP response through the TCP connection.
	
	printf("Sending response\n--------------------\n%s\n",response);
	int sent = send(
		clientfd,
		response,
		strlen(response),
		0
	);

	if (sent == -1)
	{
		perror("send");
		close(clientfd);
		close(sockfd);
		return EXIT_FAILURE;
	}

	close(clientfd);
	close(sockfd);

	return EXIT_SUCCESS;
}
