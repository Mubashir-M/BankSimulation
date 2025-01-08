cmake_minimum_required(VERSION 3.10)

# Set project name
project(BankSimulation)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing
enable_testing()

# Include the headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the main project source files
add_library(bankaccount src/BankAccount.cpp)
add_library(bank src/Bank.cpp)

# Add the main executable (for the application)
add_executable(BankSimulation src/main.cpp)
target_link_libraries(BankSimulation bankaccount bank)

# Find the GoogleTest package (installed via Homebrew or manually)
find_package(GTest REQUIRED)

# Add the test executable (for the test suite)
add_executable(test_bank_system Tests/test_BankAccount.cpp Tests/test_bank.cpp)

# Link the test executable with GoogleTest and the main libraries
target_link_libraries(test_bank_system bankaccount bank GTest::GTest GTest::Main pthread)

# Register the test to run with CTest
add_test(NAME test_bank_system COMMAND test_bank_system)
