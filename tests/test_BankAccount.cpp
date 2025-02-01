#include <gtest/gtest.h>
#include "BankAccount.h"

class BankAccountTest : public ::testing::Test {
protected:
    BankAccount* account1;

    // SetUp is called before each test
    void SetUp() override {
        std::string account_id1 = "12345";
        std::string name1 = "John Doe";
        std::string hashed_password = BankAccount::hash_password("55550");
        // Create and initialize the BankAccount object
        account1 = new BankAccount(account_id1, name1, 1000.0, hashed_password);
    }

    // TearDown is called after each test
    void TearDown() override {
        delete account1;  // Clean up any allocated memory
    }
};

// Test case: Create account with positive initial balance
TEST_F(BankAccountTest, CreateAccount) {
    EXPECT_EQ(account1->get_account_id(), "12345");
    EXPECT_EQ(account1->get_owner_name(), "John Doe");
    EXPECT_DOUBLE_EQ(account1->get_balance(), 1000.0);
}

// Test case: Deposit money into the account
TEST_F(BankAccountTest, Deposit) {
    account1->deposit(50.0);
    EXPECT_DOUBLE_EQ(account1->get_balance(), 1050.0);
}

// Test case: Withdraw money from the account
TEST_F(BankAccountTest, Withdraw) {
    account1->withdraw(500.0);
    EXPECT_DOUBLE_EQ(account1->get_balance(), 500.0);
}

// Test case: Get Account Id
TEST_F(BankAccountTest, Id) {
    EXPECT_EQ(account1->get_account_id(), "12345");
}

// Test case: Get owner name
TEST_F(BankAccountTest, Name) {
    EXPECT_EQ(account1->get_owner_name(), "John Doe");
}
