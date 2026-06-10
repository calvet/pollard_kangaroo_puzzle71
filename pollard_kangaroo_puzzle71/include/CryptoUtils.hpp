#pragma once

#include "Types.hpp"
#include <secp256k1.h>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace Kangaroo {

class CryptoUtils {
public:
    CryptoUtils();
    ~CryptoUtils();

    // Prevent copying
    CryptoUtils(const CryptoUtils&) = delete;
    CryptoUtils& operator=(const CryptoUtils&) = delete;

    static std::string uint256ToHex(const UInt256& data);
    static UInt256 hexToUint256(const std::string& hex);
    
    bool verifyKey(const UInt256& private_key, const std::string& target_pubkey_hex);
    
    // Elliptic curve operations
    void addPoints(UInt256& result_x, const UInt256& p1_x, const UInt256& p2_x);
    void multiplyBase(UInt256& result_x, const UInt256& scalar);

private:
    secp256k1_context* ctx;
};

} // namespace Kangaroo
