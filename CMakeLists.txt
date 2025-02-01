cmake_minimum_required(VERSION 3.10)
project(BankSimulation)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for your project
include_directories(${PROJECT_SOURCE_DIR}/include)

# SDL2
include_directories("/opt/homebrew/Cellar/sdl2/2.30.11/include/SDL2")
link_directories("/opt/homebrew/Cellar/sdl2/2.30.11/lib")

# GLEW
include_directories("/opt/homebrew/Cellar/glew/2.2.0_1/include")
link_directories("/opt/homebrew/Cellar/glew/2.2.0_1/lib")

# OpenSSL
set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl@3)
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})  # Use OpenSSL's include directory

# Add source libraries
add_library(bankaccount src/BankAccount.cpp src/Transactions.cpp)
add_library(uihandler src/UIHandler.cpp)
add_library(bank src/Bank.cpp)
target_link_libraries(bank bankaccount)

# Main executable
add_executable(BankSimulation src/main.cpp)

if(APPLE)
    target_link_libraries(BankSimulation bank uihandler SDL2 GLEW "-framework OpenGL" OpenSSL::SSL OpenSSL::Crypto)
else()
    target_link_libraries(BankSimulation bank uihandler SDL2 GLEW OpenGL OpenSSL::SSL OpenSSL::Crypto)
endif()

# Include and add ImGui sources
include_directories(external/imgui)
file(GLOB IMGUI_SOURCES 
    external/imgui/imgui.cpp 
    external/imgui/imgui_demo.cpp 
    external/imgui/imgui_draw.cpp 
    external/imgui/imgui_tables.cpp 
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_sdl2.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
target_sources(BankSimulation PRIVATE ${IMGUI_SOURCES})

# Add GoogleTest for testing
find_package(GTest REQUIRED)
enable_testing()

# Add your test executable (this already includes the necessary files for tests)
add_executable(test_bank_system 
    tests/test_BankAccount.cpp 
    tests/test_Bank.cpp 
    tests/test_Transactions.cpp
)

# Link libraries to test executable
target_link_libraries(test_bank_system 
    bank
    uihandler
    GTest::GTest 
    GTest::Main 
    pthread
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Register the test with CTest
add_test(NAME test_bank_system COMMAND test_bank_system)
