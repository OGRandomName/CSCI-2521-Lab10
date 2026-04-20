/**
 * @file Lab10_kcherry8.cpp
 * @author Kenneth Cherry
 * @date 2026-04-19
 * @brief Banking system demonstrating inheritance, polymorphism, file output,
 *        and daily account balancing. Includes Customer, Account, and
 *        SavingsAccount classes merged into a single submission file.
 */

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <fstream>
#include <iomanip>
using namespace std;

/* ============================================================
   CUSTOMER CLASS
   ============================================================ */

/**
 * @brief Represents a bank customer with an ID and email.
 */
class Customer {
private:
    string customerID;
    string email;

public:
    /**
     * @brief Default constructor.
     */
    Customer();

    /**
     * @brief Constructs a Customer with ID and email.
     * @param id The customer's unique ID.
     * @param emailAddr The customer's email address.
     */
    Customer(const string& id, const string& emailAddr);

    /**
     * @brief Gets the customer's ID.
     * @return The customer ID.
     */
    string getCustomerID() const;

    /**
     * @brief Gets the customer's email.
     * @return The email address.
     */
    string getEmail() const;

    /**
     * @brief Prints customer information.
     */
    void printCustomerInfo() const;
};

Customer::Customer() {
    customerID = "Unknown";
    email = "none@example.com";
}

Customer::Customer(const string& id, const string& emailAddr) {
    customerID = id;
    email = emailAddr;
}

string Customer::getCustomerID() const {
    return customerID;
}

string Customer::getEmail() const {
    return email;
}

void Customer::printCustomerInfo() const {
    cout << "Customer ID: " << customerID << endl;
    cout << "Email: " << email << endl;
}

/* ============================================================
   ACCOUNT CLASS
   ============================================================ */

/**
 * @brief Base class representing a bank account.
 */
class Account {
protected:
    string accountNumber;
    double balance;
    Customer owner;

    vector<double> deposits;
    vector<double> withdrawals;

    /**
     * @brief Prints a summary of daily account activity.
     */
    void printSummary(double previousBalance, double totalDeposits,
                      double totalWithdrawals, double newBalance) const;

public:
    /**
     * @brief Default constructor.
     */
    Account();

    /**
     * @brief Constructs an Account with number, balance, and owner.
     */
    Account(const string& accNum, double initialBalance, const Customer& customer);

    /**
     * @brief Adds a deposit transaction.
     * @return True if valid, false otherwise.
     */
    bool deposit(double amount);

    /**
     * @brief Adds a withdrawal transaction.
     * @return True if valid, false otherwise.
     */
    bool withdraw(double amount);

    /**
     * @brief Performs end-of-day processing.
     */
    virtual void closeOfBusiness();

    /**
     * @brief Gets the current balance.
     */
    double getBalance() const;

    /**
     * @brief Gets the account number.
     */
    string getAccountNumber() const;

    /**
     * @brief Gets the associated customer.
     */
    Customer getCustomer() const;

    /**
     * @brief Prints account information.
     */
    virtual void printAccountInfo() const;

    /**
     * @brief Prints pending transactions.
     */
    void printTransactions() const;
};

Account::Account() {
    accountNumber = "0000000000";
    balance = 0.0;
    owner = Customer();
}

Account::Account(const string& accNum, double initBal, const Customer& cust)
    : accountNumber(accNum), balance(initBal), owner(cust) {}

bool Account::deposit(double amount) {
    if (amount > 0) {
        deposits.push_back(amount);
        return true;
    }
    return false;
}

bool Account::withdraw(double amount) {
    if (amount > 0) {
        withdrawals.push_back(amount);
        return true;
    }
    return false;
}

void Account::closeOfBusiness() {
    double totalDeposits = accumulate(deposits.begin(), deposits.end(), 0.0);
    double totalWithdrawals = accumulate(withdrawals.begin(), withdrawals.end(), 0.0);

    double previousBalance = balance;
    double newBalance = previousBalance + totalDeposits - totalWithdrawals;

    printSummary(previousBalance, totalDeposits, totalWithdrawals, newBalance);

    balance = newBalance;
    deposits.clear();
    withdrawals.clear();
}

void Account::printSummary(double previousBalance, double totalDeposits,
                           double totalWithdrawals, double newBalance) const {
    cout << "\n--- Daily Summary ---\n";
    cout << "Account Number: " << accountNumber << endl;
    cout << "Previous Balance: $" << previousBalance << endl;
    cout << "Total Deposits: $" << totalDeposits << endl;
    cout << "Total Withdrawals: $" << totalWithdrawals << endl;
    cout << "New Balance after COB: $" << newBalance << endl << endl;
}

