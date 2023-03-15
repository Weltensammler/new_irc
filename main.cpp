#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sstream>

int main()
{
	/* Create a socket */
	/*	-difference AF(adress family) and PF(protocol family) PF was intended to 
		handle several protocols this never happened so AF is what you should use
		-difference INET(v4) and INET6(v6) is the size of their address space
		 INET6 improved features:
		 -better routing and security
		 -eliminates the need for address translation, which is necessary in INET
		-SOCK_STREAM type provides sequenced, reliable, two-way connection based byte streams
		 SOCK_DGRAM socket supports datagrams (connectionless, unreliable messages of a fixed
		 (typically small) maximum length)
		-SOCK_RAW sockets provide access to internal network protocols and interfaces (only super-user)*/
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	//? check what the sockets are
	if (listening == -1)
	{
		std::cerr << "Can't create a socket!" << std::endl;
		return (-1);
	}
	//TODO add setsockopt function and research
	// Bind the socket to a IP / Port 
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_addr.s_addr = htonl(INADDR_ANY);
	// inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	hint.sin_port = htons(54000); // here we should do the byteorder network/host using htons and htonl
	if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Can't bind to IP/Port!" << std::endl;
		return (-2);
	}
	// Mark the socket for listening in 
	if (listen(listening, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen!" << std::endl;
		return (-3);
	}
	// Accept a call
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
	if (clientSocket == -1)
	{
		std::cerr << "Problem with client connecting!" << std::endl;
		return (-3);
	}

	// Close the listening socket
	close(listening);
	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	int result = getnameinfo((sockaddr *)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0);
	if (result)
	{
		std::cout << host << " connected on " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
	}
	
	// While receiving display message, echo message 
	char buf[4096];
	while (true)
	{
		//Clear the buffer
		memset(buf, 0, 4096);
		//Wait for a message
		int bytesRecv = recv(clientSocket, buf, 4096, 0);
		if (bytesRecv == - 1)
		{
			std::cerr << "There was a connection issue!" << std::endl;
			break;
		}
		if (bytesRecv == 0)
		{
			std::cerr << "The Client disconnected!" << std::endl;
			break;
		}
		//Display message
		std::cout << "Received: " << std::string(buf, 0 , bytesRecv) << std::endl;
		
		//Send message
		std::string nick = "you";
		std::string user = "you";
		std::string channel = "#ch1";
		std::string message = buf;

		std::ostringstream cmd;
		cmd << "NICK " << nick << "\r\n"
		<< "USER " << user << " 0 * :" << user << "\r\n"
		<< "JOIN " << channel << "\r\n"
		<< "PRIVMSG " << channel << " :" << message << "\r\n";
		std::string cmd_str = cmd.str();

		send(clientSocket, cmd_str.c_str(), cmd_str.size(), 0);
		// send(clientSocket, "001 ben welcome to ft_irc", 26, 0);
	}
	
	// close socket
	close(clientSocket);
}
