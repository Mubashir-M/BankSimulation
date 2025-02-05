#ifndef UIHANDLER_H
#define UIHANDLER_H

#include "Bank.h"
#include <string>
#include <memory>

class UIHandler {
public:
    // Constructor
    UIHandler(Bank& bank);

    // Login and logged-in window functions
    void showLoginWindow(bool& loggedIn, std::string& userName, std::string& bankId, std::string& password, std::string& errorMessage, char nameBuffer[128], char idBuffer[128], char passwordBuffer[128], int bufferSize);
    void showLoggedInWindow(bool& loggedIn, std::string& userName, std::string& bankId, std::string& transferId, std::string& errorMessage, double& amount, char nameBuffer[128], char transferIdBuffer[128], char idBuffer[128], char amountBuffer[128], int bufferSize);

private:
    Bank& m_bank;  // Bank reference
    std::shared_ptr<BankAccount> m_account; // Account shared pointer
};

#endif // UIHANDLER_H
