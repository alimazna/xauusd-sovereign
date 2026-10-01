#include "MT5AdapterManager.h"

#include <algorithm>
#include <chrono>

namespace xauusd::mt5 {
namespace {

std::time_t now_seconds() {
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

bool stale(std::time_t heartbeat) {
    if (heartbeat == 0) return false;
    const auto current = now_seconds();
    return current >= heartbeat && static_cast<std::uint64_t>(current - heartbeat) > 30u;
}

} // namespace

MT5AdapterManager::MT5AdapterManager() {
    for (std::size_t i = 0; i < adapters_.size(); ++i) {
        adapters_[i].status.timeframe = kTimeframes[i];
    }
}

MT5AdapterManager::Entry* MT5AdapterManager::find(const std::string& timeframe) {
    const auto it = std::find_if(adapters_.begin(), adapters_.end(),
                                 [&](const Entry& e) { return e.status.timeframe == timeframe; });
    return it == adapters_.end() ? nullptr : &*it;
}

const MT5AdapterManager::Entry* MT5AdapterManager::find(const std::string& timeframe) const {
    const auto it = std::find_if(adapters_.begin(), adapters_.end(),
                                 [&](const Entry& e) { return e.status.timeframe == timeframe; });
    return it == adapters_.end() ? nullptr : &*it;
}

void MT5AdapterManager::register_adapter(const std::string& timeframe) {
    std::lock_guard lock(mutex_);
    if (auto* entry = find(timeframe)) {
        entry->status.connected = true;
        entry->status.last_heartbeat = now_seconds();
        entry->status.last_activity = entry->status.last_heartbeat;
    }
}

void MT5AdapterManager::unregister_adapter(const std::string& timeframe) {
    std::lock_guard lock(mutex_);
    if (auto* entry = find(timeframe)) {
        entry->status.connected = false;
    }
}

void MT5AdapterManager::record_tick(const std::string& timeframe) {
    std::lock_guard lock(mutex_);
    if (auto* entry = find(timeframe)) {
        entry->status.connected = true;
        entry->status.last_activity = now_seconds();
        ++entry->status.ticks_received;
    }
}

void MT5AdapterManager::record_bar(const std::string& timeframe) {
    std::lock_guard lock(mutex_);
    if (auto* entry = find(timeframe)) {
        entry->status.connected = true;
        entry->status.last_activity = now_seconds();
        ++entry->status.bars_received;
    }
}

void MT5AdapterManager::record_heartbeat(const std::string& timeframe) {
    std::lock_guard lock(mutex_);
    if (auto* entry = find(timeframe)) {
        entry->status.connected = true;
        entry->status.last_heartbeat = now_seconds();
        entry->status.last_activity = entry->status.last_heartbeat;
    }
}

bool MT5AdapterManager::is_connected(const std::string& timeframe) const {
    std::lock_guard lock(mutex_);
    const auto* entry = find(timeframe);
    return entry != nullptr && entry->status.connected && !stale(entry->status.last_activity);
}

bool MT5AdapterManager::all_connected() const {
    std::lock_guard lock(mutex_);
    return std::all_of(adapters_.begin(), adapters_.end(), [](const Entry& entry) {
        return entry.status.connected && !stale(entry.status.last_activity);
    });
}

AdapterStatus MT5AdapterManager::status(const std::string& timeframe) const {
    std::lock_guard lock(mutex_);
    const auto* entry = find(timeframe);
    return entry ? entry->status : AdapterStatus{timeframe, false, 0, 0, 0, 0};
}

std::vector<AdapterStatus> MT5AdapterManager::all_status() const {
    std::lock_guard lock(mutex_);
    std::vector<AdapterStatus> result;
    result.reserve(adapters_.size());
    for (const auto& entry : adapters_) {
        result.push_back(entry.status);
        if (result.back().connected && stale(result.back().last_activity)) {
            result.back().connected = false;
        }
    }
    return result;
}

} // namespace xauusd::mt5
