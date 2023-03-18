#include "Server.hpp"

int main()
{
	Server	server(54000, "1234");

	server.createServer();
	while (1)
	{
		switch (poll(server._polls, 1024, 42000))
		{
		case 0:
			std::cout << "Waiting..." << std::endl;
			break;
		case -1:
			std::cout << "There is an error" << std::endl;
			break;
		default:
			// std::cout << "begin of the default switch" << std::endl;
			server.connectUser();
			break;
		}
	}

	// Close the listening socket
	// close(server); //!should we close it ?
	return (0);
}
