#include "KangarooEngine.hpp"
#include "Config.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

using namespace Kangaroo;

std::atomic<bool> keep_running{true};

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Shutting down..." << std::endl;
    keep_running = false;
}

void displayStats(const Statistics& stats) {
    std::cout << "\r" << std::setfill(' ') 
              << "Puzzle: " << PUZZLE_NUMBER 
              << " | Time: " << stats.elapsed_seconds << "s"
              << " | DPs: " << stats.distinguished_points_found
              << " | Steps: " << stats.estimated_steps
              << " | Speed: " << std::fixed << std::setprecision(2) << stats.steps_per_second << " s/s"
              << " | Threads: " << stats.active_threads
              << " | Mem: " << stats.memory_usage_estimate / 1024 << " KB"
              << std::flush;
}

int main() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    std::cout << "==========================================" << std::endl;
    std::cout << "   Pollard Kangaroo - Bitcoin Puzzle 71   " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Educational demonstration of the Pollard Kangaroo algorithm." << std::endl;
    std::cout << "Lower Bound: 2^70" << std::endl;
    std::cout << "Upper Bound: 2^71 - 1" << std::endl;
    std::cout << "Target PubKey: " << PUZZLE_71_PUBKEY << std::endl;
    std::cout << "Threads: " << MAX_WORKER_THREADS << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    KangarooEngine engine;

    if (engine.loadCheckpoint()) {
        std::cout << "Existing checkpoint detected. Resuming..." << std::endl;
    } else {
        std::cout << "No checkpoint found. Starting fresh." << std::endl;
    }

    engine.start();

    auto last_checkpoint = std::chrono::steady_clock::now();

    while (keep_running) {
        displayStats(engine.getStatistics());

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_checkpoint).count() >= CHECKPOINT_INTERVAL_SECONDS) {
            std::cout << std::endl;
            engine.saveCheckpoint();
            last_checkpoint = now;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    engine.stop();
    std::cout << "\nGraceful shutdown complete." << std::endl;

    return 0;
}
