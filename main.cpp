#include "Server.hpp"

void validateArguments(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		std::cout << "Not enough arguments!" << std::endl;
		exit(-1);
	}
	if (argc > 2)
	{
		if (argc > 3)
		{
			std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
			std::cerr << "Too many arguments!" << std::endl;
			exit(-1);
		}
		if (strlen(argv[2]) == 0)
		{
			std::cerr << "Password can't be an empty string" << std::endl;
			exit(-1);
		}
	}
	try
	{
		if ((std::stoi(argv[1])) < 0)
			std::cerr << "Invalid argument: port can't be negativ" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << "Invalid argument: port" << std::endl;
		exit(-1);
	}
}


int main(int argc, char **argv)
{
	validateArguments(argc, argv);
	Server	server(std::stoi(argv[1]), argv[2]);
	server.createServer();
	server.mainLoop();

	// Close the listening socket
	// close(server); //!should we close it ?
	return (0);
}
