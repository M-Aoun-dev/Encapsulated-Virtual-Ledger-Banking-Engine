// |---------------- Encapsulated Virtual Ledger Banking Engine ----------------|

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// |---------------------- Constants ----------------------|

const char ACCOUNT_FILE[] = "accounts.txt";
const char TRANSACTION_FILE[] = "transactions.txt";

const int MIN_ACCOUNT_NUMBER = 1000;
const int MAX_ACCOUNT_NUMBER = 999999;

// |---------------------- Structures ----------------------|
struct Transaction {
    int accountNumber;
    string type;
    double amount;
    double balanceAfter;
    string date;
    string description;
};

// |---------------------- Helper Functions ----------------------|
void printLine(char ch = '-', int len = 75) {
    for (int i = 0; i < len; i++) {
        cout << ch;
    }
    cout << endl;
}

void printHeader(const string& title) {
    cout << endl;
    cout << "|-------------------------------------------------------------------------------| " << endl;
    cout << "  " << title << endl;
    cout << "|-------------------------------------------------------------------------------| " << endl;
}

void pauseScreen() {
    cout << endl;
    cout << "Press Enter to continue...";
    cin.get();
}

void clearInputLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// geting INT input
int getIntInput(const char message[], int min, int max) {
    int value;
    while (true) {
        cout << message;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        clearInputLine();

        cout << "Invalid Input! Enter a value between " << min << " and " << max << "." << endl;
    }
}

// getting DOUBLE input
double getDoubleInput(const char message[], double min, double max) {
    double value;
    while (true) {
        cout << message;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        clearInputLine();

        cout << "Invalid Amount! Please try again." << endl;
    }
}

// getting STRING input
string getTextInput(const char message[]) {
    string value;
    while (true) {
        cout << message;
        getline(cin, value);
        if (!value.empty()) {
            return value;
        }

        cout << "Input cannot be empty." << endl;
    }
}

string getDateTime() {
    return "Transaction";
}

// Checking Validity of Account Number
bool isValidAccountNumber(int accountNumber) {
    return accountNumber >= MIN_ACCOUNT_NUMBER &&
        accountNumber <= MAX_ACCOUNT_NUMBER;
}

// Checking Validity of Amount
bool isValidAmount(double amount) {
    return amount > 0;
}

void printMoney(double amount) {
    cout << fixed << setprecision(2) << amount;
}

// |---------------------- String Parsing Helper Function ----------------------|

string getField(const string& line, int fieldNumber) {
    size_t start = 0;
    size_t end = 0;

    for (int i = 0; i < fieldNumber; i++) {
        end = line.find('|', start);
        if (end == string::npos) {
            return "";
        }

        start = end + 1;
    }

    end = line.find('|', start);

    if (end == string::npos) {
        return line.substr(start);
    }
    return line.substr(start, end - start);
}

// |---------------------- Base Class ----------------------|
class BankAccount {
private:
    int accountNumber;
    string accountHolder;
    double balance;
protected:
    double getProtectedBalance() const {
        return balance;
    }

    void setProtectedBalance(double newBalance) {
        balance = newBalance;
    }

public:
    BankAccount(
        int number,
        const string& holder,
        double initialBalance
    ) {
        accountNumber = number;
        accountHolder = holder;
        balance = initialBalance;
    }

    virtual ~BankAccount() {}

    int getAccountNumber() const {
        return accountNumber;
    }

    string getAccountHolder() const {
        return accountHolder;
    }

    double getBalance() const {
        return balance;
    }

    bool deposit(double amount) {
        if (!isValidAmount(amount)) {
            return false;
        }

        balance += amount;
        return true;
    }

    bool withdraw(double amount) {
        if (!isValidAmount(amount)) {
            return false;
        }
        if (amount > balance) {
            return false;
        }

        balance -= amount;
        return true;
    }

    virtual double calculateAccountValue() const = 0;

    virtual string getAccountType() const = 0;

    virtual double getSpecialValue() const = 0;

    virtual void displayCalculation() const = 0;

