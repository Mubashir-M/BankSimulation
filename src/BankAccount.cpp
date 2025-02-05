#include "BankAccount.h"
#include "TransactionTypes.h"

#include <openssl/evp.h>
#include <openssl/sha.h>

#include <stdexcept>
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>




BankAccount::BankAccount(const std::string& id, const std::string& name, double initial_balance, const std::string& hashed_password)
    : account_id_(id), owner_name_(name),balance_(initial_balance), hashed_password_(hashed_password){
        if(initial_balance <0){
            throw std::invalid_argument("Initial balance cannot be  negative.");
        }
}

double BankAccount::get_balance() const {
    return balance_;
}

const std::string& BankAccount::get_account_id() const {
    return account_id_;
};

const std::string& BankAccount::get_owner_name() const {
    return owner_name_;
};


bool BankAccount::check_password(const std::string& password) const{
    return hashed_password_ == hash_password(password);
}

std::string BankAccount::generate_transaction_id(){
    return "TX-" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
}

std::string BankAccount::hash_password(const std::string& password) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    // Initialize the SHA256 context using the new API
    EVP_MD_CTX* sha256_ctx = EVP_MD_CTX_new();
    if (sha256_ctx == nullptr) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    // Initialize and update the context with the password
    if (EVP_DigestInit_ex(sha256_ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(sha256_ctx);
        throw std::runtime_error("Failed to initialize SHA256");
    }

    if (EVP_DigestUpdate(sha256_ctx, password.c_str(), password.length()) != 1) {
        EVP_MD_CTX_free(sha256_ctx);
        throw std::runtime_error("Failed to update SHA256 hash");
    }

    // Finalize the hash
    if (EVP_DigestFinal_ex(sha256_ctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(sha256_ctx);
        throw std::runtime_error("Failed to finalize SHA256 hash");
    }

    EVP_MD_CTX_free(sha256_ctx);

    // Convert the hash to a hex string
    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; i++) {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)hash[i];
    }

    return ss.str();
}