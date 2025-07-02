#pragma once
#define VECTOR_INTERFACE_H

// Interface (Abstract Base Class) for Vector operations
template <typename T>
class IVector {
public:
    virtual ~IVector() = default;  // Virtual destructor for proper polymorphic deletion

    // Capacity operations
    virtual size_t size() const = 0;      // Current number of elements
    virtual bool empty() const = 0;       // True if vector is empty
    virtual size_t capacity() const = 0;  // Current allocated storage capacity

    // Element access operations
    virtual T& operator[](size_t index) = 0;             // Unchecked access (mutable)
    virtual const T& operator[](size_t index) const = 0; // Unchecked access (const)
    virtual T& at(size_t index) = 0;                     // Bounds-checked access (mutable)
    virtual const T& at(size_t index) const = 0;         // Bounds-checked access (const)
    virtual T& front() = 0;                              // First element (mutable)
    virtual const T& front() const = 0;                  // First element (const)
    virtual T& back() = 0;                               // Last element (mutable)
    virtual const T& back() const = 0;                   // Last element (const)

    // Modifier operations
    virtual void push_back(const T& value) = 0;  // Add element at end
    virtual void pop_back() = 0;                // Remove last element
    virtual void clear() = 0;                   // Remove all elements
    virtual void reserve(size_t new_capacity) = 0; // Reserve storage capacity
};

template <typename T>
class CustomVector : public IVector<T> {
private:
    T* data = nullptr;         // Pointer to dynamically allocated array
    size_t vec_size = 0;       // Current number of elements
    size_t vec_capacity = 0;   // Current allocated capacity

    // Internal function to resize the underlying storage
    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];  // Allocate new storage

        // Move existing elements to new storage
        for (size_t i = 0; i < vec_size; ++i) {
            new_data[i] = std::move(data[i]);  // Use move semantics for efficiency
        }

        delete[] data;           // Free old storage
        data = new_data;         // Update data pointer
        vec_capacity = new_capacity; // Update capacity
    }

public:
    // Default constructor - creates empty vector
    CustomVector() = default;

    // Constructor with initial size
    CustomVector(size_t initial_size)
        : vec_size(initial_size), vec_capacity(initial_size) {
        data = new T[vec_capacity];  // Allocate storage
    }

    // Destructor - releases allocated memory
    ~CustomVector() {
        delete[] data;
    }

    // Copy constructor - creates deep copy
    CustomVector(const CustomVector& other)
        : vec_size(other.vec_size), vec_capacity(other.vec_capacity) {
        data = new T[vec_capacity];
        // Copy elements from source
        for (size_t i = 0; i < vec_size; ++i) {
            data[i] = other.data[i];  // Uses T's copy assignment
        }
    }

    // Move constructor - transfers ownership
    CustomVector(CustomVector&& other) noexcept
        : data(other.data), vec_size(other.vec_size), vec_capacity(other.vec_capacity) {
        // Leave source in valid but empty state
        other.data = nullptr;
        other.vec_size = 0;
        other.vec_capacity = 0;
    }

    // Copy assignment operator
    CustomVector& operator=(const CustomVector& other) {
        if (this != &other) {  // Protect against self-assignment
            delete[] data;      // Free existing storage

            vec_size = other.vec_size;
            vec_capacity = other.vec_capacity;
            data = new T[vec_capacity];

            // Copy elements
            for (size_t i = 0; i < vec_size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move assignment operator
    CustomVector& operator=(CustomVector&& other) noexcept {
        if (this != &other) {  // Protect against self-assignment
            delete[] data;      // Free existing storage

            // Take ownership of other's resources
            data = other.data;
            vec_size = other.vec_size;
            vec_capacity = other.vec_capacity;

            // Leave source in valid state
            other.data = nullptr;
            other.vec_size = 0;
            other.vec_capacity = 0;
        }
        return *this;
    }

    // IVector interface implementation

    size_t size() const override { return vec_size; }
    bool empty() const override { return vec_size == 0; }
    size_t capacity() const override { return vec_capacity; }

    // Unchecked element access
    T& operator[](size_t index) override { return data[index]; }
    const T& operator[](size_t index) const override { return data[index]; }

    // Checked element access with bounds checking
    T& at(size_t index) override {
        if (index >= vec_size) throw "Index out of bounds";
        return data[index];
    }
    const T& at(size_t index) const override {
        if (index >= vec_size) throw "Index out of bounds";
        return data[index];
    }

    // Access first and last elements
    T& front() override { return data[0]; }
    const T& front() const override { return data[0]; }
    T& back() override { return data[vec_size - 1]; }
    const T& back() const override { return data[vec_size - 1]; }

    // Add element to the end
    void push_back(const T& value) override {
        if (vec_size >= vec_capacity) {
            // If we're out of space, grow by doubling (or start with 1 if empty)
            reserve(vec_capacity == 0 ? 1 : vec_capacity * 2);
        }
        data[vec_size++] = value;  // Copy the value and increment size
    }

    // Remove last element
    void pop_back() override {
        if (vec_size > 0) {
            --vec_size;  // Simply decrement size - doesn't destruct the element
        }
    }

    // Clear all elements (doesn't deallocate memory)
    void clear() override {
        vec_size = 0;
    }

    // Reserve storage capacity
    void reserve(size_t new_capacity) override {
        if (new_capacity > vec_capacity) {
            reallocate(new_capacity);  // Only grow, never shrink
        }
    }
};