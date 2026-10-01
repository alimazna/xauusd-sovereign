#pragma once

#include "NotificationService.h"
#include "TelegramGateway.h"

#include <cstddef>
#include <string>

namespace xauusd::sovereign {

class TelegramOrchestrator {
public:
    TelegramOrchestrator() = default;

    // Message sending
    bool send_status(const std::string& chat_id, const std::string& text);
    bool send_alert(NotificationPriority priority,
                    const std::string& title,
                    const std::string& body);

    // Notification queue
    bool enqueue_notification(const NotificationRecord& record);
    bool mark_notification_delivered(const EntityId& notification_id);
    std::size_t undelivered_count() const;

    // Availability
    bool gateway_available() const;

    // Enable/disable real Telegram HTTP delivery. Useful for deterministic tests.
    void set_live_delivery(bool enabled);

    // Reset
    void clear();

private:
    TelegramGateway     gateway_;
    NotificationService notifications_;
};

} // namespace xauusd::sovereign
