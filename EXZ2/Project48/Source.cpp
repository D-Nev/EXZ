#include <iostream>
#include <vector>
#include <string>
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

    // ����� �������� �� ��������
    Wallet* findWallet(const string& name) {
        for (size_t i = 0; i < wallets.size(); i++) {
            if (wallets[i].name == name) return &wallets[i];
        }
        return nullptr;
    }

    void addwallet() {
        Wallet newWallet;
        cout << "������� ��������: ";
        cin >> newWallet.name;
        cout << "��� ��������� �����? (1 - ��, 0 - ���): ";
        cin >> newWallet.isCredit;
        newWallet.balance = 0.0;
        newWallet.creditLimit = newWallet.isCredit ? inputCreditLimit() : 0.0;
        wallets.push_back(newWallet);
        cout << "���������!\n";
    }

    double inputCreditLimit() {
        double limit;
        cout << "������� ��������� �����: ";
        cin >> limit;
        return limit;
    }

    void deposit() {
        string name;
        double amount;
        cout << "������� ��������: ";
        cin >> name;
        cout << "������� �����: ";
        cin >> amount;
        Wallet* it = findWallet(name);
        if (it) {
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
        Wallet* it = findWallet(name);
        if (it && (it->balance + it->creditLimit) >= amount) {
            it->balance -= amount;
            transactions.push_back({ category, amount, date });
            cout << "����� ���������! ������: " << it->balance << "\n";
        }
        else {
            cout << "������������ �������!\n";
        }
    }

    void bubbleSortTransactions(vector<Transaction>& arr) {
        size_t n = arr.size();
        for (size_t i = 0; i < n - 1; i++) {
            for (size_t j = 0; j < n - i - 1; j++) {
                if (arr[j].amount < arr[j + 1].amount) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    void showrpts() {
        cout << "������ �� ������:\n";
        for (const auto& t : transactions) {
            cout << "����: " << t.date << " | ���������: " << t.category << " | �����: " << t.amount << "\n";
        }
        showtopexpns();
        showtopcategories();

    }

    void showtopexpns() {
        vector<Transaction> sorted = transactions;
        bubbleSortTransactions(sorted);
        cout << "\n���-3 ����:\n";
        for (size_t i = 0; i < (sorted.size() < 3 ? sorted.size() : 3); i++) {
            cout << sorted[i].date << " " << sorted[i].category << " " << sorted[i].amount << "\n";
        }
    }

    //���-3 ���������
    void showtopcategories() {
        struct CategorySum {
            string category;
            double total;
        };

        vector<CategorySum> categories;
        for (const auto& t : transactions) {
            bool found = false;
            for (size_t i = 0; i < categories.size(); i++) {
                if (categories[i].category == t.category) {
                    categories[i].total += t.amount;
                    found = true;
                    break;
                }
            }
            if (!found) {
                categories.push_back({ t.category, t.amount });
            }
        }

        for (size_t i = 0; i < categories.size(); i++) {
            for (size_t j = 0; j < categories.size() - i - 1; j++) {
                if (categories[j].total < categories[j + 1].total) {
                    swap(categories[j], categories[j + 1]);
                }
            }
        }

        cout << "\n���-3 ���������:\n";
        for (size_t i = 0; i < (categories.size() < 3 ? categories.size() : 3); i++) {
            cout << categories[i].category << " " << categories[i].total << "\n";
        }
    }

    void savetofile() {
        ofstream file("reports.txt");
        if (!file) {
            cout << "������ ���������� � ����!\n";
            return;
        }
        file << "������ �� ������:\n";
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
