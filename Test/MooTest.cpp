// Relax, you can deleted this lines
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include "Moo.hpp"

using namespace Moo;

// Anonymous namespace
namespace
{
	using Register = std::vector <std::string>;

	testing::AssertionResult RegisterIsInsertedWithoutAlteration(
			const Register& registerInserted, const Register& registerOfRow)
	{
		// If both register have size different mean that
		// have values different and such so the register
		// was altered during the insertion to table.
		if (registerInserted.size() not_eq registerOfRow.size())
		{
			return testing::AssertionFailure() << "The size of both register is different.";
		}

		// Compare each value of both registers
		for (int i = 0; i < registerInserted.size(); ++i)
		{
			// If the values of register are different mean
			// that a value's register was altered during the
			// insertion to table.
			if (registerInserted[i] not_eq registerOfRow[i])
			{
				return testing::AssertionFailure() << "The register not are equals. "
												   << "The value expected is: \'"
												   << registerInserted[i]
												   << "\' but instead is: "
												   << '\'' << registerOfRow[i] << '\'';
			}
		}

		return testing::AssertionSuccess();
	}

	TEST(MooTable, Empty)
	{
		Table tab({ "Name", "Gender", "Age", "DOB", "Height", "Weight" });

		// The table always is empty to be created
		EXPECT_EQ(tab.GetSize(), 0);

		EXPECT_EQ(tab.IsEmpty(), true);
	}

	TEST(MooTable, SizeIncrease)
	{
		Table tab({ "Name", "Gender", "Age", "DOB", "Height", "Weight" });

		tab.Insert({ "Funanya Radomir", "Female", "3", "6/1/2015", "3 ft 1 in", "29.5 lbs" });
		EXPECT_EQ(tab.GetSize(), 1);

		tab.Insert({ "Reanna Shekhar", "Female", "15", "10/31/2003", "5 ft 6 in", "130 lbs" });
		EXPECT_EQ(tab.GetSize(), 2);
	}

	TEST(MooTable, Insert)
	{
		const Register registerInserted = { "Funanya Radomir", "Female", "3", "6/1/2015", "3 ft 1 in", "29.5 lbs" };

		Table tab({ "Name", "Gender", "Age", "DOB", "Height", "Weight" });
		tab.Insert({ "Funanya Radomir", "Female", "3", "6/1/2015", "3 ft 1 in", "29.5 lbs" });

		EXPECT_TRUE(RegisterIsInsertedWithoutAlteration(registerInserted, tab.GetRow(0)));
	}

	TEST(MooTable, MethodLoadThrowException)
	{
		Table tab;

		ASSERT_ANY_THROW(tab.Load("ThisFileNotExist.csv"));
	}
}

int main()
{
	// Necessary for avoid warning for part of Google
	testing::InitGoogleTest();
	// Execute all the Test
	return RUN_ALL_TESTS();
}