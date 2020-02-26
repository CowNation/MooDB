// Relax, you can deleted this lines
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include "MooDb.hpp"

using namespace Moo;

// Anonymous namespace
namespace
{
	TEST(MooTable, InsertDateAndSizeIncreaseInOne)
	{
		Table tab({ "Name", "Gender", "Age", "DOB", "Height", "Weight" });
		tab.Insert({ "Funanya Radomir", "Female", "3", "6/1/2015", "3 ft 1 in", "29.5 lbs" });

		EXPECT_TRUE(1 == tab.SizeInRows());
	}
}
