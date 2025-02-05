#include "DataBaseManager.h"
#include "BankAccount.h"
#include "TransactionTypes.h"

#include <iostream>
#include <vector>

DataBaseManager::DataBaseManager(const std::string& db_file) : db_(db_file, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE){
    initialize();
}

void DataBaseManager::initialize(){
    try {
        db_.exec("CREATE TABLE IF NOT EXISTS BankAccounts ("
                 "id TEXT PRIMARY KEY, "
                 "name TEXT NOT NULL, "
                 "balance REAL NOT NULL, "
                 "hashed_password TEXT NOT NULL);");

        db_.exec("CREATE TABLE IF NOT EXISTS Transactions ("
                 "transaction_id TEXT PRIMARY KEY, "
                 "transaction_type TEXT NOT NULL, "
                 "amount REAL NOT NULL, "
                 "account_id TEXT NOT NULL, "
                 "recipient_account_id TEXT, "
                 "description TEXT, "
                 "FOREIGN KEY(account_id) REFERENCES BankAccounts(id), "
                 "FOREIGN KEY(recipient_account_id) REFERENCES BankAccounts(id))");
        
        db_.exec("PRAGMA foreign_keys = ON;");

        std::cout << "Database initialized successfully!" << std::endl;
    } catch (const std::exception& e){
        std::cerr<<"Error initliazing the database: "<<e.what() <<std::endl;
    }
};

DataBaseManager::~DataBaseManager() {
    // No need to manually close the SQLite database; it's handled automatically.
    std::cout << "DataBaseManager destroyed and connection automatically closed." << std::endl;
}

void DataBaseManager::create_account(const std::string& id, const std::string& password, const std::string& name, double initial_balance){

    try {

        // Check if the account ID already exists
        SQLite::Statement check_query(db_, "SELECT COUNT(*) FROM BankAccounts WHERE id = ?");
        check_query.bind(1, id);
        check_query.executeStep();

        if (check_query.getColumn(0).getInt() > 0) {
            throw std::invalid_argument("Account with this id already exists.");
        }

        std::string hashed_password = BankAccount::hash_password(password);

        // Prepare the SQL statement to insert the new account into the BankAccounts table
        SQLite::Statement query(db_, "INSERT INTO BankAccounts (id, name, balance, hashed_password) VALUES (?, ?, ?, ?)");

        // Bind the parameters to the SQL query
        query.bind(1, id);               // Bind account ID
        query.bind(2, name);             // Bind account name
        query.bind(3, initial_balance);  // Bind initial balance
        query.bind(4, hashed_password);  // Bind the hashed password

        // Execute the query
        query.exec();

        std::cout << "Account created successfully for ID: " << id << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error creating account: " << e.what() << std::endl;
        throw;

    }
};

void DataBaseManager::delete_account(const std::string&id){
    try {
        SQLite::Statement query (db_, "DELETE FROM BankAccounts WHERE id = ?");

        query.bind(1,id);
        query.exec();

        std::cout <<"Account with id "<<id<<" deleted successfully."<<std::endl;
    } catch (const std::exception&e) {
        std::cerr << "Error in deleting account: " << e.what() <<std::endl;
    }
};

std::shared_ptr<BankAccount> DataBaseManager::get_account(const std::string& id) {
     try {
       SQLite::Statement query (db_, "SELECT id, name, balance, hashed_password FROM BankAccounts WHERE id = ?");
        query.bind(1,id);

        if (query.executeStep()) {  
            auto account = std::make_shared<BankAccount>(
                query.getColumn(0).getString(),  // id
                query.getColumn(1).getString(),  // name
                query.getColumn(2).getDouble(),   // balance
                query.getColumn(3).getString()  // hashed_password
            );

            std::cout << "Account with id " << id << " fetched successfully." << std::endl;
            return account;
        } else {
            std::cerr << "Account with id " << id << " not found." << std::endl;
            return nullptr;  // Return nullptr if no account was found
        }
    } catch (const std::exception&e) {
        std::cerr << "Error in fetching account: " << e.what() <<std::endl;
        return nullptr;
    }
};

