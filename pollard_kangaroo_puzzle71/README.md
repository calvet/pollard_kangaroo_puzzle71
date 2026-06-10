# Pollard Kangaroo - Bitcoin Puzzle 71

## Project Overview
This project is a complete Windows-native C++ application that demonstrates the Pollard Kangaroo (also known as Pollard Lambda) algorithm applied to the Bitcoin Puzzle Challenge #71. It is designed to run exclusively on the CPU.

## Educational Disclaimer
* **This implementation is intended for educational and demonstrative purposes.**
* Solving Puzzle 71 using only two CPU threads is computationally impractical.
* The software is intended to illustrate the Pollard Kangaroo approach rather than provide a realistic attack tool.

## Explanation of Pollard Kangaroo
The Pollard Kangaroo algorithm is a method for solving the discrete logarithm problem. It is particularly useful when the solution is known to lie within a specific interval. The algorithm uses two "kangaroos":
* **Tame Kangaroo:** Starts at a known position (e.g., the upper bound of the interval) and makes a series of pseudo-random jumps.
* **Wild Kangaroo:** Starts at the target position (the public key) and makes the same sequence of pseudo-random jumps.

If the wild kangaroo's path intersects the tame kangaroo's path, they will continue to jump together. By detecting this collision, the discrete logarithm (the private key) can be calculated.

## Explanation of Distinguished Points
To efficiently detect collisions without storing every jump, the algorithm uses "Distinguished Points". A point is considered distinguished if it meets a specific condition, such as having a certain number of trailing zero bits in its x-coordinate. Only distinguished points are stored in a hash map. When both kangaroos land on the same distinguished point, a collision is detected.

## Build Instructions
### Prerequisites
* Windows 10 or Windows 11 (64-bit)
* Microsoft Visual C++ (MSVC) Compiler (latest stable Visual Studio Build Tools)
* CMake (version 3.15 or higher)
* Git

### Steps
1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd pollard_kangaroo_puzzle71
   ```
2. Configure the project using CMake:
   ```bash
   cmake -B build -S .
   ```
3. Build the project:
   ```bash
   cmake --build build --config Release
   ```

## Usage Instructions
Run the compiled executable from the command line:
```bash
./build/Release/PollardKangarooPuzzle71.exe
```
The application will automatically start searching for the private key. It will display real-time statistics and save checkpoints periodically.

## Limitations of CPU Execution
This implementation relies solely on the CPU. Cryptographic operations, particularly elliptic curve point additions, are computationally expensive. Without GPU acceleration, the number of steps per second is significantly limited.

## Expected Performance Notes
Given the vast search space of Puzzle 71 ($2^{70}$ to $2^{71}-1$), finding the private key using a CPU-only implementation is expected to take an impractically long time. This project serves as a proof-of-concept and educational tool.

## How to Replace the Puzzle 71 Public Key Constant
The official Puzzle 71 public key is represented as a constant placeholder in the source code. To use the actual public key, locate the `PUZZLE_71_PUBKEY` constant in `include/Config.hpp` and replace it with the correct hexadecimal string.

## How Checkpoint Recovery Works
The application saves its state every 60 seconds to the `/checkpoints` directory. This includes the positions of the tame and wild kangaroos, the database of distinguished points, and progress statistics. On startup, the application automatically detects existing checkpoints and resumes from the last saved state, ensuring no progress is lost in case of an interruption.
