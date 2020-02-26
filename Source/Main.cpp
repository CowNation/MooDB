#include <iostream>
#include <vector>
#include <string>
#include "MooDb.hpp"

template <class T>
void PrintVector(std::vector <T> vec)
{
	std::cout << std::endl;
	for (int i = 0; i < vec.size(); i++)
	{
		std::cout << vec[i];
		if (i != vec.size() - 1)
			std::cout << ", ";
	}
}

template <class T>
void PrintVector(std::vector <std::reference_wrapper <T>> vec)
{
	std::string buffer = "\n";
	buffer.reserve(250);

	for (auto item: vec)
	{
		buffer += item.get() + ", ";
	}

	// Remove the last characters {, }
	buffer.pop_back();
	buffer.pop_back();

	std::cout << buffer;
}

template <class T>
void PrintVector(std::vector <T*> vec)
{
	std::cout << std::endl;
	for (int i = 0; i < vec.size(); i++)
	{
		std::cout << *vec[i];
		if (i != vec.size() - 1)
			std::cout << ", ";
	}
}

int main()
{
	Table tab({ "Name", "Gender", "Age", "DOB", "Height", "Weight" });
	tab.Insert({ "Funanya Radomir", "Female", "3", "6/1/2015", "3 ft 1 in", "29.5 lbs" });
	tab.Insert({ "Reanna Shekhar", "Female", "15", "10/31/2003", "5 ft 6 in", "130 lbs" });
	tab.Print();
	tab.Save("Patients.csv");

	Table hp({ "Name", "Stock", "RequiresID" });
	hp.Insert({ "Ibuprofin", "423,243", "False" });
	hp.Insert({ "Psuedophedrine", "345,223", "True" });
	hp.Insert({ "Tylenol", "452,334", "False" });
	hp.Print();
	hp.Save("Medications.csv");

	std::vector <std::vector <std::string>> r = hp.GetRowsWithValue("RequiresID", "True");

	for (const auto& i : r)
	{
		PrintVector(i);
		std::cout << std::endl;
	}

	Table tb = Table::Load("Patients.csv");
	tb.Print();

	tb.GetColumn("Name").at(1).get() = "REDACTED";
	PrintVector <std::string>(tb.GetColumn("Name"));

	tb.GetRow(0).at(0) = "REDACTED"; // sets the first value in the first row
	PrintVector(tb.GetRow(0));
}