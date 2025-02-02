#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

class FinanceManager {
public:
    struct Wallet {
        string name;
        double balance;
        bool isCredit;
        double creditLimit;
    };

    struct Transaction {
        string category;
        double amount;
        string date;
    };

    void menu() {
        int choice;
        do {
            cout << "\n������� ���������� ���������\n";
            cout << "1. �������� �������/�����\n";
            cout << "2. ���������\n";
            cout << "3. ������ �����\n";
            cout << "4. �������� ������\n";
            cout << "5. ��������� ������ � ����\n";
            cout << "0. �����\n";
            cout << "�������� �����: ";
            cin >> choice;

            switch (choice) {
            case 1:
                addwallet();
                break;
            case 2:
                deposit();
                break;
            case 3:
                addexpn();
                break;
            case 4:
                showrpts();
                break;
            case 5:
                savetofile();
                break;
            case 0:
                cout << "����� �� �������...\n";
                break;
            default:
                cout << "�������� �����. ���������� �����.\n";
            }
        } while (choice != 0);
    }

private:
    vector<Wallet> wallets;
    vector<Transaction> transactions;

    void addwallet() {
        Wallet newWallet;
        cout << "������� ��������: ";
        cin >> newWallet.name;
        cout << "��� ��������� �����? (1 - ��, 0 - ���): ";
        cin >> newWallet.isCredit;
        newWallet.balance = 0.0;
        if (newWallet.isCredit) {
            cout << "������� ��������� �����: ";
            cin >> newWallet.creditLimit;
        }
        else {
            newWallet.creditLimit = 0.0;
        }
        wallets.push_back(newWallet);
        cout << "���������!\n";
    }

    void deposit() {
        string name;
        double amount;
        cout << "������� ��������: ";
        cin >> name;
        cout << "������� �����: ";
        cin >> amount;

        auto it = find_if(wallets.begin(), wallets.end(), [&name](const Wallet& w) { return w.name == name; });
        if (it != wallets.end()) {
            it->balance += amount;
            cout << "������ ��������: " << it->balance << "\n";
        }
        else {
            cout << "������� �� ������!\n";
        }
    }

    void addexpn() {
        string name, category, date;
        double amount;
        cout << "������� �������� ��������: ";
        cin >> name;
        cout << "������� ���������: ";
        cin >> category;
        cout << "������� �����: ";
        cin >> amount;
        cout << "������� ���� (����-��-��): ";
        cin >> date;

        auto it = find_if(wallets.begin(), wallets.end(), [&name](const Wallet& w) { return w.name == name; });
        if (it != wallets.end() && (it->balance + it->creditLimit) >= amount) {
            it->balance -= amount;
            transactions.push_back({ category, amount, date });
            cout << "����� ���������! ������: " << it->balance << "\n";
        }
        else {
            cout << "������������ �������!\n";
        }
    }

    void showrpts() {
        cout << "������ �� ��������:\n";
        for (const auto& t : transactions) {
            cout << "����: " << t.date << " | ���������: " << t.category << " | �����: " << t.amount << "\n";
        }
        showtopexpns();
        showtopcats();
    }

    void showtopexpns() {
        vector<Transaction> sorted = transactions;
        sort(sorted.begin(), sorted.end(), [](const Transaction& a, const Transaction& b) {
            return a.amount > b.amount;
            });
        cout << "\n���-3 ����:\n";
        for (size_t i = 0; i < min(sorted.size(), size_t(3)); i++) {
            cout << sorted[i].date << " " << sorted[i].category << " " << sorted[i].amount << "\n";
        }
    }

    void showtopcats() {
        vector<pair<string, double>> categorySums;
        for (const auto& t : transactions) {
            auto it = find_if(categorySums.begin(), categorySums.end(), [&t](const pair<string, double>& p) {
                return p.first == t.category;
                });
            if (it != categorySums.end()) {
                it->second += t.amount;
            }
            else {
                categorySums.push_back({ t.category, t.amount });
            }
        }

        sort(categorySums.begin(), categorySums.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
        cout << "\n���-3 ���������:\n";
        for (size_t i = 0; i < min(categorySums.size(), size_t(3)); i++) {
            cout << categorySums[i].first << " " << categorySums[i].second << "\n";
        }
    }

    void savetofile() {
        ofstream file("reports.txt");
        if (!file) {
            cout << "������ ���������� � ����!\n";
            return;
        }
        file << "������ �� ��������:\n";
        for (const auto& t : transactions) {
            file << "����: " << t.date << " | ���������: " << t.category << " | �����: " << t.amount << "\n";
        }
        file.close();
        cout << "������ ��������� � reports.txt\n";
    }
};

int main() {
    setlocale(LC_ALL, "");
    FinanceManager system;
    system.menu();
    return 0;
}
