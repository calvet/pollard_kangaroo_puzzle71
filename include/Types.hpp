#pragma once

#include <vector>
#include <array>
#include <string>
#include <cstdint>

namespace Kangaroo {

// Represents a 256-bit value (e.g., a private key or a coordinate)
using UInt256 = std::array<uint8_t, 32>;

enum class KangarooType {
    TAME,
    WILD
};

struct DistinguishedPoint {
    UInt256 x_coordinate;
    uint64_t accumulated_distance;
    KangarooType type;
};

struct KangarooState {
    UInt256 current_x;
    uint64_t accumulated_distance;
    KangarooType type;
};

struct Statistics {
    uint64_t elapsed_seconds = 0;
    uint64_t distinguished_points_found = 0;
    uint64_t collisions_detected = 0;
    uint64_t estimated_steps = 0;
    double steps_per_second = 0.0;
    int active_threads = 0;
    size_t memory_usage_estimate = 0;
};

} // namespace Kangaroo
