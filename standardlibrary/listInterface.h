#pragma once

// Macro to prevent multiple inclusions of this header
#define LIST_INTERFACE_H

// Interface (Abstract Base Class) for List operations
// This defines the standard operations any list implementation should provide
template <typename T>
class IList {
public:
    virtual ~IList() = default;  // Virtual destructor for proper polymorphic deletion

    // Capacity operations
    virtual size_t size() const = 0;     // Returns number of elements in list
    virtual bool empty() const = 0;      // Checks if list is empty

    // Element access operations
    virtual T& front() = 0;              // Access first element (mutable)
    virtual const T& front() const = 0;  // Access first element (const)
    virtual T& back() = 0;               // Access last element (mutable)
    virtual const T& back() const = 0;   // Access last element (const)

    // Modifier operations
    virtual void push_front(const T& value) = 0;  // Add element at beginning
    virtual void push_back(const T& value) = 0;   // Add element at end
    virtual void pop_front() = 0;                 // Remove first element
    virtual void pop_back() = 0;                  // Remove last element
    virtual void clear() = 0;                     // Remove all elements
};

// Node class for doubly-linked list implementation
template <typename T>
class ListNode {
public:
    T data;             // The actual data stored in the node
    ListNode* next;     // Pointer to the next node in the list
    ListNode* prev;     // Pointer to the previous node in the list

    // Constructor initializes node with given value and null pointers
    ListNode(const T& val) : data(val), next(nullptr), prev(nullptr) {}
};

// Custom doubly-linked list implementation of IList interface
template <typename T>
class CustomList : public IList<T> {
private:
    ListNode<T>* head;   // Pointer to first node in the list
    ListNode<T>* tail;   // Pointer to last node in the list
    size_t list_size;    // Current number of elements in the list

public:
    // Default constructor - initializes empty list
    CustomList() : head(nullptr), tail(nullptr), list_size(0) {}

    // Destructor - deallocates all nodes when list is destroyed
    ~CustomList() {
        clear();
    }

    // Copy constructor - creates deep copy of another list
    CustomList(const CustomList& other) : head(nullptr), tail(nullptr), list_size(0) {
        // Iterate through other list and copy each element
        ListNode<T>* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

    // Move constructor - transfers ownership of resources from another list
    CustomList(CustomList&& other) noexcept
        : head(other.head), tail(other.tail), list_size(other.list_size) {
        // Reset the source list to avoid double deletion
        other.head = nullptr;
        other.tail = nullptr;
        other.list_size = 0;
    }

    // Copy assignment operator - makes deep copy of another list
    CustomList& operator=(const CustomList& other) {
        if (this != &other) {  // Protect against self-assignment
            clear();           // Clear current contents

            // Copy each element from the other list
            ListNode<T>* current = other.head;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // Move assignment operator - transfers ownership of resources
    CustomList& operator=(CustomList&& other) noexcept {
        if (this != &other) {  // Protect against self-assignment
            clear();           // Clear current contents

            // Take ownership of other list's resources
            head = other.head;
            tail = other.tail;
            list_size = other.list_size;

            // Reset the source list
            other.head = nullptr;
            other.tail = nullptr;
            other.list_size = 0;
        }
        return *this;
    }

    // Returns the number of elements in the list
    size_t size() const override { return list_size; }

    // Checks if the list is empty
    bool empty() const override { return list_size == 0; }

    // Access the first element (mutable version)
    T& front() override {
        return head->data;  // Note: Should check for empty list in production code
    }

    // Access the first element (const version)
    const T& front() const override {
        return head->data;  // Note: Should check for empty list in production code
    }

    // Access the last element (mutable version)
    T& back() override {
        return tail->data;  // Note: Should check for empty list in production code
    }

    // Access the last element (const version)
    const T& back() const override {
        return tail->data;  // Note: Should check for empty list in production code
    }

    // Add element to the front of the list
    void push_front(const T& value) override {
        ListNode<T>* new_node = new ListNode<T>(value);  // Create new node

        if (head == nullptr) {
            // List is empty - new node becomes both head and tail
            head = tail = new_node;
        }
        else {
            // Link new node before current head
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        list_size++;
    }

    // Add element to the back of the list
    void push_back(const T& value) override {
        ListNode<T>* new_node = new ListNode<T>(value);  // Create new node

        if (tail == nullptr) {
            // List is empty - new node becomes both head and tail
            head = tail = new_node;
        }
        else {
            // Link new node after current tail
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        list_size++;
    }

    // Remove element from the front of the list
    void pop_front() override {
        if (head == nullptr) return;  // Nothing to do for empty list

        ListNode<T>* temp = head;  // Save current head
        head = head->next;         // Move head to next node

        if (head != nullptr) {
            head->prev = nullptr;  // New head has no previous node
        }
        else {
            // List is now empty - update tail
            tail = nullptr;
        }

        delete temp;    // Delete old head
        list_size--;    // Decrement size
    }

    // Remove element from the back of the list
    void pop_back() override {
        if (tail == nullptr) return;  // Nothing to do for empty list

        ListNode<T>* temp = tail;  // Save current tail
        tail = tail->prev;        // Move tail to previous node

        if (tail != nullptr) {
            tail->next = nullptr;  // New tail has no next node
        }
        else {
            // List is now empty - update head
            head = nullptr;
        }

        delete temp;    // Delete old tail
        list_size--;    // Decrement size
    }

    // Remove all elements from the list
    void clear() override {
        while (head != nullptr) {
            ListNode<T>* temp = head;  // Save current node
            head = head->next;         // Move to next node
            delete temp;               // Delete saved node
        }
        tail = nullptr;    // Reset tail
        list_size = 0;     // Reset size
    }
};