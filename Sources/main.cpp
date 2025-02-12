#include "../Includes/Server.hpp"

Server serv;

void signalHandler(int signum)
{
	std::cout << BLUE << "\nSignal interrompu (" << signum << "). Nettoyage..." << END << std::endl;
	serv.finish();
	exit(signum);
}

int main(int ac, char* *av)
{
	signal(SIGINT, signalHandler);
	try
	{
			if(serv.checkArguments(ac,av))
				return(1);
			if (serv.execServer())
				return(1);
	}
	catch (const std::exception &error)
		{
		std::cout << RED << error.what() << END << std::endl;
		}
	return(0);
}