#include "../../src/mt5/cpp/MT5Integration.h"
#include "../../src/mt5/cpp/MT5Protocol.h"
#include "../../src/contracts/foundation/EntityId.h"
#include "../../src/contracts/foundation/ShadowLedger.h"
#include "../../src/contracts/foundation/RuntimeEngine.h"

#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    using test_socket_t = SOCKET;
    static constexpr test_socket_t kInvalidSocket = INVALID_SOCKET;
#else
    #include <arpa/inet.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <unistd.h>
    using test_socket_t = int;
    static constexpr test_socket_t kInvalidSocket = -1;
#endif

using namespace xauusd::mt5;

namespace {
void close_socket(test_socket_t s) {
#ifdef _WIN32
    if (s != INVALID_SOCKET) { ::shutdown(s, SD_BOTH); ::closesocket(s); }
#else
    if (s >= 0) { ::shutdown(s, SHUT_RDWR); ::close(s); }
#endif
}

bool send_all(test_socket_t socket, const std::uint8_t* data, std::size_t size) {
    std::size_t sent=0;
    while(sent<size) {
#ifdef _WIN32
        int n=::send(socket,reinterpret_cast<const char*>(data+sent),static_cast<int>(size-sent),0);
#else
        ssize_t n=::send(socket,data+sent,size-sent,MSG_NOSIGNAL);
#endif
        if(n<=0) return false;
        sent += static_cast<std::size_t>(n);
    }
    return true;
}

bool read_exact(test_socket_t socket, std::uint8_t* data, std::size_t size, int timeout_ms) {
    std::size_t received=0;
    while(received<size) {
        fd_set set; FD_ZERO(&set); FD_SET(socket,&set);
        timeval tv{timeout_ms/1000,(timeout_ms%1000)*1000};
#ifdef _WIN32
        if(::select(0,&set,nullptr,nullptr,&tv)<=0) return false;
#else
        if(::select(socket+1,&set,nullptr,nullptr,&tv)<=0) return false;
#endif
#ifdef _WIN32
        int n=::recv(socket,reinterpret_cast<char*>(data+received),static_cast<int>(size-received),0);
#else
        ssize_t n=::recv(socket,data+received,size-received,0);
#endif
        if(n<=0) return false;
        received += static_cast<std::size_t>(n);
    }
    return true;
}

test_socket_t connect_client(std::uint16_t port) {
    test_socket_t s=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    assert(s!=kInvalidSocket);
    sockaddr_in addr{}; addr.sin_family=AF_INET; addr.sin_port=htons(port); addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    assert(::connect(s,reinterpret_cast<const sockaddr*>(&addr),sizeof(addr))==0);
    return s;
}

void send_handshake(test_socket_t client, const std::string& tf, std::uint32_t seq) {
    const std::string json = "{\"tf\":\"" + tf + "\",\"version\":1,\"token\":\"test-token\"}";
    const auto wire=encode_message(MessageType::HANDSHAKE,json,seq);
    assert(send_all(client,wire.data(),wire.size()));
    std::uint8_t header[24]{}; assert(read_exact(client,header,sizeof(header),1000));
    MessageHeader decoded{}; assert(decode_header(header,sizeof(header),decoded));
    assert(decoded.type==static_cast<std::uint16_t>(MessageType::HANDSHAKE_ACK));
    std::vector<std::uint8_t> payload(decoded.payload_size);
    if(!payload.empty()) assert(read_exact(client,payload.data(),payload.size(),1000));
    const std::string ack(payload.begin(),payload.end());
    assert(ack.find("\"tf\":\""+tf+"\"")!=std::string::npos);
}

} // namespace

