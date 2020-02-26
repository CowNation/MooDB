#include "Moo.hpp"

using namespace Moo;

std::vector <std::string> Table::SplitCSV(std::string_view str)
{
	std::string data;
	std::vector <std::string> buffer;

	std::size_t beginData = 0;
	std::size_t positionComma = str.find(',');

	while (positionComma != std::string::npos)
	{
		data = str.substr(beginData, positionComma - beginData);
		buffer.emplace_back(data);
		data.clear();

		beginData = ++positionComma;
		positionComma = str.find(',', beginData);
	}

	data = str.substr(beginData);
	buffer.emplace_back(data);

	return buffer;
}

void Table::Print()
{
	std::vector <int> ColumnOffsets(Columns.size(), -1);

	for (auto& i : Data)
	{
		for (int j = 0; j < i.size(); j++)
		{
			if ((i[j].length() - Columns[j].length() > ColumnOffsets[j] || ColumnOffsets[j] == -1) &&
				i[j].length() > Columns[j].length())
				ColumnOffsets[j] = i[j].length() - Columns[j].length();
		} // we are looping horizontally
	} // we are looping vertically

	std::string Headers;
	std::string Seperator;

	Headers += "| ";
	for (int i = 0; i < Columns.size(); i++)
	{
		Headers += Columns[i];
		if (ColumnOffsets[i] != -1)
		{
			for (int j = 0; j < ColumnOffsets[i]; j++)
			{
				Headers += " ";
			}
		}
		Headers += " | ";
	}

	Seperator += "+";
	for (int i = 0; i < Headers.length() - 3; i++)
	{
		if (Headers.at(i + 1) == '|')
			Seperator += "+";
		else
			Seperator += "-";
	}

	Seperator += "+";

	std::cout << Seperator << "\n" << Headers << "\n" << Seperator;

	for (auto& i : Data)
	{
		std::cout << "\n| ";
		for (int j = 0; j < i.size(); j++)
		{
			std::cout << i[j];

			if (ColumnOffsets[j] != -1)
			{
				if (i[j].length() < (ColumnOffsets[j] + Columns[j].length()))
				{
					for (int p = 0; p < (ColumnOffsets[j] + Columns[j].length()) - i[j].length(); p++)
					{
						std::cout << " ";
					}
				}
			}
			else
			{
				for (int p = 0; p < Columns[j].length() - i[j].length(); p++)
				{
					std::cout << " ";
				}
			}

			std::cout << " | ";
		} // we are looping horizontally
	} // we are looping vertically

	std::cout << std::endl << Seperator << std::endl;
}

void Table::Insert(const std::vector <std::string>&& data)
{
	if (data.size() != Columns.size())
	{
		throw Moo::Exception("Inserted data does not match columns");
	}

	Data.emplace_back(data);
}

void Table::Save(const std::string&& fileName)
{
	CowConfig writer(fileName);

	std::string tableColumns;

	for (const auto& Column : Columns)
	{
		tableColumns += Column + ",";
	}

	// Deleted the last character {,}
	tableColumns.pop_back();

	writer.WriteLine("", tableColumns);

	// Store the information content in the row.
	std::string rowData;

	for (std::vector <std::string>& row : Data)
	{
		for (std::string& data : row)
		{
			// Is possible have data with an comma in middle,
			// example: numbers like 123,456
			// For avoid break the information need
			// manage this cases.
			if (data.find(',') != std::string::npos)
			{
				// If the data is of type {123,456} we need
				// write in the file like: {"123,456"}
				rowData += "\"" + data + "\",";
			}
			else
			{
				rowData += data + ",";
			}
		}

		// Deleted the last character {,}
		rowData.pop_back();

		writer.WriteLine("", rowData);

		rowData.clear();
	}
}

void Table::Load(std::string_view fileName)
{
	CowConfig loader;
	loader.OpenFile(fileName);

	std::vector <std::string> Lines = std::move(loader.GetLines());

	// first line is columns
	this->Columns = std::move(SplitCSV(Lines[0]));

	for (int i = 1; i < Lines.size(); i++)
	{
		this->Data.emplace_back(std::move(SplitCSV(Lines[i])));
	}
}

std::vector <std::string>& Table::GetRow(int Index)
{
	if (Index > Data.size() || Index < 0)
		throw "Row at index " + std::to_string(Index) + " is out of bounds";
	return Data[Index];
}

std::vector <std::vector <std::string>> Table::GetRowsWithValue(std::string nColumn, std::string ColumnValue)
{
	std::vector <std::vector <std::string>> ret;
	std::vector <std::reference_wrapper <std::string>> vColumn = GetColumn(nColumn);

	for (int i = 0; i < vColumn.size(); i++)
	{
		if (vColumn[i].get() == ColumnValue)
			ret.push_back(GetRow(i));
	}

	return ret;
}

std::vector <std::reference_wrapper <std::string>> Table::GetColumn(std::string ColumnName)
{
	int ColumnIndex = -1;
	for (int i = 0; i < Columns.size(); i++)
	{
		if (Columns[i] == ColumnName)
		{
			ColumnIndex = i;
			break;
		}
	}

	if (ColumnIndex == -1)
		throw "Could not find column '" + ColumnName + "'";

	std::vector <std::reference_wrapper <std::string>> ret;

	for (auto& i : Data)
	{
		ret.emplace_back(i[ColumnIndex]);
	}

	return ret;
}

Table::Table(std::vector <std::string>&& columns) noexcept : Columns(std::move(columns))
{

}

unsigned int Table::GetSize() const noexcept
{
	return Data.size();
}

bool Table::IsEmpty() const noexcept
{
	return Data.empty();
}
