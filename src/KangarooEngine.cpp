#include "KangarooEngine.hpp"
#include "Config.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <cmath>

namespace Kangaroo {

KangarooEngine::KangarooEngine() {
    initJumpTable();
    start_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

KangarooEngine::~KangarooEngine() {
    stop();
}

void KangarooEngine::initJumpTable() {
    // Create a jump table with 32 entries
    for (int i = 0; i < 32; ++i) {
        Jump j;
        // In a real implementation, these would be EC points G * 2^i
        // Here we use a deterministic pseudo-random offset for demo purposes
        for (int k = 0; k < 32; ++k) j.x_offset[k] = static_cast<uint8_t>((i + k) * 13);
        j.distance = static_cast<uint64_t>(std::pow(2, i % 20));
        jump_table.push_back(j);
    }
}

void KangarooEngine::start() {
    if (running) return;
    running = true;

    // Initialize kangaroos if not loaded from checkpoint
    if (tame_kangaroos.empty()) {
        UInt256 lower = CryptoUtils::hexToUint256(PUZZLE_LOWER_BOUND);
        tame_kangaroos.push_back({lower, 0, KangarooType::TAME});
        
        UInt256 target_x = CryptoUtils::hexToUint256(PUZZLE_71_PUBKEY.substr(2, 64));
        wild_kangaroos.push_back({target_x, 0, KangarooType::WILD});
    }

    int thread_count = std::min(static_cast<int>(std::thread::hardware_concurrency()), MAX_WORKER_THREADS);
    for (int i = 0; i < thread_count; ++i) {
        workers.emplace_back(&KangarooEngine::workerThread, this, i);
    }
}

void KangarooEngine::stop() {
    running = false;
    for (auto& t : workers) {
        if (t.joinable()) t.join();
    }
    workers.clear();
}

void KangarooEngine::workerThread(int thread_id) {
    (void)thread_id; // Unused for now
    
    while (running) {
        // Alternar entre tame e wild para simplificar a demonstração
        for (auto& k : tame_kangaroos) {
            // Pseudo-random jump based on current x
            int jump_idx = k.current_x[31] % jump_table.size();
            crypto.addPoints(k.current_x, k.current_x, jump_table[jump_idx].x_offset);
            k.accumulated_distance += jump_table[jump_idx].distance;
            total_steps++;

            if (isDistinguished(k.current_x)) {
                handleCollision({k.current_x, k.accumulated_distance, k.type});
            }
        }

        for (auto& k : wild_kangaroos) {
            int jump_idx = k.current_x[31] % jump_table.size();
            crypto.addPoints(k.current_x, k.current_x, jump_table[jump_idx].x_offset);
            k.accumulated_distance += jump_table[jump_idx].distance;
            total_steps++;

            if (isDistinguished(k.current_x)) {
                handleCollision({k.current_x, k.accumulated_distance, k.type});
            }
        }
        
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

bool KangarooEngine::isDistinguished(const UInt256& x) const {
    // Check if the last DISTINGUISHED_BITS bits are zero
    // For simplicity in demo, we check trailing bytes
    int bytes_to_check = DISTINGUISHED_BITS / 8;
    for (int i = 0; i < bytes_to_check; ++i) {
        if (x[31 - i] != 0) return false;
    }
    return true;
}

void KangarooEngine::handleCollision(const DistinguishedPoint& new_dp) {
    std::lock_guard<std::mutex> lock(db_mutex);
    std::string x_hex = CryptoUtils::uint256ToHex(new_dp.x_coordinate);
    
    auto it = dp_database.find(x_hex);
    if (it != dp_database.end()) {
        if (it->second.type != new_dp.type) {
            // Collision found between Tame and Wild!
            std::cout << "\n[!] COLLISION DETECTED!" << std::endl;
            // In a real implementation, calculate private key:
            // key = tame_dist - wild_dist + start_offset
            // For demo, we just log it.
        }
    } else {
        dp_database[x_hex] = new_dp;
        dp_count++;
    }
}

void KangarooEngine::saveCheckpoint() {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (!std::filesystem::exists(CHECKPOINT_DIR)) {
        std::filesystem::create_directories(CHECKPOINT_DIR);
    }

    std::ofstream f(CHECKPOINT_DIR + "/state.dat", std::ios::binary);
    if (!f) return;

    // Save tame states
    size_t tame_size = tame_kangaroos.size();
    f.write(reinterpret_cast<char*>(&tame_size), sizeof(tame_size));
    f.write(reinterpret_cast<char*>(tame_kangaroos.data()), tame_size * sizeof(KangarooState));

    // Save wild states
    size_t wild_size = wild_kangaroos.size();
    f.write(reinterpret_cast<char*>(&wild_size), sizeof(wild_size));
    f.write(reinterpret_cast<char*>(wild_kangaroos.data()), wild_size * sizeof(KangarooState));

    // Save DP database size
    size_t db_size = dp_database.size();
    f.write(reinterpret_cast<char*>(&db_size), sizeof(db_size));
    
    std::cout << "Checkpoint saved." << std::endl;
}

bool KangarooEngine::loadCheckpoint() {
    std::string path = CHECKPOINT_DIR + "/state.dat";
    if (!std::filesystem::exists(path)) return false;

    std::ifstream f(path, std::ios::binary);
    if (!f) return false;

    try {
        size_t tame_size;
        f.read(reinterpret_cast<char*>(&tame_size), sizeof(tame_size));
        tame_kangaroos.resize(tame_size);
        f.read(reinterpret_cast<char*>(tame_kangaroos.data()), tame_size * sizeof(KangarooState));

        size_t wild_size;
        f.read(reinterpret_cast<char*>(&wild_size), sizeof(wild_size));
        wild_kangaroos.resize(wild_size);
        f.read(reinterpret_cast<char*>(wild_kangaroos.data()), wild_size * sizeof(KangarooState));

        return true;
    } catch (...) {
        return false;
    }
}

Statistics KangarooEngine::getStatistics() const {
    Statistics stats;
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    stats.elapsed_seconds = static_cast<uint64_t>(difftime(now, start_time));
    stats.distinguished_points_found = dp_count;
    stats.estimated_steps = total_steps;
    stats.active_threads = static_cast<int>(workers.size());
    if (stats.elapsed_seconds > 0) {
        stats.steps_per_second = static_cast<double>(total_steps) / stats.elapsed_seconds;
    }
    stats.memory_usage_estimate = dp_database.size() * sizeof(DistinguishedPoint);
    return stats;
}

} // namespace Kangaroo
