#pragma once

#include "IDataAdapter.h"

#include <cstddef>
#include <map>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

class MockDataAdapter final : public IDataAdapter {
public:
    MockDataAdapter() = default;

    void set_connected(bool connected) noexcept;

    void set_symbol_spec(const SymbolSpec& spec);

    void set_recent_bars(
        Timeframe timeframe,
        std::vector<Bar> bars);

    void set_latest_tick(const Tick& tick);

    bool is_connected() const override;

    bool fetch_symbol_spec(SymbolSpec& out_spec) override;

    bool fetch_recent_bars(
        Timeframe timeframe,
        std::size_t count,
        std::vector<Bar>& out_bars) override;

    bool fetch_latest_tick(Tick& out_tick) override;

private:
    bool                                  connected_{false};
    bool                                  has_spec_{false};
    SymbolSpec                            spec_{};
    std::map<Timeframe, std::vector<Bar>> bars_by_tf_;
    bool                                  has_tick_{false};
    Tick                                  tick_{};
};

} // namespace xauusd::sovereign
