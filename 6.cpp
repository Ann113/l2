#include <iostream>
#include <chrono>
#include <string>
#include "structures_from_lr1.h"  // Наши структуры

using namespace std;

// Структура для элемента хеш-таблицы с открытой адресацией
struct HashItem {
    int key;
    string value;
    bool isDeleted;
    
    HashItem() : key(-1), value(""), isDeleted(false) {}
    HashItem(int k, const string& v) : key(k), value(v), isDeleted(false) {}
};

// Хеш-таблица с открытой адресацией (используем SetArray как динамический массив)
struct OpenAddressingHashTable {
    SetArray* table;  // Используем нашу структуру вместо std::vector
    int capacity;
    int size;
    double loadFactorThreshold;
    int rehashCount;

    OpenAddressingHashTable(int initialCapacity = 8, double threshold = 0.9) 
        : capacity(initialCapacity), size(0), loadFactorThreshold(threshold), rehashCount(0) {
        table = createSet(initialCapacity);
        // Инициализируем массив пустыми элементами
        for (int i = 0; i < capacity; i++) {
            table->data[i] = "";  // Пустая строка как маркер пустой ячейки
        }
    }
    
    ~OpenAddressingHashTable() {
        destroySet(table);
    }

    // Основная хеш-функция
    int hash1(int key) {
        return key % capacity;
    }

    // Вторая хеш-функция для двойного хеширования
    int hash2(int key) {
        return 1 + (key % (capacity - 1));
    }

    // Вставка элемента
    void insert(int key, const string& value) {
        if (getLoadFactor() >= loadFactorThreshold) {
            rehash();
        }

        int index = hash1(key);
        int step = hash2(key);
        int i = 0;

        while (i < capacity) {
            // Проверяем пустую ячейку (ключ -1 или удалена)
            string cellValue = table->data[index];
            if (cellValue.empty() || cellValue == "DELETED") {
                // Сохраняем в формате "ключ:значение"
                table->data[index] = to_string(key) + ":" + value;
                size++;
                cout << "Ключ " << key << " вставлен в позицию " << index << endl;
                return;
            }
            
            // Извлекаем ключ из строки
            size_t colonPos = cellValue.find(':');
            if (colonPos != string::npos) {
                int existingKey = stoi(cellValue.substr(0, colonPos));
                if (existingKey == key) {
                    // Обновляем значение
                    table->data[index] = to_string(key) + ":" + value;
                    cout << "Ключ " << key << " обновлен в позиции " << index << endl;
                    return;
                }
            }
            
            index = (index + step) % capacity;
            i++;
        }
        
        rehash();
        insert(key, value);
    }

    // Поиск элемента
    string search(int key) {
        int index = hash1(key);
        int step = hash2(key);
        int i = 0;

        while (i < capacity) {
            string cellValue = table->data[index];
            
            if (cellValue.empty()) {
                return "Not Found";
            }
            
            if (cellValue != "DELETED") {
                size_t colonPos = cellValue.find(':');
                if (colonPos != string::npos) {
                    int existingKey = stoi(cellValue.substr(0, colonPos));
                    if (existingKey == key) {
                        return cellValue.substr(colonPos + 1);
                    }
                }
            }
            
            index = (index + step) % capacity;
            i++;
        }
        return "Not Found";
    }

    // Удаление элемента
    void remove(int key) {
        int index = hash1(key);
        int step = hash2(key);
        int i = 0;

        while (i < capacity) {
            string cellValue = table->data[index];
            
            if (cellValue.empty()) {
                cout << "Ключ " << key << " не найден для удаления" << endl;
                return;
            }
            
            if (cellValue != "DELETED") {
                size_t colonPos = cellValue.find(':');
                if (colonPos != string::npos) {
                    int existingKey = stoi(cellValue.substr(0, colonPos));
                    if (existingKey == key) {
                        table->data[index] = "DELETED";
                        size--;
                        cout << "Ключ " << key << " удален из позиции " << index << endl;
                        return;
                    }
                }
            }
            
            index = (index + step) % capacity;
            i++;
        }
        cout << "Ключ " << key << " не найден для удаления" << endl;
    }

    // Получение коэффициента загрузки
    double getLoadFactor() {
        return static_cast<double>(size) / capacity;
    }

