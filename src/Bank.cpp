#include "Bank.h"
#include "BankAccount.h"
#include "Transactions.h"

#include <vector>
#include <iostream>
#include <memory>

void Bank::create_account(const std::string& id, const std::string& name, double initial_balance) {
    if (accounts_.find(id) != accounts_.end()) {
        // Throw an exception if account already exists
        throw std::invalid_argument("Account with this id already exists.");
    }

    BankAccount new_account(id, name, initial_balance);
    accounts_.emplace(id, new_account);
    std::cout << "Created a new account successfully." << std::endl;
}

void Bank::delete_account(const std::string& id){
    if(accounts_.find(id) == accounts_.end()){
        throw std::invalid_argument("Account with the given id does not exist.");
    }
    accounts_.erase(id);
        std::cout<<"Account deleted successfully."<<std::endl;
}

void Bank::deposit_to_account(const std::string& id, double amount){
    auto account = accounts_.find(id);
    if(account == accounts_.end()){
        throw std::invalid_argument("Account does not exist");
    }
    account->second.deposit(amount);

}
void Bank::withdraw_from_account(const std::string& id, double amount){
    auto account = accounts_.find(id);
    if(account == accounts_.end()){
        throw std::invalid_argument("Account does not exist.");
    }
    account->second.withdraw(amount);
}
void Bank::transfer(const std::string& from_id, const std::string& to_id, double amount){

    auto account_from = accounts_.find(from_id);
    auto account_to = accounts_.find(to_id);

    if (account_from == accounts_.end()) {
        throw std::invalid_argument("The sender account ID does not exist.");
    }
    if (account_to == accounts_.end()) {
        throw std::invalid_argument("The recipient account ID does not exist.");
    }

    try {
        account_from->second.withdraw(amount);
        account_to->second.deposit(amount);

        std::string transaction_id = account_from->second.generate_transaction_id();

        Transactions transaction_sender(
            transaction_id, 
            TransactionTypes::TRANSFER, 
            -amount, 
            account_from->second.get_account_id(),
            account_to->second.get_account_id(),
            "Transfer to account " + to_id
        );

        Transactions transaction_recipient(
            transaction_id, 
            TransactionTypes::TRANSFER, 
            amount, 
            account_to->second.get_account_id(),
            account_from->second.get_account_id(),
            "Transfer from account " + from_id
        );
        account_from->second.add_transaction(transaction_sender);
        account_to->second.add_transaction(transaction_recipient);

    } catch (const std::exception& e) {
        throw std::runtime_error("Transfer failed: " + std::string(e.what()));
    }
}
const std::unordered_map<std::string, BankAccount>& Bank::get_accounts() const {
        return accounts_;
}

BankAccount& Bank::get_account(const std::string& id){
    auto account = accounts_.find(id);
    if(account != accounts_.end()){
        return account->second;
    }
    throw std::invalid_argument("Account does not exist.");
}
std::vector<std::string> Bank::list_accounts() const{
    std::vector<std::string> accounts_list;

    for (auto& account: accounts_){
        accounts_list.push_back(account.first);
    }
    return accounts_list;
}
