#pragma once

#include "ITelegramGateway.h"
#include "TelegramConfig.h"

#include <cstddef>
#include <string>
#include <vector>

namespace xauusd::sovereign {

class TelegramGateway final : public ITelegramGateway {
public:
    TelegramGateway();
    explicit TelegramGateway(telegram_config::Config config);

    bool send(const TelegramMessage& message) override;
    bool send_alert(const TelegramAlert& alert) override;
    bool is_available() const override;
    std::vector<TelegramMessage> recent_messages() const override;
    std::size_t messages_sent() const override;

    // Test helpers / deterministic test mode.
    void set_available(bool available);
    void set_next_send_result(bool result);
    void clear();
    void set_live_delivery(bool enabled);

    const telegram_config::Config& config() const { return config_; }

private:
    bool send_http(const std::string& token,
                   const std::string& chat_id,
                   const std::string& text,
                   std::string& failure_reason) const;
    bool select_bot(const std::string& chat_id,
                    std::string& token,
                    std::string& default_chat_id) const;
    void record(const TelegramMessage& message);

    telegram_config::Config config_;
    bool available_{true};
    bool next_send_result_{true};
    bool live_delivery_{true};
    std::vector<TelegramMessage> messages_;
    std::size_t sent_count_{0};
};

} // namespace xauusd::sovereign
