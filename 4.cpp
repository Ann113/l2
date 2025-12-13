#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include "structures_from_lr1.h"

using namespace std;

struct WordDictionary {
    SetArray* words;
    
    WordDictionary() {
        words = createSet(10);
    }
    
    ~WordDictionary() {
        destroySet(words);
    }
    
    void addWord(const string& word) {
        setInsert(words, word);
    }
    
    bool contains(const string& word) {
        return setContains(words, word);
    }
};

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
    cout << "Проверка ударений" << endl;
    
    int n;
    cout << "Введите количество слов в словаре: ";
    
    if (!(cin >> n)) {
        cout << "Ошибка: введите целое число!" << endl;
        return 1;
    }
    
    if (n <= 0) {
        cout << "Ошибка: количество слов должно быть положительным!" << endl;
        return 1;
    }

    WordDictionary dict;
    string word;
    cout << "Введите слова словаря:" << endl;
    
    for (int i = 0; i < n; i++) {
        if (!(cin >> word)) {
            cout << "Ошибка при вводе слова!" << endl;
            return 1;
        }
        
        if (!containsOnlyLetters(word)) {
            cout << "Ошибка: слово должно содержать только буквы! Получено: '" << word << "'" << endl;
            return 1;
        }
        
        dict.addWord(word);
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
        
        if (!containsOnlyLetters(word)) {
            cout << "Ошибка: слово в тексте содержит небуквенные символы: '" << word << "'" << endl;
            return 1;
        }

        int stresses = countStresses(word);
        
        if (dict.contains(word)) {
            if (stresses != 1) {
                errors++;
                cout << "Ошибка в слове " << word << " (есть в словаре, но ударение неверное)" << endl;
            }
        } else {
            if (stresses != 1) {
                errors++;
                cout << "Ошибка в слове " << word << " (нет в словаре, ударение должно быть одно)" << endl;
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