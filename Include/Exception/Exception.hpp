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

		/**
		 * Is important remember that the construct accept for parameters
		 * a string temporal, this mean that the string after of will be
		 * passed for parameters not will be valid, if you intent use this
		 * string you probably get a error in runtime.
		 *
		 * @param _message Message informative about of the exception.
		 */
		explicit Exception(std::string&& _message) : message(std::move(_message))
		{
		};

		void PrintMessage() const
		{
			std::cerr << message << std::endl;
		}

	};
}


#endif //MOONDB_EXCEPTION_HPP