std::shared_ptr<Transactions> DataBaseManager::get_transaction(const std::string& transaction_id) {
    try {

        SQLite::Statement query(db_, 
            "SELECT transaction_id, transaction_type, amount, account_id, recipient_account_id, description "
            "FROM Transactions WHERE transaction_id = ?");

        query.bind(1, transaction_id);

        if (query.executeStep()) {
            std::string transaction_id = query.getColumn(0).getText();
            std::string transaction_type = query.getColumn(1).getText();
            double amount = query.getColumn(2).getDouble();
            std::string account_id = query.getColumn(3).getText();
            std::string recipient_account_id = query.getColumn(4).getText();
            std::string description = query.getColumn(5).getText();

            TransactionTypes type = string_to_enum_types(transaction_type);

            // Return the created transaction as a shared pointer
            auto transaction = std::make_shared<Transactions>(transaction_id, type, amount, 
                                                              account_id, recipient_account_id, description);
            return transaction;
        } else {
            // If no transaction is found, return nullptr
            std::cerr << "Transaction with ID " << transaction_id << " not found." << std::endl;
            return nullptr;
        }

    } catch (const std::exception& e) {
        // Handle any errors related to the database
        std::cerr << "Error retrieving transaction " << transaction_id << ": " << e.what() << std::endl;
        return nullptr;
    }
}

std::vector<std::string> DataBaseManager::list_accounts() const {
    std::vector<std::string> account_ids;
    
    try {
        
         SQLite::Statement query(db_, "SELECT id FROM BankAccounts");

        while (query.executeStep()) {
            std::string account_id = query.getColumn(0).getString();
            account_ids.push_back(account_id);
        }
        
        std::cout << "Successfully listed all account IDs." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in listing accounts: " << e.what() << std::endl;
    }

    return account_ids;
};

void DataBaseManager::add_transaction(const std::string& transaction_id, TransactionTypes type, const std::string& account_id, const std::string& recipient_account_id, double amount, const std::string& description) {
    try {
        std::string transaction_type;
        try {
            transaction_type = enum_to_string_types(type);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid transaction type for transaction " << transaction_id << ": " << e.what() << std::endl;
            return;
        }

        SQLite::Statement query(db_, 
            "INSERT INTO Transactions (transaction_id, transaction_type, amount, account_id, recipient_account_id, description) "
            "VALUES (?,?,?,?,?,?)");

        query.bind(1, transaction_id);
        query.bind(2, transaction_type);
        query.bind(3, amount);
        query.bind(4, account_id);
        query.bind(5, recipient_account_id);
        query.bind(6, description);

        query.exec();

        std::cout << "Transaction " << transaction_id << " added successfully." << std::endl;
    } catch (const std::exception& e) {
        // Print detailed error message
        std::cerr << "Error adding transaction " << transaction_id << ": " << e.what() << std::endl;
    }
}

void DataBaseManager::deposit_to_account(const std::string& id, double amount) {
    try {
        SQLite::Statement query(db_, "UPDATE BankAccounts SET balance = balance + ? WHERE id = ?");
        query.bind(1, amount);
        query.bind(2, id);
        query.exec();
    
        std::string transaction_id = BankAccount::generate_transaction_id();
        add_transaction(transaction_id, TransactionTypes::DEPOSIT, id, id, amount, "Deposit into account");

        std::cout << "Deposited " << amount << " into the account with id " << id << "." << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error depositing to the account: " << e.what() << std::endl;
    }
}


