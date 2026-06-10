enable_testing()

add_executable(unit_tests
    unit_tests.cpp
    ../src/CryptoUtils.cpp
)

target_link_libraries(unit_tests
    GTest::gtest_main
    secp256k1::secp256k1
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
