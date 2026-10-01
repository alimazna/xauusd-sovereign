#include "TelegramGateway.h"

#include <curl/curl.h>

#include <algorithm>
#include <string>
#include <utility>

namespace xauusd::sovereign {
namespace {

struct CurlGlobalInit {
    CurlGlobalInit() { curl_global_init(CURL_GLOBAL_DEFAULT); }
    ~CurlGlobalInit() { curl_global_cleanup(); }
};

const CurlGlobalInit kCurlGlobalInit{};

std::size_t discard_body(char* ptr, std::size_t size, std::size_t count, void*) {
    return size * count;
}

} // namespace

TelegramGateway::TelegramGateway()
    : config_(telegram_config::load()) {}

TelegramGateway::TelegramGateway(telegram_config::Config config)
    : config_(std::move(config)) {}

bool TelegramGateway::select_bot(const std::string& chat_id,
                                 std::string& token,
                                 std::string& default_chat_id) const {
    if (!chat_id.empty() && chat_id == config_.governance.default_chat_id &&
        config_.governance.enabled && !config_.governance.token.empty()) {
        token = config_.governance.token;
        default_chat_id = config_.governance.default_chat_id;
        return true;
    }
    if (config_.operations.enabled && !config_.operations.token.empty()) {
        token = config_.operations.token;
        default_chat_id = config_.operations.default_chat_id;
        return true;
    }
    if (config_.governance.enabled && !config_.governance.token.empty()) {
        token = config_.governance.token;
        default_chat_id = config_.governance.default_chat_id;
        return true;
    }
    return false;
}

bool TelegramGateway::send_http(const std::string& token,
                                const std::string& chat_id,
                                const std::string& text,
                                std::string& failure_reason) const {
    if (token.empty()) {
        failure_reason = "Telegram bot token is not configured";
        return false;
    }
    if (chat_id.empty()) {
        failure_reason = "Telegram chat ID is not configured";
        return false;
    }
    if (text.empty()) {
        failure_reason = "Telegram message text is empty";
        return false;
    }

    CURL* curl = curl_easy_init();
    if (curl == nullptr) {
        failure_reason = "curl_easy_init failed";
        return false;
    }

    char* escaped_chat = curl_easy_escape(curl, chat_id.c_str(), 0);
    char* escaped_text = curl_easy_escape(curl, text.c_str(), 0);
    if (escaped_chat == nullptr || escaped_text == nullptr) {
        if (escaped_chat != nullptr) curl_free(escaped_chat);
        if (escaped_text != nullptr) curl_free(escaped_text);
        curl_easy_cleanup(curl);
        failure_reason = "Telegram form encoding failed";
        return false;
    }

    const std::string url = "https://api.telegram.org/bot" + token + "/sendMessage";
    const std::string post_fields =
        "chat_id=" + std::string(escaped_chat) + "&text=" + std::string(escaped_text);

    curl_free(escaped_chat);
    curl_free(escaped_text);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_body);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, static_cast<long>(config_.send_timeout_ms));
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, static_cast<long>(config_.send_timeout_ms));
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    const CURLcode result = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK) {
        failure_reason = curl_easy_strerror(result);
        return false;
    }
    if (http_code < 200 || http_code >= 300) {
        failure_reason = "Telegram API HTTP status " + std::to_string(http_code);
        return false;
    }
    return true;
}

void TelegramGateway::record(const TelegramMessage& message) {
    messages_.push_back(message);
    if (messages_.size() > 1000) {
        messages_.erase(messages_.begin(), messages_.begin() + 500);
    }
}

bool TelegramGateway::send(const TelegramMessage& message) {
    TelegramMessage recorded = message;
    if (!available_) {
        recorded.sent = false;
        recorded.failure_reason = "gateway unavailable";
        record(recorded);
        return false;
    }

    if (!live_delivery_) {
        const bool result = next_send_result_;
        recorded.sent = result;
        if (!result && recorded.failure_reason.empty()) {
            recorded.failure_reason = "test-mode send failed";
        }
        record(recorded);
        if (result) ++sent_count_;
        next_send_result_ = true;
        return result;
    }

    std::string token;
    std::string default_chat_id;
    if (!select_bot(recorded.chat_id, token, default_chat_id)) {
        recorded.sent = false;
        recorded.failure_reason = "No configured Telegram bot is enabled";
        record(recorded);
        return false;
    }
    if (recorded.chat_id.empty()) {
        recorded.chat_id = default_chat_id;
    }

    std::string failure_reason;
    const bool result = send_http(token, recorded.chat_id, recorded.text, failure_reason);
    recorded.sent = result;
    recorded.failure_reason = result ? std::string{} : std::move(failure_reason);
    record(recorded);
    if (result) ++sent_count_;
    return result;
}

bool TelegramGateway::send_alert(const TelegramAlert& alert) {
    const std::string text = alert.body.empty() ? alert.title : alert.title + "\n" + alert.body;
    TelegramMessage message(
        alert.alert_id,
        TelegramMessageType::ALERT,
        std::string{},
        text,
        alert.created_at,
        false,
        std::string{});
    return send(message);
}

bool TelegramGateway::is_available() const {
    return available_;
}

std::vector<TelegramMessage> TelegramGateway::recent_messages() const {
    constexpr std::size_t kRecentLimit = 100;
    if (messages_.size() <= kRecentLimit) return messages_;
    return std::vector<TelegramMessage>(messages_.end() - static_cast<std::ptrdiff_t>(kRecentLimit), messages_.end());
}

std::size_t TelegramGateway::messages_sent() const {
    return sent_count_;
}

void TelegramGateway::set_available(bool available) {
    available_ = available;
}

void TelegramGateway::set_next_send_result(bool result) {
    next_send_result_ = result;
}

void TelegramGateway::clear() {
    available_ = true;
    next_send_result_ = true;
    messages_.clear();
    sent_count_ = 0;
}

void TelegramGateway::set_live_delivery(bool enabled) {
    live_delivery_ = enabled;
}

} // namespace xauusd::sovereign
