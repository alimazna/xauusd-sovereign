#pragma once

#include "EntityId.h"
#include "NotificationRecord.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class INotificationService {
public:
    virtual ~INotificationService() = default;

    virtual bool enqueue(const NotificationRecord& record) = 0;
    virtual bool mark_delivered(const EntityId& notification_id) = 0;
    virtual std::vector<NotificationRecord> all() const = 0;
    virtual std::vector<NotificationRecord> undelivered() const = 0;
    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
