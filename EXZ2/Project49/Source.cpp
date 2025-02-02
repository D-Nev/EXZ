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
            cout << "\nСистема управления финансами\n";
            cout << "1. Добавить кошелек/карту\n";
            cout << "2. Пополнить\n";
            cout << "3. Внести трату\n";
            cout << "4. Показать отчеты\n";
            cout << "5. Сохранить отчеты в файл\n";
            cout << "0. Выход\n";
            cout << "Выберите опцию: ";
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
                cout << "Выход из системы...\n";
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } while (choice != 0);
    }

private:
    vector<Wallet> wallets;
    vector<Transaction> transactions;

    void addwallet() {
        Wallet newWallet;
        cout << "Введите название: ";
        cin >> newWallet.name;
        cout << "Это кредитная карта? (1 - да, 0 - нет): ";
        cin >> newWallet.isCredit;
        newWallet.balance = 0.0;
        if (newWallet.isCredit) {
            cout << "Введите кредитный лимит: ";
            cin >> newWallet.creditLimit;
        }
        else {
            newWallet.creditLimit = 0.0;
        }
        wallets.push_back(newWallet);
        cout << "Добавлено!\n";
    }

    void deposit() {
        string name;
        double amount;
        cout << "Введите название: ";
        cin >> name;
        cout << "Введите сумму: ";
        cin >> amount;

        auto it = find_if(wallets.begin(), wallets.end(), [&name](const Wallet& w) { return w.name == name; });
        if (it != wallets.end()) {
            it->balance += amount;
            cout << "Баланс обновлен: " << it->balance << "\n";
        }
        else {
            cout << "Кошелек не найден!\n";
        }
    }

    void addexpn() {
        string name, category, date;
        double amount;
        cout << "Введите название кошелька: ";
        cin >> name;
        cout << "Введите категорию: ";
        cin >> category;
        cout << "Введите сумму: ";
        cin >> amount;
        cout << "Введите дату (ГГГГ-ММ-ДД): ";
        cin >> date;

        auto it = find_if(wallets.begin(), wallets.end(), [&name](const Wallet& w) { return w.name == name; });
        if (it != wallets.end() && (it->balance + it->creditLimit) >= amount) {
            it->balance -= amount;
            transactions.push_back({ category, amount, date });
            cout << "Трата добавлена! Баланс: " << it->balance << "\n";
        }
        else {
            cout << "Недостаточно средств!\n";
        }
    }

    void showrpts() {
        cout << "Отчеты по затратам:\n";
        for (const auto& t : transactions) {
            cout << "Дата: " << t.date << " | Категория: " << t.category << " | Сумма: " << t.amount << "\n";
        }
        showtopexpns();
        showtopcats();
    }

    void showtopexpns() {
        vector<Transaction> sorted = transactions;
        sort(sorted.begin(), sorted.end(), [](const Transaction& a, const Transaction& b) {
            return a.amount > b.amount;
            });
        cout << "\nТОП-3 трат:\n";
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
        cout << "\nТОП-3 категории:\n";
        for (size_t i = 0; i < min(categorySums.size(), size_t(3)); i++) {
            cout << categorySums[i].first << " " << categorySums[i].second << "\n";
        }
    }

    void savetofile() {
        ofstream file("reports.txt");
        if (!file) {
            cout << "Ошибка сохранения в файл!\n";
            return;
        }
        file << "Отчеты по затратам:\n";
        for (const auto& t : transactions) {
            file << "Дата: " << t.date << " | Категория: " << t.category << " | Сумма: " << t.amount << "\n";
        }
        file.close();
        cout << "Отчеты сохранены в reports.txt\n";
    }
};

int main() {
    setlocale(LC_ALL, "");
    FinanceManager system;
    system.menu();
    return 0;
}
