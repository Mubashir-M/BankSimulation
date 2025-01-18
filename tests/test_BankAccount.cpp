#include <gtest/gtest.h>
#include "BankAccount.h"

// Test case: Create account with positive initial balance
TEST(BankAccountTest, CreateAccount) {
    BankAccount account("12345", "John Doe", 100.0);
    EXPECT_EQ(account.get_account_id(), "12345");
    EXPECT_EQ(account.get_owner_name(), "John Doe");
    EXPECT_DOUBLE_EQ(account.get_balance(), 100.0);
}


// Test case: Deposit money into the account
TEST(BankAccountTest, Deposit) {
    BankAccount account("12345", "John Doe", 100.0);
    account.deposit(50.0);
    EXPECT_DOUBLE_EQ(account.get_balance(), 150.0);
}

// Test case: Widthdraw money from the account
TEST(BankAccountTest, Withdraw) {
    BankAccount account("12345", "John Doe", 1000.0);
    account.withdraw(500.0);
    EXPECT_DOUBLE_EQ(account.get_balance(), 500.0);
}

// Test case: Get Account Id
TEST(BankAccountTest, Id) {
    BankAccount account("12345", "John Doe", 1000.0);
    EXPECT_EQ(account.get_account_id(), "12345");
}

// Test case: Get owner name
TEST(BankAccountTest, Name) {
    BankAccount account("12345", "John Doe", 1000.0);
    EXPECT_EQ(account.get_owner_name(), "John Doe");
}