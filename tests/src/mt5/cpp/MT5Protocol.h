#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace xauusd::mt5 {

constexpr std::uint32_t kProtocolMagic = 0x58415553u;
constexpr std::uint16_t kProtocolVersion = 0x0001u;
constexpr std::uint16_t kDefaultPort = 5555u;
constexpr std::uint32_t kMaxPayloadSize = 1024u * 1024u;
constexpr std::uint32_t kHeartbeatTimeoutSeconds = 30u;

// Network representation is explicitly little-endian, independent of host ABI.
// The packed type documents the on-wire layout; encode/decode perform field-wise
// conversion so the protocol remains correct on big-endian hosts too.
#pragma pack(push, 1)
struct MessageHeader {
    std::uint32_t magic;
    std::uint16_t version;
    std::uint16_t type;
    std::uint32_t payload_size;
    std::uint32_t sequence;
    std::uint32_t timestamp;
    std::uint32_t checksum;
};
#pragma pack(pop)

static_assert(sizeof(MessageHeader) == 24, "MessageHeader must be 24 bytes");

enum class MessageType : std::uint16_t {
    HANDSHAKE = 0x0001,
    TICK = 0x0002,
    BAR_CLOSED = 0x0003,
    SYMBOL_SPEC = 0x0004,
    HEARTBEAT = 0x0005,
    COMMAND_RESULT = 0x0006,
    DISCONNECT = 0x0007,

    HANDSHAKE_ACK = 0x0101,
    NEW_ORDER = 0x0102,
    CLOSE_ORDER = 0x0103,
    MODIFY_ORDER = 0x0104,
    CANCEL_ORDER = 0x0105,
    SYMBOL_REQUEST = 0x0106,
    HEARTBEAT_ACK = 0x0107,
    SHUTDOWN = 0x0108
};

std::uint32_t compute_crc32(const std::uint8_t* data, std::size_t size);

std::vector<std::uint8_t> encode_message(
    MessageType type,
    const std::string& json_payload,
    std::uint32_t sequence);

bool decode_header(
    const std::uint8_t* buffer,
    std::size_t buffer_size,
    MessageHeader& out_header);

bool verify_payload_crc(const MessageHeader& header,
                        const std::uint8_t* payload,
                        std::size_t payload_size);

const char* message_type_name(MessageType type) noexcept;

} // namespace xauusd::mt5
