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

		static std::vector <std::string> SplitCSV(std::string_view str);

	public:

		Table() = default;

		explicit Table(std::vector <std::string>&& columns);

		void Print();

		void Insert(const std::vector <std::string>&& data);

		void Save(const std::string&& fileName);

		static Table Load(std::string_view fileName);

		std::vector <std::string>& GetRow(int Index);

		/**
		 * @param nColumn Name of the column to search.
		 * @param ColumnValue Filter, if the value of column is equal to it, this row will be returned
		 * @return A vector of vectors of strings where that row's column value equals the 'ColumnValue'
		 */
		std::vector <std::vector <std::string>> GetRowsWithValue(std::string nColumn, std::string ColumnValue);

		std::vector <std::reference_wrapper <std::string>> GetColumn(std::string ColumnName);

		/**
		 * @return The number of registers inserted to table. Zero {0} if is empty.
		 */
		[[nodiscard]] unsigned int GetSize() const
		{
			return Data.size();
		}

	};
}