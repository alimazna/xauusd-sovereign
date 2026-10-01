#pragma once

#include <cstdint>

namespace xauusd::sovereign {

// Interface only — no real MT5 API in this batch.
class MT5Bridge {
public:
    MT5Bridge() = default;
    virtual ~MT5Bridge() = default;

    // All methods are inert in this prototype.
    virtual bool is_connected() const;
    virtual bool connect();
    virtual void disconnect();

    virtual bool send_order(double volume, double price,
                            double stop_loss, double take_profit);

    virtual std::uint64_t positions_count() const;
};

} // namespace xauusd::sovereign
