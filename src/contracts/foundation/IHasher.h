// src/contracts/foundation/IHasher.h
#pragma once

#include "HashAlgorithm.h"
#include "HashDigest.h"

#include <cstdint>
#include <span>

namespace xauusd::sovereign {

class IHasher {
public:
    virtual ~IHasher() = default;

    virtual HashDigest hash(
        HashAlgorithm algorithm,
        std::span<const std::uint8_t> data) const = 0;
};

} // namespace xauusd::sovereign