    virtual void displayAccount() const {
        cout << left
            << setw(18) << accountNumber
            << setw(25) << accountHolder
            << setw(18) << getAccountType()
            << right
            << setw(14) << fixed << setprecision(2)
            << balance
            << endl;
    }
};

// |---------------------- Savings Account ----------------------|
class SavingsAccount : public BankAccount {
private:
    double annualInterestRate;
public:
    SavingsAccount(
        int number,
        const string& holder,
        double initialBalance,
        double interestRate
    )
        : BankAccount(number, holder, initialBalance) {
        annualInterestRate = interestRate;
    }

    double getInterestRate() const {
        return annualInterestRate;
    }

    void setInterestRate(double rate) {
        if (rate >= 0 && rate <= 100) {
            annualInterestRate = rate;
        }
    }

    string getAccountType() const override {
        return "Savings";
    }

    double getSpecialValue() const override {
        return annualInterestRate;
    }

    double calculateAccountValue() const override {
        double interest =
            getBalance() * annualInterestRate / 100.0;

        return getBalance() + interest;
    }

    void displayCalculation() const override {
        double interest =
            getBalance() * annualInterestRate / 100.0;

        cout << "Account Type       : Savings" << endl;
        cout << "Annual Interest    : " << annualInterestRate << "%" << endl;
        cout << "Estimated Interest : ";
        printMoney(interest);
        cout << endl;
        cout << "Estimated Value    : ";
        printMoney(calculateAccountValue());
        cout << endl;
    }
};

//  Checking Account 
class CheckingAccount : public BankAccount {
private:
    double monthlyFee;
public:
    CheckingAccount(
        int number,
        const string& holder,
        double initialBalance,
        double fee
    )
        : BankAccount(number, holder, initialBalance) {
        monthlyFee = fee;
    }

    double getMonthlyFee() const {
        return monthlyFee;
    }

    void setMonthlyFee(double fee) {
        if (fee >= 0) {
            monthlyFee = fee;
        }
    }

    string getAccountType() const override {
        return "Checking";
    }

    double getSpecialValue() const override {
        return monthlyFee;
    }

    double calculateAccountValue() const override {
        double result = getBalance() - monthlyFee;

        if (result < 0) {
            result = 0;
        }

        return result;
    }

    void displayCalculation() const override {
        cout << "Account Type       : Checking" << endl;
        cout << "Monthly Fee        : ";
        printMoney(monthlyFee);
        cout << endl;
        cout << "Estimated Value    : ";
        printMoney(calculateAccountValue());
        cout << endl;
    }
};

// |---------------------- Global Account Storage ----------------------|
vector<BankAccount*> accounts;

//  Find Account 
int findAccountIndex(int accountNumber) {
    for (int i = 0; i < static_cast<int>(accounts.size()); i++) {
        if (accounts[i]->getAccountNumber() == accountNumber) {
            return i;
        }
    }
    return -1;
}

bool accountExists(int accountNumber) {
    return findAccountIndex(accountNumber) != -1;
}

BankAccount* getAccount(int accountNumber) {
    int index = findAccountIndex(accountNumber);
    if (index == -1) {
        return nullptr;
    }

    return accounts[index];
}

//  Transaction File Handling 

bool saveTransaction(
    int accountNumber,
    const string& type,
    double amount,
    double balanceAfter,
    const string& description
) {
    ofstream file(TRANSACTION_FILE, ios::app);

    if (!file.is_open()) {
        cout << "Error: Cannot open transaction file." << endl;
        return false;
    }

    file << accountNumber << '|'
        << type << '|'
        << fixed << setprecision(2)
        << amount << '|'
        << balanceAfter << '|'
        << getDateTime() << '|'
        << description
        << endl;

    file.close();

    return !file.fail();
}

