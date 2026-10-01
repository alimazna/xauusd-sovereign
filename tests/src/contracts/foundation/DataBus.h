#pragma once

#include "IDataBus.h"

#include <cstddef>
#include <functional>
#include <vector>

namespace xauusd::sovereign {

class DataBus final : public IDataBus {
public:
    DataBus() = default;

    void subscribe_ticks(TickHandler handler) override;
    void subscribe_bars(BarHandler handler) override;

    void publish_tick(const Tick& tick) override;
    void publish_bar(const Bar& bar) override;

    std::size_t tick_subscriber_count() const noexcept;
    std::size_t bar_subscriber_count() const noexcept;

    void clear_subscribers();

private:
    std::vector<TickHandler> tick_handlers_;
    std::vector<BarHandler>  bar_handlers_;
};

} // namespace xauusd::sovereign
