#include <iostream>
#include <vector>

int main()
{
	std::vector<std::string> test;

	test.push_back("JOIN");
	test.push_back("#ch1,#ch2,#ch3");

	std::vector<std::string>	singleArgs;
	std::string					args = test[1];

	while (args.find(',') != std::string::npos)
	{
		size_t index = args.find(',');
		std::string substring = args.substr(0, index);
		args = args.substr(index + 1, args.length());
		singleArgs.push_back(substring);
	}
	singleArgs.push_back(args);
	std::cout <<  singleArgs[0] << "\n" << singleArgs[1] << "\n" << singleArgs[2] << std::endl;
	return(0);
}