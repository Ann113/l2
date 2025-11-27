#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

int countStresses(const string& word) {
    int count = 0;
    for (char c : word) {
        if (isupper(c)) count++;
    }
    return count;
}

bool containsOnlyLetters(const string& word) {
    for (char c : word) {
        if (!isalpha(c)) return false;
    }
    return true;
}

int main() {
    cout << "Задание 4: Проверка ударений" << endl;
    
    int n;
    cout << "Введите количество слов в словаре: ";
    
    // Проверка ввода числа
    if (!(cin >> n)) {
        cout << "Ошибка: введите целое число!" << endl;
        return 1;
    }
    
    if (n <= 0) {
        cout << "Ошибка: количество слов должно быть положительным!" << endl;
        return 1;
    }

    unordered_set<string> dict;
    string word;
    cout << "Введите слова словаря:" << endl;
    
    for (int i = 0; i < n; i++) {
        if (!(cin >> word)) {
            cout << "Ошибка при вводе слова!" << endl;
            return 1;
        }
        
        // Проверка, что слово состоит только из букв
        if (!containsOnlyLetters(word)) {
            cout << "Ошибка: слово должно содержать только буквы! Получено: '" << word << "'" << endl;
            return 1;
        }
        
        dict.insert(word);
    }

    cin.ignore();
    string line;
    cout << "Введите текст для проверки: ";
    
    if (!getline(cin, line)) {
        cout << "Ошибка при вводе текста!" << endl;
        return 1;
    }
    
    if (line.empty()) {
        cout << "Ошибка: текст не может быть пустым!" << endl;
        return 1;
    }

    stringstream ss(line);
    int errors = 0;
    int wordCount = 0;

    while (ss >> word) {
        wordCount++;
        
        // Проверка, что слово в тексте состоит только из букв
        if (!containsOnlyLetters(word)) {
            cout << "Ошибка: слово в тексте содержит небуквенные символы: '" << word << "'" << endl;
            return 1;
        }

        int stresses = countStresses(word);
        
        if (dict.count(word)) {
            if (stresses != 1) {
                errors++;
                cout << "Ошибка в слове " << word << endl;
            }
        } else {
            if (stresses != 1) {
                errors++;
                cout << "Ошибка в слове " << word << endl;
            }
        }
    }

    if (wordCount == 0) {
        cout << "Ошибка: в тексте нет слов для проверки!" << endl;
        return 1;
    }

    cout << "Общее количество ошибок: " << errors << endl;
    return 0;
}