////////////////////////////////////////////////////////////
//
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
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
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
////////////////////////////////////////////////////////////

#ifndef cppCowConfig
#define cppCowConfig

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <sstream>

#include "Exception/Exception.hpp"

class CowConfig
{

private:

	std::ofstream WriteConfig;
	std::fstream ReadConfig;
	bool FirstLine = true, FirstRead = true;
	std::string_view FileName;
	std::vector <std::string> Lines;

	///////////////////
	void RemoveSubStr(std::string substr, std::string& str)
	{
		size_t pos;

		while ((pos = str.find(substr)) != std::string::npos)
		{
			str.erase(pos, substr.length());
		}
	}

	///////////////////
	void ReadAllLines()
	{
		std::string str;
		while (std::getline(ReadConfig, str))
		{
			Lines.push_back(str);
		}
	}

	///////////////////
	int FindElement(std::string Section, std::string offsetText)
	{
		Section = "[" + Section + "]";
		bool SectionFound;
		for (int i = 0; i < Lines.size(); i++)
		{
			if (Lines[i] == Section || Section == "[]")
				SectionFound = true;
			else if (SectionFound && Lines[i].find("[") != std::string::npos && Lines[i].find("]") != std::string::npos)
				break;
			if (SectionFound && Lines[i].find(offsetText) != std::string::npos)
				return i;
		}
		return -1;
	}

	///////////////////
	int pRead(std::string Section, std::string offsetText)
	{
		if (FirstRead)
			ReadAllLines();
		return FindElement(Section, offsetText);
	}

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
			throw MoonDB::Exception("FileNotFoundException");
		}
	}


	std::vector <std::string> GetLines()
	{
		std::string str;
		std::vector <std::string> ret;
		while (std::getline(ReadConfig, str))
		{
			ret.push_back(str);
		}
		return ret;
	}

	//////////////////
	void WriteLine(std::string offsetText, std::string valToWrite)
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

	template <class c>
	void WriteLine(std::string offsetText, c valToWrite)
	{
		if (ReadConfig.is_open())
			ReadConfig.close(); //Close fstream file before writing to the file

		if (FirstLine)
		{
			WriteConfig.open(FileName.data());
			FirstLine = false;
		}

		WriteConfig << offsetText << std::to_string(valToWrite) << std::endl;

		ReadConfig.open(FileName.data());
	}

	void Section(std::string SectionText)
	{
		SectionText = "[" + SectionText + "]";
		WriteLine("", SectionText);
	}

	//////////////////
	template <class T>
	T Read(std::string Section, std::string offsetText)
	{
		int _Read = pRead(Section, offsetText);
		if (_Read == -1)
			return 0;
		else
		{
			try
			{
				std::string temp = Lines[_Read];
				RemoveSubStr(offsetText, temp);
				T ret;
				std::istringstream(temp) >> ret;
				return ret;
			}
			catch (...)
			{
				return T();
			}
		}
	}
};

#endif
