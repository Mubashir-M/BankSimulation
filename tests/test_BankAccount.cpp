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

// Test case: Get Account Id
TEST_F(BankAccountTest, Id) {
    EXPECT_EQ(account1->get_account_id(), "12345");
}

// Test case: Get owner name
TEST_F(BankAccountTest, Name) {
    EXPECT_EQ(account1->get_owner_name(), "John Doe");
}

// Test case: Get Balance
TEST_F(BankAccountTest, Balance) {
    EXPECT_EQ(account1->get_balance(), 1000.0);
}
