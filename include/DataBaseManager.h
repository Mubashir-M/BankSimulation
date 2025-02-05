#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "BankAccount.h"
#include "Transactions.h"
#include "TransactionTypes.h"

#include <SQLiteCpp/SQLiteCpp.h>

#include <string>
#include <memory>
#include <vector>


class DataBaseManager {
public:
    // Constructor and Destructor
    DataBaseManager(const std::string& db_file);
    virtual ~DataBaseManager();

    // Function to initialize database tables
    virtual void initialize();

    // Function to create a new bank account in the database
    virtual void create_account(const std::string& id, const std::string& password, const std::string& name, double initial_balance);

    // Function to delete an account from the database
    virtual void delete_account(const std::string& id);

    // Function to get an account from the database by ID
    virtual std::shared_ptr<BankAccount> get_account(const std::string& id);

     // Function to get an transaction from the database by transaction ID
    virtual std::shared_ptr<Transactions> get_transaction(const std::string& transaction_id);

    // Function to list all account IDs
    virtual std::vector<std::string> list_accounts() const;


    // Function to add a transaction
    virtual void add_transaction(const std::string& transaction_id, TransactionTypes type,  const std::string& account_id, const std::string& recipient_account_id, double amount, const std::string& description);

    // Function to deposit money into an account
    virtual void deposit_to_account(const std::string& id, double amount);

    // Function to withdraw money from an account
    virtual void withdraw_from_account(const std::string& id, double amount);

    // Function to transfer money between two accounts
    virtual void transfer(const std::string& from_id, const std::string& to_id, double amount);

    virtual std::string enum_to_string_types (TransactionTypes type) const;
    virtual TransactionTypes string_to_enum_types (const std::string& type_str) const;

private:
    SQLite::Database db_;  // SQLite database object
};

#endif // DATABASE_MANAGER_H
