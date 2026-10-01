#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"
#include "PanelVisibility.h"
#include "SelectionState.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

namespace xauusd::ui {

struct UiState {
    PanelVisibility panels;
    SelectionState selection;
    bool exit_requested{false};
    std::string status_message;
    float notification_ttl_seconds{4.0f};
};

inline void set_notification(UiState& state, std::string message) {
    state.status_message = std::move(message);
    state.notification_ttl_seconds = 4.0f;
}

inline std::string format_id(const xauusd::sovereign::EntityId& id) {
    static constexpr char hex[] = "0123456789ABCDEF";
    std::string result;
    result.reserve(34);
    const auto& bytes = id.bytes();
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            result.push_back('-');
        }
        const auto value = bytes[i];
        result.push_back(hex[(value >> 4) & 0x0F]);
        result.push_back(hex[value & 0x0F]);
    }
    return result;
}

inline std::string format_timestamp(const xauusd::sovereign::Timestamp& timestamp) {
    return std::to_string(timestamp.value());
}

inline std::string format_version(const xauusd::sovereign::Version& version) {
    return std::to_string(version.value());
}

} // namespace xauusd::ui
