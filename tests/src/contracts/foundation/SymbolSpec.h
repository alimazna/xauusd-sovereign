#pragma once

#include "Timestamp.h"
#include <cstdint>
#include <string>
#include <utility>

namespace xauusd::sovereign {

class SymbolSpec {
public:
    std::string   broker{};
    std::string   server{};
    std::string   symbol{};
    std::uint32_t digits{0};
    double        point{0.0};
    double        tick_size{0.0};
    double        tick_value{0.0};
    double        contract_size{0.0};
    double        volume_min{0.0};
    double        volume_max{0.0};
    double        volume_step{0.0};
    double        stops_level{0.0};
    double        freeze_level{0.0};
    Timestamp     observed_at{};

    SymbolSpec() = default;

    SymbolSpec(
        std::string broker,
        std::string server,
        std::string symbol,
        std::uint32_t digits,
        double point,
        double tick_size,
        double tick_value,
        double contract_size,
        double volume_min,
        double volume_max,
        double volume_step,
        double stops_level,
        double freeze_level,
        Timestamp observed_at = Timestamp{})
        : broker(std::move(broker)),
          server(std::move(server)),
          symbol(std::move(symbol)),
          digits(digits),
          point(point),
          tick_size(tick_size),
          tick_value(tick_value),
          contract_size(contract_size),
          volume_min(volume_min),
          volume_max(volume_max),
          volume_step(volume_step),
          stops_level(stops_level),
          freeze_level(freeze_level),
          observed_at(observed_at) {}
};

} // namespace xauusd::sovereign
