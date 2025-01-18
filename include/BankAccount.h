#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <iostream>
#include <string>
#include <vector>

#include "Transactions.h"

class BankAccount{
    public:
        BankAccount(const std::string& id, const std::string& name, double initial_balance);
        void deposit(double amount);
        void withdraw(double amount);
        double get_balance() const;
        const std::string& get_owner_name() const;
        const std::string& get_account_id()const;

        std::vector<Transactions> get_transctions() const;
        std::string generate_transaction_id() const;
        void add_transaction(Transactions transaction);

    private:
        std::string account_id_;
        std::string owner_name_;
        double balance_;
        std::vector<Transactions> transactions_;
};

#endif // BANK_ACCOUNT_H