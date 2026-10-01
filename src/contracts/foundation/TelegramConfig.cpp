#include "TelegramConfig.h"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace xauusd::sovereign::telegram_config {
namespace {

std::string trim(std::string value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }
    const auto last = value.find_last_not_of(" \t\r\n");
    value = value.substr(first, last - first + 1);
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
        value = value.substr(1, value.size() - 2);
    }
    return value;
}

std::unordered_map<std::string, std::string> load_local_env() {
    std::unordered_map<std::string, std::string> values;
    std::ifstream input("config/telegram.local.env");
    if (!input) {
        return values;
    }

    std::string line;
    while (std::getline(input, line)) {
        line = trim(line);
        if (line.empty() || line.front() == '#') {
            continue;
        }
        const auto pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }
        const auto key = trim(line.substr(0, pos));
        const auto value = trim(line.substr(pos + 1));
        if (!key.empty()) {
            values[key] = value;
        }
    }
    return values;
}

std::string value(const std::unordered_map<std::string, std::string>& local,
                  const char* key) {
    if (const char* env = std::getenv(key); env != nullptr && *env != '\0') {
        return env;
    }
    const auto it = local.find(key);
    return it == local.end() ? std::string{} : it->second;
}

bool bool_value(const std::unordered_map<std::string, std::string>& local,
                const char* key, bool fallback) {
    const auto raw = value(local, key);
    if (raw.empty()) {
        return fallback;
    }
    return raw == "1" || raw == "true" || raw == "TRUE" || raw == "yes" || raw == "on";
}

int int_value(const std::unordered_map<std::string, std::string>& local,
              const char* key, int fallback) {
    const auto raw = value(local, key);
    if (raw.empty()) {
        return fallback;
    }
    try {
        return std::stoi(raw);
    } catch (...) {
        return fallback;
    }
}

} // namespace

Config load() {
    const auto local = load_local_env();

    Config config;
    config.operations.token = value(local, "XAUS_TELEGRAM_OPERATIONS_TOKEN");
    config.governance.token = value(local, "XAUS_TELEGRAM_GOVERNANCE_TOKEN");
    config.operations.default_chat_id = value(local, "XAUS_TELEGRAM_PRIMARY_CHAT_ID");
    config.governance.default_chat_id = value(local, "XAUS_TELEGRAM_GOVERNANCE_CHAT_ID");

    config.operations.enabled = bool_value(
        local, "XAUS_TELEGRAM_ENABLE_OPERATIONS", !config.operations.token.empty());
    config.governance.enabled = bool_value(
        local, "XAUS_TELEGRAM_ENABLE_GOVERNANCE", !config.governance.token.empty());
    config.require_authentication = bool_value(
        local, "XAUS_TELEGRAM_REQUIRE_AUTH", true);
    config.max_messages_per_minute = int_value(
        local, "XAUS_TELEGRAM_MAX_MESSAGES_PER_MINUTE", 30);
    config.send_timeout_ms = int_value(
        local, "XAUS_TELEGRAM_SEND_TIMEOUT_MS", 5000);

    return config;
}

} // namespace xauusd::sovereign::telegram_config
