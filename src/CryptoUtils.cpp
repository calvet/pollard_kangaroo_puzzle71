#include "CryptoUtils.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstring>

namespace Kangaroo {

CryptoUtils::CryptoUtils() {
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    if (!ctx) {
        throw std::runtime_error("Failed to initialize secp256k1 context");
    }
}

CryptoUtils::~CryptoUtils() {
    if (ctx) {
        secp256k1_context_destroy(ctx);
    }
}

std::string CryptoUtils::uint256ToHex(const UInt256& data) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 32; ++i) {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}

UInt256 CryptoUtils::hexToUint256(const std::string& hex) {
    UInt256 data;
    std::string clean_hex = hex;
    if (clean_hex.length() > 64) clean_hex = clean_hex.substr(clean_hex.length() - 64);
    while (clean_hex.length() < 64) clean_hex = "0" + clean_hex;

    for (int i = 0; i < 32; ++i) {
        std::string byteString = clean_hex.substr(i * 2, 2);
        data[i] = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
    }
    return data;
}

bool CryptoUtils::verifyKey(const UInt256& private_key, const std::string& target_pubkey_hex) {
    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, private_key.data())) {
        return false;
    }

    unsigned char output[33];
    size_t output_len = 33;
    secp256k1_ec_pubkey_serialize(ctx, output, &output_len, &pubkey, SECP256K1_EC_COMPRESSED);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 33; ++i) {
        ss << std::setw(2) << static_cast<int>(output[i]);
    }

    return ss.str() == target_pubkey_hex;
}

void CryptoUtils::addPoints(UInt256& result_x, const UInt256& p1_x, const UInt256& p2_x) {
    // Note: Simplified for demonstration. In a real Pollard Kangaroo, 
    // we would handle full EC point additions.
    // For this educational demo, we'll simulate the point transition.
    for (int i = 0; i < 32; ++i) {
        result_x[i] = p1_x[i] ^ p2_x[i]; // XOR simulation
    }
}

void CryptoUtils::multiplyBase(UInt256& result_x, const UInt256& scalar) {
    secp256k1_pubkey pubkey;
    if (secp256k1_ec_pubkey_create(ctx, &pubkey, scalar.data())) {
        unsigned char output[33];
        size_t output_len = 33;
        secp256k1_ec_pubkey_serialize(ctx, output, &output_len, &pubkey, SECP256K1_EC_COMPRESSED);
        std::memcpy(result_x.data(), output + 1, 32); // Use x-coordinate
    }
}

} // namespace Kangaroo
