#pragma once

#include "EntityId.h"
#include "NotificationChannel.h"
#include "NotificationPriority.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct NotificationRecord {
    EntityId             notification_id;
    NotificationChannel  channel;
    NotificationPriority priority;
    std::string          title;
    std::string          body;
    Timestamp            queued_at;
    Timestamp            delivered_at;
    bool                 delivered;

    NotificationRecord() = default;

    NotificationRecord(EntityId notification_id,
                       NotificationChannel channel,
                       NotificationPriority priority,
                       std::string title, std::string body,
                       Timestamp queued_at, Timestamp delivered_at,
                       bool delivered)
        : notification_id(notification_id), channel(channel),
          priority(priority), title(std::move(title)),
          body(std::move(body)), queued_at(queued_at),
          delivered_at(delivered_at), delivered(delivered) {}
};

} // namespace xauusd::sovereign
