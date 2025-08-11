#include <iostream>
using namespace std;

class BankAccount {
protected:
    int accountNumber;
    char accountHolderName[50];
    double balance;

public:
    BankAccount(int accNum, const char name[], double bal) {
        accountNumber = accNum;
        int i = 0;
        while (name[i] != '\0' && i < 49) {
            accountHolderName[i] = name[i];
            i++;
        }
        accountHolderName[i] = '\0';
        balance = bal;
    }

    virtual ~BankAccount() {}

    void deposit(double amount) {
        if (amount > 0) {
        balance += amount;
        cout << "Deposited: " << amount << endl;
        cout << "Total Balance: " << balance << endl;
        } else {
        cout << "Invalid deposit amount." << endl;
        }
    }
    virtual void withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        cout << "Withdrew: " << amount << endl;
        cout << "Total Balance: " << balance << endl;
    } else {
        cout << "Insufficient funds or invalid amount." << endl;
    }
}

    virtual double getBalance() {
        return balance;
    }

    virtual void displayAccountInfo() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: " << balance << endl;
    }

    int getAccountNumber() const {
        return accountNumber;
    }
};

class SavingsAccount : public BankAccount {
public:
    SavingsAccount(int accNum, const char name[], double bal)
        : BankAccount(accNum, name, bal) {}

    void displayAccountInfo() const override {
        BankAccount::displayAccountInfo();
        cout << "Account Type: Savings" << endl;
    }
};

class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CheckingAccount(int accNum, const char name[], double bal, double limit)
        : BankAccount(accNum, name, bal), overdraftLimit(limit) {}

    void withdraw(double amount) override {
    if (amount > 0 && amount <= balance + overdraftLimit) {
        balance -= amount;
        cout << "Withdrew: " << amount << endl;
        cout << "Total Balance: " << balance << endl;
    } else {
        cout << "Withdrawal exceeds overdraft limit." << endl;
    }
}

    void displayAccountInfo() const override {
        BankAccount::displayAccountInfo();
        cout << "Account Type: Checking" << endl;
        cout << "Overdraft Limit: " << overdraftLimit << endl;
    }
};

class FixedDepositAccount : public BankAccount {
private:
    int term; 
    double interestRate;
    bool interestAdded;

public:
    FixedDepositAccount(int accNum, const char name[], double bal, int t)
        : BankAccount(accNum, name, bal), term(t), interestRate(7.0), interestAdded(false) {}

    double calculateInterest() const {
        return balance * (interestRate / 100) * (term / 12.0);
    }

    double getBalance() override {
        if (!interestAdded) {
            balance += calculateInterest();
            interestAdded = true;
        }
        return balance;
    }

    void displayAccountInfo() const override {
        BankAccount::displayAccountInfo();
        cout << "Account Type: Fixed Deposit" << endl;
        cout << "Term: " << term << " months" << endl;
        cout << "Interest Rate: " << interestRate << "%" << endl;
        cout << "Interest Earned: " << calculateInterest() << endl;
    }
};

class BankingSystem {
private:
    BankAccount* accounts[100];
    int accountCount;

public:
    BankingSystem() : accountCount(0) {}

    ~BankingSystem() {
        for (int i = 0; i < accountCount; i++) {
            delete accounts[i];
        }
    }

    void createAccount() {
        if (accountCount >= 100) {
            cout << "Cannot create more accounts." << endl;
            return;
        }

        int choice;
        cout << endl << "Select Account Type:" << endl;
        cout << "1. Savings" << endl;
        cout << "2. Checking" << endl;
        cout << "3. Fixed Deposit" << endl;
        cout << "Choice: ";
        cin >> choice;

        int accNum;
        char name[50];
        double balance;

        cout << "Enter Account Number: ";
        cin >> accNum;
        cout << "Enter Account Holder Name (no spaces): ";
        cin >> name;
        cout << "Enter Initial Balance: ";
        cin >> balance;

        if (choice == 1) {
            accounts[accountCount++] = new SavingsAccount(accNum, name, balance);
        } 
        else if (choice == 2) {
            double limit;
            cout << "Enter Overdraft Limit: ";
            cin >> limit;
            accounts[accountCount++] = new CheckingAccount(accNum, name, balance, limit);
        } 
        else if (choice == 3) {
            int term;
            cout << "Enter Term (in months): ";
            cin >> term;
            accounts[accountCount++] = new FixedDepositAccount(accNum, name, balance, term);
        } 
        else {
            cout << "Invalid choice." << endl;
        }
    }

    BankAccount* findAccount(int accNum) {
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i]->getAccountNumber() == accNum) {
                return accounts[i];
            }
        }
        return nullptr;
    }

    void depositToAccount() {
        int accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;
        BankAccount* acc = findAccount(accNum);
        if (acc) {
            cout << "Enter Amount to Deposit: ";
            cin >> amount;
            acc->deposit(amount);
        } else {
            cout << "Account not found." << endl;
        }
    }

    void withdrawFromAccount() {
        int accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;
        BankAccount* acc = findAccount(accNum);
        if (acc) {
            cout << "Enter Amount to Withdraw: ";
            cin >> amount;
            acc->withdraw(amount);
        } else {
            cout << "Account not found." << endl;
        }
    }

    void checkBalance() {
        int accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;
        BankAccount* acc = findAccount(accNum);
        if (acc) {
            cout << "Current Balance: " << acc->getBalance() << endl;
        } else {
            cout << "Account not found." << endl;
        }
    }

    void displayAccountInfo() {
        int accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;
        BankAccount* acc = findAccount(accNum);
        if (acc) {
            acc->displayAccountInfo();
        } else {
            cout << "Account not found." << endl;
        }
    }
};

int main() {
    BankingSystem bank;
    int choice;

    do {
        cout << endl << "===== BANKING SYSTEM MENU =====" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Check Balance" << endl;
        cout << "5. Display Account Info" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: bank.createAccount();
        break;
        case 2: bank.depositToAccount(); 
        break;
        case 3: bank.withdrawFromAccount(); 
        break;
        case 4: bank.checkBalance(); 
        break;
        case 5: bank.displayAccountInfo(); 
        break;
        case 6: cout << "Exiting..." << endl; 
        break;
        default: cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 6);

    return 0;
}
