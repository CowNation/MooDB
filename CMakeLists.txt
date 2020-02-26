CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(MoonDB LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

# Support to Code Coverage with gcov in gcc
SET(CMAKE_CXX_FLAGS "--coverage")

INCLUDE_DIRECTORIES(Include)

ADD_EXECUTABLE(MoonDB Source/Main.cpp Include/Exception/Exception.hpp)

# Find and link the library gtest for purpose of test
INCLUDE_DIRECTORIES(Library/GoogleTest)
# Find and link the .so .a .lib. .dll
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/Library/GoogleTest)


ADD_DEFINITIONS(-DGTEST_HAS_PTHREAD=1)

ADD_EXECUTABLE(MooDB.Test Test/MooDBTest.cpp)
TARGET_LINK_LIBRARIES(MooDB.Test PUBLIC GoogleTestMain)
TARGET_LINK_LIBRARIES(MooDB.Test PUBLIC GoogleTest)
# Is very important that this line will be putted after of link GoogleTest {gtest}
TARGET_LINK_LIBRARIES(MooDB.Test PUBLIC pthread)

ADD_TEST(MooDBAllTest MooDB.Test)