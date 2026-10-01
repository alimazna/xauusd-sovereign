#pragma once

#include "Bar.h"
#include "Tick.h"

#include <functional>

namespace xauusd::sovereign {

class IDataBus {
public:
    using TickHandler = std::function<void(const Tick&)>;
    using BarHandler  = std::function<void(const Bar&)>;

    virtual ~IDataBus() = default;

    virtual void subscribe_ticks(TickHandler handler) = 0;
    virtual void subscribe_bars(BarHandler handler) = 0;

    virtual void publish_tick(const Tick& tick) = 0;
    virtual void publish_bar(const Bar& bar) = 0;
};

} // namespace xauusd::sovereign