int main() {
#ifdef _WIN32
    WSADATA wsa{}; assert(WSAStartup(MAKEWORD(2,2),&wsa)==0);
#endif
    constexpr std::uint16_t port=15556;
    xauusd::sovereign::ShadowLedger ledger;
    xauusd::sovereign::RuntimeEngine runtime(nullptr,nullptr,&ledger);
    MT5Integration integration(port,runtime,ledger,"test-token");
    assert(integration.start());

    auto bad=connect_client(port);
    const std::string bad_json = "{\"tf\":\"M1\",\"version\":1,\"token\":\"wrong-token\"}";
    const auto bad_wire=encode_message(MessageType::HANDSHAKE,bad_json,1);
    assert(send_all(bad,bad_wire.data(),bad_wire.size()));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    close_socket(bad);

    auto m15=connect_client(port);
    auto m30=connect_client(port);
    send_handshake(m15,"M15",1);
    send_handshake(m30,"M30",1);

    // A second authenticated M15 route must be refused as ambiguous.
    auto duplicate=connect_client(port);
    const std::string dup_json = "{\"tf\":\"M15\",\"version\":1,\"token\":\"test-token\"}";
    const auto dup_wire=encode_message(MessageType::HANDSHAKE,dup_json,1);
    assert(send_all(duplicate,dup_wire.data(),dup_wire.size()));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    const std::array<std::uint8_t,16> bytes{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    const xauusd::sovereign::EntityId id(bytes);
    assert(integration.submit_new_order("M15",id,"XAUUSD","BUY",0.01,2650.0,2640.0,2670.0,"test"));

    std::uint8_t header[24]{}; assert(read_exact(m15,header,sizeof(header),1000));
    MessageHeader decoded{}; assert(decode_header(header,sizeof(header),decoded));
    assert(decoded.type==static_cast<std::uint16_t>(MessageType::NEW_ORDER));
    std::vector<std::uint8_t> payload(decoded.payload_size);
    if(!payload.empty()) assert(read_exact(m15,payload.data(),payload.size(),1000));
    const std::string order(payload.begin(),payload.end());
    assert(order.find("\"tf\":\"M15\"")!=std::string::npos);
    assert(order.find("\"side\":\"BUY\"")!=std::string::npos);

    assert(integration.submit_close_order("M15",id,1001));
    assert(read_exact(m15,header,sizeof(header),1000));
    assert(decode_header(header,sizeof(header),decoded));
    assert(decoded.type==static_cast<std::uint16_t>(MessageType::CLOSE_ORDER));
    payload.assign(decoded.payload_size,0);
    if(!payload.empty()) assert(read_exact(m15,payload.data(),payload.size(),1000));
    const std::string close_order(payload.begin(),payload.end());
    assert(close_order.find("\"tf\":\"M15\"")!=std::string::npos);
    assert(close_order.find("\"ticket\":1001")!=std::string::npos);

    assert(integration.submit_modify_order("M15",id,1002,2641.0,2661.0));
    assert(read_exact(m15,header,sizeof(header),1000));
    assert(decode_header(header,sizeof(header),decoded));
    assert(decoded.type==static_cast<std::uint16_t>(MessageType::MODIFY_ORDER));
    payload.assign(decoded.payload_size,0);
    if(!payload.empty()) assert(read_exact(m15,payload.data(),payload.size(),1000));
    const std::string modify_order(payload.begin(),payload.end());
    assert(modify_order.find("\"tf\":\"M15\"")!=std::string::npos);
    assert(modify_order.find("\"ticket\":1002")!=std::string::npos);

    assert(integration.submit_cancel_order("M15",id,1003));
    assert(read_exact(m15,header,sizeof(header),1000));
    assert(decode_header(header,sizeof(header),decoded));
    assert(decoded.type==static_cast<std::uint16_t>(MessageType::CANCEL_ORDER));
    payload.assign(decoded.payload_size,0);
    if(!payload.empty()) assert(read_exact(m15,payload.data(),payload.size(),1000));
    const std::string cancel_order(payload.begin(),payload.end());
    assert(cancel_order.find("\"tf\":\"M15\"")!=std::string::npos);
    assert(cancel_order.find("\"ticket\":1003")!=std::string::npos);

    fd_set set; FD_ZERO(&set); FD_SET(m30,&set); timeval tv{0,200000};
#ifdef _WIN32
    const int ready=::select(0,&set,nullptr,nullptr,&tv);
#else
    const int ready=::select(m30+1,&set,nullptr,nullptr,&tv);
#endif
    assert(ready==0); // M30 must not receive an M15 command.

    close_socket(duplicate);
    close_socket(m15); close_socket(m30); integration.stop();
#ifdef _WIN32
    WSACleanup();
#endif
    std::cout << "MT5 Integration routing/auth tests: PASS\n";
    return 0;
}
