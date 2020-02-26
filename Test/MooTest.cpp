// Relax, you can deleted this lines
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include "MooDb.hpp"

using namespace Moo;

// Anonymous namespace
namespace
{
	TEST(MooTable, Empty)
	{
		Table tab({ "Name", "Gender", "Age", "DOB", "Height", "Weight" });

		// The table always is empty to be created
		EXPECT_EQ(tab.SizeInRows(), 0);
	}

	TEST(MooTable, SizeIncrease)
	{
		Table tab({ "Name", "Gender", "Age", "DOB", "Height", "Weight" });
		tab.Insert({ "Funanya Radomir", "Female", "3", "6/1/2015", "3 ft 1 in", "29.5 lbs" });

		// The size of table increase in one
		EXPECT_EQ(tab.SizeInRows(), 1);
	}
}

int main()
{
	// Necessary for avoid warning for part of Google
	testing::InitGoogleTest();
	// Execute all the Test
	return RUN_ALL_TESTS();
}