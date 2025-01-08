#include "gtest/gtest.h"
#include "../src/Bank.h"
#include "../src/BankAccount.h"


class BankTest : public ::testing::Test {
protected:
    Bank bank;

    void SetUp() override {
        std::string account_id1 = "12345";
        std::string name1 = "John Doe";
        double initial_balance1 = 1000.0;
        bank.create_account(account_id1, name1, initial_balance1);

        std::string account_id2 = "67890";
        std::string name2 = "Jane Doe";
        double initial_balance2 = 1500.0;
        bank.create_account(account_id2, name2, initial_balance2);

    }

};

// Test case for creating a new account successfully
TEST_F(BankTest, CreateAccount_Success) {
    std::string account_id = "1111";
    std::string name = "New Account";
    double initial_balance = 1000.0;

    // Create the account
    EXPECT_NO_THROW(bank.create_account(account_id, name, initial_balance));


    std::vector<std::string> accounts = bank.list_accounts();
    EXPECT_EQ(accounts.size(), 3);
    auto it = bank.get_accounts().find(account_id);
    EXPECT_TRUE(it != bank.get_accounts().end());
}

// Test case for trying to create an account with an existing ID (should throw an exception)
TEST_F(BankTest, CreateAccount_AlreadyExists) {
    std::string account_id = "12345";
    std::string name = "John Doe";
    double initial_balance = 1000.0;

    // Try creating the same account again (should throw exception)
    EXPECT_THROW(bank.create_account(account_id, name, initial_balance),
                 std::invalid_argument);
}

TEST_F(BankTest, WithdrawFromAccount_Success) {
    std::string account_id = "12345";
    double withdrawal_amount = 500.0;

    BankAccount& account = bank.get_account(account_id);
    EXPECT_EQ(account.get_balance(), 1000.0);

    // Withdraw money from the account
    EXPECT_NO_THROW(bank.withdraw_from_account(account_id, withdrawal_amount));

    // Ensure the balance is updated correctly
    EXPECT_EQ(account.get_balance(), 500.0);
}