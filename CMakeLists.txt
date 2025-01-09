cmake_minimum_required(VERSION 3.10)

# Project name
project(BankSimulation)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Libraries
add_library(bankaccount src/BankAccount.cpp)
add_library(bank src/Bank.cpp)
target_link_libraries(bank bankaccount)

# Main executable
add_executable(BankSimulation src/main.cpp)
target_link_libraries(BankSimulation bank)

# GoogleTest
find_package(GTest REQUIRED)
enable_testing()

# Test executable
add_executable(test_bank_system tests/test_BankAccount.cpp tests/test_Bank.cpp)
target_link_libraries(test_bank_system bankaccount bank GTest::GTest GTest::Main pthread)

# Register test
add_test(NAME test_bank_system COMMAND test_bank_system)
