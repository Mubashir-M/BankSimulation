#include "Transactions.h"
#include "BankAccount.h"

#include <iostream>

Transactions::Transactions(
    std::string transaction_id,
    TransactionTypes type,
    double amount,
    std::string account_id,
    std::string recipient_account_id,
    std::string description)
    : transaction_id_(std::move(transaction_id)),
      type_(type),
      amount_(amount),
      account_id_(account_id),
      recipient_account_id_(recipient_account_id),
      description_(std::move(description)) {}

std::string Transactions::get_id() const {
    return transaction_id_;
};

double Transactions::get_amount() const {
    return amount_;
};

std::string Transactions::get_account_id() const {
    return account_id_;
};

std::string Transactions::get_recipient_account_id() const {
    return recipient_account_id_;
};

std::string Transactions::get_description() const {
    return description_;
};

TransactionTypes Transactions::get_transaction_type() const {
    return type_;
};

