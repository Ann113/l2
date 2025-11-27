#include <iostream>
#include <string>
#include <unordered_set>
#include <fstream>
#include <filesystem>

using namespace std;

class SimpleSet {
private:
    unordered_set<string> elements;
    
public:
    void SETADD(const string& element) {
        if (element.empty()) {
            cerr << "Ошибка: Попытка добавить пустой элемент" << endl;
            return;
        }
        elements.insert(element);
    }
    
    void SETDEL(const string& element) {
        if (element.empty()) {
            cerr << "Ошибка: Попытка удалить пустой элемент" << endl;
            return;
        }
        elements.erase(element);
    }
    
    bool SET_AT(const string& element) {
        if (element.empty()) {
            cerr << "Ошибка: Попытка проверить пустой элемент" << endl;
            return false;
        }
        return elements.find(element) != elements.end();
    }
    
    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка: Не удалось открыть файл " << filename << " для чтения" << endl;
            return false;
        }
        
        elements.clear();
        string element;
        int count = 0;
        while (file >> element) {
            if (!element.empty()) {
                elements.insert(element);
                count++;
            }
        }
        
        cout << "Загружено " << count << " элементов из файла " << filename << endl;
        return true;
    }
    
    bool saveToFile(const string& filename) {
        string temp_filename = filename + ".tmp";
        ofstream file(temp_filename);
        
        if (!file.is_open()) {
            cerr << "Ошибка: Не удалось открыть файл " << temp_filename << " для записи" << endl;
            return false;
        }
        
        for (const auto& element : elements) {
            file << element << endl;
        }
        
        file.close();
        
        try {
            filesystem::rename(temp_filename, filename);
            cout << "Сохранено " << elements.size() << " элементов в файл " << filename << endl;
            return true;
        } catch (const filesystem::filesystem_error& e) {
            cerr << "Ошибка при сохранении файла: " << e.what() << endl;
            return false;
        }
    }
    
    size_t size() const {
        return elements.size();
    }
};

void printUsage(const string& programName) {
    cout << "Использование: " << programName << " --file <имя файла> --query <команда:элемент>" << endl;
    cout << "Примеры:" << endl;
    cout << "  " << programName << " --file data.txt --query SETADD:apple" << endl;
    cout << "  " << programName << " --file data.txt --query SET_AT:apple" << endl;
    cout << "  " << programName << " --file data.txt --query SETDEL:apple" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Ошибка: Неверное количество аргументов" << endl;
        printUsage(argv[0]);
        return 1;
    }
    
    string filename, query;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }
    
    if (filename.empty() || query.empty()) {
        cerr << "Ошибка: Не указаны обязательные аргументы --file или --query" << endl;
        printUsage(argv[0]);
        return 1;
    }
    
    size_t colon_pos = query.find(':');
    if (colon_pos == string::npos || colon_pos == 0 || colon_pos == query.length() - 1) {
        cerr << "Ошибка: Неверный формат запроса. Используйте КОМАНДА:ЭЛЕМЕНТ" << endl;
        printUsage(argv[0]);
        return 1;
    }
    
    string command = query.substr(0, colon_pos);
    string value = query.substr(colon_pos + 1);
    
    if (value.empty()) {
        cerr << "Ошибка: Пустое значение элемента" << endl;
        return 1;
    }
    
    SimpleSet set;
    
    if (!filesystem::exists(filename)) {
        cout << "Файл " << filename << " не существует. Будет создан новый." << endl;
    } else {
        if (!set.loadFromFile(filename)) {
            cerr << "Ошибка: Не удалось загрузить данные из файла" << endl;
            return 1;
        }
    }
    
    bool success = true;
    
    if (command == "SETADD") {
        cout << "Добавление элемента: '" << value << "'" << endl;
        set.SETADD(value);
        success = set.saveToFile(filename);
        if (success) {
            cout << "Элемент успешно добавлен. Всего элементов: " << set.size() << endl;
        }
    } 
    else if (command == "SETDEL") {
        cout << "Удаление элемента: '" << value << "'" << endl;
        set.SETDEL(value);
        success = set.saveToFile(filename);
        if (success) {
            cout << "Элемент удален. Всего элементов: " << set.size() << endl;
        }
    }
    else if (command == "SET_AT") {
        cout << "Проверка наличия элемента: '" << value << "'" << endl;
        bool exists = set.SET_AT(value);
        cout << "Результат: " << (exists ? "true" : "false") << endl;
    }
    else {
        cerr << "Ошибка: Неизвестная команда: " << command << endl;
        cout << "Доступные команды: SETADD, SETDEL, SET_AT" << endl;
        return 1;
    }
    
    if (!success) {
        cerr << "Ошибка при выполнении операции" << endl;
        return 1;
    }
    
    return 0;
}