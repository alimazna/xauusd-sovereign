#include "../../src/mt5/cpp/MT5Protocol.h"
#include "../../src/mt5/cpp/MT5Server.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    using test_socket_t = SOCKET;
    static constexpr test_socket_t kInvalidSocket = INVALID_SOCKET;
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <unistd.h>
    using test_socket_t = int;
    static constexpr test_socket_t kInvalidSocket = -1;
#endif

using namespace xauusd::mt5;

namespace {

void close_test_socket(test_socket_t socket) {
#ifdef _WIN32
    if (socket != INVALID_SOCKET) { ::shutdown(socket, SD_BOTH); ::closesocket(socket); }
#else
    if (socket >= 0) { ::shutdown(socket, SHUT_RDWR); ::close(socket); }
#endif
}

bool send_all(test_socket_t socket, const std::uint8_t* data, std::size_t size) {
    std::size_t sent = 0;
    while (sent < size) {
#ifdef _WIN32
        const int n = ::send(socket, reinterpret_cast<const char*>(data + sent),
                             static_cast<int>(size - sent), 0);
#else
        const ssize_t n = ::send(socket, data + sent, size - sent, MSG_NOSIGNAL);
#endif
        if (n <= 0) return false;
        sent += static_cast<std::size_t>(n);
    }
    return true;
}

bool read_exact(test_socket_t socket, std::uint8_t* data, std::size_t size, int timeout_ms) {
    std::size_t received = 0;
    while (received < size) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(socket, &set);
        timeval tv{timeout_ms / 1000, (timeout_ms % 1000) * 1000};
#ifdef _WIN32
        if (::select(0, &set, nullptr, nullptr, &tv) <= 0) return false;
#else
        if (::select(socket + 1, &set, nullptr, nullptr, &tv) <= 0) return false;
#endif
#ifdef _WIN32
        const int n = ::recv(socket, reinterpret_cast<char*>(data + received),
                             static_cast<int>(size - received), 0);
#else
        const ssize_t n = ::recv(socket, data + received, size - received, 0);
#endif
        if (n <= 0) return false;
        received += static_cast<std::size_t>(n);
    }
    return true;
}

} // namespace

int main() {
#ifdef _WIN32
    WSADATA wsa{};
    assert(WSAStartup(MAKEWORD(2, 2), &wsa) == 0);
#endif

    constexpr std::uint16_t port = 15555;
    MT5Server server(port);
    std::atomic<bool> received{false};
    std::atomic<int> message_count{0};
    std::string received_payload;
    std::atomic<std::uint64_t> client_id{0};
    server.set_message_handler([&](MT5Server::ClientId id, std::uint16_t type, const std::string& payload) {
        ++message_count;
        if (type == static_cast<std::uint16_t>(MessageType::HANDSHAKE) &&
            payload.find("M15") != std::string::npos) {
            client_id.store(id);
            assert(server.authenticate_client(id,"M15"));
            received_payload = payload;
            received.store(true);
        }
    });

    assert(server.start());

    test_socket_t client = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(client != kInvalidSocket);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    assert(::connect(client, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == 0);

    const std::string handshake = R"({"tf":"M15","version":1})";
    const auto wire = encode_message(MessageType::HANDSHAKE, handshake, 1);
    assert(send_all(client, wire.data(), wire.size()));

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (!received.load() && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    assert(received.load());
    assert(received_payload == handshake);
    assert(server.connected_clients() == 1);

    assert(server.send_to_timeframe("M15", static_cast<std::uint16_t>(MessageType::HEARTBEAT_ACK), "{}") != 0);
    std::uint8_t header_bytes[24]{};
    assert(read_exact(client, header_bytes, sizeof(header_bytes), 1000));
    MessageHeader header{};
    assert(decode_header(header_bytes, sizeof(header_bytes), header));
    assert(header.type == static_cast<std::uint16_t>(MessageType::HEARTBEAT_ACK));
    assert(header.payload_size == 2);
    std::uint8_t payload[2]{};
    assert(read_exact(client, payload, sizeof(payload), 1000));
    assert(payload[0] == '{' && payload[1] == '}');
    const int before_replay = message_count.load();
    assert(send_all(client, wire.data(), wire.size()));
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    assert(message_count.load() == before_replay);

    close_test_socket(client);
    server.stop();
    assert(!server.is_running());

#ifdef _WIN32
    WSACleanup();
#endif
    std::cout << "MT5 Server tests: PASS\n";
    return 0;
}
