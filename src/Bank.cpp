#include "Bank.h"
#include "BankAccount.h"

#include <vector>

void Bank::create_account(const std::string& id, const std::string& name, double initial_balance) {
    if (accounts.find(id) != accounts.end()) {
        // Throw an exception if account already exists
        throw std::invalid_argument("Account with this id already exists.");
    }

    BankAccount new_account(id, name, initial_balance);
    accounts.emplace(id, new_account);
    std::cout << "Created a new account successfully." << std::endl;
}

void Bank::delete_account(const std::string& id){
    if(accounts.find(id) == accounts.end()){
        throw std::invalid_argument("Account with the given id does not exist.");
    }
    accounts.erase(id);
        std::cout<<"Account with the id "<<id<<" deleted successfully.";
}

void Bank::deposit_to_account(const std::string& id, double amount){
    auto account = accounts.find(id);
    if(account == accounts.end()){
        throw std::invalid_argument("Account does not exist");
    }
    account->second.deposit(amount);

}
void Bank::withdraw_from_account(const std::string& id, double amount){
    auto account = accounts.find(id);
    if(account == accounts.end()){
        throw std::invalid_argument("Account does not exist.");
    }
    account->second.withdraw(amount);
}
void Bank::transfer(const std::string& from_id, const std::string& to_id, double amount){

    auto account_from = accounts.find(from_id);
    auto account_to = accounts.find(to_id);

    if (account_from == accounts.end()) {
        throw std::invalid_argument("The sender account ID does not exist.");
    }
    if (account_to == accounts.end()) {
        throw std::invalid_argument("The recipient account ID does not exist.");
    }

    try {
        account_from->second.withdraw(amount);
        account_to->second.deposit(amount);
    } catch (const std::exception& e) {
        throw std::runtime_error("Transfer failed: " + std::string(e.what()));
    }
}
const std::unordered_map<std::string, BankAccount>& Bank::get_accounts() const {
        return accounts;
}

BankAccount& Bank::get_account(const std::string& id){
    auto account = accounts.find(id);
    if(account != accounts.end()){
        return account->second;
    }
    throw std::invalid_argument("Account does not exist.");
}
std::vector<std::string> Bank::list_accounts() const{
    std::vector<std::string> accounts_list;

    for (auto& account: accounts){
        accounts_list.push_back(account.first);
    }
    return accounts_list;
}