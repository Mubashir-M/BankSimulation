#ifndef BANK_H
#define BANK_H

#include "BankAccount.h"
#include "Transactions.h"
#include "DataBaseManager.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>


class Bank {
public:
    // Constructor
    Bank(DataBaseManager& db_manager);

    // Account management functions
    void create_account(const std::string& id, const std::string& password, const std::string& name, double initial_balance);
    void delete_account(const std::string& id);

    // Transaction functions
    void deposit_to_account(const std::string& id, double amount);
    void withdraw_from_account(const std::string& id, double amount);
    void transfer(const std::string& from_id, const std::string& to_id, double amount);

    // Accessor functions
    std::vector<std::string> list_accounts() const;
    std::shared_ptr<BankAccount> get_account(const std::string& id) const;

private:
    // Reference to the DataBaseManager instance
    DataBaseManager& db_manager_;
};

#endif // BANK_H
