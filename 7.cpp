#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

// Узел двусвязного списка
struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;
    
    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// LRU кэш
struct LRUCache {
    int capacity;
    unordered_map<int, Node*> cache; // Хэш-таблица для быстрого доступа
    Node* head; // Фиктивная голова
    Node* tail; // Фиктивный хвост
    
    LRUCache(int cap) {
        capacity = cap;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }
    
    ~LRUCache() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    
    // Добавить узел в начало
    void addToHead(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
    
    // Удалить узел из списка
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    // Переместить узел в начало
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }
    
    // Удалить наименее используемый элемент
    void removeLRU() {
        Node* lru = tail->prev;
        removeNode(lru);
        cache.erase(lru->key);
        delete lru;
    }
    
    // Получить значение по ключу
    int get(int key) {
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            moveToHead(node);
            return node->value;
        }
        return -1;
    }
    
    // Установить значение по ключу
    void set(int key, int value) {
        if (cache.find(key) != cache.end()) {
            // Ключ существует - обновить и переместить в начало
            Node* node = cache[key];
            node->value = value;
            moveToHead(node);
        } else {
            // Новый ключ
            if (cache.size() >= capacity) {
                removeLRU();
            }
            Node* newNode = new Node(key, value);
            cache[key] = newNode;
            addToHead(newNode);
        }
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
    vector<int> results; // Результаты GET запросов
    
    cout << "Введите запросы (SET key value или GET key):" << endl;
    
    for (int i = 0; i < Q; i++) {
        string command;
        cout << "Запрос " << (i + 1) << ": ";
        getline(cin, command);
        
        if (command.find("SET") == 0) {
            int key, value;
            if (sscanf(command.c_str(), "SET %d %d", &key, &value) == 2) {
                cache.set(key, value);
            } else {
                cout << "Ошибка: неверный формат команды SET. Используйте: SET key value" << endl;
            }
        } else if (command.find("GET") == 0) {
            int key;
            if (sscanf(command.c_str(), "GET %d", &key) == 1) {
                int result = cache.get(key);
                results.push_back(result);
            } else {
                cout << "Ошибка: неверный формат команды GET. Используйте: GET key" << endl;
            }
        } else {
            cout << "Ошибка: неизвестная команда. Используйте SET или GET" << endl;
        }
    }
    
    // Вывод результатов GET запросов
    for (size_t i = 0; i < results.size(); i++) {
        if (i > 0) cout << " ";
        cout << results[i];
    }
    cout << endl;
}

int main() {
    cout << "LRU" << endl;
    
    try {
        processQueries();
    } catch (const exception& e) {
        cout << "Произошла ошибка: " << e.what() << endl;
    }
    
    return 0;
}