#ifndef STRUCTURES_FROM_LR1_H
#define STRUCTURES_FROM_LR1_H

#include <string>
using namespace std;
//стек
struct StackNode {
    string data;
    StackNode* next;
};

struct Stack {
    StackNode* top;
    int size;
};

Stack* createStack();
void destroyStack(Stack* stack);
void push(Stack* stack, const string& value);
string pop(Stack* stack);
string peek(Stack* stack);
bool isEmptyStack(Stack* stack);

//множество
struct SetArray {
    string* data;
    int size;
    int capacity;
};

SetArray* createSet(int initialCapacity);
void destroySet(SetArray* set);
void setInsert(SetArray* set, const string& value);
bool setContains(SetArray* set, const string& value);
void setRemove(SetArray* set, const string& value);

//список
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

//хеш
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