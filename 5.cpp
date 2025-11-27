#include <iostream>
#include <algorithm>
#include <climits>
#include <limits>

using namespace std;

struct Node {
    int value;
    Node* left;
    Node* right;
    Node(int v) : value(v), left(nullptr), right(nullptr) {}
};

Node* insert(Node* root, int value) {
    if (!root) return new Node(value);
    if (value < root->value)
        root->left = insert(root->left, value);
    else
        root->right = insert(root->right, value);
    return root;
}

int checkBalanced(Node* root, bool& isBalanced) {
    if (!root || !isBalanced) return 0;
    
    int leftHeight = checkBalanced(root->left, isBalanced);
    int rightHeight = checkBalanced(root->right, isBalanced);
    
    if (abs(leftHeight - rightHeight) > 1) {
        isBalanced = false;
    }
    
    return 1 + max(leftHeight, rightHeight);
}

bool isBalancedOptimized(Node* root) {
    if (!root) return true; // Пустое дерево считается сбалансированным
    bool balanced = true;
    checkBalanced(root, balanced);
    return balanced;
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Функция для ввода числа
bool safeInput(int& value) {
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// Функция для проверки выделения памяти
Node* createNode(int value) {
    Node* newNode = nullptr;
    try {
        newNode = new Node(value);
    } catch (const bad_alloc& e) {
        cerr << "Ошибка выделения памяти: " << e.what() << endl;
        return nullptr;
    }
    return newNode;
}

Node* insertWithCheck(Node* root, int value) {
    if (!root) {
        return createNode(value);
    }
    
    Node* newNode = nullptr;
    if (value < root->value) {
        newNode = insertWithCheck(root->left, value);
        if (!newNode) return nullptr; // Ошибка выделения памяти
        root->left = newNode;
    } else {
        newNode = insertWithCheck(root->right, value);
        if (!newNode) return nullptr; // Ошибка выделения памяти
        root->right = newNode;
    }
    return root;
}

int main() {
    Node* root = nullptr;
    int x;
    bool inputError = false;
    
    cout << "Введите последовательность целых чисел (0 для окончания ввода):" << endl;
    
    // Ввод последовательности с проверкой ошибок
    while (true) {
        if (!safeInput(x)) {
            cerr << "Ошибка: введено нечисловое значение. Пожалуйста, введите целое число." << endl;
            inputError = true;
            continue;
        }
        
        if (x == 0) {
            break; // Конец ввода
        }
        
        Node* newRoot = insertWithCheck(root, x);
        if (!newRoot) {
            cerr << "Невозможно выделить память для нового узла." << endl;
            freeTree(root);
            return 1;
        }
        root = newRoot;
    }
    
    // Проверка, был ли ввод вообще
    if (inputError && !root) {
        cerr << "Ошибка: не удалось корректно ввести последовательность." << endl;
        return 1;
    }
    
    // Проверка сбалансированности и вывод результата
    bool balanced = isBalancedOptimized(root);
    cout << (balanced ? "YES" : "NO") << endl;
    
    // Освобождение памяти
    freeTree(root);
    root = nullptr;
    
    return 0;
}