#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

struct Node {
    string query;
    string result;
    Node* prev;
    Node* next;

    Node(string q, string r) : query(q), result(r), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void moveToFront(Node* node) {
        if (node == head) return;

        // Remove node
        if (node->prev) node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;

        if (node == tail) tail = node->prev;

        // Move to front
        node->prev = nullptr;
        node->next = head;

        if (head) head->prev = node;
        head = node;

        if (tail == nullptr) tail = head;
    }

    void appendToFront(Node* node) {
        node->prev = nullptr;
        node->next = head;

        if (head) head->prev = node;
        head = node;

        if (!tail) tail = head;
    }

    void removeFromTail() {
        if (!tail) return;

        Node* oldTail = tail;
        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            // Only one node
            head = tail = nullptr;
        }

        delete oldTail;
    }

    Node* getTail() {
        return tail;
    }
};

class LRUCache {
private:
    int MAX_SIZE;
    int size;
    unordered_map<string, Node*> lookup;
    DoublyLinkedList list;

public:
    LRUCache(int maxSize) : MAX_SIZE(maxSize), size(0) {}

    string get(string query) {
        if (lookup.find(query) == lookup.end()) {
            return "";  // cache miss
        }
        Node* node = lookup[query];
        list.moveToFront(node);
        return node->result;
    }

    void set(string query, string result) {
        if (lookup.find(query) != lookup.end()) {
            // Update existing entry
            Node* node = lookup[query];
            node->result = result;
            list.moveToFront(node);
        } else {
            // New entry
            if (size == MAX_SIZE) {
                Node* oldTail = list.getTail();
                if (oldTail) {
                    lookup.erase(oldTail->query);
                    list.removeFromTail();
                    size--;
                }
            }

            Node* newNode = new Node(query, result);
            list.appendToFront(newNode);
            lookup[query] = newNode;
            size++;
        }
    }
};
