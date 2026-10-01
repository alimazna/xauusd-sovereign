#pragma once

#include "INotificationService.h"

#include <array>
#include <cstdint>
#include <map>

namespace xauusd::sovereign {

class NotificationService final : public INotificationService {
public:
    NotificationService() = default;

    bool enqueue(const NotificationRecord& record) override;
    bool mark_delivered(const EntityId& notification_id) override;
    std::vector<NotificationRecord> all() const override;
    std::vector<NotificationRecord> undelivered() const override;
    std::size_t size() const override;

    void clear();

private:
    std::vector<NotificationRecord> records_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};

} // namespace xauusd::sovereign
