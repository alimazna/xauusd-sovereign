#include "EngineIdentity.h"
#include "Clock.h"
#include "TelegramConfig.h"
#include "TelegramGateway.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace xauusd::sovereign;

#define CHECK(expr) do { \
    if (!(expr)) { std::cerr << "CHECK failed: " #expr << "\n"; return 1; } \
} while (false)

int main() {
    const auto config = telegram_config::load();
    CHECK(!config.operations.token.empty());
    CHECK(!config.governance.token.empty());
    CHECK(config.operations.enabled);
    CHECK(config.governance.enabled);

    TelegramGateway gateway(config);
    gateway.set_live_delivery(false);
    TelegramMessage message(
        detail::make_id("telegram_test", 1, 2, 3), TelegramMessageType::STATUS_UPDATE, "123", "hello", detail::now_timestamp(), false, {});
    gateway.set_next_send_result(true);
    CHECK(gateway.send(message));
    CHECK(gateway.messages_sent() == 1);
    CHECK(gateway.recent_messages().back().chat_id == "123");

    gateway.set_available(false);
    CHECK(!gateway.send(message));
    CHECK(gateway.recent_messages().back().failure_reason == "gateway unavailable");

    TelegramGateway live_gateway(config);
    live_gateway.set_available(true);
    live_gateway.set_live_delivery(true);
    TelegramMessage no_chat(
        detail::make_id("telegram_test_no_chat", 1, 2, 3),
        TelegramMessageType::STATUS_UPDATE,
        "",
        "hello",
        detail::now_timestamp(),
        false,
        {});
    CHECK(live_gateway.config().operations.enabled);
    CHECK(live_gateway.config().governance.enabled);
    CHECK(!live_gateway.send(no_chat));
    CHECK(live_gateway.recent_messages().back().failure_reason == "Telegram chat ID is not configured");

    std::cout << "Telegram config/gateway test PASS\n";
    return 0;
}
