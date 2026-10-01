#pragma once

#include <string>

namespace xauusd::sovereign {
namespace telegram_config {

struct BotConfig {
    std::string token;
    std::string default_chat_id;
    bool enabled{false};
};

struct Config {
    BotConfig operations;
    BotConfig governance;
    bool require_authentication{true};
    int max_messages_per_minute{30};
    int send_timeout_ms{5000};
};

// Loads configuration from environment variables first, then from
// config/telegram.local.env in the current working directory.
// The local env file is intentionally git-ignored because it contains secrets.
Config load();

} // namespace telegram_config
} // namespace xauusd::sovereign
