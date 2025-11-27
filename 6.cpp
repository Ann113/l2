#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <string>

using namespace std;

// Структура для элемента хеш-таблицы с открытой адресацией
struct HashItem {
    int key;
    string value;
    bool isDeleted;
    
    HashItem() : key(-1), value(""), isDeleted(false) {}
    HashItem(int k, const string& v) : key(k), value(v), isDeleted(false) {}
};

// Хеш-таблица с открытой адресацией
struct OpenAddressingHashTable {
    vector<HashItem> table;
    int capacity;
    int size;
    double loadFactorThreshold;
    int rehashCount;

    OpenAddressingHashTable(int initialCapacity = 8, double threshold = 0.9) 
        : capacity(initialCapacity), size(0), loadFactorThreshold(threshold), rehashCount(0) {
        table.resize(capacity);
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
            if (table[index].key == -1 || table[index].isDeleted) {
                table[index] = HashItem(key, value);
                size++;
                cout << "Ключ " << key << " вставлен в позицию " << index << endl;
                return;
            }
            if (table[index].key == key) {
                table[index].value = value;
                cout << "Ключ " << key << " обновлен в позиции " << index << endl;
                return;
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

        while (i < capacity && table[index].key != -1) {
            if (!table[index].isDeleted && table[index].key == key) {
                return table[index].value;
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

        while (i < capacity && table[index].key != -1) {
            if (!table[index].isDeleted && table[index].key == key) {
                table[index].isDeleted = true;
                size--;
                cout << "Ключ " << key << " удален из позиции " << index << endl;
                return;
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
        cout << "\n Реструктуризация таблицы с открытой адресацией" << endl;
        cout << "Старая емкость: " << capacity << " -> Новая емкость: " << capacity * 2 << endl;
        cout << "Коэффициент загрузки: " << getLoadFactor() << endl;

        vector<HashItem> oldTable = table;
        int oldCapacity = capacity;
        
        capacity *= 2;
        table.clear();
        table.resize(capacity);
        size = 0;

        for (int i = 0; i < oldCapacity; i++) {
            if (oldTable[i].key != -1 && !oldTable[i].isDeleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
        cout << "Реструктуризация завершена!" << endl;
    }

    // Вывод всех элементов
    void printAll() {
        cout << "\nСодержимое таблицы (Открытая адресация)" << endl;
        bool isEmpty = true;
        for (int i = 0; i < capacity; i++) {
            if (table[i].key != -1 && !table[i].isDeleted) {
                cout << "  Индекс " << i << ": ключ=" << table[i].key 
                     << ", значение='" << table[i].value << "'" << endl;
                isEmpty = false;
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
            if (table[i].isDeleted) deletedCount++;
        }
        
        cout << "\nСтатистика открытой адресации" << endl;
        cout << "Размер: " << size << endl;
        cout << "Емкость: " << capacity << endl;
        cout << "Коэффициент загрузки: " << getLoadFactor() << endl;
        cout << "Удаленных элементов: " << deletedCount << endl;
        cout << "Количество реструктуризаций: " << rehashCount << endl;
    }
};

// Хеш-таблица с методом цепочек
struct ChainingHashTable {
    vector<list<pair<int, string>>> table;
    int capacity;
    int size;
    double loadFactorThreshold;
    int rehashCount;

    ChainingHashTable(int initialCapacity = 8, double threshold = 0.9) 
        : capacity(initialCapacity), size(0), loadFactorThreshold(threshold), rehashCount(0) {
        table.resize(capacity);
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
        for (auto& item : table[index]) {
            if (item.first == key) {
                item.second = value;
                cout << "Ключ " << key << " обновлен в корзине " << index << endl;
                return;
            }
        }
        table[index].push_back(make_pair(key, value));
        size++;
        cout << "Ключ " << key << " вставлен в корзину " << index << endl;
    }

    // Поиск элемента
    string search(int key) {
        int index = hash(key);
        for (const auto& item : table[index]) {
            if (item.first == key) {
                return item.second;
            }
        }
        return "Not Found";
    }

    // Удаление элемента
    void remove(int key) {
        int index = hash(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                table[index].erase(it);
                size--;
                cout << "Ключ " << key << " удален из корзины " << index << endl;
                return;
            }
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

        vector<list<pair<int, string>>> oldTable = table;
        int oldCapacity = capacity;
        
        capacity *= 2;
        table.clear();
        table.resize(capacity);
        size = 0;

        for (int i = 0; i < oldCapacity; i++) {
            for (const auto& item : oldTable[i]) {
                int newIndex = hash(item.first);
                table[newIndex].push_back(item);
                size++;
            }
        }
        cout << "Реструктуризация завершена!" << endl;
    }

    // Вывод всех элементов
    void printAll() {
        cout << "\nСодержимое таблицы (Метод цепочек)" << endl;
        bool isEmpty = true;
        for (int i = 0; i < capacity; i++) {
            if (!table[i].empty()) {
                cout << "  Корзина " << i << " (" << table[i].size() << " элементов): ";
                for (const auto& item : table[i]) {
                    cout << "[" << item.first << ":'" << item.second << "] ";
                }
                cout << endl;
                isEmpty = false;
            }
        }
        if (isEmpty) {
            cout << "  Таблица пуста" << endl;
        }
    }

    // Вывод статистики
    void printStats() {
        cout << "\nСтатистика метода цепочек " << endl;
        cout << "Размер: " << size << endl;
        cout << "Емкость: " << capacity << endl;
        cout << "Коэффициент загрузки: " << getLoadFactor() << endl;
        cout << "Количество реструктуризаций: " << rehashCount << endl;
        
        int maxChain = 0;
        int emptyBuckets = 0;
        for (int i = 0; i < capacity; i++) {
            if (table[i].empty()) {
                emptyBuckets++;
            }
            if (table[i].size() > maxChain) {
                maxChain = table[i].size();
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
    
    interactiveMode();
    
    return 0;
}