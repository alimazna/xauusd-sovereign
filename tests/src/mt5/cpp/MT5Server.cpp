#include "MT5Server.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <limits>

#ifndef _WIN32
    #include <arpa/inet.h>
    #include <cerrno>
    #include <fcntl.h>
    #include <netinet/in.h>
    #include <sys/select.h>
    #include <unistd.h>
#else
    #include <mstcpip.h>
#endif

namespace xauusd::mt5 {
namespace {

std::uint64_t steady_ms() {
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count());
}

void close_socket(xauusd_mt5_socket_t socket) noexcept {
#ifdef _WIN32
    if (socket != INVALID_SOCKET) {
        ::shutdown(socket, SD_BOTH);
        ::closesocket(socket);
    }
#else
    if (socket >= 0) {
        ::shutdown(socket, SHUT_RDWR);
        ::close(socket);
    }
#endif
}

bool socket_valid(xauusd_mt5_socket_t socket) noexcept {
#ifdef _WIN32
    return socket != INVALID_SOCKET;
#else
    return socket >= 0;
#endif
}

void set_reuse_address(xauusd_mt5_socket_t socket) {
    int opt = 1;
#ifdef _WIN32
    ::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR,
                 reinterpret_cast<const char*>(&opt), sizeof(opt));
#else
    ::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif
}

} // namespace

MT5Server::MT5Server(std::uint16_t port) noexcept
    : port_(port),
#ifdef _WIN32
      listen_socket_(INVALID_SOCKET)
#else
      listen_socket_(-1)
#endif
{}

MT5Server::~MT5Server() {
    stop();
}

bool MT5Server::start() {
    if (running_.exchange(true)) {
        return true;
    }

#ifdef _WIN32
    WSADATA wsa_data{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        running_.store(false);
        return false;
    }
    winsock_started_ = true;
#endif

    listen_socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!socket_valid(listen_socket_)) {
#ifdef _WIN32
        WSACleanup();
        winsock_started_ = false;
#endif
        running_.store(false);
        return false;
    }

    set_reuse_address(listen_socket_);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (::bind(listen_socket_, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) ==
#ifdef _WIN32
        SOCKET_ERROR
#else
        -1
#endif
        || ::listen(listen_socket_, SOMAXCONN) ==
#ifdef _WIN32
        SOCKET_ERROR
#else
        -1
#endif
    ) {
        close_socket(listen_socket_);
#ifdef _WIN32
        listen_socket_ = INVALID_SOCKET;
        WSACleanup();
        winsock_started_ = false;
#else
        listen_socket_ = -1;
#endif
        running_.store(false);
        return false;
    }

    accept_thread_ = std::thread(&MT5Server::accept_loop, this);
    std::cerr << "[MT5] Server listening on 127.0.0.1:" << port_ << "\n";
    return true;
}

void MT5Server::stop() {
    const bool was_running = running_.exchange(false);
    if (was_running && socket_valid(listen_socket_)) {
        close_socket(listen_socket_);
#ifdef _WIN32
        listen_socket_ = INVALID_SOCKET;
#else
        listen_socket_ = -1;
#endif
    }

    if (accept_thread_.joinable()) {
        accept_thread_.join();
    }

    std::vector<std::shared_ptr<ClientState>> clients;
    {
        std::lock_guard lock(clients_mutex_);
        clients = clients_;
    }
    for (const auto& client : clients) {
        close_client(client);
    }
    for (const auto& client : clients) {
        if (client->thread.joinable()) {
            client->thread.join();
        }
    }
    {
        std::lock_guard lock(clients_mutex_);
        clients_.clear();
    }

#ifdef _WIN32
    if (winsock_started_) {
        WSACleanup();
        winsock_started_ = false;
    }
#endif
}

bool MT5Server::is_running() const noexcept {
    return running_.load();
}

void MT5Server::set_message_handler(MessageHandler handler) {
    std::lock_guard lock(handler_mutex_);
    handler_ = std::move(handler);
}

std::size_t MT5Server::connected_clients() const {
    std::lock_guard lock(clients_mutex_);
    return static_cast<std::size_t>(std::count_if(
        clients_.begin(), clients_.end(), [](const auto& client) { return client->active.load(); }));
}

std::uint32_t MT5Server::send(std::uint16_t type, const std::string& json_payload) {
    if (!is_running() || json_payload.size() > kMaxPayloadSize) {
        return 0;
    }

    const auto message = encode_message(
        static_cast<MessageType>(type), json_payload, ++sequence_);
    if (message.empty()) {
        return 0;
    }

    std::vector<std::shared_ptr<ClientState>> clients;
    {
        std::lock_guard lock(clients_mutex_);
        clients = clients_;
    }

    std::size_t delivered = 0;
    for (const auto& client : clients) {
        if (!client->active.load() || !client->authenticated.load()) {
            continue;
        }
        std::lock_guard send_lock(client->send_mutex);
        if (send_all(client->socket, message.data(), message.size())) {
            ++delivered;
        } else {
            close_client(client);
        }
    }
    return delivered == 0 ? 0u : sequence_.load();
}