//  Account File Handling
bool saveAllAccounts() {
    ofstream file(ACCOUNT_FILE);

    if (!file.is_open()) {
        cout << "Unable to open account file." << endl;
        return false;
    }

    for (int i = 0; i < static_cast<int>(accounts.size()); i++) {
        BankAccount* account = accounts[i];

        file << account->getAccountType() << '|'
            << account->getAccountNumber() << '|'
            << account->getAccountHolder() << '|'
            << fixed << setprecision(2)
            << account->getBalance() << '|'
            << account->getSpecialValue()
            << endl;
    }
    file.close();

    if (file.fail()) {
        cout << "Error writing account data." << endl;
        return false;
    }
    return true;
}

void loadAccounts() {
    ifstream file(ACCOUNT_FILE);

    if (!file.is_open()) {
        return;
    }

    string line;

    while (getline(file, line)) {
        string type = getField(line, 0);
        string numberText = getField(line, 1);
        string holder = getField(line, 2);
        string balanceText = getField(line, 3);
        string specialText = getField(line, 4);

        if (numberText.empty() || balanceText.empty() || specialText.empty()) {
            continue;
        }

        int number = 0;
        double balance = 0;
        double special = 0;

        try {
            number = stoi(numberText);
            balance = stod(balanceText);
            special = stod(specialText);
        }
        catch (...) {
            continue;
        }

        if (!isValidAccountNumber(number) || balance < 0 || special < 0 || accountExists(number)) {
            continue;
        }

        BankAccount* account = nullptr;

        if (type == "Savings" && special <= 100) {
            account = new SavingsAccount(
                number,
                holder,
                balance,
                special
            );
        }
        else if (type == "Checking") {
            account = new CheckingAccount(
                number,
                holder,
                balance,
                special
            );
        }

        if (account != nullptr) {
            accounts.push_back(account);
        }
    }

    file.close();
}

//  Create Account 

void createAccount() {
    printHeader("CREATE BANK ACCOUNT");

    int type = getIntInput(
        "Select Account Type (1-Savings, 2-Checking): ",
        1, 2
    );

    int number;

    while (true) {
        number = getIntInput(
            "Enter Account Number (1000-999999): ",
            MIN_ACCOUNT_NUMBER,
            MAX_ACCOUNT_NUMBER
        );

        if (!accountExists(number)) {
            break;
        }
        cout << "Account number already exists." << endl;
    }

    string holder = getTextInput(
        "Enter Account Holder Name: "
    );

    double initialBalance = getDoubleInput(
        "Enter Initial Deposit: ",
        0, 1000000000
    );

    BankAccount* account = nullptr;

    if (type == 1) {
        double rate = getDoubleInput(
            "Enter Annual Interest Rate (0-100): ",
            0, 100
        );

        account = new SavingsAccount(
            number,
            holder,
            initialBalance,
            rate
        );
    }
    else {
        double fee = getDoubleInput(
            "Enter Monthly Account Fee: ",
            0,
            1000000
        );

        account = new CheckingAccount(
            number,
            holder,
            initialBalance,
            fee
        );
    }

    accounts.push_back(account);

    if (!saveAllAccounts()) {
        cout << "Warning: Account could not be saved."
            << endl;
    }

    if (initialBalance > 0) {
        saveTransaction(
            number,
            "OpeningDeposit",
            initialBalance,
            initialBalance,
            "Initial account deposit"
        );
    }

    cout << endl;
    cout << "Account Created Successfully!" << endl;

    pauseScreen();
}

//  Deposit Money 

void depositMoney() {
    printHeader("DEPOSIT MONEY");

    int number = getIntInput(
        "Enter Account Number: ",
        MIN_ACCOUNT_NUMBER,
        MAX_ACCOUNT_NUMBER
    );

    BankAccount* account = getAccount(number);

    if (account == nullptr) {
        cout << "Account Not Found." << endl;
        pauseScreen();
        return;
    }

    double amount = getDoubleInput(
        "Enter Deposit Amount: ",
        0.01, 1000000000
    );

    double oldBalance = account->getBalance();

    if (!account->deposit(amount)) {
        cout << "Deposit Failed." << endl;
        pauseScreen();
        return;
    }

    if (!saveAllAccounts()) {
        account->withdraw(amount);

        cout << "Deposit could not be saved." << endl;
        pauseScreen();
        return;
    }

    saveTransaction(
        number,
        "Deposit",
        amount,
        account->getBalance(),
        "Cash deposit"
    );

    cout << endl;
    cout << "Deposit Successful!" << endl;

    cout << "Previous Balance: ";
    printMoney(oldBalance);
    cout << endl;

    cout << "New Balance     : ";
    printMoney(account->getBalance());
    cout << endl;

    pauseScreen();
}

