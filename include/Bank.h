#ifndef BANK_H
#define BANK_H
#include <iostream>
#include <unordered_map>
#include <vector>

#include "BankAccount.h"

class Bank {
    public:
        void create_account(const std::string& id, const std::string& name, double initial_balance);
        void delete_account(const std::string& id);
        void deposit_to_account(const std::string& id, double amount);
        void withdraw_from_account(const std::string& id, double amount);
        void transfer(const std::string& from_id, const std::string& to_id, double amount);
        const std::unordered_map<std::string, BankAccount>& get_accounts() const;
        BankAccount& get_account(const std::string& id);
        std::vector<std::string> list_accounts() const;
    private:
        std::unordered_map<std::string, BankAccount> accounts;
};

#endif //BANK_H
