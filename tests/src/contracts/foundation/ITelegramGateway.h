#pragma once

#include "TelegramAlert.h"
#include "TelegramMessage.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class ITelegramGateway {
public:
    virtual ~ITelegramGateway() = default;

    virtual bool send(const TelegramMessage& message) = 0;
    virtual bool send_alert(const TelegramAlert& alert) = 0;
    virtual bool is_available() const = 0;
    virtual std::vector<TelegramMessage> recent_messages() const = 0;
    virtual std::size_t messages_sent() const = 0;
};

} // namespace xauusd::sovereign
