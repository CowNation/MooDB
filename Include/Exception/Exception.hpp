#ifndef MOONDB_EXCEPTION_HPP
#define MOONDB_EXCEPTION_HPP

#include <string>
#include <iostream>

namespace Moo
{
	class Exception : std::exception
	{

	private:

		const std::string message;

	public:

		explicit Exception(const std::string&& _message) : message(_message)
		{
		};

		void PrintMessage() const
		{
			std::cerr << message << std::endl;
		}

	};
}


#endif //MOONDB_EXCEPTION_HPP
