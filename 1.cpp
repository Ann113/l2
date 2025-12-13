#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include "structures_from_lr1.h"

using namespace std;

// Стек для булевых значений
struct BoolStackNode {
    bool data;
    BoolStackNode* next;
};

struct BoolStack {
    BoolStackNode* top;
    int size;
    
    BoolStack() : top(nullptr), size(0) {}
};

void boolPush(BoolStack* stack, bool value) {
    BoolStackNode* newNode = new BoolStackNode;
    newNode->data = value;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

bool boolPop(BoolStack* stack) {
    if (stack->top == nullptr) {
        throw "Stack is empty";
    }
    BoolStackNode* temp = stack->top;
    bool value = temp->data;
    stack->top = stack->top->next;
    delete temp;
    stack->size--;
    return value;
}

bool boolPeek(BoolStack* stack) {
    if (stack->top == nullptr) {
        throw "Stack is empty";
    }
    return stack->top->data;
}

bool boolIsEmpty(BoolStack* stack) {
    return stack->top == nullptr;
}

// Функция для проверки корректности выражения
bool isValidExpression(const string& expr) {
    BoolStack parentheses;
    bool lastWasOp = true;
    bool lastWasUnary = false;

    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        if (c == ' ') continue;
        
        if (isdigit(c)) {
            if (c != '0' && c != '1') {
                return false;
            }
            if (!lastWasOp && !lastWasUnary) {
                return false;
            }
            lastWasOp = false;
            lastWasUnary = false;
        }
        else if (c == '(') {
            if (!lastWasOp && !lastWasUnary) {
                return false;
            }
            boolPush(&parentheses, true);
            lastWasOp = true;
            lastWasUnary = false;
        }
        else if (c == ')') {
            if (boolIsEmpty(&parentheses) || lastWasOp) {
                return false;
            }
            boolPop(&parentheses);
            lastWasOp = false;
            lastWasUnary = false;
        }
        else if (c == '!') {
            lastWasOp = true;
            lastWasUnary = true;
        }
        else if (c == '&' || c == '|' || c == '^') {
            if (lastWasOp && !lastWasUnary) {
                return false;
            }
            lastWasOp = true;
            lastWasUnary = false;
        }
        else {
            return false;
        }
    }
    
    if (lastWasOp && !lastWasUnary) {
        return false;
    }
    
    return boolIsEmpty(&parentheses);
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
    BoolStack values;
    // Для операторов будем использовать стек из ЛР1
    Stack* ops = createStack();
    bool expectOperand = true;

    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == ' ') continue;

        if (isdigit(expr[i])) {
            if (!expectOperand) {
                throw invalid_argument("Неправильная позиция операнда");
            }
            boolPush(&values, expr[i] == '1');
            expectOperand = false;
        }
        else if (expr[i] == '(') {
            if (!expectOperand) {
                throw invalid_argument("Неправильная позиция открывающей скобки");
            }
            push(ops, "(");
            expectOperand = true;
        }
        else if (expr[i] == ')') {
            if (expectOperand) {
                throw invalid_argument("Неправильная позиция закрывающей скобки");
            }
            
            while (!isEmptyStack(ops) && peek(ops) != "(") {
                string opStr = pop(ops);
                char op = opStr[0];
                
                if (op == '!') {
                    if (boolIsEmpty(&values)) throw invalid_argument("Недостаточно операндов для !");
                    bool val = boolPop(&values);
                    boolPush(&values, !val);
                } else {
                    if (values.size < 2) throw invalid_argument("Недостаточно операндов для бинарной операции");
                    bool b = boolPop(&values);
                    bool a = boolPop(&values);
                    boolPush(&values, applyOp(a, b, op));
                }
            }
            
            if (isEmptyStack(ops)) {
                throw invalid_argument("Непарная закрывающая скобка");
            }
            pop(ops); // удаляем '('
            expectOperand = false;
        }
        else if (expr[i] == '!') {
            push(ops, "!");
            expectOperand = true;
        }
        else {
            // Бинарный оператор
            if (expectOperand) {
                throw invalid_argument("Неправильная позиция бинарного оператора");
            }
            
            char currentOp = expr[i];
            int currentPriority = priority(currentOp);
            
            while (!isEmptyStack(ops) && peek(ops) != "(" && 
                   priority(peek(ops)[0]) >= currentPriority) {
                string opStr = pop(ops);
                char op = opStr[0];
                
                if (op == '!') {
                    if (boolIsEmpty(&values)) throw invalid_argument("Недостаточно операндов для !");
                    bool val = boolPop(&values);
                    boolPush(&values, !val);
                } else {
                    if (values.size < 2) throw invalid_argument("Недостаточно операндов для бинарной операции");
                    bool b = boolPop(&values);
                    bool a = boolPop(&values);
                    boolPush(&values, applyOp(a, b, op));
                }
            }
            
            string opStr(1, currentOp);
            push(ops, opStr);
            expectOperand = true;
        }
    }

    // Обработка оставшихся операций
    while (!isEmptyStack(ops)) {
        string opStr = pop(ops);
        char op = opStr[0];
        
        if (op == '!') {
            if (boolIsEmpty(&values)) throw invalid_argument("Недостаточно операндов для !");
            bool val = boolPop(&values);
            boolPush(&values, !val);
        } else {
            if (values.size < 2) throw invalid_argument("Недостаточно операндов для бинарной операции");
            bool b = boolPop(&values);
            bool a = boolPop(&values);
            boolPush(&values, applyOp(a, b, op));
        }
    }

    if (values.size != 1) {
        throw invalid_argument("Некорректное выражение");
    }

    destroyStack(ops);
    return boolPop(&values);
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
        catch (const char* msg) {
            cout << "Ошибка: " << msg << endl;
        }
        
        cout << endl;
    }
    
    cout << "Программа завершена." << endl;
    return 0;
}