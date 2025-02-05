#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "DataBaseManager.h"
#include "Bank.h"
#include "BankAccount.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

// Mock class for DataBaseManager
class MockDataBaseManager : public DataBaseManager {
public:
    MockDataBaseManager(const std::string& db_file) : DataBaseManager(db_file) {}

    MOCK_METHOD(void, create_account, (const std::string&, const std::string&, const std::string&, double), (override));
    MOCK_METHOD(void, delete_account, (const std::string&), (override));
    MOCK_METHOD(std::shared_ptr<BankAccount>, get_account, (const std::string&), (override));
    MOCK_METHOD(std::shared_ptr<Transactions>, get_transaction, (const std::string&), (override));
    MOCK_METHOD(std::vector<std::string>, list_accounts, (), (const, override)); // Make sure const here
    MOCK_METHOD(void, add_transaction, (const std::string&, TransactionTypes, const std::string&, const std::string&, double, const std::string&), (override));
    MOCK_METHOD(void, deposit_to_account, (const std::string&, double), (override));
    MOCK_METHOD(void, withdraw_from_account, (const std::string&, double), (override));
    MOCK_METHOD(void, transfer, (const std::string&, const std::string&, double), (override));
    MOCK_METHOD(std::string, enum_to_string_types, (TransactionTypes), (const, override)); // Make sure const here
    MOCK_METHOD(TransactionTypes, string_to_enum_types, (const std::string&), (const, override)); // Make sure const here
};


class BankTest : public ::testing::Test {
protected:
    MockDataBaseManager mock_db{"mock_db_file.db"};
    Bank bank;

    BankTest() : bank(mock_db) {}

    void SetUp() override {
        std::cout << "Setting up BankTest...\n";

        EXPECT_CALL(mock_db, list_accounts())
            .WillRepeatedly(Return(std::vector<std::string>{})); // Start empty

        EXPECT_CALL(mock_db, create_account("12345", "55550", "John Doe", 1000.0))
            .Times(1);
        EXPECT_CALL(mock_db, create_account("67890", "55551", "Jane Doe", 1500.0))
            .Times(1);

        std::cout << "Creating account 12345...\n";
        bank.create_account("12345", "55550", "John Doe", 1000.0);
        
        std::cout << "Creating account 67890...\n";
        bank.create_account("67890", "55551", "Jane Doe", 1500.0);

        EXPECT_CALL(mock_db, list_accounts())
            .WillRepeatedly(Return(std::vector<std::string>{"12345", "67890"})); // Now accounts exist

        std::cout << "Finished SetUp()\n";
    }

};

// Test case for creating a new account successfully
TEST_F(BankTest, CreateAccount_Success) {
    EXPECT_CALL(mock_db, create_account("1111", "55552", "New Account", 1000.0)).Times(1);

    EXPECT_CALL(mock_db, list_accounts())
        .WillOnce(Return(std::vector<std::string>{"12345", "67890"})) // First call: before account creation
        .WillOnce(Return(std::vector<std::string>{"12345", "67890", "1111"})); // Second call: after creation

    EXPECT_NO_THROW(bank.create_account("1111", "55552", "New Account", 1000.0));

    std::vector<std::string> accounts = bank.list_accounts();
    EXPECT_EQ(accounts.size(), 3);
}


// Test case for trying to create an account with an existing ID (should throw an exception)
TEST_F(BankTest, CreateAccount_AlreadyExists) {
    EXPECT_CALL(mock_db, list_accounts())
    .WillRepeatedly(Return(std::vector<std::string>{"12345", "67890"}));

    EXPECT_THROW(bank.create_account("12345", "55553", "John Doe", 1000.0), std::invalid_argument);
}

