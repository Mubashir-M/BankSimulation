cmake_minimum_required(VERSION 3.10)

# Project name
project(BankSimulation)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for your project
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Main executable
add_executable(BankSimulation src/main.cpp)

# Link SDL2 library
target_link_libraries(BankSimulation ${SDL2_LIBRARIES})

# Find GLEW package
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Find OpenSSL package
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Add source libraries
add_library(bankaccount src/BankAccount.cpp src/Transactions.cpp)
add_library(uihandler src/UIHandler.cpp)
add_library(bank src/Bank.cpp)

target_link_libraries(bank bankaccount)
target_link_libraries(bankaccount OpenSSL::SSL OpenSSL::Crypto)

target_link_libraries(uihandler bank)

# Link libraries based on the platform
if(APPLE)
    target_link_libraries(BankSimulation 
        bank 
        uihandler 
        GLEW::GLEW 
        "-framework OpenGL"
        OpenSSL::SSL 
        OpenSSL::Crypto
        dl
    )
else()
    target_link_libraries(BankSimulation 
        bank 
        uihandler 
        GLEW::GLEW 
        OpenGL 
        OpenSSL::SSL 
        OpenSSL::Crypto
        dl
    )
endif()

# Include and add ImGui sources
include_directories(external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/backends)

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

# Add your test executable
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