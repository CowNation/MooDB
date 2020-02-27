// cpp-CowConfig
// Copyright (C) 2018 CowCorp
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the author be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//

#pragma once

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "Exception/Exception.hpp"

namespace Moo
{


	class CowConfig
	{

	private:

		std::ofstream WriteConfig;
		std::ifstream ReadConfig;
		bool FirstLine = true;
		std::string_view FileName;

	public:

		CowConfig() = default;

		explicit CowConfig(std::string_view fileName) noexcept : FileName(fileName)
		{

		};

		~CowConfig()
		{
			if (ReadConfig.is_open())
				ReadConfig.close();
			if (WriteConfig.is_open())
				WriteConfig.close();
		}

		/**
		 * Will open file with specified filename and will return whether the file is open
		 * @param fileName Filename
		 * @return True if the file is open, otherwise false.
		 */
		void OpenFile(std::string_view fileName)
		{
			ReadConfig.open(fileName.data());

			if (not ReadConfig.is_open())
			{
				std::string message = "The file \'";
				message += fileName.data();
				message += "\' can't found.";

				throw Exception(std::move(message));
			}
		}


		std::vector <std::string> GetLines()
		{
			std::string data;
			std::vector <std::string> buffer;

			while (std::getline(ReadConfig, data))
			{
				buffer.push_back(data);
			}

			return buffer;
		}

		void WriteLine(std::string_view offsetText, std::string_view valToWrite)
		{
			if (ReadConfig.is_open())
				ReadConfig.close(); //Close fstream file before writing to the file

			if (FirstLine)
			{
				WriteConfig.open(FileName.data());
				FirstLine = false;
			}

			WriteConfig << offsetText << valToWrite << std::endl;

			ReadConfig.open(FileName.data());
		}
	};

}