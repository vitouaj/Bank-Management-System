#include <iostream>
#include <string>
#include <list>
#include <map>
#include <chrono>

class Utils{
public:
    static std::string generateID(const std::string& nationalID) {
        if (nationalID == "") {
            std::cout << "national id not provided" << std::endl;
            return "";
        }
        std::string id = "aba_" + nationalID;
        return id;
    }

    static std::string timepoint_to_date_time_str(std::chrono::system_clock::time_point tp) {
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);

        char dt[80];
        std::strftime(dt, sizeof(dt), "%d-%m-%Y %H:%M:%S", std::localtime(&tt));
        return dt;
    }
};

class Account{
private:
    double balance;
    std::string accountId;
    std::string accountHolderName;
    std::chrono::system_clock::time_point createdDate;
public:
    Account(const std::string& name): balance(0), createdDate(std::chrono::system_clock::now()) {
        accountHolderName = name;
        std::cout << "Successfully Created new Account..." << std::endl;
    }
    Account(const std::string& name, const std::string& acc_id, double amount) {
        accountHolderName = name;
        accountId = acc_id;
        balance = amount;
        createdDate = std::chrono::system_clock::now();
        std::cout << "Successfully Created Account..." << std::endl;
    }
    void printAccount() {
        std::cout << "========================================================" << std::endl;
        std::cout << "==        Account ID: " << accountId << std::endl;
        std::cout << "==        Account HolderName: " << accountHolderName << std::endl;
        std::cout << "==        Account Balance: " << balance << std::endl;
        std::cout << "==        Created Date: " << Utils::timepoint_to_date_time_str(createdDate) << std::endl;
        std::cout << "========================================================\n" << std::endl;
    }
    double getBalance() const {
        return balance;
    }
    std::string getAccountId() const {
        return accountId;
    }
    std::string getAccountHolderName() const {
        return accountHolderName;
    }
    std::string getCreatedDate() const {
        return Utils::timepoint_to_date_time_str(createdDate);
    }

    void deposit(double amount);
    void withdraw(double amount);
    void transfer(double amount, const std::string& acc_id);

    bool enough(double amount) {
        if (amount >= balance) {
            return false;
        }
        return true;
    }
};

class Bank{
private:
    std::map<std::string, Account> accountList;

public:
    void printAccountList() {
        std::map<std::string, Account>::iterator it = accountList.begin();
        while (it != accountList.end()) {
            std::cout << "AccountID: " << it->first << ", Account Holder: " << it->second.getAccountHolderName() << ", Created on " << it->second.getCreatedDate() << std::endl;
            ++it;
        }
    }
    void createAccount() {
        std::string name;
        std::string nationalID;
        double amount;
        std::cout << "enter your name: "; std::cin >> name;
        std::cout << "enter your nationalID: "; std::cin >> nationalID;
        std::cout << "enter your initial amount: "; std::cin >> amount;

        std::string acc_id = Utils::generateID(nationalID);

        Account newAccount(name, acc_id, amount);
        accountList.insert(std::make_pair(acc_id, newAccount));
    }
    void deleteAccount() {
        std::string acc_id;
        std::cout << "enter your account ID: "; std::cin >> acc_id;
        auto it = accountList.find(acc_id);

        if (it != accountList.end()) {
            accountList.erase(it);
            std::cout << "Successfully delete account " << acc_id << std::endl;
        } else {
            std::cout << "account not found" << std::endl;
        }
    }
    void deposit() {
        std::string acc_id;
        std::cout << "enter your account ID: "; std::cin >> acc_id;

        auto it = accountList.find(acc_id);

        if (it == accountList.end()) {
            std::cout << "account not found..." << std::endl;
            return;
        }
        double amount;
        std::cout << "enter deposit amount: "; std::cin >> amount;
        it->second.deposit(amount);
        return;
    }
    void check_balance() {
        std::string acc_id;
        std::cout << "enter your account ID: "; std::cin >> acc_id;
        auto it = accountList.find(acc_id);

        if (it == accountList.end()) {
            std::cout << "account not found..." << std::endl;
            return;
        }
        it->second.printAccount();
    }
    void withdraw() {
        std::string acc_id;
        std::cout << "enter your account ID: "; std::cin >> acc_id;
        auto it = accountList.find(acc_id);

        if (it == accountList.end()) {
            std::cout << "account not found..." << std::endl;
            return;
        }
        double amount;
        std::cout << "enter withdraw amount: "; std::cin >> amount;
        it->second.withdraw(amount);
    }

    void transfer() {
        std::string acc_id;
        std::string racc_id;
        double amount;

        std::cout << "enter sender account ID: "; std::cin >> acc_id;
        auto it = accountList.find(acc_id);
        if (it == accountList.end()) {
            std::cout << "sender account not found..." << std::endl;
            return;
        }

        std::cout << "enter receiver account ID: "; std::cin >> racc_id;
        auto rit = accountList.find(racc_id);
        if (it == accountList.end()) {
            std::cout << "receiver account not found..." << std::endl;
            return;
        }

        std::cout << "enter amount: "; std::cin >> amount;

        if (it->second.enough(amount)) {
            it->second.withdraw(amount);
            rit->second.deposit(amount);
        } else {
            std::cout << "not enough balance to transfer...\n";
            return;
        }


        std::cout << amount << " is credited to account " << rit->second.getAccountId() << " from account " << it->second.getAccountId() << std::endl;
    }
};


void Account::deposit(double amount) {
    Account::balance += amount;
    std::cout << "Successfully Deposit " << amount << " to account " << Account::accountId << std::endl;
}

void Account::withdraw(double amount) {
    if (amount > Account::balance) {
        std::cout << "Insufficient Amount to withdraw..." << std::endl;
        return;
    }
    Account::balance -= amount;
    std::cout << "Successfully Withdraw " << amount << " from account " << Account::accountId << std::endl;
}

void menu() {
    std::cout << "\n==============  WELCOME TO VITOU BANK  =================" << std::endl;
    std::cout << "1. print all account list" << std::endl;
    std::cout << "2. create new account" << std::endl;
    std::cout << "3. delete account" << std::endl;
    std::cout << "4. deposit account" << std::endl;
    std::cout << "5. withdraw account" << std::endl;
    std::cout << "6. check balance" << std::endl;
    std::cout << "7. transfer money" << std::endl;
}
int main()
{
    system("cls");

    Bank bank;
    while (true) {
        int choice;
        menu();
        std::cout << "choice: "; std::cin >> choice;
        std::cout << "--------------------------------------------------------" << std::endl;
        switch(choice) {
        case 1:
            bank.printAccountList();
            break;
        case 2:
            bank.createAccount();
            break;
        case 3:
            bank.deleteAccount();
            break;
        case 4:
            bank.deposit();
            break;
        case 5:
            bank.withdraw();
            break;
        case 6:
            bank.check_balance();
            break;
        case 7:
            bank.transfer();
            break;
        default:
            std::cout << "Invalid Command! Program Terminated..." << std::endl;
            exit(0);
            break;
        }
    }
}
