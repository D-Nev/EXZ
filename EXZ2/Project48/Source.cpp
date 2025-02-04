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

    // Поиск кошелька по названию
    Wallet* findWallet(const string& name) {
        for (size_t i = 0; i < wallets.size(); i++) {
            if (wallets[i].name == name) return &wallets[i];
        }
        return nullptr;
    }

    void addwallet() {
        Wallet newWallet;
        cout << "Введите название: ";
        cin >> newWallet.name;
        cout << "Это кредитная карта? (1 - да, 0 - нет): ";
        cin >> newWallet.isCredit;
        newWallet.balance = 0.0;
        newWallet.creditLimit = newWallet.isCredit ? inputCreditLimit() : 0.0;
        wallets.push_back(newWallet);
        cout << "Добавлено!\n";
    }

    double inputCreditLimit() {
        double limit;
        cout << "Введите кредитный лимит: ";
        cin >> limit;
        return limit;
    }

    void deposit() {
        string name;
        double amount;
        cout << "Введите название: ";
        cin >> name;
        cout << "Введите сумму: ";
        cin >> amount;
        Wallet* it = findWallet(name);
        if (it) {
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
        Wallet* it = findWallet(name);
        if (it && (it->balance + it->creditLimit) >= amount) {
            it->balance -= amount;
            transactions.push_back({ category, amount, date });
            cout << "Трата добавлена! Баланс: " << it->balance << "\n";
        }
        else {
            cout << "Недостаточно средств!\n";
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
        cout << "Отчеты по тратам:\n";
        for (const auto& t : transactions) {
            cout << "Дата: " << t.date << " | Категория: " << t.category << " | Сумма: " << t.amount << "\n";
        }
        showtopexpns();
        showtopcategories();

    }

    void showtopexpns() {
        vector<Transaction> sorted = transactions;
        bubbleSortTransactions(sorted);
        cout << "\nТОП-3 трат:\n";
        for (size_t i = 0; i < (sorted.size() < 3 ? sorted.size() : 3); i++) {
            cout << sorted[i].date << " " << sorted[i].category << " " << sorted[i].amount << "\n";
        }
    }

    //ТОП-3 категории
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

        cout << "\nТОП-3 категорий:\n";
        for (size_t i = 0; i < (categories.size() < 3 ? categories.size() : 3); i++) {
            cout << categories[i].category << " " << categories[i].total << "\n";
        }
    }

    void savetofile() {
        ofstream file("reports.txt");
        if (!file) {
            cout << "Ошибка сохранения в файл!\n";
            return;
        }
        file << "Отчеты по тратам:\n";
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
