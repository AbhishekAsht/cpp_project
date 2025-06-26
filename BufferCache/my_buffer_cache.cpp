#include "my_buffer_cache.h"
#include <algorithm>
#include <cstring>
#include <iostream>

myBufferCache::myBufferCache(size_t cache_size)
    : m_capacity(cache_size), m_buffers(cache_size) {
    if (cache_size == 0) {
        throw std::invalid_argument("Cache size must be greater than 0");
    }
}

myBufferCache::~myBufferCache() {
    bsync(); // Ensure all dirty buffers are written to disk
}

myBufferCache::Buffer* myBufferCache::getblk(int block_number) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if block is already in cache
    if (Buffer* buf = find_buffer(block_number)) {
        m_hits++;
        remove_from_lru(buf); // Will be added back when released
        return buf;
    }

    m_misses++;
    return allocate_buffer(block_number);
}

void myBufferCache::brelse(Buffer* buffer, bool mark_dirty) {
    if (!buffer) return;

    std::lock_guard<std::mutex> lock(m_mutex);

    if (mark_dirty) {
        buffer->dirty = true;
    }

    add_to_lru(buffer);
}

void myBufferCache::bwrite(Buffer* buffer) {
    if (!buffer || !buffer->valid) return;

    std::lock_guard<std::mutex> lock(m_mutex);
    write_to_disk(*buffer);
    buffer->dirty = false;
    m_disk_writes++;
}

void myBufferCache::bsync() {
    std::lock_guard<std::mutex> lock(m_mutex);

    for (auto& buf : m_buffers) {
        if (buf.valid && buf.dirty) {
            write_to_disk(buf);
            buf.dirty = false;
            m_disk_writes++;
        }
    }
}

myBufferCache::Buffer* myBufferCache::find_buffer(int block_number) {
    auto it = m_block_map.find(block_number);
    return (it != m_block_map.end()) ? it->second : nullptr;
}

myBufferCache::Buffer* myBufferCache::allocate_buffer(int block_number) {
    Buffer* buffer = nullptr;

    // Try to find an unused buffer
    for (auto& buf : m_buffers) {
        if (!buf.valid) {
            buffer = &buf;
            break;
        }
    }

    // If all buffers are in use, evict the LRU one
    if (!buffer && !m_lru_list.empty()) {
        buffer = m_lru_list.back();

        // Write back if dirty
        if (buffer->dirty) {
            write_to_disk(*buffer);
            m_disk_writes++;
        }

        // Remove from map
        m_block_map.erase(buffer->block_number);
    }

    if (buffer) {
        // Initialize new buffer
        buffer->block_number = block_number;
        buffer->dirty = false;
        buffer->valid = true;
        read_from_disk(block_number, *buffer);

        // Add to map
        m_block_map[block_number] = buffer;
    }

    return buffer;
}

void myBufferCache::add_to_lru(Buffer* buffer) {
    if (!buffer) return;

    // Remove if already in list
    remove_from_lru(buffer);

    // Add to front (MRU position)
    m_lru_list.push_front(buffer);
}

void myBufferCache::remove_from_lru(Buffer* buffer) {
    if (!buffer) return;

    auto it = std::find(m_lru_list.begin(), m_lru_list.end(), buffer);
    if (it != m_lru_list.end()) {
        m_lru_list.erase(it);
    }
}

void myBufferCache::read_from_disk(int block_number, Buffer& buffer) {
    // Simulate disk read - in real system this would be actual disk I/O
    std::cout << "Reading block " << block_number << " from disk\n";
    memset(buffer.data, 0, sizeof(buffer.data));
    snprintf(buffer.data, sizeof(buffer.data), "Data for block %d", block_number);
}

void myBufferCache::write_to_disk(const Buffer& buffer) {
    // Simulate disk write - in real system this would be actual disk I/O
    std::cout << "Writing block " << buffer.block_number << " to disk\n";
}

size_t myBufferCache::size() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_block_map.size();
}

size_t myBufferCache::hits() const {
    return m_hits;
}

size_t myBufferCache::misses() const {
    return m_misses;
}

size_t myBufferCache::disk_writes() const {
    return m_disk_writes;
}