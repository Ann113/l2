#include "structures_from_lr1.h"
#include <iostream>
#include <algorithm>
using namespace std;

//стек
Stack* createStack() {
    Stack* stack = new Stack;
    stack->top = nullptr;
    stack->size = 0;
    return stack;
}

void destroyStack(Stack* stack) {
    while (stack->top != nullptr) {
        StackNode* temp = stack->top;
        stack->top = stack->top->next;
        delete temp;
    }
    delete stack;
}

void push(Stack* stack, const string& value) {
    StackNode* newNode = new StackNode;
    newNode->data = value;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

string pop(Stack* stack) {
    if (stack->top == nullptr) {
        return "";
    }
    StackNode* temp = stack->top;
    string value = temp->data;
    stack->top = stack->top->next;
    delete temp;
    stack->size--;
    return value;
}

string peek(Stack* stack) {
    if (stack->top == nullptr) {
        return "";
    }
    return stack->top->data;
}

bool isEmptyStack(Stack* stack) {
    return stack->top == nullptr;
}

//множество
SetArray* createSet(int initialCapacity = 10) {
    SetArray* set = new SetArray;
    set->capacity = initialCapacity;
    set->size = 0;
    set->data = new string[set->capacity];
    return set;
}

void destroySet(SetArray* set) {
    delete[] set->data;
    delete set;
}

void resizeSet(SetArray* set) {
    int newCapacity = set->capacity * 2;
    string* newData = new string[newCapacity];
    
    for (int i = 0; i < set->size; i++) {
        newData[i] = set->data[i];
    }
    
    delete[] set->data;
    set->data = newData;
    set->capacity = newCapacity;
}

void setInsert(SetArray* set, const string& value) {
    // Проверяем, есть ли уже такой элемент
    for (int i = 0; i < set->size; i++) {
        if (set->data[i] == value) {
            return; // Уже существует
        }
    }
    
    // Добавляем новый элемент
    if (set->size >= set->capacity) {
        resizeSet(set);
    }
    
    set->data[set->size] = value;
    set->size++;
}

bool setContains(SetArray* set, const string& value) {
    for (int i = 0; i < set->size; i++) {
        if (set->data[i] == value) {
            return true;
        }
    }
    return false;
}

void setRemove(SetArray* set, const string& value) {
    for (int i = 0; i < set->size; i++) {
        if (set->data[i] == value) {
            // Сдвигаем все элементы после i на одну позицию влево
            for (int j = i; j < set->size - 1; j++) {
                set->data[j] = set->data[j + 1];
            }
            set->size--;
            return;
        }
    }
}

//для списка
ListNode* createListNode(int key, int value) {
    ListNode* node = new ListNode;
    node->key = key;
    node->value = value;
    node->prev = nullptr;
    node->next = nullptr;
    return node;
}

List* createList() {
    List* list = new List;
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
    return list;
}

void destroyList(List* list) {
    ListNode* current = list->head;
    while (current != nullptr) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
    delete list;
}

void addToFront(List* list, ListNode* node) {
    node->next = list->head;
    node->prev = nullptr;
    
    if (list->head != nullptr) {
        list->head->prev = node;
    }
    list->head = node;
    
    if (list->tail == nullptr) {
        list->tail = node;
    }
    list->size++;
}

void removeNode(List* list, ListNode* node) {
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    list->size--;
}

void moveToFront(List* list, ListNode* node) {
    removeNode(list, node);
    addToFront(list, node);
}

//хеш
HashTable* createHashTable(int capacity) {
    HashTable* ht = new HashTable;
    ht->capacity = capacity;
    ht->size = 0;
    ht->table = new HashEntry[capacity];
    for (int i = 0; i < capacity; i++) {
        ht->table[i].occupied = false;
    }
    return ht;
}

void destroyHashTable(HashTable* ht) {
    delete[] ht->table;
    delete ht;
}

int hashFunction(int key, int capacity) {
    return abs(key) % capacity;
}

void hashInsert(HashTable* ht, int key, ListNode* value) {
    int index = hashFunction(key, ht->capacity);
    int startIndex = index;
    
    // Линейное пробирование
    while (ht->table[index].occupied) {
        if (ht->table[index].key == key) {
            ht->table[index].value = value;
            return;
        }
        index = (index + 1) % ht->capacity;
        if (index == startIndex) {
            // Таблица полна (на практике нужно рехеширование)
            return;
        }
    }
    
    ht->table[index].key = key;
    ht->table[index].value = value;
    ht->table[index].occupied = true;
    ht->size++;
}

ListNode* hashFind(HashTable* ht, int key) {
    int index = hashFunction(key, ht->capacity);
    int startIndex = index;
    
    while (ht->table[index].occupied) {
        if (ht->table[index].key == key) {
            return ht->table[index].value;
        }
        index = (index + 1) % ht->capacity;
        if (index == startIndex) {
            break;
        }
    }
    
    return nullptr;
}

void hashRemove(HashTable* ht, int key) {
    int index = hashFunction(key, ht->capacity);
    int startIndex = index;
    
    while (ht->table[index].occupied) {
        if (ht->table[index].key == key) {
            ht->table[index].occupied = false;
            ht->size--;
            
            // Рехеширование для линейного пробирования
            int nextIndex = (index + 1) % ht->capacity;
            while (ht->table[nextIndex].occupied) {
                int currentKey = ht->table[nextIndex].key;
                ListNode* currentValue = ht->table[nextIndex].value;
                ht->table[nextIndex].occupied = false;
                ht->size--;
                hashInsert(ht, currentKey, currentValue);
                nextIndex = (nextIndex + 1) % ht->capacity;
            }
            return;
        }
        index = (index + 1) % ht->capacity;
        if (index == startIndex) {
            break;
        }
    }
}