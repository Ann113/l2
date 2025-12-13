#ifndef STRUCTURES_FROM_LR1_H
#define STRUCTURES_FROM_LR1_H

#include <string>

// ========== СТЕК ==========
struct StackNode {
    std::string data;
    StackNode* next;
};

struct Stack {
    StackNode* top;
    int size;
};

Stack* createStack();
void destroyStack(Stack* stack);
void push(Stack* stack, const std::string& value);
std::string pop(Stack* stack);
std::string peek(Stack* stack);
bool isEmptyStack(Stack* stack);

// ========== МНОЖЕСТВО ==========
struct SetArray {
    std::string* data;
    int size;
    int capacity;
};

SetArray* createSet(int initialCapacity);
void destroySet(SetArray* set);
void setInsert(SetArray* set, const std::string& value);
bool setContains(SetArray* set, const std::string& value);
void setRemove(SetArray* set, const std::string& value);

// ========== СПИСОК для LRU ==========
struct ListNode {
    int key;
    int value;
    ListNode* prev;
    ListNode* next;
};

struct List {
    ListNode* head;
    ListNode* tail;
    int size;
};

ListNode* createListNode(int key, int value);
List* createList();
void destroyList(List* list);
void addToFront(List* list, ListNode* node);
void removeNode(List* list, ListNode* node);
void moveToFront(List* list, ListNode* node);

// ========== ХЕШ-ТАБЛИЦА для LRU ==========
struct HashEntry {
    int key;
    ListNode* value;
    bool occupied;
};

struct HashTable {
    HashEntry* table;
    int capacity;
    int size;
};

HashTable* createHashTable(int capacity);
void destroyHashTable(HashTable* ht);
void hashInsert(HashTable* ht, int key, ListNode* value);
ListNode* hashFind(HashTable* ht, int key);
void hashRemove(HashTable* ht, int key);

#endif