#include <gtest/gtest.h>
#include "../include/CryptoUtils.hpp"
#include "../include/Config.hpp"
#include "../include/Types.hpp"

using namespace Kangaroo;

class CryptoTest : public ::testing::Test {
protected:
    CryptoUtils crypto;
};

TEST_F(CryptoTest, HexConversion) {
    std::string original = "0000000000000000000000000000000000000000000000000000000000000001";
    UInt256 data = CryptoUtils::hexToUint256(original);
    EXPECT_EQ(data[31], 1);
    EXPECT_EQ(data[0], 0);
    
    std::string converted = CryptoUtils::uint256ToHex(data);
    EXPECT_EQ(original, converted);
}

TEST_F(CryptoTest, KeyVerification) {
    // Private key 1
    UInt256 priv;
    std::fill(priv.begin(), priv.end(), 0);
    priv[31] = 1;
    
    // Compressed public key for private key 1
    std::string pub_hex = "0279be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798";
    
    EXPECT_TRUE(crypto.verifyKey(priv, pub_hex));
}

TEST_F(CryptoTest, PointAdditionSimulation) {
    UInt256 p1 = {0};
    p1[31] = 0x01;
    UInt256 p2 = {0};
    p2[31] = 0x02;
    UInt256 result;
    
    crypto.addPoints(result, p1, p2);
    // Our simulation uses XOR
    EXPECT_EQ(result[31], 0x03);
}

TEST(ConfigTest, Constants) {
    EXPECT_EQ(PUZZLE_NUMBER, 71);
    EXPECT_EQ(MAX_WORKER_THREADS, 2);
    EXPECT_EQ(DISTINGUISHED_BITS, 20);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