double Account::getBalance() const {
    return balance;
}

string Account::getAccountNumber() const {
    return accountNumber;
}

Customer Account::getCustomer() const {
    return owner;
}

void Account::printAccountInfo() const {
    cout << "Account Number: " << accountNumber << endl;
    cout << "Current Balance: $" << balance << endl;
    owner.printCustomerInfo();
}

void Account::printTransactions() const {
    cout << "Deposits: ";
    for (double d : deposits) cout << "$" << d << " ";
    cout << "\nWithdrawals: ";
    for (double w : withdrawals) cout << "$" << w << " ";
    cout << endl;
}

/* ============================================================
   SAVINGSACCOUNT CLASS
   ============================================================ */

/**
 * @brief Savings account with daily interest and file output.
 */
class SavingsAccount : public Account {
protected:
    float interest;

public:
    /**
     * @brief Default constructor.
     */
    SavingsAccount();

    /**
     * @brief Constructs a SavingsAccount.
     */
    SavingsAccount(const string& accNum, double initialBalance,
                   float interest, const Customer& customer);

    /**
     * @brief Applies interest and finalizes daily transactions.
     */
    void closeOfBusiness() override;

    /**
     * @brief Prints account information.
     */
    void printAccountInfo() const override;

    /**
     * @brief Saves account details to a text file.
     */
    void Save() const;
};

SavingsAccount::SavingsAccount() : interest(1) {}

SavingsAccount::SavingsAccount(const string& accNum, double initialBalance,
                               float interest, const Customer& customer)
    : Account(accNum, initialBalance, customer) {
    this->interest = interest;
}

void SavingsAccount::closeOfBusiness() {
    double totalDeposits = accumulate(deposits.begin(), deposits.end(), 0.0);
    double totalWithdrawals = accumulate(withdrawals.begin(), withdrawals.end(), 0.0);

    double previousBalance = balance;
    double interestToApply = previousBalance <= 0 ? 0 : interest;
    double newBalance = (previousBalance + totalDeposits - totalWithdrawals) * (1 + interestToApply);

    printSummary(previousBalance, totalDeposits, totalWithdrawals, newBalance);

    balance = newBalance;
    deposits.clear();
    withdrawals.clear();
}

void SavingsAccount::printAccountInfo() const {
    cout << "Account Number: " << accountNumber << endl;
    cout << "Current Balance: $" << balance << endl;
    cout << "Interest Rate: " << interest * 100 << "%" << endl;
    owner.printCustomerInfo();
}

void SavingsAccount::Save() const {
    ofstream outFile("account_summary.txt");

    outFile << fixed << setprecision(2);
    outFile << "Account Number: " << accountNumber << "\n\n";
    outFile << "Current Balance: $" << balance << "\n\n";
    outFile << "Daily Interest Rate: " << interest << "\n";

    outFile.close();
}

/* ============================================================
   MENU FUNCTIONS
   ============================================================ */

void printMenu() {
    cout << "\nCSCC Bank and Trust\n";
    cout << "1. Deposit\n";
    cout << "2. Withdraw\n";
    cout << "3. Close of Business\n";
    cout << "4. Save Account to File\n";
    cout << "5. Quit\n";
}

int getMenuOption() {
    int choice;
    do {
        printMenu();
        cin >> choice;
        if (choice < 1 || choice > 5)
            cout << "Invalid option. Try again.\n";
    } while (choice < 1 || choice > 5);
    return choice;
}

double getAmount(const string& label) {
    double amt;
    do {
        cout << "Enter " << label << " amount: $";
        cin >> amt;
        if (amt <= 0) cout << "Amount must be greater than $0.\n";
    } while (amt <= 0);
    return amt;
}

/* ============================================================
   MAIN FUNCTION
   ============================================================ */

/**
 * @brief Entry point for the banking application.
 * @return 0 on success.
 */
int main() {
    Customer customer("0123456789", "customer@cscc.edu");
    Account* account = new SavingsAccount("5423678409", 100.00, .05, customer);

    int choice = getMenuOption();
    while (choice != 5) {
        switch (choice) {
        case 1:
            account->deposit(getAmount("deposit"));
            break;
        case 2:
            account->withdraw(getAmount("withdrawal"));
            break;
        case 3:
            account->closeOfBusiness();
            break;
        case 4:
            static_cast<SavingsAccount*>(account)->Save();
            cout << "Saved to account_summary.txt\n";
            break;
        }
        choice = getMenuOption();
    }

    delete account;
    return 0;
}
