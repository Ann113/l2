#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

// Функция для проверки корректности выражения
bool isValidExpression(const string& expr) {
    stack<char> parentheses;
    bool lastWasOp = true; // true если последним был оператор (в начале выражения это true)
    bool lastWasUnary = false;

    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        if (c == ' ') continue;
        
        if (isdigit(c)) {
            if (c != '0' && c != '1') {
                return false; // Только 0 и 1 разрешены
            }
            if (!lastWasOp && !lastWasUnary) {
                return false; // Два числа подряд
            }
            lastWasOp = false;
            lastWasUnary = false;
        }
        else if (c == '(') {
            if (!lastWasOp && !lastWasUnary) {
                return false; // Перед открывающей скобкой должно быть что-то
            }
            parentheses.push('(');
            lastWasOp = true;
            lastWasUnary = false;
        }
        else if (c == ')') {
            if (parentheses.empty() || lastWasOp) {
                return false; // Непарная скобка или оператор перед закрывающей скобкой
            }
            parentheses.pop();
            lastWasOp = false;
            lastWasUnary = false;
        }
        else if (c == '!') {
            lastWasOp = true;
            lastWasUnary = true;
        }
        else if (c == '&' || c == '|' || c == '^') {
            if (lastWasOp && !lastWasUnary) {
                return false; // Два бинарных оператора подряд
            }
            lastWasOp = true;
            lastWasUnary = false;
        }
        else {
            return false; // Недопустимый символ
        }
    }
    
    if (lastWasOp && !lastWasUnary) {
        return false; // Выражение заканчивается оператором
    }
    
    return parentheses.empty(); // Все скобки должны быть закрыты
}

int priority(char op) {
    if (op == '!') return 3;
    if (op == '&') return 2;
    if (op == '|' || op == '^') return 1;
    return 0;
}

bool applyOp(bool a, bool b, char op) {
    switch(op) {
        case '&': return a && b;
        case '|': return a || b;
        case '^': return a != b;
        default: return false;
    }
}

bool eval(const string& expr) {
    stack<bool> values;
    stack<char> ops;
    bool expectOperand = true; // Ожидаем операнд в начале

    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == ' ') continue;

        if (isdigit(expr[i])) {
            if (!expectOperand) {
                throw invalid_argument("Неправильная позиция операнда");
            }
            values.push(expr[i] == '1');
            expectOperand = false;
        }
        else if (expr[i] == '(') {
            if (!expectOperand) {
                throw invalid_argument("Неправильная позиция открывающей скобки");
            }
            ops.push('(');
            expectOperand = true;
        }
        else if (expr[i] == ')') {
            if (expectOperand) {
                throw invalid_argument("Неправильная позиция закрывающей скобки");
            }
            while (!ops.empty() && ops.top() != '(') {
                char op = ops.top(); ops.pop();
                if (op == '!') {
                    if (values.empty()) throw invalid_argument("Недостаточно операндов для !");
                    bool val = values.top(); values.pop();
                    values.push(!val);
                } else {
                    if (values.size() < 2) throw invalid_argument("Недостаточно операндов для бинарной операции");
                    bool b = values.top(); values.pop();
                    bool a = values.top(); values.pop();
                    values.push(applyOp(a, b, op));
                }
            }
            if (ops.empty()) {
                throw invalid_argument("Непарная закрывающая скобка");
            }
            ops.pop(); // удаляем '('
            expectOperand = false;
        }
        else {
            // Обработка операторов
            if (expr[i] == '!') {
                ops.push('!');
                expectOperand = true;
            } else {
                // Бинарный оператор
                if (expectOperand) {
                    throw invalid_argument("Неправильная позиция бинарного оператора");
                }
                
                while (!ops.empty() && ops.top() != '(' && 
                       priority(ops.top()) >= priority(expr[i])) {
                    char op = ops.top(); ops.pop();
                    if (op == '!') {
                        if (values.empty()) throw invalid_argument("Недостаточно операндов для !");
                        bool val = values.top(); values.pop();
                        values.push(!val);
                    } else {
                        if (values.size() < 2) throw invalid_argument("Недостаточно операндов для бинарной операции");
                        bool b = values.top(); values.pop();
                        bool a = values.top(); values.pop();
                        values.push(applyOp(a, b, op));
                    }
                }
                ops.push(expr[i]);
                expectOperand = true;
            }
        }
    }

    // Обработка оставшихся операций
    while (!ops.empty()) {
        char op = ops.top(); ops.pop();
        if (op == '!') {
            if (values.empty()) throw invalid_argument("Недостаточно операндов для !");
            bool val = values.top(); values.pop();
            values.push(!val);
        } else {
            if (values.size() < 2) throw invalid_argument("Недостаточно операндов для бинарной операции");
            bool b = values.top(); values.pop();
            bool a = values.top(); values.pop();
            values.push(applyOp(a, b, op));
        }
    }

    if (values.size() != 1) {
        throw invalid_argument("Некорректное выражение");
    }

    return values.top();
}

int main() {
    cout << "Вычисление логического выражения" << endl;
    cout << "Поддерживаемые операции:" << endl;
    cout << "  ! - отрицание (высший приоритет)" << endl;
    cout << "  & - логическое И" << endl;
    cout << "  | - логическое ИЛИ" << endl;
    cout << "  ^ - исключающее ИЛИ (низший приоритет)" << endl;
    cout << "Операнды: 0 (ложь) и 1 (истина)" << endl;
    cout << endl;
    
    while (true) {
        cout << "Введите логическое выражение (или 'exit' для выхода): ";
        string expr;
        getline(cin, expr);
        
        if (expr == "exit" || expr == "выход") {
            break;
        }
        
        if (expr.empty()) {
            continue;
        }
        
        try {
            // Проверка корректности выражения
            if (!isValidExpression(expr)) {
                throw invalid_argument("Некорректное выражение");
            }
            
            bool result = eval(expr);
            cout << "Результат: " << result << " (" << (result ? "истина" : "ложь") << ")" << endl;
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
            cout << "Пожалуйста, проверьте правильность ввода выражения" << endl;
        }
        
        cout << endl;
    }
    
    cout << "Программа завершена." << endl;
    return 0;
}