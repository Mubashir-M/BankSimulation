#include "Bank.h"
#include "BankAccount.h"
#include "UIHandler.h"

#include <SDL.h>
#include "GL/glew.h"

// ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"


#include <stdio.h>
#include <iostream>
#include <cstring>

// Next: Expand this to have buttons for withdraw, deposit and transfer
// Refactor code into own .cpp files.
// Create a database to write to and read from about accounts. Use get_accounts() in Bank.h
int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow("BankSimulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // Initialize ImGui context
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Initialize ImGui with SDL2 and OpenGL
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 120");

    // Main loop
    std::string userName;  // String to store user name
    std::string bankId;    // String to store bank ID
    std::string password;    // String to store bank ID
    std::string transferId;    // String to store bank ID

    bool loggedIn = false;
    bool running = true;
    DataBaseManager db_manager("bank.db");
    Bank bank(db_manager);
    UIHandler uihandler(bank);
    std::string errorMessage;
    char nameBuffer[128] = "";
    char idBuffer[128] = "";
    char amountBuffer[128] ="";
    char transferIdBuffer[128] = "";
    char passwordBuffer[128] = "";
    double amount = 0;
    int bufferSize = 128;
    
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // Set running to false to exit the loop
            }
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        // Start a new frame for ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (!loggedIn){
            uihandler.showLoginWindow(loggedIn, userName, bankId, password, errorMessage, nameBuffer, idBuffer, passwordBuffer, bufferSize);
        } else {
            uihandler.showLoggedInWindow(loggedIn, userName, bankId, transferId, errorMessage, amount, nameBuffer, idBuffer, transferIdBuffer, amountBuffer, bufferSize);
        }

        // Rendering
        ImGui::Render();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // Background color
        glClear(GL_COLOR_BUFFER_BIT); // Clear screen
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window); // Swap the window to display the content
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(glContext); // Delete OpenGL context
    SDL_DestroyWindow(window); // Destroy SDL window
    SDL_Quit(); // Quit SDL subsystems

    return 0;
}
