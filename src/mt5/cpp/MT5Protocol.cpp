#include "MT5Protocol.h"

#include <array>
#include <chrono>
#include <cstring>

namespace xauusd::mt5 {
namespace {

constexpr std::uint32_t kCrcPolynomial = 0xEDB88320u;

std::array<std::uint32_t, 256> make_crc32_table() {
    std::array<std::uint32_t, 256> table{};
    for (std::uint32_t i = 0; i < table.size(); ++i) {
        std::uint32_t c = i;
        for (int j = 0; j < 8; ++j) {
            c = (c & 1u) ? (kCrcPolynomial ^ (c >> 1u)) : (c >> 1u);
        }
        table[i] = c;
    }
    return table;
}

const std::array<std::uint32_t, 256>& crc_table() {
    static const auto table = make_crc32_table();
    return table;
}

void put_u16_le(std::uint8_t* dst, std::uint16_t value) {
    dst[0] = static_cast<std::uint8_t>(value & 0xFFu);
    dst[1] = static_cast<std::uint8_t>((value >> 8u) & 0xFFu);
}

void put_u32_le(std::uint8_t* dst, std::uint32_t value) {
    dst[0] = static_cast<std::uint8_t>(value & 0xFFu);
    dst[1] = static_cast<std::uint8_t>((value >> 8u) & 0xFFu);
    dst[2] = static_cast<std::uint8_t>((value >> 16u) & 0xFFu);
    dst[3] = static_cast<std::uint8_t>((value >> 24u) & 0xFFu);
}

std::uint16_t get_u16_le(const std::uint8_t* src) {
    return static_cast<std::uint16_t>(src[0]) |
           static_cast<std::uint16_t>(src[1] << 8u);
}

std::uint32_t get_u32_le(const std::uint8_t* src) {
    return static_cast<std::uint32_t>(src[0]) |
           (static_cast<std::uint32_t>(src[1]) << 8u) |
           (static_cast<std::uint32_t>(src[2]) << 16u) |
           (static_cast<std::uint32_t>(src[3]) << 24u);
}

std::uint32_t epoch_seconds() {
    const auto now = std::chrono::system_clock::now();
    return static_cast<std::uint32_t>(
        std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count());
}

} // namespace

std::uint32_t compute_crc32(const std::uint8_t* data, std::size_t size) {
    if (data == nullptr && size != 0) {
        return 0;
    }
    const auto& table = crc_table();
    std::uint32_t crc = 0xFFFFFFFFu;
    for (std::size_t i = 0; i < size; ++i) {
        crc = table[(crc ^ data[i]) & 0xFFu] ^ (crc >> 8u);
    }
    return crc ^ 0xFFFFFFFFu;
}

std::vector<std::uint8_t> encode_message(
    MessageType type,
    const std::string& json_payload,
    std::uint32_t sequence) {
    if (json_payload.size() > kMaxPayloadSize) {
        return {};
    }

    MessageHeader header{};
    header.magic = kProtocolMagic;
    header.version = kProtocolVersion;
    header.type = static_cast<std::uint16_t>(type);
    header.payload_size = static_cast<std::uint32_t>(json_payload.size());
    header.sequence = sequence;
    header.timestamp = epoch_seconds();
    header.checksum = compute_crc32(
        reinterpret_cast<const std::uint8_t*>(json_payload.data()), json_payload.size());

    std::vector<std::uint8_t> out(sizeof(MessageHeader) + json_payload.size());
    // Field-wise serialization avoids host-endian assumptions.
    put_u32_le(out.data() + 0, header.magic);
    put_u16_le(out.data() + 4, header.version);
    put_u16_le(out.data() + 6, header.type);
    put_u32_le(out.data() + 8, header.payload_size);
    put_u32_le(out.data() + 12, header.sequence);
    put_u32_le(out.data() + 16, header.timestamp);
    put_u32_le(out.data() + 20, header.checksum);
    if (!json_payload.empty()) {
        std::memcpy(out.data() + sizeof(MessageHeader), json_payload.data(), json_payload.size());
    }
    return out;
}

bool decode_header(
    const std::uint8_t* buffer,
    std::size_t buffer_size,
    MessageHeader& out_header) {
    if (buffer == nullptr || buffer_size < sizeof(MessageHeader)) {
        return false;
    }

    out_header.magic = get_u32_le(buffer + 0);
    out_header.version = get_u16_le(buffer + 4);
    out_header.type = get_u16_le(buffer + 6);
    out_header.payload_size = get_u32_le(buffer + 8);
    out_header.sequence = get_u32_le(buffer + 12);
    out_header.timestamp = get_u32_le(buffer + 16);
    out_header.checksum = get_u32_le(buffer + 20);

    if (out_header.magic != kProtocolMagic || out_header.version != kProtocolVersion) {
        return false;
    }
    if (out_header.payload_size > kMaxPayloadSize) {
        return false;
    }
    return true;
}

bool verify_payload_crc(const MessageHeader& header,
                        const std::uint8_t* payload,
                        std::size_t payload_size) {
    if (payload_size != header.payload_size || payload_size > kMaxPayloadSize) {
        return false;
    }
    return compute_crc32(payload, payload_size) == header.checksum;
}

const char* message_type_name(MessageType type) noexcept {
    switch (type) {
        case MessageType::HANDSHAKE: return "HANDSHAKE";
        case MessageType::TICK: return "TICK";
        case MessageType::BAR_CLOSED: return "BAR_CLOSED";
        case MessageType::SYMBOL_SPEC: return "SYMBOL_SPEC";
        case MessageType::HEARTBEAT: return "HEARTBEAT";
        case MessageType::COMMAND_RESULT: return "COMMAND_RESULT";
        case MessageType::DISCONNECT: return "DISCONNECT";
        case MessageType::HANDSHAKE_ACK: return "HANDSHAKE_ACK";
        case MessageType::NEW_ORDER: return "NEW_ORDER";
        case MessageType::CLOSE_ORDER: return "CLOSE_ORDER";
        case MessageType::MODIFY_ORDER: return "MODIFY_ORDER";
        case MessageType::CANCEL_ORDER: return "CANCEL_ORDER";
        case MessageType::SYMBOL_REQUEST: return "SYMBOL_REQUEST";
        case MessageType::HEARTBEAT_ACK: return "HEARTBEAT_ACK";
        case MessageType::SHUTDOWN: return "SHUTDOWN";
        default: return "UNKNOWN";
    }
}

} // namespace xauusd::mt5
