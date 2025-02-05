#include "Bank.h"
#include "DataBaseManager.h"

#include <algorithm>

Bank::Bank(DataBaseManager& db_manager) : db_manager_(db_manager) {}

void Bank::create_account(const std::string& id, const std::string& password, const std::string& name, double initial_balance) {
    std::vector<std::string> existing_accounts = db_manager_.list_accounts();
    
    if (std::find(existing_accounts.begin(), existing_accounts.end(), id) != existing_accounts.end()) {
        throw std::invalid_argument("Account with ID " + id + " already exists.");
    }

    db_manager_.create_account(id, password, name, initial_balance);
}


void Bank::delete_account(const std::string& id) {
    db_manager_.delete_account(id);
}

void Bank::deposit_to_account(const std::string& id, double amount) {
    db_manager_.deposit_to_account(id, amount);
}

void Bank::withdraw_from_account(const std::string& id, double amount) {
    db_manager_.withdraw_from_account(id, amount);
}

void Bank::transfer(const std::string& from_id, const std::string& to_id, double amount) {
    db_manager_.transfer(from_id, to_id, amount);
}

std::vector<std::string> Bank::list_accounts() const {
    return db_manager_.list_accounts();
}

std::shared_ptr<BankAccount> Bank::get_account(const std::string& id) const {
    return db_manager_.get_account(id);
}