// Test case for deleting an account
TEST_F(BankTest, DeleteAccount_Success) {
    EXPECT_CALL(mock_db, delete_account("12345")).Times(1);
    EXPECT_CALL(mock_db, list_accounts()).WillRepeatedly(Return(std::vector<std::string>{"67890"}));

    EXPECT_NO_THROW(bank.delete_account("12345"));

    std::vector<std::string> accounts = bank.list_accounts();
    EXPECT_EQ(accounts.size(), 1);
}

// Test case for depositing money into an account
TEST_F(BankTest, DepositToAccount_Success) {
    EXPECT_CALL(mock_db, deposit_to_account("12345", 500.0)).Times(1);

    // Mock get_account to return an updated balance after deposit
    BankAccount updated_account("12345", "John Doe", 1500.0, BankAccount::hash_password("55550"));
    std::shared_ptr<BankAccount> updatedAccountPtr = std::make_shared<BankAccount>(updated_account);

    EXPECT_CALL(mock_db, get_account("12345")).WillOnce(Return(updatedAccountPtr));

    EXPECT_NO_THROW(bank.deposit_to_account("12345", 500.0));

    // Verify the balance is now 1500
    std::shared_ptr<BankAccount> retrievedAccount = bank.get_account("12345");
    EXPECT_EQ(retrievedAccount->get_balance(), 1500.0);
}


// Test case for withdrawing money from an account
TEST_F(BankTest, WithdrawFromAccount_Success) {
    EXPECT_CALL(mock_db, withdraw_from_account("12345", 500.0)).Times(1);

    BankAccount updated_account("12345", "John Doe", 500.0, BankAccount::hash_password("55550"));
    std::shared_ptr<BankAccount> updatedAccountPtr = std::make_shared<BankAccount>(updated_account);

    EXPECT_CALL(mock_db, get_account("12345")).WillOnce(Return(updatedAccountPtr));
    EXPECT_NO_THROW(bank.withdraw_from_account("12345", 500.0));

    std::shared_ptr<BankAccount> retrievedAccount = bank.get_account("12345");
    EXPECT_EQ(retrievedAccount->get_balance(), 500.0);
}

// Test case for transferring money between accounts
TEST_F(BankTest, TransferFromAccountToAnother_Success) {
    EXPECT_CALL(mock_db, transfer("12345", "67890", 500.0)).Times(1);

    BankAccount updated_account_from("12345", "John Doe", 500.0, BankAccount::hash_password("55550"));
    BankAccount updated_account_to("67890", "Jane Doe", 2000.0, BankAccount::hash_password("55551"));

    std::shared_ptr<BankAccount> updated_account_from_ptr = std::make_shared<BankAccount>(updated_account_from);
    std::shared_ptr<BankAccount> updated_account_to_ptr = std::make_shared<BankAccount>(updated_account_to);

    EXPECT_CALL(mock_db, get_account("12345")).WillOnce(Return(updated_account_from_ptr));
    EXPECT_CALL(mock_db, get_account("67890")).WillOnce(Return(updated_account_to_ptr));

     std::shared_ptr<BankAccount> retrievedAccount_from = bank.get_account("12345");
     std::shared_ptr<BankAccount> retrievedAccount_to = bank.get_account("67890");

    EXPECT_NO_THROW(bank.transfer("12345", "67890", 500.0));

    EXPECT_EQ(retrievedAccount_from->get_balance(), 500.0);
    EXPECT_EQ(retrievedAccount_to->get_balance(), 2000.0);
}

// Test case for getting account details
TEST_F(BankTest, GetAccount_Success) {
    BankAccount account("12345", "John Doe", 1000.0, BankAccount::hash_password("55550"));
    std::shared_ptr<BankAccount> accountPtr = std::make_shared<BankAccount>(account);
    
    EXPECT_CALL(mock_db, get_account("12345")).WillOnce(Return(accountPtr));

    std::shared_ptr<BankAccount> retrievedAccount = bank.get_account("12345");
    EXPECT_EQ(retrievedAccount->get_balance(), 1000.0);
    EXPECT_EQ(retrievedAccount->get_owner_name(), "John Doe");
}
