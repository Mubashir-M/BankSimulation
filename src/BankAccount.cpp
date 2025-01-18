#include <stdexcept>
#include <string>
#include <iostream>

#include "BankAccount.h"
#include "TransactionTypes.h"

BankAccount::BankAccount(const std::string& id, const std::string& name, double initial_balance)
    : account_id_(id), owner_name_(name),balance_(initial_balance){
        if(initial_balance <0){
            throw std::invalid_argument("Initial balance cannot be  negative.");
        }
};

void BankAccount::deposit(double amount){
    if (amount <= 0) {
        throw std::invalid_argument("Deposit amount must be positive.");
    }
    balance_+=amount;

    std::string transaction_id = generate_transaction_id();
    TransactionTypes type = TransactionTypes::DEPOSIT;

    Transactions new_transaction(transaction_id, type, amount, account_id_, account_id_, "Deposit into account");

    add_transaction(new_transaction);
    std::cout << "Deposited "<<amount<<" into the account."<<std::endl;
};

void BankAccount::withdraw(double amount){
    if (amount <= 0) {
        throw std::invalid_argument("Withdrawal amount must be positive.");
    }
    if (amount > balance_) {
        throw std::runtime_error("Insufficient funds for withdrawal.");
    }

    balance_-=amount;

    std::string transaction_id = generate_transaction_id();
    TransactionTypes type = TransactionTypes::WITHDRAWAL;

    Transactions new_transaction(transaction_id, type, amount, account_id_, account_id_, "Withdrawal from account");

    add_transaction(new_transaction);
    std::cout << "Widthdrew "<<amount<<" from the account."<<std::endl;
};

double BankAccount::get_balance() const{
    return balance_;
};

const std::string& BankAccount::get_account_id() const {
    return account_id_;
};

const std::string& BankAccount::get_owner_name() const {
    return owner_name_;
};

std::vector<Transactions> BankAccount::get_transctions() const {
    return transactions_;
};

std::string BankAccount::generate_transaction_id() const {
    return "TX-" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
}

void BankAccount::add_transaction(Transactions transaction) {
    transactions_.push_back(transaction);
};
