#pragma once

#include "Types.hpp"
#include "CryptoUtils.hpp"
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <vector>
#include <thread>

namespace Kangaroo {

class KangarooEngine {
public:
    KangarooEngine();
    ~KangarooEngine();

    void start();
    void stop();
    
    void saveCheckpoint();
    bool loadCheckpoint();

    Statistics getStatistics() const;

private:
    void workerThread(int thread_id);
    bool isDistinguished(const UInt256& x) const;
    void handleCollision(const DistinguishedPoint& new_dp);
    
    // Jump table for kangaroos
    struct Jump {
        UInt256 x_offset;
        uint64_t distance;
    };
    std::vector<Jump> jump_table;
    void initJumpTable();

    std::atomic<bool> running{false};
    std::vector<std::thread> workers;
    
    // Thread-safe storage for distinguished points
    mutable std::mutex db_mutex;
    std::unordered_map<std::string, DistinguishedPoint> dp_database;

    CryptoUtils crypto;
    std::atomic<uint64_t> total_steps{0};
    std::atomic<uint64_t> dp_count{0};
    
    // Kangaroo states
    std::vector<KangarooState> tame_kangaroos;
    std::vector<KangarooState> wild_kangaroos;
    
    uint64_t start_time;
};

} // namespace Kangaroo
