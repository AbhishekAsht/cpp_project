#include "my_buffer_cache.h"
#include <thread>
#include <vector>
#include <iostream>

void worker_thread(myBufferCache& cache, int thread_id) {
    for (int i = 0; i < 5; ++i) {
        int block_num = thread_id * 10 + i;

        // Get buffer from cache
        auto* buf = cache.getblk(block_num);

        if (buf) {
            // Simulate work with the buffer
            std::cout << "Thread " << thread_id << " working with block "
                << block_num << std::endl;

            // Modify the buffer (mark as dirty)
            cache.brelse(buf, true);
        }
    }
}

int main() {
    const size_t CACHE_SIZE = 5;
    const int NUM_THREADS = 3;

    myBufferCache cache(CACHE_SIZE);
    std::vector<std::thread> threads;

    // Create worker threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(worker_thread, std::ref(cache), i);
    }

    // Wait for threads to complete
    for (auto& t : threads) {
        t.join();
    }

    // Sync all dirty buffers to disk
    cache.bsync();

    // Print cache statistics
    std::cout << "\nCache Statistics:\n";
    std::cout << "  Size: " << cache.size() << "/" << CACHE_SIZE << "\n";
    std::cout << "  Hits: " << cache.hits() << "\n";
    std::cout << "  Misses: " << cache.misses() << "\n";
    std::cout << "  Disk writes: " << cache.disk_writes() << "\n";

    return 0;
}