//  Withdraw Money 

void withdrawMoney() {
    printHeader("WITHDRAW MONEY");

    int number = getIntInput(
        "Enter Account Number: ",
        MIN_ACCOUNT_NUMBER,
        MAX_ACCOUNT_NUMBER
    );

    BankAccount* account = getAccount(number);

    if (account == nullptr) {
        cout << "Account Not Found." << endl;
        pauseScreen();
        return;
    }

    double amount = getDoubleInput(
        "Enter Withdrawal Amount: ",
        0.01, 1000000000
    );

    double oldBalance = account->getBalance();

    if (!account->withdraw(amount)) {
        cout << "Withdrawal Failed." << endl;
        cout << "Insufficient Balance or Invalid Amount." << endl;

        pauseScreen();
        return;
    }

    if (!saveAllAccounts()) {
        account->deposit(amount);

        cout << "Withdrawal could not be saved." << endl;
        pauseScreen();
        return;
    }

    saveTransaction(
        number,
        "Withdrawal",
        amount,
        account->getBalance(),
        "Cash withdrawal"
    );

    cout << endl;
    cout << "Withdrawal Successful!" << endl;

    cout << "Previous Balance: ";
    printMoney(oldBalance);
    cout << endl;

    cout << "New Balance     : ";
    printMoney(account->getBalance());
    cout << endl;

    pauseScreen();
}

//  Transfer Funds 

void transferFunds() {
    printHeader("TRANSFER FUNDS");

    int senderNumber = getIntInput(
        "Enter Sender Account Number: ",
        MIN_ACCOUNT_NUMBER,
        MAX_ACCOUNT_NUMBER
    );

    int receiverNumber = getIntInput(
        "Enter Receiver Account Number: ",
        MIN_ACCOUNT_NUMBER,
        MAX_ACCOUNT_NUMBER
    );

    if (senderNumber == receiverNumber) {
        cout << "Sender and receiver must be different." << endl;
        pauseScreen();
        return;
    }

    BankAccount* sender = getAccount(senderNumber);
    BankAccount* receiver = getAccount(receiverNumber);

    if (sender == nullptr || receiver == nullptr) {
        cout << "Sender or Receiver Account Not Found." << endl;
        pauseScreen();
        return;
    }

    double amount = getDoubleInput(
        "Enter Transfer Amount: ",
        0.01, 1000000000
    );

    if (amount > sender->getBalance()) {
        cout << "Insufficient Balance." << endl;
        pauseScreen();
        return;
    }

    double senderOldBalance = sender->getBalance();
    double receiverOldBalance = receiver->getBalance();

    if (!sender->withdraw(amount)) {
        cout << "Transfer Failed." << endl;
        pauseScreen();
        return;
    }

    if (!receiver->deposit(amount)) {
        sender->deposit(amount);
        cout << "Transfer Failed." << endl;
        pauseScreen();
        return;
    }

    if (!saveAllAccounts()) {
        receiver->withdraw(amount);
        sender->deposit(amount);

        cout << "Transfer could not be saved." << endl;
        pauseScreen();
        return;
    }

    saveTransaction(
        senderNumber,
        "TransferOut",
        amount,
        sender->getBalance(),
        "Transfer to account " + to_string(receiverNumber)
    );

    saveTransaction(
        receiverNumber,
        "TransferIn",
        amount,
        receiver->getBalance(),
        "Transfer from account " + to_string(senderNumber)
    );

    cout << endl;
    cout << "Transfer Successful!" << endl;

    cout << "Sender Previous Balance: ";
    printMoney(senderOldBalance);
    cout << endl;

    cout << "Sender New Balance     : ";
    printMoney(sender->getBalance());
    cout << endl;

    cout << "Receiver Previous Balance: ";
    printMoney(receiverOldBalance);
    cout << endl;

    cout << "Receiver New Balance     : ";
    printMoney(receiver->getBalance());
    cout << endl;

    pauseScreen();
}