void DataBaseManager::withdraw_from_account(const std::string& id, double amount) {
    try {

        SQLite::Statement query(db_, "SELECT balance FROM BankAccounts WHERE id = ?");
        query.bind(1, id);
        
        if (query.executeStep()) {
            double current_balance = query.getColumn(0).getDouble();

            if (current_balance >= amount) {
                SQLite::Statement update_query(db_, "UPDATE BankAccounts SET balance = balance - ? WHERE id = ?");
                update_query.bind(1, amount);
                update_query.bind(2, id);
                update_query.exec();

                std::cout << "Withdrew " << amount << " from the account with id " << id << "." << std::endl;

                std::string transaction_id = BankAccount::generate_transaction_id();

                add_transaction(transaction_id, TransactionTypes::WITHDRAWAL, id, id, amount, "Withdrawal from account");
            } else {
                std::cerr << "Insufficient funds in the account with id " << id << "." << std::endl;
            }
        } else {
            std::cerr << "Account with id " << id << " not found." << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Error withdrawing from the account: " << e.what() << std::endl;
    }
}


void DataBaseManager::transfer(const std::string& from_id, const std::string& to_id, double amount) {
    try {
        db_.exec("BEGIN TRANSACTION;");  // Start the transaction

        // Checks for source and destination accounts
        SQLite::Statement query(db_, "SELECT id, balance FROM BankAccounts WHERE id = ?");
        
        // Check source account
        query.bind(1, from_id);
        if (!query.executeStep()) {
            std::cerr << "Source account with id " << from_id << " does not exist." << std::endl;
            db_.exec("ROLLBACK;");  // Rollback if error
            return;
        }
        
        double from_balance = query.getColumn(1).getDouble();
        
        // Check destination account
        query.bind(1, to_id);
        if (!query.executeStep()) {
            std::cerr << "Destination account with id " << to_id << " does not exist." << std::endl;
            db_.exec("ROLLBACK;");  // Rollback if error
            return;
        }

        // Check if there are sufficient funds
        if (from_balance < amount) {
            std::cerr << "Insufficient funds in the source account " << from_id << "." << std::endl;
            db_.exec("ROLLBACK;");  // Rollback if error
            return;
        }

        // Update source and destination balances
        SQLite::Statement update_from(db_, "UPDATE BankAccounts SET balance = balance - ? WHERE id = ?");
        update_from.bind(1, amount);
        update_from.bind(2, from_id);
        update_from.exec();

        SQLite::Statement update_to(db_, "UPDATE BankAccounts SET balance = balance + ? WHERE id = ?");
        update_to.bind(1, amount);
        update_to.bind(2, to_id);
        update_to.exec();

        // Record the transaction for source (debit)
        std::string transaction_id_from = BankAccount::generate_transaction_id();
        add_transaction(transaction_id_from, TransactionTypes::TRANSFER, from_id, to_id, -amount, "Transfer to account " + to_id);

        // Record the transaction for destination (credit)
        std::string transaction_id_to = BankAccount::generate_transaction_id();
        add_transaction(transaction_id_to, TransactionTypes::TRANSFER, to_id, from_id, amount, "Transfer from account " + from_id);

        db_.exec("COMMIT;");  // Commit the transaction

        std::cout << "Successfully transferred " << amount << " from account " << from_id << " to account " << to_id << "." << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Error during transfer: " << e.what() << std::endl;
        db_.exec("ROLLBACK;");  // Rollback if any exception occurs
    }
}


std::string DataBaseManager::enum_to_string_types(TransactionTypes type) const {
    switch(type) {
        case TransactionTypes::DEPOSIT:
            return "DEPOSIT";
        case TransactionTypes::TRANSFER:
            return "TRANSFER";
        case TransactionTypes::WITHDRAWAL:
            return "WITHDRAWAL";
        default:
            std::cerr << "Unknown transaction type" << std::endl;
            throw std::invalid_argument("Unknown transaction type");
    }
}

TransactionTypes DataBaseManager::string_to_enum_types(const std::string& type_str) const {
    if (type_str == "DEPOSIT") {
        return TransactionTypes::DEPOSIT;
    } else if (type_str == "TRANSFER") {
        return TransactionTypes::TRANSFER;
    } else if (type_str == "WITHDRAWAL") {
        return TransactionTypes::WITHDRAWAL;
    } else {
        throw std::invalid_argument("Unknown transaction type: " + type_str);
    }
}
