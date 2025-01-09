#include <iostream>
#include <string>
#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

class BankAccount{
    public:
        BankAccount(const std::string& id, const std::string& name, double initial_balance);
        void deposit(double amount);
        void withdraw(double amount);
        double get_balance() const;
        const std::string& get_owner_name() const;
        const std::string& get_account_id()const;
    private:
        std::string account_id;
        std::string owner_name;
        double balance;
};

#endif // BANK_ACCOUNT_H