//  Delete Account 
void deleteAccount() {
    printHeader("DELETE BANK ACCOUNT");

    int number = getIntInput(
        "Enter Account Number to Delete: ",
        MIN_ACCOUNT_NUMBER,
        MAX_ACCOUNT_NUMBER
    );

    int index = findAccountIndex(number);

    if (index == -1) {
        cout << "Account Not Found." << endl;
        pauseScreen();
        return;
    }

    BankAccount* account = accounts[index];
    double balance = account->getBalance();

    cout << endl;
    cout << "Account Number : " << account->getAccountNumber() << endl;
    cout << "Account Holder : " << account->getAccountHolder() << endl;
    cout << "Account Type   : " << account->getAccountType() << endl;
    cout << "Current Balance: ";
    printMoney(balance);
    cout << endl;

    BankAccount* receiver = nullptr;
    BankAccount* newAccount = nullptr;
    int option = 0;
    int receiverNumber = 0;
    double withdrawnAmount = 0;

    if (balance > 0) {
        cout << endl;
        cout << "[1] Transfer to Existing Account" << endl;
        cout << "[2] Create New Account and Transfer" << endl;
        cout << "[3] Withdraw Money" << endl;
        cout << "[0] Cancel Deletion" << endl;

        option = getIntInput("Choose an option: ", 0, 3);

        if (option == 0) {
            cout << "Account deletion cancelled." << endl;
            pauseScreen();
            return;
        }

        if (option == 1) {
            receiverNumber = getIntInput(
                "Enter Receiver Account Number: ",
                MIN_ACCOUNT_NUMBER,
                MAX_ACCOUNT_NUMBER
            );

            if (receiverNumber == number) {
                cout << "Cannot transfer to the same account." << endl;
                pauseScreen();
                return;
            }

            receiver = getAccount(receiverNumber);

            if (receiver == nullptr) {
                cout << "Receiver Account Not Found." << endl;
                pauseScreen();
                return;
            }

            if (!receiver->deposit(balance)) {
                cout << "Transfer Failed." << endl;
                pauseScreen();
                return;
            }

            if (!account->withdraw(balance)) {
                receiver->withdraw(balance);
                cout << "Transfer Failed." << endl;
                pauseScreen();
                return;
            }
        }
        else if (option == 2) {
            int type = getIntInput(
                "Select Account Type (1-Savings, 2-Checking): ",
                1,
                2
            );

            int newNumber;

            while (true) {
                newNumber = getIntInput(
                    "Enter New Account Number (1000-999999): ",
                    MIN_ACCOUNT_NUMBER,
                    MAX_ACCOUNT_NUMBER
                );

                if (!accountExists(newNumber)) {
                    break;
                }

                cout << "Account number already exists." << endl;
            }

            string holder = getTextInput(
                "Enter New Account Holder Name: "
            );

            if (type == 1) {
                double rate = getDoubleInput(
                    "Enter Annual Interest Rate (0-100): ",
                    0,
                    100
                );

                newAccount = new SavingsAccount(
                    newNumber,
                    holder,
                    balance,
                    rate
                );
            }
            else {
                double fee = getDoubleInput(
                    "Enter Monthly Account Fee: ",
                    0,
                    1000000
                );

                newAccount = new CheckingAccount(
                    newNumber,
                    holder,
                    balance,
                    fee
                );
            }
        }
        else if (option == 3) {
            cout << endl;
            cout << "Available Balance: ";
            printMoney(balance);
            cout << endl;

            withdrawnAmount = getDoubleInput(
                "Enter Amount to Withdraw: ",
                0.01,
                balance
            );

            if (!account->withdraw(withdrawnAmount)) {
                cout << "Withdrawal Failed." << endl;
                pauseScreen();
                return;
            }

            balance = account->getBalance();

            saveTransaction(
                number,
                "AccountClosureWithdrawal",
                withdrawnAmount,
                balance,
                "Money withdrawn before account deletion"
            );

            cout << "Withdrawal Successful!" << endl;
            cout << "Withdrawn Amount: ";
            printMoney(withdrawnAmount);
            cout << endl;
            cout << "Remaining Balance: ";
            printMoney(balance);
            cout << endl;

            if (balance > 0) {
                cout << endl;
                cout << "[1] Transfer Remaining Balance to Existing Account" << endl;
                cout << "[2] Create New Account and Transfer Remaining Balance" << endl;
                cout << "[0] Cancel Deletion" << endl;

                option = getIntInput("Choose an option: ", 0, 2);

                if (option == 0) {
                    account->deposit(withdrawnAmount);

                    saveTransaction(
                        number,
                        "WithdrawalReversal",
                        withdrawnAmount,
                        account->getBalance(),
                        "Withdrawal reversed because account deletion was cancelled"
                    );

                    cout << "Account deletion cancelled." << endl;
                    pauseScreen();
                    return;
                }

                balance = account->getBalance();

                if (option == 1) {
                    receiverNumber = getIntInput(
                        "Enter Receiver Account Number: ",
                        MIN_ACCOUNT_NUMBER,
                        MAX_ACCOUNT_NUMBER
                    );

                    if (receiverNumber == number) {
                        account->deposit(withdrawnAmount);
                        cout << "Cannot transfer to the same account." << endl;
                        pauseScreen();
                        return;
                    }

                    receiver = getAccount(receiverNumber);

                    if (receiver == nullptr) {
                        account->deposit(withdrawnAmount);
                        cout << "Receiver Account Not Found." << endl;
                        pauseScreen();
                        return;
                    }

                    if (!receiver->deposit(balance)) {
                        account->deposit(withdrawnAmount);
                        cout << "Transfer Failed." << endl;
                        pauseScreen();
                        return;
                    }

                    if (!account->withdraw(balance)) {
                        receiver->withdraw(balance);
                        account->deposit(withdrawnAmount);
                        cout << "Transfer Failed." << endl;
                        pauseScreen();
                        return;
                    }
                }
                else if (option == 2) {
                    int type = getIntInput(
                        "Select Account Type (1-Savings, 2-Checking): ",
                        1,
                        2
                    );

                    int newNumber;

                    while (true) {
                        newNumber = getIntInput(
                            "Enter New Account Number (1000-999999): ",
                            MIN_ACCOUNT_NUMBER,
                            MAX_ACCOUNT_NUMBER
                        );

                        if (!accountExists(newNumber)) {
                            break;
                        }

                        cout << "Account number already exists." << endl;
                    }

                    string holder = getTextInput(
                        "Enter New Account Holder Name: "
                    );

                    if (type == 1) {
                        double rate = getDoubleInput(
                            "Enter Annual Interest Rate (0-100): ",
                            0,
                            100
                        );

                        newAccount = new SavingsAccount(
                            newNumber,
                            holder,
                            balance,
                            rate
                        );
                    }
                    else {
                        double fee = getDoubleInput(
                            "Enter Monthly Account Fee: ",
                            0,
                            1000000
                        );

                        newAccount = new CheckingAccount(
                            newNumber,
                            holder,
                            balance,
                            fee
                        );
                    }
                }
            }
        }
    }

    accounts.erase(accounts.begin() + index);

    if (newAccount != nullptr) {
        accounts.push_back(newAccount);
    }

    if (!saveAllAccounts()) {
        if (newAccount != nullptr) {
            accounts.pop_back();
            delete newAccount;
        }

        accounts.insert(accounts.begin() + index, account);

        if (receiver != nullptr) {
            receiver->withdraw(balance);
            account->deposit(balance);
        }

        saveAllAccounts();

        cout << "Account deletion failed because data could not be saved." << endl;
        pauseScreen();
        return;
    }

    if (receiver != nullptr) {
        saveTransaction(
            number,
            "AccountClosureTransferOut",
            balance,
            0,
            "Balance transferred to account " + to_string(receiverNumber)
        );

        saveTransaction(
            receiverNumber,
            "AccountClosureTransferIn",
            balance,
            receiver->getBalance(),
            "Balance received from account " + to_string(number)
        );
    }
    else if (newAccount != nullptr) {
        saveTransaction(
            number,
            "AccountClosureTransferOut",
            balance,
            0,
            "Balance transferred to new account " +
            to_string(newAccount->getAccountNumber())
        );

        saveTransaction(
            newAccount->getAccountNumber(),
            "AccountOpeningTransferIn",
            balance,
            balance,
            "Balance received from closed account " + to_string(number)
        );
    }

    delete account;

    cout << endl;
    cout << "Account Deleted Successfully!" << endl;

    if (option == 3) {
        cout << "Money withdrawn before account closure: ";
        printMoney(withdrawnAmount);
        cout << endl;
    }

    if (receiver != nullptr) {
        cout << "Remaining balance transferred to account "
            << receiverNumber << "." << endl;
    }
    else if (newAccount != nullptr) {
        cout << "New account created successfully." << endl;
        cout << "Remaining balance transferred to account "
            << newAccount->getAccountNumber() << "." << endl;
    }

    pauseScreen();
}


