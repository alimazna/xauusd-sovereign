#include "../../src/mt5/cpp/MT5Protocol.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

using namespace xauusd::mt5;

int main() {
    static_assert(sizeof(MessageHeader) == 24);

    const std::uint8_t known[] = {'1','2','3','4','5','6','7','8','9'};
    assert(compute_crc32(known, sizeof(known)) == 0xCBF43926u);

    const std::string payload = R"({"tf":"M15","bid":2645.50})";
    const auto encoded = encode_message(MessageType::TICK, payload, 7);
    assert(encoded.size() == 24 + payload.size());

    // Verify the documented little-endian wire layout directly.
    assert(encoded[0] == 0x53 && encoded[1] == 0x55 && encoded[2] == 0x41 && encoded[3] == 0x58);
    assert(encoded[4] == 0x01 && encoded[5] == 0x00);
    assert(encoded[6] == 0x02 && encoded[7] == 0x00);
    assert(encoded[8] == payload.size() && encoded[9] == 0 && encoded[10] == 0 && encoded[11] == 0);
    assert(encoded[12] == 7 && encoded[13] == 0 && encoded[14] == 0 && encoded[15] == 0);

    MessageHeader header{};
    assert(decode_header(encoded.data(), encoded.size(), header));
    assert(header.magic == kProtocolMagic);
    assert(header.version == kProtocolVersion);
    assert(header.type == static_cast<std::uint16_t>(MessageType::TICK));
    assert(header.payload_size == payload.size());
    assert(header.sequence == 7);
    assert(verify_payload_crc(header, encoded.data() + 24, payload.size()));

    auto corrupted = encoded;
    corrupted.back() ^= 0x01u;
    MessageHeader corrupted_header{};
    assert(decode_header(corrupted.data(), corrupted.size(), corrupted_header));
    assert(!verify_payload_crc(corrupted_header, corrupted.data() + 24, payload.size()));

    assert(!decode_header(encoded.data(), 23, header));
    const auto too_large = std::string(kMaxPayloadSize + 1u, 'x');
    assert(encode_message(MessageType::TICK, too_large, 1).empty());

    std::cout << "MT5 Protocol tests: PASS\n";
    return 0;
}
