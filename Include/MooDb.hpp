#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <string_view>

#include "CowConfig.hpp"

namespace Moo
{

	class Table
	{

	private:

		/**
		 * A horizonal vector containing the column's names
		 */
		std::vector <std::string> Columns;

		/**
		 * A vertical vector (list) of horizontal vectors containing string data
		 */
		std::vector <std::vector <std::string> > Data;

		static std::vector <std::string> SplitCSV(std::string_view str)
		{
			std::string built;
			std::vector <std::string> ret;

			for (char c : str)
			{
				if (c == ',')
				{
					ret.push_back(built);
					built = "";
				}
				else
					built += c;
			}
			return ret;
		}

	public:

		void Insert(const std::vector <std::string>&& data)
		{
			if (data.size() != Columns.size())
			{
				throw Moo::Exception("Inserted data does not match columns");
			}

			Data.emplace_back(data);
		}

		void Print()
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

		void Save(const std::string&& fileName)
		{
			CowConfig cfg(fileName);

			std::string tableColumns;

			for (const auto& Column : Columns)
			{
				tableColumns += Column + ",";
			}

			cfg.WriteLine("", tableColumns);

			for (auto& p : Data)
			{
				std::string rowData;

				for (auto& q : p)
				{
					if (q.find(',') != std::string::npos)
						rowData += "\"" + q + "\",";
					else
						rowData += q + ",";
				} // looping horizontally

				cfg.WriteLine("", rowData);
			} // looping vertically
		}

		static Table Load(std::string_view fileName)
		{
			CowConfig cfg;

			try
			{
				cfg.OpenFile(fileName);
			}
			catch (Moo::Exception& exception)
			{
				exception.PrintMessage();
				std::exit(1);
			}

			std::vector <std::string> Lines = cfg.GetLines();

			// first line is columns
			Table ret(SplitCSV(Lines[0]));

			for (int i = 1; i < Lines.size(); i++)
			{
				ret.Data.push_back(SplitCSV(Lines[i]));
			}

			return ret;
		}

		Table() = default;

		explicit Table(std::vector <std::string>&& columns) : Columns(std::move(columns))
		{
		};

		std::vector <std::string>& GetRow(int Index)
		{
			if (Index > Data.size() || Index < 0)
				throw "Row at index " + std::to_string(Index) + " is out of bounds";
			return Data[Index];
		}

		std::vector <std::vector <std::string>> GetRowsWithValue(std::string nColumn, std::string ColumnValue)
		{
			std::vector <std::vector <std::string>> ret;
			std::vector <std::reference_wrapper <std::string>> vColumn = GetColumn(nColumn);

			for (int i = 0; i < vColumn.size(); i++)
			{
				if (vColumn[i].get() == ColumnValue)
					ret.push_back(GetRow(i));
			}

			return ret;
		} // will return a vector of vectors of strings where that row's column value equals the 'ColumnValue'

		std::vector <std::reference_wrapper <std::string>> GetColumn(std::string ColumnName)
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

		[[nodiscard]] unsigned int SizeInRows() const
		{
			return Data.size();
		}

	};
}