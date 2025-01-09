#include "BankAccount.h"
#include <stdexcept>
#include <string>
#include <iostream>

BankAccount::BankAccount(const std::string& id, const std::string& name, double initial_balance)
    : account_id(id), owner_name(name),balance(initial_balance){
        if(initial_balance <0){
            throw std::invalid_argument("Initial balance cannot be  negative.");
        }
};

void BankAccount::deposit(double amount){
    if (amount <= 0) {
        throw std::invalid_argument("Deposit amount must be positive.");
    }
    balance+=amount;
    std::cout << "Deposited "<<amount<<" into the account."<<std::endl;
};

void BankAccount::withdraw(double amount){
    if (amount <= 0) {
        throw std::invalid_argument("Withdrawal amount must be positive.");
    }
    if (amount > balance) {
        throw std::runtime_error("Insufficient funds for withdrawal.");
    }

    balance-=amount;
    std::cout << "Widthdrew "<<amount<<" from the account."<<std::endl;
}

double BankAccount::get_balance() const{
    return balance;
}

const std::string& BankAccount::get_account_id() const {
    return account_id;
}

const std::string& BankAccount::get_owner_name() const {
    return owner_name;
}