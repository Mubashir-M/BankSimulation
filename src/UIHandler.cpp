#include "UIHandler.h"

#include "imgui.h"
#include <iostream>
#include <string>

UIHandler::UIHandler(Bank& bank) : m_bank(bank), m_account(nullptr) {}

void UIHandler::showLoginWindow(bool& loggedIn, std::string& userName, std::string& bankId, std::string& password, std::string& errorMessage, char nameBuffer[128], char idBuffer[128], char passwordBuffer[128], int bufferSize) {
    ImGui::SetNextWindowSize(ImVec2(500, 300));
    ImGui::Begin("Login");

    ImGui::SetNextItemWidth(200);
    if (ImGui::InputText("User name", nameBuffer, bufferSize)) {
        userName = nameBuffer;
    }

    ImGui::SetNextItemWidth(200);
    if (ImGui::InputText("Bank ID", idBuffer, bufferSize)) {
        bankId = idBuffer;
    }

    ImGui::SetNextItemWidth(200);
    if (ImGui::InputText("Password", passwordBuffer, bufferSize, ImGuiInputTextFlags_Password)) {
        password = passwordBuffer;
    }

    if (ImGui::Button("Login")) {
        try {
            m_account = m_bank.get_account(bankId);
            if (m_account && m_account->check_password(password)){
                loggedIn = true;
                std::cout << "Logged in successfully!" << std::endl;
                // Clear the name and id buffers after successful login
                for (int i = 0; i < bufferSize; ++i) {
                    nameBuffer[i] = '\0';
                    idBuffer[i] = '\0';
                    passwordBuffer[i] = '\0';
                }
            } else {
                throw std::invalid_argument("Incorrect password.");
            };
        } catch (const std::invalid_argument& e) {
            std::cout << "Login failed: " << e.what() << std::endl;
            errorMessage = e.what(); // Set error message if login fails
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Create Account")) {
       // Check if the fields are empty before creating the account
        if (userName.empty() || bankId.empty() || password.empty()) {
            errorMessage = "Please fill in all fields.";
            std::cerr << errorMessage << std::endl;
        } else {
            try {
                m_bank.create_account(bankId, password, userName, 0.0);
                errorMessage.clear();
                loggedIn = true;
                std::cout << "Created an account for " << userName << "." << std::endl;
                nameBuffer[0] = '\0';
                idBuffer[0] = '\0';
                passwordBuffer[0] = '\0';
            } catch (const std::invalid_argument& e) {
                errorMessage = e.what(); // Store the error message
                std::cerr << "Error: " << errorMessage << std::endl;
            }
        }
    }

    if (!errorMessage.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", errorMessage.c_str());
    }
    ImGui::End();
}

void UIHandler::showLoggedInWindow(bool& loggedIn, std::string& userName, std::string& bankId, std::string& transferId, std::string& errorMessage, double& amount, char nameBuffer[128], char transferIdBuffer[128], char idBuffer[128], char amountBuffer[128], int bufferSize) {
    ImGui::SetNextWindowSize(ImVec2(500, 300));
    ImGui::Begin("Logged In");

    ImGui::Text("Welcome to the Bank Simulation!");
    ImGui::Text("Bank Name: %s", m_account->get_owner_name().c_str());
    ImGui::Text("Your bank account has a balance of %.2f", m_account->get_balance());

    // Button to perform the transfer
    if (ImGui::Button("Transfer Money")) {
        if (amount <= 0) {
            errorMessage = "Amount must be greater than zero!";
        } else if (transferId.empty()) {
            errorMessage = "Target account ID cannot be empty!";
        } else {
            try {
                // Attempt the transfer
                m_bank.transfer(bankId, transferId, amount);
                errorMessage.clear();
                m_account = m_bank.get_account(bankId);

                // **Reset the fields after a successful transfer**
                amount = 0.0;
                amountBuffer[0] = '\0';  // Clear input buffer
                transferId.clear();
                transferIdBuffer[0] = '\0';  // Clear input buffer
            } catch (const std::invalid_argument& e) {
                errorMessage = std::string("Transfer failed: ") + e.what();
            } catch (const std::exception& e) {
                // Catch other types of exceptions (general fallback)
                errorMessage = "Unexpected error: " + std::string(e.what());
                std::cerr << errorMessage << std::endl;  // Optionally log the error
            }

        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Deposit")) {
        // Deposit functionality here
        if (amount <= 0) {
            errorMessage = "Amount must be greater than zero!";
        } else {
            try {
                double old_balance = m_account->get_balance();
                // Perform the deposit operation
                m_bank.deposit_to_account(bankId, amount);

                // If the deposit succeeds, reset the variables
                if ( m_bank.get_account(bankId)->get_balance() - old_balance  == amount){
                    m_account = m_bank.get_account(bankId);
                    amount = 0.0;
                    amountBuffer[0] = '\0';

                    std::cout << "Deposit successful." << std::endl; // Inform the user of success (optional)
                }

            } catch (const std::invalid_argument& e) {
                // Handle any invalid argument exceptions (e.g., account not found)
                errorMessage = "Deposit failed: " + std::string(e.what());
                std::cerr << errorMessage << std::endl;  // Optionally log the error

            } catch (const std::exception& e) {
                // Catch other types of exceptions (general fallback)
                errorMessage = "Unexpected error: " + std::string(e.what());
                std::cerr << errorMessage << std::endl;  // Optionally log the error
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Withdraw")) {
        // Withdraw functionality here
        if (amount <= 0) {
            errorMessage = "Amount must be greater than zero!";
        } else {
            try {
                m_bank.withdraw_from_account(bankId, amount);
                amount = 0.0;
                amountBuffer[0] = '\0';
                m_account = m_bank.get_account(bankId);
            } catch (const std::invalid_argument& e){
                errorMessage = std::string("Widthdraw failed: ") + e.what();
            } catch (const std::exception& e) {
                // Catch other types of exceptions (general fallback)
                errorMessage = "Unexpected error: " + std::string(e.what());
                std::cerr << errorMessage << std::endl;  // Optionally log the error
            }
        }
    }    
    // Handle amount input and conversion from string to int
    if (ImGui::InputText("Amount", amountBuffer, bufferSize)) {
        try {
            errorMessage.clear();
            amount = std::stod(amountBuffer);  // Convert string to integer
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid input for amount: " << e.what() << std::endl;
            amount = 0.0;
            errorMessage = "Invald input for amount!";
        } catch (const std::out_of_range& e) {
            std::cerr << "Amount out of range: " << e.what() << std::endl;
            amount = 0.0;
            errorMessage = "Amount out of range!";
        }
    }

    // Handle target account ID input (should remain a string)
    if (ImGui::InputText("Target account ID", transferIdBuffer, bufferSize)) {
        errorMessage.clear();
        transferId = transferIdBuffer; // Store input directly as string (not converting to int)
    }

    if (ImGui::Button("Logout")) {
        loggedIn = false;
        userName.clear();
        bankId.clear();
        nameBuffer[0] = '\0';
        idBuffer[0] = '\0';
        std::cout << "Logged out successfully!" << std::endl;
    }
    

    if (!errorMessage.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", errorMessage.c_str());
    }

    ImGui::End();
}
