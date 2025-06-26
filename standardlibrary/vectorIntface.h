#pragma once
#define VECTOR_INTERFACE_H

template <typename T>
class IVector {
public:
    virtual ~IVector() = default;

    // Capacity
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual size_t capacity() const = 0;

    // Element access
    virtual T& operator[](size_t index) = 0;
    virtual const T& operator[](size_t index) const = 0;
    virtual T& at(size_t index) = 0;
    virtual const T& at(size_t index) const = 0;
    virtual T& front() = 0;
    virtual const T& front() const = 0;
    virtual T& back() = 0;
    virtual const T& back() const = 0;

    // Modifiers
    virtual void push_back(const T& value) = 0;
    virtual void pop_back() = 0;
    virtual void clear() = 0;
    virtual void reserve(size_t new_capacity) = 0;
};

template <typename T>
class CustomVector : public IVector<T> {
private:
    T* data = nullptr;
    size_t vec_size = 0;
    size_t vec_capacity = 0;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];

        for (size_t i = 0; i < vec_size; ++i) {
            new_data[i] = std::move(data[i]);
        }

        delete[] data;
        data = new_data;
        vec_capacity = new_capacity;
    }

public:
    CustomVector() = default;

    CustomVector(size_t initial_size) : vec_size(initial_size), vec_capacity(initial_size) {
        data = new T[vec_capacity];
    }

    ~CustomVector() {
        delete[] data;
    }

    // Copy constructor
    CustomVector(const CustomVector& other) : vec_size(other.vec_size), vec_capacity(other.vec_capacity) {
        data = new T[vec_capacity];
        for (size_t i = 0; i < vec_size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Move constructor
    CustomVector(CustomVector&& other) noexcept
        : data(other.data), vec_size(other.vec_size), vec_capacity(other.vec_capacity) {
        other.data = nullptr;
        other.vec_size = 0;
        other.vec_capacity = 0;
    }

    // Assignment operators
    CustomVector& operator=(const CustomVector& other) {
        if (this != &other) {
            delete[] data;
            vec_size = other.vec_size;
            vec_capacity = other.vec_capacity;
            data = new T[vec_capacity];
            for (size_t i = 0; i < vec_size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    CustomVector& operator=(CustomVector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            vec_size = other.vec_size;
            vec_capacity = other.vec_capacity;
            other.data = nullptr;
            other.vec_size = 0;
            other.vec_capacity = 0;
        }
        return *this;
    }

    // IVector implementation
    size_t size() const override { return vec_size; }
    bool empty() const override { return vec_size == 0; }
    size_t capacity() const override { return vec_capacity; }

    T& operator[](size_t index) override { return data[index]; }
    const T& operator[](size_t index) const override { return data[index]; }

    T& at(size_t index) override {
        if (index >= vec_size) throw "Index out of bounds";
        return data[index];
    }

    const T& at(size_t index) const override {
        if (index >= vec_size) throw "Index out of bounds";
        return data[index];
    }

    T& front() override { return data[0]; }
    const T& front() const override { return data[0]; }
    T& back() override { return data[vec_size - 1]; }
    const T& back() const override { return data[vec_size - 1]; }

    void push_back(const T& value) override {
        if (vec_size >= vec_capacity) {
            reserve(vec_capacity == 0 ? 1 : vec_capacity * 2);
        }
        data[vec_size++] = value;
    }

    void pop_back() override {
        if (vec_size > 0) {
            --vec_size;
        }
    }

    void clear() override {
        vec_size = 0;
    }

    void reserve(size_t new_capacity) override {
        if (new_capacity > vec_capacity) {
            reallocate(new_capacity);
        }
    }
};