    // Реструктуризация таблицы
    void rehash() {
        rehashCount++;
        cout << "\nРеструктуризация таблицы с открытой адресацией" << endl;
        cout << "Старая емкость: " << capacity << " -> Новая емкость: " << capacity * 2 << endl;
        cout << "Коэффициент загрузки: " << getLoadFactor() << endl;

        // Сохраняем старые данные
        SetArray* oldTable = table;
        int oldCapacity = capacity;
        
        // Создаем новую таблицу
        capacity *= 2;
        table = createSet(capacity);
        size = 0;
        
        // Инициализируем новую таблицу
        for (int i = 0; i < capacity; i++) {
            table->data[i] = "";
        }

        // Переносим данные из старой таблицы
        for (int i = 0; i < oldCapacity; i++) {
            string cellValue = oldTable->data[i];
            if (!cellValue.empty() && cellValue != "DELETED") {
                size_t colonPos = cellValue.find(':');
                if (colonPos != string::npos) {
                    int key = stoi(cellValue.substr(0, colonPos));
                    string value = cellValue.substr(colonPos + 1);
                    insert(key, value);
                }
            }
        }
        
        destroySet(oldTable);
        cout << "Реструктуризация завершена!" << endl;
    }

    // Вывод всех элементов
    void printAll() {
        cout << "\nСодержимое таблицы (Открытая адресация)" << endl;
        bool isEmpty = true;
        for (int i = 0; i < capacity; i++) {
            string cellValue = table->data[i];
            if (!cellValue.empty() && cellValue != "DELETED") {
                size_t colonPos = cellValue.find(':');
                if (colonPos != string::npos) {
                    cout << "  Индекс " << i << ": ключ=" << cellValue.substr(0, colonPos) 
                         << ", значение='" << cellValue.substr(colonPos + 1) << "'" << endl;
                    isEmpty = false;
                }
            }
        }
        if (isEmpty) {
            cout << "  Таблица пуста" << endl;
        }
    }

    // Вывод статистики
    void printStats() {
        int deletedCount = 0;
        for (int i = 0; i < capacity; i++) {
            if (table->data[i] == "DELETED") deletedCount++;
        }
        
        cout << "\nСтатистика открытой адресации" << endl;
        cout << "Размер: " << size << endl;
        cout << "Емкость: " << capacity << endl;
        cout << "Коэффициент загрузки: " << getLoadFactor() << endl;
        cout << "Удаленных элементов: " << deletedCount << endl;
        cout << "Количество реструктуризаций: " << rehashCount << endl;
    }
};

// Узел для цепочек
struct ChainNode {
    int key;
    string value;
    ChainNode* next;
    
    ChainNode(int k, const string& v) : key(k), value(v), next(nullptr) {}
};

// Хеш-таблица с методом цепочек (используем наш список)
struct ChainingHashTable {
    ChainNode** table;  // Массив указателей на списки
    int capacity;
    int size;
    double loadFactorThreshold;
    int rehashCount;

    ChainingHashTable(int initialCapacity = 8, double threshold = 0.9) 
        : capacity(initialCapacity), size(0), loadFactorThreshold(threshold), rehashCount(0) {
        table = new ChainNode*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }
    
    ~ChainingHashTable() {
        for (int i = 0; i < capacity; i++) {
            ChainNode* current = table[i];
            while (current != nullptr) {
                ChainNode* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] table;
    }

    // Хеш-функция
    int hash(int key) {
        return key % capacity;
    }

    // Вставка элемента
    void insert(int key, const string& value) {
        if (getLoadFactor() >= loadFactorThreshold) {
            rehash();
        }

        int index = hash(key);
        
        // Проверяем, существует ли уже ключ
        ChainNode* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                cout << "Ключ " << key << " обновлен в корзине " << index << endl;
                return;
            }
            current = current->next;
        }
        
        // Добавляем новый узел в начало списка
        ChainNode* newNode = new ChainNode(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
        cout << "Ключ " << key << " вставлен в корзину " << index << endl;
    }

    // Поиск элемента
    string search(int key) {
        int index = hash(key);
        ChainNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        return "Not Found";
    }

