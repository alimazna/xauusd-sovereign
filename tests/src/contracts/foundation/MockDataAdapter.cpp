#include "MockDataAdapter.h"

#include <algorithm>

namespace xauusd::sovereign {

void MockDataAdapter::set_connected(bool connected) noexcept {
    connected_ = connected;
}

void MockDataAdapter::set_symbol_spec(const SymbolSpec& spec) {
    spec_ = spec;
    has_spec_ = true;
}

void MockDataAdapter::set_recent_bars(
    Timeframe timeframe,
    std::vector<Bar> bars) {
    bars_by_tf_[timeframe] = std::move(bars);
}

void MockDataAdapter::set_latest_tick(const Tick& tick) {
    tick_ = tick;
    has_tick_ = true;
}

bool MockDataAdapter::is_connected() const {
    return connected_;
}

bool MockDataAdapter::fetch_symbol_spec(SymbolSpec& out_spec) {
    if (!has_spec_) {
        return false;
    }
    out_spec = spec_;
    return true;
}

bool MockDataAdapter::fetch_recent_bars(
    Timeframe timeframe,
    std::size_t count,
    std::vector<Bar>& out_bars) {
    auto it = bars_by_tf_.find(timeframe);
    if (it == bars_by_tf_.end() || it->second.empty()) {
        return false;
    }

    const auto& stored = it->second;
    if (count >= stored.size()) {
        out_bars = stored;
    } else {
        out_bars.assign(stored.end() - static_cast<std::ptrdiff_t>(count), stored.end());
    }
    return true;
}

bool MockDataAdapter::fetch_latest_tick(Tick& out_tick) {
    if (!has_tick_) {
        return false;
    }
    out_tick = tick_;
    return true;
}

} // namespace xauusd::sovereign
