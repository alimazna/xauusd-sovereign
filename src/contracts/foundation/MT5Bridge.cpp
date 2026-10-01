#include "MT5Bridge.h"

namespace xauusd::sovereign {

bool MT5Bridge::is_connected() const {
    return false;
}

bool MT5Bridge::connect() {
    return false;
}

void MT5Bridge::disconnect() {
}

bool MT5Bridge::send_order(double volume, double price,
                           double stop_loss, double take_profit) {
    (void)volume;
    (void)price;
    (void)stop_loss;
    (void)take_profit;
    return false;
}

std::uint64_t MT5Bridge::positions_count() const {
    return 0;
}

} // namespace xauusd::sovereign