    // Удаление элемента
    void remove(int key) {
        int index = hash(key);
        ChainNode* current = table[index];
        ChainNode* prev = nullptr;
        
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    // Удаляем первый элемент
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                size--;
                cout << "Ключ " << key << " удален из корзины " << index << endl;
                return;
            }
            prev = current;
            current = current->next;
        }
        cout << "Ключ " << key << " не найден для удаления" << endl;
    }

    // Получение коэффициента загрузки
    double getLoadFactor() {
        return static_cast<double>(size) / capacity;
    }

    // Реструктуризация таблицы
    void rehash() {
        rehashCount++;
        cout << "\nРеструктуризация таблицы с цепочками" << endl;
        cout << "Старая емкость: " << capacity << " -> Новая емкость: " << capacity * 2 << endl;
        cout << "Коэффициент загрузки: " << getLoadFactor() << endl;

        // Сохраняем старую таблицу
        ChainNode** oldTable = table;
        int oldCapacity = capacity;
        
        // Создаем новую таблицу
        capacity *= 2;
        table = new ChainNode*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
        size = 0;

        // Переносим элементы из старой таблицы
        for (int i = 0; i < oldCapacity; i++) {
            ChainNode* current = oldTable[i];
            while (current != nullptr) {
                ChainNode* next = current->next;
                // Вставляем в новую таблицу
                int newIndex = hash(current->key);
                current->next = table[newIndex];
                table[newIndex] = current;
                size++;
                
                current = next;
            }
        }
        
        delete[] oldTable;
        cout << "Реструктуризация завершена!" << endl;
    }

    // Вывод всех элементов
    void printAll() {
        cout << "\nСодержимое таблицы (Метод цепочек)" << endl;
        bool isEmpty = true;
        for (int i = 0; i < capacity; i++) {
            if (table[i] != nullptr) {
                cout << "  Корзина " << i << ": ";
                ChainNode* current = table[i];
                int count = 0;
                while (current != nullptr) {
                    cout << "[" << current->key << ":'" << current->value << "] ";
                    current = current->next;
                    count++;
                }
                cout << "(" << count << " элементов)" << endl;
                isEmpty = false;
            }
        }
        if (isEmpty) {
            cout << "  Таблица пуста" << endl;
        }
    }

    // Вывод статистики
    void printStats() {
        cout << "\nСтатистика метода цепочек" << endl;
        cout << "Размер: " << size << endl;
        cout << "Емкость: " << capacity << endl;
        cout << "Коэффициент загрузки: " << getLoadFactor() << endl;
        cout << "Количество реструктуризаций: " << rehashCount << endl;
        
        int maxChain = 0;
        int emptyBuckets = 0;
        for (int i = 0; i < capacity; i++) {
            if (table[i] == nullptr) {
                emptyBuckets++;
            } else {
                int chainLength = 0;
                ChainNode* current = table[i];
                while (current != nullptr) {
                    chainLength++;
                    current = current->next;
                }
                if (chainLength > maxChain) {
                    maxChain = chainLength;
                }
            }
        }
        
        cout << "Пустых корзин: " << emptyBuckets << " (" << (emptyBuckets * 100.0 / capacity) << "%)" << endl;
        cout << "Максимальная длина цепочки: " << maxChain << endl;
    }
};

// Функция для интерактивной работы с хеш-таблицей
void interactiveMode() {
    OpenAddressingHashTable oaHT;
    ChainingHashTable chHT;
    
    int choice;
    
    do {
        cout << "1. Добавить элемент" << endl;
        cout << "2. Найти элемент" << endl;
        cout << "3. Удалить элемент" << endl;
        cout << "4. Показать все элементы" << endl;
        cout << "5. Показать статистику" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                int key;
                string value;
                cout << "Введите ключ (целое число): ";
                cin >> key;
                cout << "Введите значение: ";
                cin.ignore();
                getline(cin, value);
                
                oaHT.insert(key, value);
                chHT.insert(key, value);
                break;
            }
            case 2: {
                int key;
                cout << "Введите ключ для поиска: ";
                cin >> key;
                
                string resultOA = oaHT.search(key);
                string resultCH = chHT.search(key);
                
                cout << "Результат поиска:" << endl;
                cout << "  Открытая адресация: ключ " << key << " -> '" << resultOA << "'" << endl;
                cout << "  Метод цепочек: ключ " << key << " -> '" << resultCH << "'" << endl;
                break;
            }
            case 3: {
                int key;
                cout << "Введите ключ для удаления: ";
                cin >> key;
                
                oaHT.remove(key);
                chHT.remove(key);
                break;
            }
            case 4: {
                oaHT.printAll();
                chHT.printAll();
                break;
            }
            case 5: {
                oaHT.printStats();
                chHT.printStats();
                break;
            }
            case 0:
                cout << "Выход из программы" << endl;
                break;
            default:
                cout << "Неверный выбор! Попробуйте снова." << endl;
        }
    } while (choice != 0);
}

int main() {
    cout << "Хеш-таблицы с собственными структурами" << endl;
    
    interactiveMode();
    
    return 0;
}