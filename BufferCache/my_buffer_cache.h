#pragma once
#ifndef UNIX_BUFFER_CACHE_H
#define UNIX_BUFFER_CACHE_H

#include <unordered_map>
#include <list>
#include <mutex>
#include <functional>

class myBufferCache {
public:
    // Represents a disk block in memory
    struct Buffer {
        int block_number;       // Disk block number
        bool dirty;             // Whether the block has been modified
        bool valid;             // Whether the data is valid
        char data[4096];        // Block data (typically 4KB in Unix systems)

        Buffer() : block_number(-1), dirty(false), valid(false) {}
    };

    // Constructor with configurable cache size
    explicit myBufferCache(size_t cache_size);
    ~myBufferCache();

    // Main interface methods
    Buffer* getblk(int block_number);
    void brelse(Buffer* buffer, bool mark_dirty = false);
    void bwrite(Buffer* buffer);  // Write buffer to disk
    void bsync();                // Sync all dirty buffers to disk

    // Statistics
    size_t size() const;
    size_t hits() const;
    size_t misses() const;
    size_t disk_writes() const;

private:
    // Cache storage and metadata
    size_t m_capacity;
    size_t m_hits = 0;
    size_t m_misses = 0;
    size_t m_disk_writes = 0;

    // Actual buffer storage
    std::vector<Buffer> m_buffers;

    // LRU management
    std::list<Buffer*> m_lru_list;
    std::unordered_map<int, Buffer*> m_block_map;

    // Synchronization
    mutable std::mutex m_mutex;

    // Disk I/O simulation (would be replaced with actual disk ops in real system)
    void read_from_disk(int block_number, Buffer& buffer);
    void write_to_disk(const Buffer& buffer);

    // Helper methods
    Buffer* find_buffer(int block_number);
    Buffer* allocate_buffer(int block_number);
    void add_to_lru(Buffer* buffer);
    void remove_from_lru(Buffer* buffer);
};

#endif // UNIX_BUFFER_CACHE_H