//  Search Account 
void searchAccount() {
    printHeader("SEARCH ACCOUNT");

    int number = getIntInput(
        "Enter Account Number: ",
        MIN_ACCOUNT_NUMBER,
        MAX_ACCOUNT_NUMBER
    );

    BankAccount* account = getAccount(number);

    if (account == nullptr) {
        cout << "Account Not Found." << endl;
        pauseScreen();
        return;
    }

    cout << endl;
    cout << "Account Number : " << account->getAccountNumber() << endl;
    cout << "Account Holder : " << account->getAccountHolder() << endl;
    cout << "Account Type   : " << account->getAccountType() << endl;
    cout << "Current Balance: ";
    printMoney(account->getBalance());
    cout << endl;
    account->displayCalculation();

    pauseScreen();
}

//  Display All Accounts 
void displayAllAccounts() {
    printHeader("ALL BANK ACCOUNTS");

    if (accounts.empty()) {
        cout << "No Accounts Found." << endl;
        pauseScreen();
        return;
    }

    cout << left
        << setw(18) << "Account Number"
        << setw(25) << "Account Holder"
        << setw(18) << "Account Type"
        << right
        << setw(14) << "Balance"
        << endl;

    printLine('-', 75);

    for (int i = 0; i < static_cast<int>(accounts.size()); i++) {
        accounts[i]->displayAccount();
    }

    printLine('-', 75);

    cout << "Total Accounts: " << accounts.size() << endl;

    pauseScreen();
}

