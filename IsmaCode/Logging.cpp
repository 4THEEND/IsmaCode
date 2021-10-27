#include "Logging.h"

namespace Log {
	void Info(const std::string info)
	{
		std::cout << "[*]Info: " << info << std::endl;
	}

	void Warning(const std::string warning)
	{
		std::cout << "[*]Warning: " << warning << std::endl;
	}

	void Error(const std::string error)
	{
		std::cout << "[!]Error: " << error << std::endl;
	}
};
