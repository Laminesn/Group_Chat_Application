//NAME: Lamine M. Deen
//main class for Server side application


// HEADERS IMPORTED FOR APPLICATION
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>



// FUNCTION AND CLASS DECLARATION

// class for accepted client address for multithreading
struct acceptedSocket {
	int acceptedSocketFD;
	struct sockaddr_in address;
	int error;
	bool acceptedSuccess;
};

// function to return pointer to address of accepted client connection
struct acceptedSocket *  acceptIncomingConnect(int serverSocket);

// function to receive and print messages
void receiveAndPrintData(int socketFD);

// function to start connecting to clients with multiple threads
void startAcceptConnections (int serverSocket);

// function to call function that accepts connection concurrently
void acceptNewConnectionAndExecute (int serverSocket);

// function to create threads for each client connection
void receiveAndPrintDataThreading (struct acceptedSocket *clientSocket);



 
// FUNCTION IMPLEMENTATION

// function to start connecting to clients with multiple threads
void startAcceptConnections (int serverSocket) {

	printf("Calling:   startAcceptConnections\n");

	while (true) {
		// call function to create thread and accept client connection
		struct acceptedSocket* clientSocket = acceptIncomingConnect(serverSocket);

		//Since server will block connections create threads
		receiveAndPrintDataThreading(clientSocket);
	}

}


// function to accept incoming connection from client
struct acceptedSocket *  acceptIncomingConnect (int serverSocket) {

	printf("Calling:   acceptIncomingConnect\n");
	int clientSocketFD;
	//create client address datastructure and find size
	struct sockaddr_in clientAddress;
	int clientAddressSize = sizeof (struct sockaddr_in);

	//returns new socket when passively accepting incoming client connections
	//will not return until a connection is established
	//accept(server socket, port address datastructure*, address size*)
	if ( (clientSocketFD = accept(serverSocket, (struct sockaddr *)&clientAddress,
									 (struct sockaddr *)&clientAddressSize)) < 0) {
		printf("Server Rejected Connection\n");
		exit(1);
	} else {
		printf("Server accepted connection at %d\n", clientSocketFD);
	}

	//create struct fill attributes and allocate memory for client
	struct acceptedSocket* AcceptedSocket = malloc(sizeof(struct acceptedSocket));
	AcceptedSocket->address = clientAddress; 
	AcceptedSocket->acceptedSocketFD = clientSocketFD;
	AcceptedSocket->acceptedSuccess = clientSocketFD > 0;

	if (!AcceptedSocket->acceptedSuccess)
		AcceptedSocket->error = clientSocketFD;


	return AcceptedSocket;
}



// function to receive and print data with multiple threads
void receiveAndPrintDataThreading (struct acceptedSocket *clientSocket) {

	printf("Calling:   receiveAndPrintDataThreading\n");
	//create id and thread for each client
	pthread_t id;
	// create(thread id, thread attribute, function, function arguments)
	// thread for receiving and printing messages
	pthread_create(&id, NULL, receiveAndPrintData, clientSocket->acceptedSocketFD);

}


// function to receive messages and print them
void receiveAndPrintData (int socketFD) {

	printf("Calling:   receiveAndPrintData\n");
	//create storage buffer to receive messages
	char buffer[1024];

	//infinite loop to recieve input from User
	while (true) {
		//receive message over the net work and via the correct port/file descriptor
		//recieve(client accept socket, buffer storage, size of message, specification flags)
		ssize_t charNum = recv(socketFD , buffer, 1024, 0); 

		//if there is a message
		if (charNum > 0) {
			//empty buffer for next message
			buffer[charNum] = 0;
			printf("Response was: %s\n", buffer);

		}
		//if there is no message
		if (charNum == 0) {
			printf("Nothing was recieved!!!!!!!!!!!\n");
			break;
		}
		//if there is something wrong with the connection
		if (charNum < 0) {
			printf("There is something wrong!!!!!!!!!!!!\n");
		}


	}

	//close client socket
	close(socketFD);

}



// MAIN FUNCTION

int main() {

	int serverSocket;

	// create socket (domain: internet, type: stream of bytes, protocol: Tranport Control Protocol (TCP))
	// protocol is 0 because the previous arguments specify TCP. FD = File Descriptor
	if ( (serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket not created\n");
		exit(1);
	} else {
		printf("Server Socket was created\n");
	}


	//ip address to connect to
	char* ip = "127.0.0.1" ;

	//address class for client socket to find right port to connect to
	//set the attributes of the class
	//usually choose port higher than 2000 in case of admin rights
	struct sockaddr_in address;
	address.sin_family = AF_INET; //IPv4
	//host to network short used to convert port to unsigned short in big Endian format
	address.sin_port = htons(3000);

	address.sin_addr.s_addr = inet_addr(ip);

	// //convert IP address to correct format to add to struct address attribute
	// //create ip (domain: internet, where to: ip, address attribute: s_addr)
	// inet_pton(AF_INET, ip, &address.sin_addr.s_addr);

	//this function binds the server socket to a specific port for identification during communication
	//bind(socket, port address datasctructure*, size of address)
	if ( bind(serverSocket, (struct sockaddr *)&address, sizeof address) < 0 ) {
		printf("Server Binding Unsuccessful\n");

		exit(1);
	} else {
		printf("Server Binding Successful\n");
	}

	//this function creates a buffer backlog to allow multiple pending connections
	//listen(socket, backlog's length)
	if ( listen(serverSocket, 10) < 0 ) {
		printf("Server Listening Unsuccessful\n");
		exit(1);
	} else {
		printf("Server Listening Successful\n");
	}



	// start accepting incoming connections concurrently
	startAcceptConnections(serverSocket);



	//stop traffic to the server until new connection
	shutdown(serverSocket, SHUT_RDWR);

	// //send message over network and via the correct port/file descriptor
	// //send(client accept socket, buffer storage, size of message, specification flags)
	// char* message = "Server is sending you an important message";

	// send(clientSocket, message, 1024, 0);



	return 0;
}