//  Account Calculation Report 

void accountCalculationReport() {
    printHeader("VIRTUAL ACCOUNT CALCULATION REPORT");

    if (accounts.empty()) {
        cout << "No Accounts Found." << endl;
        pauseScreen();
        return;
    }

    for (int i = 0; i < static_cast<int>(accounts.size()); i++) {
        cout << endl;
        printLine('-', 50);
        cout << "Account Number: " << accounts[i]->getAccountNumber() << endl;
        cout << "Holder: " << accounts[i]->getAccountHolder() << endl;
        accounts[i]->displayCalculation();
    }
    pauseScreen();
}

//  Transaction History 

void viewTransactions() {
    printHeader("TRANSACTION HISTORY");

    int number = getIntInput(
        "Enter Account Number: ",
        MIN_ACCOUNT_NUMBER,
        MAX_ACCOUNT_NUMBER
    );

    if (!accountExists(number)) {
        cout << "Account Not Found." << endl;
        pauseScreen();
        return;
    }

    ifstream file(TRANSACTION_FILE);

    if (!file.is_open()) {
        cout << "No Transaction History Available." << endl;
        pauseScreen();
        return;
    }

    string line;
    bool found = false;

    cout << left
        << setw(18) << "Type"
        << setw(14) << "Amount"
        << setw(16) << "Balance"
        << setw(27) << "Description"
        << endl;

    printLine('-', 75);

    while (getline(file, line)) {
        string accountText = getField(line, 0);
        string type = getField(line, 1);
        string amountText = getField(line, 2);
        string balanceText = getField(line, 3);
        string date = getField(line, 4);
        string description = getField(line, 5);

        int transactionAccount;

        try {
            transactionAccount = stoi(accountText);
        }
        catch (...) {
            continue;
        }

        if (transactionAccount == number) {
            found = true;
            cout << left
                << setw(18) << type
                << setw(14) << amountText
                << setw(16) << balanceText
                << setw(27) << description.substr(0, 26)
                << endl;
        }
    }

    file.close();

    if (!found) {
        cout << "No Transactions Found." << endl;
    }

    pauseScreen();
}