std::uint32_t MT5Server::send_to_client(ClientId client_id, std::uint16_t type,
                                         const std::string& json_payload) {
    if (!is_running() || json_payload.size() > kMaxPayloadSize || client_id == 0) {
        return 0;
    }
    const auto message = encode_message(static_cast<MessageType>(type), json_payload, ++sequence_);
    if (message.empty()) return 0;

    std::shared_ptr<ClientState> target;
    {
        std::lock_guard lock(clients_mutex_);
        const auto it = std::find_if(clients_.begin(), clients_.end(),
            [client_id](const auto& client) { return client->client_id == client_id; });
        if (it != clients_.end()) target = *it;
    }
    if (!target || !target->active.load() || !target->authenticated.load()) return 0;

    std::lock_guard send_lock(target->send_mutex);
    if (!send_all(target->socket, message.data(), message.size())) {
        close_client(target);
        return 0;
    }
    return sequence_.load();
}

std::uint32_t MT5Server::send_to_timeframe(const std::string& timeframe,
                                            std::uint16_t type,
                                            const std::string& json_payload) {
    if (!is_running() || timeframe.empty()) return 0;

    ClientId target_id = 0;
    {
        std::lock_guard lock(clients_mutex_);
        for (const auto& client : clients_) {
            if (client->active.load() && client->authenticated.load() &&
                client->timeframe == timeframe) {
                if (target_id != 0) {
                    std::cerr << "[MT5] Duplicate authenticated timeframe " << timeframe
                              << "; refusing ambiguous command route\n";
                    return 0;
                }
                target_id = client->client_id;
            }
        }
    }
    return send_to_client(target_id, type, json_payload);
}

bool MT5Server::authenticate_client(ClientId client_id, const std::string& timeframe) {
    if (client_id == 0 || timeframe.empty()) return false;
    std::lock_guard lock(clients_mutex_);
    auto it = std::find_if(clients_.begin(), clients_.end(),
        [client_id](const auto& client) { return client->client_id == client_id; });
    if (it == clients_.end()) return false;
    const auto& client = *it;
    if (client->authenticated.load()) {
        return client->timeframe == timeframe;
    }
    const bool duplicate = std::any_of(clients_.begin(), clients_.end(),
        [&](const auto& other) {
            return other.get() != client.get() && other->active.load() &&
                   other->authenticated.load() && other->timeframe == timeframe;
        });
    if (duplicate) return false;
    client->timeframe = timeframe;
    client->authenticated.store(true);
    return true;
}

bool MT5Server::client_matches_timeframe(ClientId client_id, const std::string& timeframe) const {
    std::lock_guard lock(clients_mutex_);
    const auto it = std::find_if(clients_.begin(), clients_.end(),
        [client_id](const auto& client) { return client->client_id == client_id; });
    return it != clients_.end() && (*it)->active.load() && (*it)->authenticated.load() &&
           (*it)->timeframe == timeframe;
}

bool MT5Server::is_client_authenticated(ClientId client_id) const {
    std::lock_guard lock(clients_mutex_);
    const auto it = std::find_if(clients_.begin(), clients_.end(),
        [client_id](const auto& client) { return client->client_id == client_id; });
    return it != clients_.end() && (*it)->active.load() && (*it)->authenticated.load();
}

std::string MT5Server::client_timeframe(ClientId client_id) const {
    std::lock_guard lock(clients_mutex_);
    const auto it = std::find_if(clients_.begin(), clients_.end(),
        [client_id](const auto& client) { return client->client_id == client_id; });
    if (it == clients_.end() || !(*it)->active.load() || !(*it)->authenticated.load()) return {};
    return (*it)->timeframe;
}

void MT5Server::disconnect_client(ClientId client_id) {
    std::shared_ptr<ClientState> target;
    {
        std::lock_guard lock(clients_mutex_);
        const auto it = std::find_if(clients_.begin(), clients_.end(),
            [client_id](const auto& client) { return client->client_id == client_id; });
        if (it != clients_.end()) target = *it;
    }
    close_client(target);
}

void MT5Server::accept_loop() {
    while (running_.load()) {
        if (!wait_readable(listen_socket_, 250)) {
            continue;
        }

        sockaddr_in client_addr{};
#ifdef _WIN32
        int addr_len = sizeof(client_addr);
#else
        socklen_t addr_len = sizeof(client_addr);
#endif
        const auto client_socket = ::accept(
            listen_socket_, reinterpret_cast<sockaddr*>(&client_addr), &addr_len);
        if (!socket_valid(client_socket)) {
            if (running_.load()) {
                continue;
            }
            break;
        }

        auto client = std::make_shared<ClientState>(client_socket, ++next_client_id_);
        client->last_activity_ms.store(steady_ms());
        {
            std::lock_guard lock(clients_mutex_);
            clients_.push_back(client);
        }
        client->thread = std::thread(&MT5Server::client_loop, this, client);
    }
}

