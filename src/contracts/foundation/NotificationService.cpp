#include "NotificationService.h"

namespace xauusd::sovereign {

bool NotificationService::enqueue(const NotificationRecord& record) {
    const auto key = record.notification_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }
    index_.emplace(key, records_.size());
    records_.push_back(record);
    return true;
}

bool NotificationService::mark_delivered(const EntityId& notification_id) {
    const auto key = notification_id.bytes();
    const auto it = index_.find(key);
    if (it == index_.end()) {
        return false;
    }
    records_[it->second].delivered = true;
    return true;
}

std::vector<NotificationRecord> NotificationService::all() const {
    return records_;
}

std::vector<NotificationRecord> NotificationService::undelivered() const {
    std::vector<NotificationRecord> result;
    for (const auto& record : records_) {
        if (!record.delivered) {
            result.push_back(record);
        }
    }
    return result;
}

std::size_t NotificationService::size() const {
    return records_.size();
}

void NotificationService::clear() {
    records_.clear();
    index_.clear();
}

} // namespace xauusd::sovereign
