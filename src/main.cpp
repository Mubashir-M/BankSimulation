#include "Bank.h"
#include "BankAccount.h"
#include <iostream>
#include <vector>

int main() {
    Bank bank;

    // Create accounts
    bank.create_account("12345", "John Doe", 1000.0);
    bank.create_account("67890", "Jane Smith", 2000.0);

    // List all accounts
    std::cout << "Accounts in the bank:\n";
    for (const auto& account_id : bank.list_accounts()) {
        std::cout << "- Account ID: " << account_id << "\n";
    }

    // Perform deletion of an account
    std::cout << "Deleting account with ID 12345...\n";
    bank.delete_account("12345");

    // List accounts after deletion
    std::cout << "Accounts after deletion:\n";
    for (const auto& account_id : bank.list_accounts()) {
        std::cout << "- Account ID: " << account_id << "\n";
    }

    return 0;
}
