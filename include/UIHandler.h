// UIHandler.h
#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <string>
#include "Bank.h"

// Function declarations
void showLoginWindow(bool& loggedIn, std::string& userName ,std::string& bankId, std::string& password, std::string& errorMessage, Bank& bank, char nameBuffer[128], char idBuffer[128], char passwordBuffer[128], int bufferSize);
void showLoggedInWindow(bool& loggedIn, std::string& userName, std::string& bankId, std::string& transferId, std::string& errorMessage, Bank& bank, double& amount, char nameBuffer[128], char idBuffer[128], char transferIdBuffer[128], char amountBuffer[128], int bufferSize);

#endif // UIHANDLER_H
