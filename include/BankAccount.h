#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <iostream>
#include <string>

class BankAccount {
public:
    // Constructor
    BankAccount(const std::string& id, const std::string& name, double initial_balance, const std::string& hashed_password);

    // Getter functions
    double get_balance() const;
    const std::string& get_owner_name() const;
    const std::string& get_account_id() const;

    // Password checking
    bool check_password(const std::string& password) const;
    static std::string hash_password(const std::string& password);
    static std::string generate_transaction_id();

private:
    std::string account_id_;
    std::string owner_name_;
    std::string hashed_password_;
    double balance_;
};

#endif // BANK_ACCOUNT_H
