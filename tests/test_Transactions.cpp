#include "gtest/gtest.h"
#include "Transactions.h"
#include "BankAccount.h"

class TransactionsTest : public ::testing::Test {
protected:
    BankAccount* test_account;
    Transactions* test_transaction;

    void SetUp() override {
        std::string hashed_password = BankAccount::hash_password("55550");
        test_account = new BankAccount("12345", "Test User", 500.0, hashed_password);

        std::string transaction_id = "TX-1001";
        TransactionTypes type = TransactionTypes::DEPOSIT;
        double amount = 100.0;
        std::string account_id = "12345";
        std::string recipient_account_id = "12345";
        std::string description = "Initial deposit";

        test_transaction = new Transactions(transaction_id, type, amount, account_id, recipient_account_id, description);
    }

    void TearDown() override {
        delete test_account;
        delete test_transaction;
    }
};

TEST_F(TransactionsTest, TestTransactionCreated) {
    EXPECT_EQ(test_transaction->get_description(), "Initial deposit");
    EXPECT_EQ(test_transaction->get_amount(), 100.0);
}


TEST_F(TransactionsTest, GetTransactionId) {
    EXPECT_EQ(test_transaction->get_id(), "TX-1001");
}

TEST_F(TransactionsTest, GetTransactionAccountId) {
    EXPECT_EQ(test_transaction->get_account_id(), "12345");
}

TEST_F(TransactionsTest, GetRecipientAccountId) {
    EXPECT_EQ(test_transaction->get_recipient_account_id(), "12345");
}

TEST_F(TransactionsTest, GetTransactionDescription) {
    EXPECT_EQ(test_transaction->get_description(), "Initial deposit");
}

TEST_F(TransactionsTest, GetTransactionType) {
    EXPECT_EQ(test_transaction->get_transaction_type(), TransactionTypes::DEPOSIT);
}

TEST_F(TransactionsTest, GetDifferentTransactionTypes) {
    Transactions deposit("TX-1002", TransactionTypes::DEPOSIT, 200.0, "12345", "", "Deposit into account");
    EXPECT_EQ(deposit.get_transaction_type(), TransactionTypes::DEPOSIT);
    
    Transactions withdrawal("TX-1003", TransactionTypes::WITHDRAWAL, 50.0, "12345", "", "ATM withdrawal");
    EXPECT_EQ(withdrawal.get_transaction_type(), TransactionTypes::WITHDRAWAL);
}

TEST_F(TransactionsTest, ZeroTransactionAmount) {
    Transactions zero_amount_transaction("TX-1004", TransactionTypes::TRANSFER, 0.0, "12345", "67890", "Zero amount transfer");
    EXPECT_DOUBLE_EQ(zero_amount_transaction.get_amount(), 0.0);
}

TEST_F(TransactionsTest, EmptyDescription) {
    Transactions empty_description_transaction("TX-1005", TransactionTypes::TRANSFER, 50.0, "12345", "67890", "");
    EXPECT_EQ(empty_description_transaction.get_description(), "");
}

TEST_F(TransactionsTest, InvalidAccountId) {
    Transactions invalid_account_id_transaction("TX-1006", TransactionTypes::TRANSFER, 100.0, "", "67890", "Invalid account ID");
    EXPECT_EQ(invalid_account_id_transaction.get_account_id(), "");
}
