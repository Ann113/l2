#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "structures_from_lr1.h"

using namespace std;

// LRU кэш
struct LRUCache {
    int capacity;
    HashTable* cache;
    List* list;
    
    LRUCache(int cap) : capacity(cap) {
        cache = createHashTable(capacity * 2); // Увеличиваем емкость для уменьшения коллизий
        list = createList();
    }
    
    ~LRUCache() {
        destroyHashTable(cache);
        destroyList(list);
    }
    
    // Получить значение по ключу
    int get(int key) {
        ListNode* node = hashFind(cache, key);
        if (node != nullptr) {
            moveToFront(list, node);
            return node->value;
        }
        return -1;
    }
    
    // Установить значение по ключу
    void set(int key, int value) {
        ListNode* node = hashFind(cache, key);
        if (node != nullptr) {
            // Ключ существует - обновить и переместить в начало
            node->value = value;
            moveToFront(list, node);
        } else {
            // Новый ключ
            if (list->size >= capacity) {
                // Удаляем наименее используемый элемент (с конца списка)
                ListNode* lruNode = list->tail;
                if (lruNode != nullptr) {
                    hashRemove(cache, lruNode->key);
                    removeNode(list, lruNode);
                    delete lruNode;
                }
            }
            
            ListNode* newNode = createListNode(key, value);
            hashInsert(cache, key, newNode);
            addToFront(list, newNode);
        }
    }
    
    int getCurrentSize() {
        return list->size;
    }
    
    void printCache() {
        cout << "Состояние кэша (" << list->size << "/" << capacity << "): ";
        ListNode* current = list->head;
        while (current != nullptr) {
            cout << current->key << ":" << current->value;
            if (current->next != nullptr) {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << endl;
    }
};

void processQueries() {
    int cap, Q;
    
    cout << "Введите емкость кэша: ";
    cin >> cap;
    
    if (cap <= 0) {
        cout << "Ошибка: емкость кэша должна быть положительным числом!" << endl;
        return;
    }
    
    cout << "Введите количество запросов: ";
    cin >> Q;
    
    if (Q <= 0) {
        cout << "Ошибка: количество запросов должно быть положительным числом!" << endl;
        return;
    }
    
    cin.ignore();
    
    LRUCache cache(cap);
    vector<int> results;
    
    cout << "Введите запросы (SET key value или GET key):" << endl;
    
    for (int i = 0; i < Q; i++) {
        string command;
        cout << "Запрос " << (i + 1) << ": ";
        getline(cin, command);
        
        if (command.find("SET") == 0) {
            int key, value;
            if (sscanf(command.c_str(), "SET %d %d", &key, &value) == 2) {
                cache.set(key, value);
                cout << "Установлено: ключ=" << key << ", значение=" << value << endl;
                cache.printCache();
            } else {
                cout << "Ошибка: неверный формат команды SET. Используйте: SET key value" << endl;
            }
        } else if (command.find("GET") == 0) {
            int key;
            if (sscanf(command.c_str(), "GET %d", &key) == 1) {
                int result = cache.get(key);
                results.push_back(result);
                cout << "Получено: ключ=" << key << ", значение=" << result << endl;
                cache.printCache();
            } else {
                cout << "Ошибка: неверный формат команды GET. Используйте: GET key" << endl;
            }
        } else {
            cout << "Ошибка: неизвестная команда. Используйте SET или GET" << endl;
        }
    }
    
    // Вывод результатов GET запросов
    if (!results.empty()) {
        cout << "\nРезультаты GET запросов: ";
        for (size_t i = 0; i < results.size(); i++) {
            if (i > 0) cout << " ";
            cout << results[i];
        }
        cout << endl;
    }
}

int main() {
    cout << "LRU Кэш" << endl;
    
    try {
        processQueries();
    } catch (const exception& e) {
        cout << "Произошла ошибка: " << e.what() << endl;
    }
    
    return 0;
}