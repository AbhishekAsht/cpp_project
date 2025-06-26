#pragma once

#define LIST_INTERFACE_H

template <typename T>
class IList {
public:
    virtual ~IList() = default;

    // Capacity
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;

    // Element access
    virtual T& front() = 0;
    virtual const T& front() const = 0;
    virtual T& back() = 0;
    virtual const T& back() const = 0;

    // Modifiers
    virtual void push_front(const T& value) = 0;
    virtual void push_back(const T& value) = 0;
    virtual void pop_front() = 0;
    virtual void pop_back() = 0;
    virtual void clear() = 0;
};

template <typename T>
class ListNode {
public:
    T data;
    ListNode* next;
    ListNode* prev;

    ListNode(const T& val) : data(val), next(nullptr), prev(nullptr) {}
};

template <typename T>
class CustomList : public IList<T> {
private:
    ListNode<T>* head = nullptr;
    ListNode<T>* tail = nullptr;
    size_t list_size = 0;

public:
    CustomList() = default;

    ~CustomList() {
        clear();
    }

    // Copy constructor
    CustomList(const CustomList& other) {
        ListNode<T>* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

    // Move constructor
    CustomList(CustomList&& other) noexcept
        : head(other.head), tail(other.tail), list_size(other.list_size) {
        other.head = nullptr;
        other.tail = nullptr;
        other.list_size = 0;
    }

    // Assignment operators
    CustomList& operator=(const CustomList& other) {
        if (this != &other) {
            clear();
            ListNode<T>* current = other.head;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    CustomList& operator=(CustomList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            list_size = other.list_size;
            other.head = nullptr;
            other.tail = nullptr;
            other.list_size = 0;
        }
        return *this;
    }

    // IList implementation
    size_t size() const override { return list_size; }
    bool empty() const override { return list_size == 0; }

    T& front() override { return head->data; }
    const T& front() const override { return head->data; }
    T& back() override { return tail->data; }
    const T& back() const override { return tail->data; }

    void push_front(const T& value) override {
        ListNode<T>* new_node = new ListNode<T>(value);

        if (head == nullptr) {
            head = tail = new_node;
        }
        else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        list_size++;
    }

    void push_back(const T& value) override {
        ListNode<T>* new_node = new ListNode<T>(value);

        if (tail == nullptr) {
            head = tail = new_node;
        }
        else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        list_size++;
    }

    void pop_front() override {
        if (head == nullptr) return;

        ListNode<T>* temp = head;
        head = head->next;

        if (head != nullptr) {
            head->prev = nullptr;
        }
        else {
            tail = nullptr;
        }

        delete temp;
        list_size--;
    }

    void pop_back() override {
        if (tail == nullptr) return;

        ListNode<T>* temp = tail;
        tail = tail->prev;

        if (tail != nullptr) {
            tail->next = nullptr;
        }
        else {
            head = nullptr;
        }

        delete temp;
        list_size--;
    }

    void clear() override {
        while (head != nullptr) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        list_size = 0;
    }
};