// src/contracts/foundation/HashAlgorithm.h
#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class HashAlgorithm : std::uint8_t {
    NONE,
    SHA256,
    SHA512,
    SHA3_256,
    BLAKE2B_256,
    BLAKE2B_512
};

} // namespace xauusd::sovereign