void MT5Server::client_loop(const std::shared_ptr<ClientState>& client) {
    std::array<std::uint8_t, sizeof(MessageHeader)> header_bytes{};
    while (running_.load() && client->active.load()) {
        const auto last = client->last_activity_ms.load();
        if (steady_ms() > last + (static_cast<std::uint64_t>(kHeartbeatTimeoutSeconds) * 1000ull)) {
            std::cerr << "[MT5] Closing idle client after 30s without protocol traffic\n";
            break;
        }

        if (!receive_exact(client->socket, header_bytes.data(), header_bytes.size(), 1000, client)) {
            continue;
        }

        MessageHeader header{};
        if (!decode_header(header_bytes.data(), header_bytes.size(), header)) {
            std::cerr << "[MT5] Invalid header; closing client\n";
            break;
        }

        std::vector<std::uint8_t> payload(header.payload_size);
        if (!payload.empty() &&
            !receive_exact(client->socket, payload.data(), payload.size(), 1000, client)) {
            continue;
        }
        if (!verify_payload_crc(header, payload.data(), payload.size())) {
            std::cerr << "[MT5] CRC mismatch; dropping message\n";
            continue;
        }

        if (client->has_last_rx_sequence) {
            const auto delta = static_cast<std::int32_t>(header.sequence - client->last_rx_sequence);
            if (delta <= 0) {
                std::cerr << "[MT5] Replay/out-of-order frame sequence=" << header.sequence
                          << " last=" << client->last_rx_sequence << "\n";
                continue;
            }
        }
        client->last_rx_sequence = header.sequence;
        client->has_last_rx_sequence = true;

        const auto message_type = static_cast<MessageType>(header.type);
        if (!client->authenticated.load() && message_type != MessageType::HANDSHAKE) {
            std::cerr << "[MT5] Client must authenticate with HANDSHAKE first\n";
            break;
        }

        client->last_activity_ms.store(steady_ms());
        std::string json(payload.begin(), payload.end());
        MessageHandler handler;
        {
            std::lock_guard lock(handler_mutex_);
            handler = handler_;
        }
        if (handler) {
            handler(client->client_id, header.type, json);
        }
    }
    close_client(client);
}

void MT5Server::close_client(const std::shared_ptr<ClientState>& client) {
    if (!client || !client->active.exchange(false)) {
        return;
    }
    close_socket(client->socket);
}

bool MT5Server::wait_readable(xauusd_mt5_socket_t socket, int timeout_ms) const {
    if (!socket_valid(socket)) {
        return false;
    }
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(socket, &read_set);
    timeval timeout{};
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
#ifdef _WIN32
    const int result = ::select(0, &read_set, nullptr, nullptr, &timeout);
#else
    const int result = ::select(socket + 1, &read_set, nullptr, nullptr, &timeout);
#endif
    return result > 0 && FD_ISSET(socket, &read_set);
}

bool MT5Server::receive_exact(
    xauusd_mt5_socket_t socket,
    std::uint8_t* buffer,
    std::size_t size,
    int timeout_ms,
    const std::shared_ptr<ClientState>& client) {
    std::size_t received = 0;
    while (received < size && running_.load() && client->active.load()) {
        if (!wait_readable(socket, timeout_ms)) {
            return false;
        }
        const std::size_t remaining = size - received;
#ifdef _WIN32
        const int chunk = ::recv(socket, reinterpret_cast<char*>(buffer + received),
                                 static_cast<int>(std::min<std::size_t>(remaining, INT_MAX)), 0);
#else
        const ssize_t chunk = ::recv(socket, buffer + received, remaining, 0);
#endif
        if (chunk <= 0) {
            client->active.store(false);
            return false;
        }
        received += static_cast<std::size_t>(chunk);
    }
    return received == size;
}

bool MT5Server::send_all(xauusd_mt5_socket_t socket, const std::uint8_t* data, std::size_t size) {
    std::size_t sent = 0;
    while (sent < size) {
#ifdef _WIN32
        const int chunk = ::send(socket, reinterpret_cast<const char*>(data + sent),
                                 static_cast<int>(std::min<std::size_t>(size - sent, INT_MAX)), 0);
#else
        const ssize_t chunk = ::send(socket, data + sent, size - sent, MSG_NOSIGNAL);
#endif
        if (chunk <= 0) {
            return false;
        }
        sent += static_cast<std::size_t>(chunk);
    }
    return true;
}

} // namespace xauusd::mt5
