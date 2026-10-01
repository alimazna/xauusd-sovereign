#pragma once

#include "../../contracts/foundation/Bar.h"
#include "../../contracts/foundation/SymbolSpec.h"
#include "../../contracts/foundation/Tick.h"

#include <cstdint>
#include <optional>
#include <string>

namespace xauusd::mt5 {

class MT5MessageParser {
public:
    static std::optional<sovereign::Tick> parse_tick(const std::string& json);
    static std::optional<sovereign::Bar> parse_bar(const std::string& json);
    static std::optional<sovereign::SymbolSpec> parse_symbol_spec(const std::string& json);

    // Minimal flat-JSON accessors used by the bridge for transport metadata.
    static std::string extract_string(const std::string& json, const std::string& key);
    static double extract_number(const std::string& json, const std::string& key);
    static std::int64_t extract_int64(const std::string& json, const std::string& key);
    static bool extract_bool(const std::string& json, const std::string& key, bool& value);

    static std::optional<sovereign::Timeframe> parse_timeframe(const std::string& value);
};

} // namespace xauusd::mt5