// |---------------------- Balance Summary ----------------------|

void balanceSummary() {
    printHeader("BALANCE SUMMARY");

    double totalBalance = 0;
    double savingsBalance = 0;
    double checkingBalance = 0;

    int savingsCount = 0;
    int checkingCount = 0;

    for (int i = 0; i < static_cast<int>(accounts.size()); i++) {
        BankAccount* account = accounts[i];

        totalBalance += account->getBalance();

        if (account->getAccountType() == "Savings") {
            savingsBalance += account->getBalance();
            savingsCount++;
        }
        else if (account->getAccountType() == "Checking") {
            checkingBalance += account->getBalance();
            checkingCount++;
        }
    }

    cout << fixed << setprecision(2);
    cout << "Total Accounts       : " << accounts.size() << endl;
    cout << "Savings Accounts     : " << savingsCount << endl;
    cout << "Checking Accounts    : " << checkingCount << endl;
    cout << "Savings Balance      : " << savingsBalance << endl;
    cout << "Checking Balance     : " << checkingBalance << endl;
    cout << "Combined Balance     : " << totalBalance << endl;
    pauseScreen();
}

// |---------------------- Main Menu ----------------------|

void showMenu() {
    cout << endl;
    cout << "|-------------------------------------------------------------------------------| " << endl;
    cout << "             VIRTUAL LEDGER BANKING ENGINE" << endl;
    cout << "|-------------------------------------------------------------------------------| " << endl;

    cout << "[1] Create Savings/Checking Account" << endl;
    cout << "[2] Deposit Money" << endl;
    cout << "[3] Withdraw Money" << endl;
    cout << "[4] Transfer Funds" << endl;
    cout << "[5] Search Account" << endl;
    cout << "[6] Display All Accounts" << endl;
    cout << "[7] View Transaction History" << endl;
    cout << "[8] Account Calculation Report" << endl;
    cout << "[9] Balance Summary" << endl;
    cout << "[10] Delete Account" << endl;
    cout << "[0] Exit" << endl;

    printLine('-', 75);
}

//  Memory Cleanup 

void releaseAccounts() {
    for (int i = 0; i < static_cast<int>(accounts.size()); i++) {
        delete accounts[i];
    }

    accounts.clear();
}

// |---------------------- Main Function ----------------------|

int main() {
    loadAccounts();
    int choice;
    while (true) {
        showMenu();

        choice = getIntInput(
            "Enter Your Choice: ",
            0, 10
        );

        switch (choice) {
        case 1:
            createAccount();
            break;
        case 2:
            depositMoney();
            break;
        case 3:
            withdrawMoney();
            break;
        case 4:
            transferFunds();
            break;
        case 5:
            searchAccount();
            break;
        case 6:
            displayAllAccounts();
            break;
        case 7:
            viewTransactions();
            break;
        case 8:
            accountCalculationReport();
            break;
        case 9:
            balanceSummary();
            break;
        case 10:
            deleteAccount();
            break;
        case 0:
            cout << endl;
            cout << "Saving account records..." << endl;

            saveAllAccounts();
            releaseAccounts();

            cout << "Thank You for Using " << "Virtual Ledger Banking Engine!" << endl;

            return 0;
        }
    }

    return 0;
}