#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "TransactionTypes.h"

#include <iostream>
#include <string>

class BankAccount;

class Transactions {
    public:
        Transactions(std::string transaction_id, TransactionTypes type, double amount, std::string account_id, std::string recipient_account_id, std::string description = "");
        std::string get_id() const;
        double get_amount() const;
        std::string get_account_id() const;
        std::string get_recipient_account_id() const;
        std::string get_description() const;
        TransactionTypes get_transaction_type() const;

    private:
        std::string account_id_;
        std::string recipient_account_id_;
        std::string description_;
        std::string transaction_id_;
        double amount_;
        TransactionTypes type_;
};

#endif //TRANSACTIONS_H