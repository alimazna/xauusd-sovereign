#pragma once
#include "EntityId.h"
#include <array>
#include <cstdint>
#include <string_view>

namespace xauusd::sovereign::detail {
inline std::uint64_t fnv1a64(const std::uint8_t* data, std::size_t n, std::uint64_t seed=14695981039346656037ULL) noexcept {
    std::uint64_t h = seed;
    for (std::size_t i=0;i<n;++i) { h ^= data[i]; h *= 1099511628211ULL; }
    return h;
}
inline void mix_u64(std::uint64_t& h, std::uint64_t v) noexcept {
    for (int i=0;i<8;++i) { h ^= static_cast<std::uint8_t>((v>>(i*8))&0xffU); h *= 1099511628211ULL; }
}
inline void mix_id(std::uint64_t& h, const EntityId& id) noexcept {
    for (const auto byte : id.bytes()) { h ^= byte; h *= 1099511628211ULL; }
}
inline EntityId make_id(std::string_view tag, std::uint64_t a=0, std::uint64_t b=0, std::uint64_t c=0) noexcept {
    auto h1=fnv1a64(reinterpret_cast<const std::uint8_t*>(tag.data()), tag.size());
    mix_u64(h1,a); mix_u64(h1,b); mix_u64(h1,c);
    auto h2=fnv1a64(reinterpret_cast<const std::uint8_t*>(tag.data()), tag.size(), 1099511628211ULL);
    mix_u64(h2,c); mix_u64(h2,b); mix_u64(h2,a);
    std::array<std::uint8_t,16> out{};
    for(int i=0;i<8;++i){ out[i]=static_cast<std::uint8_t>((h1>>(i*8))&0xffU); out[i+8]=static_cast<std::uint8_t>((h2>>(i*8))&0xffU); }
    return EntityId{out};
}
inline EntityId make_id(std::string_view tag, std::uint64_t a, std::uint64_t b, std::uint64_t c, std::uint64_t d) noexcept {
    auto h1=fnv1a64(reinterpret_cast<const std::uint8_t*>(tag.data()), tag.size());
    mix_u64(h1,a); mix_u64(h1,b); mix_u64(h1,c); mix_u64(h1,d);
    auto h2=fnv1a64(reinterpret_cast<const std::uint8_t*>(tag.data()), tag.size(), 1099511628211ULL);
    mix_u64(h2,d); mix_u64(h2,c); mix_u64(h2,b); mix_u64(h2,a);
    std::array<std::uint8_t,16> out{};
    for(int i=0;i<8;++i){ out[i]=static_cast<std::uint8_t>((h1>>(i*8))&0xffU); out[i+8]=static_cast<std::uint8_t>((h2>>(i*8))&0xffU); }
    return EntityId{out};
}
inline EntityId derive_id(std::string_view tag, const EntityId& parent, std::uint64_t a=0, std::uint64_t b=0) noexcept {
    auto h1=fnv1a64(reinterpret_cast<const std::uint8_t*>(tag.data()), tag.size());
    mix_id(h1,parent); mix_u64(h1,a); mix_u64(h1,b);
    auto h2=fnv1a64(reinterpret_cast<const std::uint8_t*>(tag.data()), tag.size(), 1099511628211ULL);
    mix_u64(h2,b); mix_u64(h2,a); mix_id(h2,parent);
    std::array<std::uint8_t,16> out{};
    for(int i=0;i<8;++i){ out[i]=static_cast<std::uint8_t>((h1>>(i*8))&0xffU); out[i+8]=static_cast<std::uint8_t>((h2>>(i*8))&0xffU); }
    return EntityId{out};
}
inline bool is_zero(const EntityId& id) noexcept { return id == EntityId{}; }
}
