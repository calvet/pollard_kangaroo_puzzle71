#pragma once

#include <string>
#include <cstdint>

namespace Kangaroo {

// Puzzle Configuration
constexpr int PUZZLE_NUMBER = 71;

// Lower bound: 2^70
const std::string PUZZLE_LOWER_BOUND = "400000000000000000"; // Hex representation

// Upper bound: 2^71 - 1
const std::string PUZZLE_UPPER_BOUND = "7fffffffffffffffff"; // Hex representation

// Public Key Target
// PLACEHOLDER: Replace with the official Puzzle 71 public key hexadecimal string.
const std::string PUZZLE_71_PUBKEY = "024ce119c96e2fa352340b1d17d1e676b757b33333333333333333333333333333"; 

// Distinguished Points Configuration
constexpr int DISTINGUISHED_BITS = 20;

// Threading Configuration
constexpr int MAX_WORKER_THREADS = 2;

// Checkpoint Configuration
constexpr int CHECKPOINT_INTERVAL_SECONDS = 60;
const std::string CHECKPOINT_DIR = "checkpoints";

// Output Configuration
const std::string OUTPUT_DIR = "output";
const std::string FOUND_KEY_FILE = "found_key.txt";

} // namespace Kangaroo
