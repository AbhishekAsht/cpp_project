# C++ Data Structures and Systems Programming Projects

This repository contains two C++ projects demonstrating systems programming and data structure implementations:

## 1. Buffer Cache Implementation (BufferCache)

A simulation of a Unix-style buffer cache system that manages disk block caching in memory. This implementation provides an efficient way to reduce disk I/O operations by maintaining frequently accessed disk blocks in memory.

### Features
- Fixed-size buffer cache with configurable capacity
- LRU (Least Recently Used) replacement policy
- Thread-safe operations with mutex synchronization
- Support for dirty block tracking and write-back
- Simulated disk I/O operations

### Key Components
- Buffer size: 4KB (standard Unix block size)
- Thread-safe operations
- Cache hit/miss statistics tracking
- Asynchronous write support

### Main Interface
- `getblk()`: Get a buffer for a specific block
- `brelse()`: Release a buffer back to the cache
- `bwrite()`: Write buffer contents to disk
- `bsync()`: Synchronize all dirty buffers to disk

## 2. Custom List Implementation (standardlibrary)

A templated implementation of a doubly-linked list data structure with a clean interface design following modern C++ practices.

### Features
- Generic type support through templates
- Complete STL-style list operations
- Memory-safe implementation
- Move semantics support
- Proper memory management

### Interface Methods
- Size operations: `size()`, `empty()`
- Element access: `front()`, `back()`
- Modifiers: `push_front()`, `push_back()`, `pop_front()`, `pop_back()`, `clear()`

### Implementation Details
- Doubly-linked list structure
- Full support for:
  - Copy construction/assignment
  - Move construction/assignment
  - Proper resource cleanup
  - Iterator support

## Building the Projects

Both projects are built using Visual Studio and include necessary project files (.sln and .vcxproj).

### Requirements
- Visual Studio 2022 or later
- C++17 or later
- Windows 10 or later

### Build Steps
1. Open the desired .sln file in Visual Studio
2. Select the desired build configuration (Debug/Release)
3. Build the solution (F7 or Ctrl+Shift+B)

## Project Structure
```
cpp_project/
├── BufferCache/           # Buffer Cache Implementation
│   ├── main.cpp
│   ├── my_buffer_cache.cpp
│   └── my_buffer_cache.h
└── standardlibrary/       # Custom List Implementation
    ├── List.cpp
    ├── listInterface.h
    └── vectorIntface.h
```

## License

This project is available under the MIT License. See the LICENSE file for more details.