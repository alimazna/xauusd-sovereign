#pragma once

#include "Timeframe.h"
#include "Bar.h"
#include "Tick.h"
#include "SymbolSpec.h"

#include <vector>

namespace xauusd::sovereign {

class IDataAdapter {
public:
    virtual ~IDataAdapter() = default;

    virtual bool is_connected() const = 0;

    virtual bool fetch_symbol_spec(SymbolSpec& out_spec) = 0;

    virtual bool fetch_recent_bars(
        Timeframe timeframe,
        std::size_t count,
        std::vector<Bar>& out_bars) = 0;

    virtual bool fetch_latest_tick(Tick& out_tick) = 0;
};

} // namespace xauusd::sovereign
