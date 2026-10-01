#include "DataBus.h"

namespace xauusd::sovereign {

void DataBus::subscribe_ticks(TickHandler handler)
{
    tick_handlers_.push_back(std::move(handler));
}

void DataBus::subscribe_bars(BarHandler handler)
{
    bar_handlers_.push_back(std::move(handler));
}

void DataBus::publish_tick(const Tick& tick)
{
    for (auto& h : tick_handlers_) {
        if (h) {
            h(tick);
        }
    }
}

void DataBus::publish_bar(const Bar& bar)
{
    for (auto& h : bar_handlers_) {
        if (h) {
            h(bar);
        }
    }
}

std::size_t DataBus::tick_subscriber_count() const noexcept
{
    return tick_handlers_.size();
}

std::size_t DataBus::bar_subscriber_count() const noexcept
{
    return bar_handlers_.size();
}

void DataBus::clear_subscribers()
{
    tick_handlers_.clear();
    bar_handlers_.clear();
}

} // namespace xauusd::sovereign
