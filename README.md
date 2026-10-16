**Virtual Ledger Banking Engine******

**Overview**
    Virtual Ledger Banking Engine is a menu-driven C++ console application designed to simulate essential banking operations. It provides an organized interface for managing bank accounts, performing financial transactions, and viewing account summaries.

**Features**
- Create Bank Accounts: Create Savings and Checking accounts with unique account numbers.
- Deposit Money: Add money to an existing account and update its balance.
- Withdraw Money: Withdraw funds while checking account balance availability.
- Transfer Funds: Transfer money between accounts with balance validation.
- Search Account: Find account details using an account number.
- Display All Accounts: View all registered accounts and their balances.
- Transaction History: View recorded deposits, withdrawals, and transfers.
- Account Calculation Report: Calculate estimated savings interest or checking account fees.
- Balance Summary: View total accounts, account types, and combined balances.
- Delete Account: Delete accounts with options to transfer or withdraw the remaining balance.
- Input Validation: Validate account numbers, amounts, and menu choices.
- File Handling: Save account information and transaction records for future sessions.

**Technologies Used**
- C++
- Object-Oriented Programming (OOP)
- Classes and Objects
- Inheritance and Polymorphism
- Encapsulation and Abstraction
- STL Vector
- File Handling (ifstream and ofstream)
- Input Validation

**Account Types**
1. Savings Account
  - Stores account holder information and balance.
  - Supports an annual interest rate.
  - Calculates estimated interest and account value.
2. Checking Account
  - Stores account holder information and balance.
  - Supports a monthly account fee.
  - Calculates estimated account value after deducting the fee.

**Data Storage**
  The application uses text files to maintain records:
    - accounts.txt — Stores account types, account numbers, account holder names, balances, and account-specific values.
     - transactions.txt — Stores transaction types, amounts, balances after transactions, and descriptions.
Account records are loaded when the application starts and saved after account operations.

**How to Run**
1. Clone or download this repository.
2. Open the Visual Studio solution file (.sln) if it is included in the repository.
3. Build the project in Visual Studio.
4. Run the application.
5. Use the console menu to select the required banking operation.
Alternatively, add the C++ source file to a compatible C++ compiler project and compile it.

**Main Menu**
The application provides the following options:
1. Create Savings/Checking Account
2. Deposit Money
3. Withdraw Money
4. Transfer Funds
5. Search Account
6. Display All Accounts
7. View Transaction History
8. Account Calculation Report
9. Balance Summary
10. Delete Account
11. Exit

**Validation and Safety Checks**
- Account numbers must be between 1000 and 999999.
- Duplicate account numbers are rejected.
- Deposits and withdrawals must use valid positive amounts.
- Withdrawals and transfers are checked against available balances.
- Transfers between the same account are not allowed.
- Account data is restored from the account file when available.

**Project Purpose**
This project was developed as an academic C++ application to demonstrate object-oriented programming, inheritance, polymorphism, file handling, input validation, and practical implementation of banking operations.
