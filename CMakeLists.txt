cmake_minimum_required(VERSION 3.10)
project(BankSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Project Directories
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(PROJECT_TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(PROJECT_EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

# Find SQLite3 (installed via package manager or manually)
find_package(SQLite3 REQUIRED)

# Try to find SQLiteCpp package
find_package(SQLiteCpp CONFIG REQUIRED)
# Check if we are building inside Docker
if(DEFINED ENV{DOCKER_BUILD})
    # Docker environment: Use the system path
    message(STATUS "Building inside Docker, using system-installed SQLiteCpp")
    set(SQLiteCpp_INCLUDE_DIRS "/usr/local/include")
    set(SQLiteCpp_LIBRARIES "/usr/local/lib/libSQLiteCpp.so")
else()
    # Local build: Use Homebrew path
    message(STATUS "Building locally, using Homebrew-installed SQLiteCpp")
    set(SQLiteCpp_INCLUDE_DIRS "/opt/homebrew/Cellar/sqlitecpp/3.3.2/include")
    set(SQLiteCpp_LIBRARIES "/opt/homebrew/Cellar/sqlitecpp/3.3.2/lib/libSQLiteCpp.a")
endif()

# Find other dependencies
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(OpenGL REQUIRED)

# Include Directories
include_directories(
    ${PROJECT_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${PROJECT_EXTERNAL_DIR}/imgui
    ${PROJECT_EXTERNAL_DIR}/imgui/backends
    ${SQLite3_INCLUDE_DIRS} 
    ${SQLiteCpp_INCLUDE_DIRS}
)

# Libraries
add_library(bankaccount STATIC ${PROJECT_SOURCE_DIR}/BankAccount.cpp)
add_library(transactions STATIC ${PROJECT_SOURCE_DIR}/Transactions.cpp)
add_library(uihandler STATIC ${PROJECT_SOURCE_DIR}/UIHandler.cpp)
add_library(bank STATIC ${PROJECT_SOURCE_DIR}/Bank.cpp)

# DatabaseManager with SQLiteCpp
add_library(databasemanager STATIC ${PROJECT_SOURCE_DIR}/DataBaseManager.cpp)
target_link_libraries(databasemanager bankaccount transactions SQLiteCpp SQLite::SQLite3)

# Link other dependencies
target_link_libraries(bankaccount OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(uihandler bank)

# Main Executable
add_executable(BankSimulation ${PROJECT_SOURCE_DIR}/main.cpp)

# Link Libraries for Executable
target_link_libraries(BankSimulation
    uihandler
    databasemanager
    GLEW::GLEW
    OpenSSL::SSL
    OpenSSL::Crypto
    OpenGL::GL
    dl
    ${SDL2_LIBRARIES}
    SQLiteCpp
)

# ImGui Sources
file(GLOB IMGUI_SOURCES
    ${PROJECT_EXTERNAL_DIR}/imgui/imgui.cpp
    ${PROJECT_EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${PROJECT_EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${PROJECT_EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${PROJECT_EXTERNAL_DIR}/imgui/imgui_widgets.cpp
    ${PROJECT_EXTERNAL_DIR}/imgui/backends/imgui_impl_sdl2.cpp
    ${PROJECT_EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)
target_sources(BankSimulation PRIVATE ${IMGUI_SOURCES})

# Google Test
set(GOOGLETEST_VERSION "v1.13.0")

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        ${GOOGLETEST_VERSION}
)

# Override googletest's minimum CMake version before fetching it
set(GOOGLETEST_FORCE_MIN_VERSION 3.10 CACHE INTERNAL "")

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_bank_system
    ${PROJECT_TEST_DIR}/test_BankAccount.cpp
    ${PROJECT_TEST_DIR}/test_Bank.cpp
    ${PROJECT_TEST_DIR}/test_Transactions.cpp
)

target_link_libraries(test_bank_system
    uihandler
    databasemanager
    GTest::gtest_main
    GTest::gmock
    pthread
)

add_test(NAME test_bank_system COMMAND test_bank_system)