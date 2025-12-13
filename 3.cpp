#include <iostream>
#include <string>
#include <cctype>
#include "structures_from_lr1.h"

using namespace std;

// Специальное множество для пар (2 символа)
struct PairSet {
    SetArray* pairs;
    
    PairSet() {
        pairs = createSet(100); // Начальная емкость 100
    }
    
    ~PairSet() {
        destroySet(pairs);
    }
    
    void insert(const string& pair) {
        if (pair.length() == 2) {
            setInsert(pairs, pair);
        }
    }
    
    bool contains(const string& pair) {
        if (pair.length() != 2) return false;
        return setContains(pairs, pair);
    }
};

// Функция для проверки корректности генома
bool isValidGenome(const string& genome) {
    if (genome.empty()) {
        cout << "Ошибка: геном не может быть пустой строкой" << endl;
        return false;
    }
    
    for (char c : genome) {
        if (!isupper(c) || c < 'A' || c > 'Z') {
            cout << "Ошибка: геном должен содержать только заглавные латинские буквы (A-Z)" << endl;
            cout << "Найден недопустимый символ: '" << c << "'" << endl;
            return false;
        }
    }
    
    return true;
}

// Функция для вычисления степени близости
int calculateSimilarity(const string& genome1, const string& genome2) {
    if (genome1.length() < 2 || genome2.length() < 2) {
        return 0;
    }

    // Создаем множество всех пар второго генома
    PairSet pairs_genome2;
    for (size_t i = 0; i + 1 < genome2.length(); i++) {
        string pair = genome2.substr(i, 2);
        pairs_genome2.insert(pair);
    }

    // Подсчитываем совпадающие пары первого генома
    int similarity = 0;
    
    for (size_t i = 0; i + 1 < genome1.length(); i++) {
        string pair = genome1.substr(i, 2);
        if (pairs_genome2.contains(pair)) {
            similarity++;
        }
    }

    return similarity;
}

int main() {
    cout << "Степень близости геномов" << endl;
    
    string genome1, genome2;
    
    // Ввод первого генома с проверкой
    while (true) {
        cout << "Введите первый геном: ";
        if (!(cin >> genome1)) {
            cout << "Ошибка ввода! Пожалуйста, введите строку." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (isValidGenome(genome1)) {
            break;
        }
    }
    
    // Ввод второго генома с проверкой
    while (true) {
        cout << "Введите второй геном: ";
        if (!(cin >> genome2)) {
            cout << "Ошибка ввода! Пожалуйста, введите строку." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (isValidGenome(genome2)) {
            break;
        }
    }
    
    // Вычисление и вывод результата
    int similarity = calculateSimilarity(genome1, genome2);
    cout << "Степень близости: " << similarity << endl;
    
    return 0;
}