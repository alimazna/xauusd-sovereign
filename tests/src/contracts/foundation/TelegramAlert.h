#pragma once

#include "EntityId.h"
#include "NotificationPriority.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct TelegramAlert {
    EntityId             alert_id;
    NotificationPriority priority;
    std::string          title;
    std::string          body;
    Timestamp            created_at;
    bool                 delivered;

    TelegramAlert() = default;

    TelegramAlert(EntityId alert_id, NotificationPriority priority,
                  std::string title, std::string body,
                  Timestamp created_at, bool delivered)
        : alert_id(alert_id), priority(priority),
          title(std::move(title)), body(std::move(body)),
          created_at(created_at), delivered(delivered) {}
};

} // namespace xauusd::sovereign
