#pragma once

#include <stdexcept>
#include <vector>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& data) : data(data), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void append(const T& data) {
        Node* newNode = new Node(data);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        ++size;
    }


    T& getByIndex(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    const T& getByIndex(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }



    void removeByIndex(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
            if (!head) {
                tail = nullptr;
            }
        } else {
            Node* current = head;
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            Node* temp = current->next;
            current->next = temp->next;
            if (temp == tail) {
                tail = current;
            }
            delete temp;
        }
        --size;
    }

    int getLength() const {
        return size;
    }
};