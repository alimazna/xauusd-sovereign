#pragma once

#include "MT5Protocol.h"

#include <atomic>
#include <cstdint>
#include <functional>
#include <string_view>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#ifdef _WIN32
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    using xauusd_mt5_socket_t = SOCKET;
#else
    #include <sys/socket.h>
    using xauusd_mt5_socket_t = int;
#endif

namespace xauusd::mt5 {

class MT5Server {
public:
    using ClientId = std::uint64_t;
    using MessageHandler = std::function<void(ClientId client_id, std::uint16_t type, const std::string& json_payload)>;

    explicit MT5Server(std::uint16_t port = kDefaultPort) noexcept;
    ~MT5Server();

    MT5Server(const MT5Server&) = delete;
    MT5Server& operator=(const MT5Server&) = delete;

    bool start();
    void stop();
    bool is_running() const noexcept;
    void set_message_handler(MessageHandler handler);
    std::uint32_t send(std::uint16_t type, const std::string& json_payload);
    std::uint32_t send_to_client(ClientId client_id, std::uint16_t type, const std::string& json_payload);
    std::uint32_t send_to_timeframe(const std::string& timeframe, std::uint16_t type, const std::string& json_payload);
    bool authenticate_client(ClientId client_id, const std::string& timeframe);
    bool client_matches_timeframe(ClientId client_id, const std::string& timeframe) const;
    bool is_client_authenticated(ClientId client_id) const;
    std::string client_timeframe(ClientId client_id) const;
    void disconnect_client(ClientId client_id);
    std::size_t connected_clients() const;

private:
    struct ClientState {
        explicit ClientState(xauusd_mt5_socket_t s, ClientId id) : socket(s), client_id(id) {}
        xauusd_mt5_socket_t socket;
        ClientId client_id{0};
        std::atomic<bool> active{true};
        std::atomic<bool> authenticated{false};
        std::string timeframe;
        bool has_last_rx_sequence{false};
        std::uint32_t last_rx_sequence{0};
        std::atomic<std::uint64_t> last_activity_ms{0};
        std::mutex send_mutex;
        std::thread thread;
    };

    void accept_loop();
    void client_loop(const std::shared_ptr<ClientState>& client);
    void close_client(const std::shared_ptr<ClientState>& client);

    bool wait_readable(xauusd_mt5_socket_t socket, int timeout_ms) const;
    bool receive_exact(xauusd_mt5_socket_t socket, std::uint8_t* buffer, std::size_t size,
                       int timeout_ms, const std::shared_ptr<ClientState>& client);
    bool send_all(xauusd_mt5_socket_t socket, const std::uint8_t* data, std::size_t size);

    std::uint16_t port_;
    xauusd_mt5_socket_t listen_socket_;
    std::atomic<bool> running_{false};
    bool winsock_started_{false};

    std::thread accept_thread_;
    mutable std::mutex clients_mutex_;
    std::vector<std::shared_ptr<ClientState>> clients_;

    mutable std::mutex handler_mutex_;
    MessageHandler handler_;
    std::atomic<std::uint32_t> sequence_{0};
    std::atomic<ClientId> next_client_id_{0};
};

} // namespace xauusd::mt5
