//main class for client side application
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 


int main() {

	int clientSocket;

	// create socket (domain: internet, type: stream of bytes, protocol: Tranport Control Protocol (TCP))
	// protocol is 0 because the previous arguments specify TCP. FD = File Descriptor
	if ( (clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket not created");
		exit(1);
	} else {
		printf("Client Socket was created\n");
	}

	//"142.251.46.174"
	//ip address to connect to
	char* ip = "127.0.0.1";

	//address class for client socket to find right port to connect to
	//set the attributes of the class
	//usually choose port higher than 2000 in case of admin rights
	struct sockaddr_in address;
	address.sin_family = AF_INET; //IPv4
	//host to network short used to convert port to unsigned short in big Endian format
	address.sin_port = htons(3000);
	//convert IP address to correct format to add to struct address attribute
	//create ip (domain: internet, where to: ip, address attribute: s_addr)
	// inet_pton(AF_INET, ip, &address.sin_addr.s_addr);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");


	//function for client to actively try to connect to server for communication
	//connect (socket, address pointer, size of address)
	if ( connect(clientSocket, (struct sockaddr *)&address, sizeof address) < 0 ) {
		printf("Client connection Unsuccessful\n");
		exit(1);
	} else {
		printf("Client connection Successful\n");
	}


	//SEND AND RECEIVE FROM CLIENT SOCKET/FILE DESCRIPTOR


	//set char to null to initialize it to allow get() to create buffer automatically
	char *line = NULL;
	size_t lineSize = 0;

	// char *message = "Life is good yessssssssssssss";

	printf("Enter Your Message Below...\n");

	//infinite loop to get input from User
	while(true) {

		//returns number of characters passed as input
		//getline(address of buffer, size of buffer, method of input)
		ssize_t charNum = getline(&line, &lineSize, stdin);

		// send(clientSocket, line, charNum, 0);

		//if input is not empty check if exit is called and send data to server
		if (charNum > 0) {
			//if user wants to exit application
			if (strcmp(line, "exit\n") == 0) {
				break;
			}
			//if nothing was typed
			if (strcmp(line, "\n") == 0) {
				break;
			}

			if ( (send(clientSocket, line, charNum, 0)) < 0 ) {
				printf("Noooooooothing was sent\n");
			} else {
				printf("Message was sent!!!!!!!!!!\n");
			}

		} 
		// //if nothing was typed
		// if (charNum == 0) {
		// 	printf("Nothing was sent!!!!!!!!\n");
		// 	break;
		// }


	}

	close(clientSocket);



	// //create storage to send messages and create message
	// char* message;
	// message = "The Client is sending messages and data files through the network!!\n";

	// //send message over network and via the correct port/file descriptor
	// //send(client accept socket, buffer storage, size of message, specification flags)
	// send(clientSocket, message, 1024, 0);

	// //create storage buffer to receive messages
	// char buffer[1024];

	// //receive message over the network and via the correct port/file descriptor
	// //recieve(client accept socket, buffer storage, size of message, specification flags)
	// recv(clientSocket, buffer, 1024, 0);

	// printf("Response was: %s\n", buffer);

	// // close(clientSocket);

	return 0;
}

